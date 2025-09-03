/* SPDX-License-Identifier: MIT
 * Copyright (c) 2025 Carter Williams
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include "lf_gen_common.h"
#include "lf_gen_functions.h"
#include "lf_gen_string.h"
#include "lf_gen_types.h"

static const char *guard_op = "LF_OP_GCC_H";
static const char *guard_op_sup = "LF_OP_GCC_SUP_H";

static const char *includes = "#include <stdbool.h>\n"
			      "#include <stddef.h>\n"
			      "#include <stdint.h>\n\n"
			      "#include <lf_cc.h>\n"
			      "#include <lf_platform.h>\n\n";

#define LF_FENCE_C "\t__asm__ __volatile__(\"\" ::: \"memory\");"
#define LF_FENCE_M "\t__atomic_thread_fence(__ATOMIC_SEQ_CST);"
#define LF_FENCE_S "\t__atomic_thread_fence(__ATOMIC_RELEASE);"
#define LF_FENCE_L "\t__atomic_thread_fence(__ATOMIC_ACQUIRE);"

#define LF_FENCE_CC "\tlf_fence_cc();"
#define LF_FENCE_FULL "\tlf_fence_full_force();"
#define LF_FENCE_STORE "\tlf_fence_store_force();"
#define LF_FENCE_LOAD "\tlf_fence_load_force();"

static const char *lf_gen_func_fence_impl_tso[] = {
	[LF_GEN_FUNC_FENCE_CC] = LF_FENCE_C,
	[LF_GEN_FUNC_FENCE_FULL_FORCE] = LF_FENCE_M,
	[LF_GEN_FUNC_FENCE_STORE_FORCE] = LF_FENCE_S,
	[LF_GEN_FUNC_FENCE_LOAD_FORCE] = LF_FENCE_L,
	[LF_GEN_FUNC_FENCE_FULL] = LF_FENCE_FULL,
	[LF_GEN_FUNC_FENCE_STORE] = LF_FENCE_CC,
	[LF_GEN_FUNC_FENCE_LOAD] = LF_FENCE_CC,
	[LF_GEN_FUNC_FENCE_STORE_LOAD] = LF_FENCE_FULL,
	[LF_GEN_FUNC_FENCE_LOAD_STORE] = LF_FENCE_CC,
	[LF_GEN_FUNC_FENCE_ATOMIC] = LF_FENCE_CC,
	[LF_GEN_FUNC_FENCE_ATOMIC_LOAD] = LF_FENCE_CC,
	[LF_GEN_FUNC_FENCE_LOAD_ATOMIC] = LF_FENCE_CC,
	[LF_GEN_FUNC_FENCE_ATOMIC_STORE] = LF_FENCE_CC,
	[LF_GEN_FUNC_FENCE_STORE_ATOMIC] = LF_FENCE_CC
};

static const char *lf_gen_func_fence_impl_wmo[] = {
	[LF_GEN_FUNC_FENCE_CC] = LF_FENCE_C,
	[LF_GEN_FUNC_FENCE_FULL_FORCE] = LF_FENCE_M,
	[LF_GEN_FUNC_FENCE_STORE_FORCE] = LF_FENCE_S,
	[LF_GEN_FUNC_FENCE_LOAD_FORCE] = LF_FENCE_L,
	[LF_GEN_FUNC_FENCE_FULL] = LF_FENCE_FULL,
	[LF_GEN_FUNC_FENCE_STORE] = LF_FENCE_STORE,
	[LF_GEN_FUNC_FENCE_LOAD] = LF_FENCE_LOAD,
	[LF_GEN_FUNC_FENCE_STORE_LOAD] = LF_FENCE_FULL,
	[LF_GEN_FUNC_FENCE_LOAD_STORE] = LF_FENCE_FULL,
	[LF_GEN_FUNC_FENCE_ATOMIC] = LF_FENCE_FULL,
	[LF_GEN_FUNC_FENCE_ATOMIC_LOAD] = LF_FENCE_FULL,
	[LF_GEN_FUNC_FENCE_LOAD_ATOMIC] = LF_FENCE_FULL,
	[LF_GEN_FUNC_FENCE_ATOMIC_STORE] = LF_FENCE_FULL,
	[LF_GEN_FUNC_FENCE_STORE_ATOMIC] = LF_FENCE_FULL
};

static void generate_fences(void)
{
	enum lf_gen_func_fence fence;
	fence_force_for_each(fence) {
		string s = lf_gen_func_fence_define(
			fence, lf_gen_func_fence_impl_tso[fence]);
		output(s.buffer);
		string_destroy(&s);
	}

	output("#if defined(LF_MEM_TSO)\n\n");

	fence_other_for_each(fence) {
		string s = lf_gen_func_fence_define(
			fence, lf_gen_func_fence_impl_tso[fence]);
		output(s.buffer);
		string_destroy(&s);
	}

	output("#elif defined(LF_MEM_WMO)\n\n");

	fence_other_for_each(fence) {
		string s = lf_gen_func_fence_define(
			fence, lf_gen_func_fence_impl_wmo[fence]);
		output(s.buffer);
		string_destroy(&s);
	}

	output("#else\n#error Unsupported memory model detected\n");

	output("#endif /* LF_MEM_TSO or LF_MEM_WMO */\n\n");
}

static void generate_all_cast_ptr(const char *impl,
				  string (*func)(enum lf_gen_type,
						 const char *))
{
	string ptr_impl;
	string_init(&ptr_impl, 128);

	for (size_t i = 0; i < strlen(impl) - 1; ++i) {
		if (impl[i] == '(' && impl[i + 1] == 'p') {
			string_append_raw(&ptr_impl, "((void **)", 10);
			string_append_raw(&ptr_impl, &impl[i + 1], 0);
			break;
		}
		string_append_char(&ptr_impl, impl[i]);
	}
	string s = func(LF_GEN_TYPE_PTR, ptr_impl.buffer);
	output(s.buffer);
	string_destroy(&ptr_impl);
	string_destroy(&s);

	enum lf_gen_type type;
	types_integral_for_each(type) {
		s = func(type, impl);
		output(s.buffer);
		string_destroy(&s);
	}
}

static void generate_loads(void)
{
	static const char *impl =
		"\treturn __atomic_load_n(p, __ATOMIC_RELAXED);";
	generate_all_cast_ptr(impl, lf_gen_func_load_define);
}

static void generate_stores(void)
{
	static const char *impl =
		"\t__atomic_store_n(p, val, __ATOMIC_RELAXED);";
	generate_all_cast_ptr(impl, lf_gen_func_store_define);
}

static void generate_swaps(void)
{
	static const char *impl =
		"\treturn __atomic_exchange_n(p, val, __ATOMIC_RELAXED);";
	generate_all_cast_ptr(impl, lf_gen_func_swap_define);
}

static void generate_cas(void)
{
	static const char *cas_impl =
		"\treturn __atomic_compare_exchange_n(p, &val_old, val_new, false, \n"
		"\t\t__ATOMIC_RELAXED, __ATOMIC_RELAXED);";

	static const char *casx_impl =
		"\t(void)__atomic_compare_exchange_n(p, &val_old, val_new, false, \n"
		"\t\t__ATOMIC_RELAXED, __ATOMIC_RELAXED);\n"
		"\treturn val_old;";

	generate_all_cast_ptr(cas_impl, lf_gen_func_cas_define);
	generate_all_cast_ptr(casx_impl, lf_gen_func_casx_define);
}

static void generate_faop_impl(enum lf_gen_type type,
			       enum lf_gen_func_category cat)
{
	static const char *ops[] = {
		[LF_GEN_FUNC_FAINC] = "add", [LF_GEN_FUNC_FADEC] = "sub",
		[LF_GEN_FUNC_FAADD] = "add", [LF_GEN_FUNC_FASUB] = "sub",
		[LF_GEN_FUNC_FAAND] = "and", [LF_GEN_FUNC_FAOR] = "or",
		[LF_GEN_FUNC_FAXOR] = "xor",
	};
	string impl;
	string_init(&impl, 64);
	string_append_raw(&impl, "\treturn __atomic_fetch_", 0);
	string_append_raw(&impl, ops[cat], 0);
	string s;
	if (cat == LF_GEN_FUNC_FAINC || cat == LF_GEN_FUNC_FADEC) {
		string_append_raw(&impl, "(p, 1, __ATOMIC_RELAXED);", 0);
		s = lf_gen_func_fainc_fadec_define(type, cat, impl.buffer);
	} else {
		string_append_raw(&impl, "(p, val, __ATOMIC_RELAXED);", 0);
		s = lf_gen_func_faop_define(type, cat, impl.buffer);
	}
	output(s.buffer);
	string_destroy(&s);
	string_destroy(&impl);
}

static enum lf_gen_func_category lf_cat_to_facat(enum lf_gen_func_category cat)
{
	// This is super hacky
	return cat + 7;
}

static void generate_op_impl(enum lf_gen_type type,
			     enum lf_gen_func_category cat)
{
	const char *type_alias = lf_gen_type_alias[type];
	string impl;
	string s;
	string_init(&impl, 64);
	string_append_raw(&impl, "\t(void)", 0);
	string_append_raw(&impl,
			  lf_gen_func_category_namespaces[lf_cat_to_facat(cat)],
			  0);
	string_append_raw(&impl, type_alias, 0);
	if (cat == LF_GEN_FUNC_INC || cat == LF_GEN_FUNC_DEC) {
		string_append_raw(&impl, "(p);", 0);
		s = lf_gen_func_inc_dec_define(type, cat, impl.buffer);
	} else {
		string_append_raw(&impl, "(p, val);", 0);
		s = lf_gen_func_op_define(type, cat, impl.buffer);
	}
	output(s.buffer);
	string_destroy(&s);
	string_destroy(&impl);
}

static void generate_reg_ops(void)
{
	enum lf_gen_type type;
	enum lf_gen_func_category func;

	/* Iterate over all the fetch and _ ops */
	funcs_faops_for_each(func) {
		types_integral_for_each(type) {
			generate_faop_impl(type, func);
		}
	}

	/* Iterate over all the non-fetch variants of the previous ops */
	funcs_ops_for_each(func) {
		types_integral_for_each(type) {
			generate_op_impl(type, func);
		}
	}
}

static void genereate_bitop_impl(enum lf_gen_type type,
				 enum lf_gen_func_category cat,
				 const char *atomic_op)
{
	const char *type_name = lf_gen_type_names[type];
	size_t type_name_len = strlen(type_name);
	string impl;
	string_init(&impl, 256);

	/* Set the mask variable */
	string_append_raw(&impl, "\t", 0);
	string_append_raw(&impl, type_name, type_name_len);
	string_append_raw(&impl, " mask = (", 0);
	string_append_raw(&impl, type_name, type_name_len);
	string_append_raw(&impl, ")((", 0);
	string_append_raw(&impl, type_name, type_name_len);
	string_append_raw(&impl, ")1 << index);\n", 0);

	/* Perform the or */
	string_append_raw(&impl, "\t", 0);
	string_append_raw(&impl, type_name, type_name_len);
	string_append_raw(&impl, " old = ", 0);
	string_append_raw(&impl, atomic_op, 0);
	string_append_raw(&impl, "\n", 0);

	/* Return the bool */
	string_append_raw(&impl, "\treturn (old & mask) != 0;", 0);

	string s = lf_gen_func_bitop_define(type, cat, impl.buffer);
	output(s.buffer);
	string_destroy(&s);
	string_destroy(&impl);
}

static void generate_bitops(void)
{
	static const char *bts_op =
		"__atomic_fetch_or(p, mask, __ATOMIC_RELAXED);";
	static const char *btr_op =
		"__atomic_fetch_and(p, ~mask, __ATOMIC_RELAXED);";
	static const char *btc_op =
		"__atomic_fetch_xor(p, mask, __ATOMIC_RELAXED);";

	enum lf_gen_type type;

	/* Generate all bts functions */
	types_integral_for_each(type) {
		genereate_bitop_impl(type, LF_GEN_FUNC_BTS, bts_op);
	}
	/* Generate all btr functions */
	types_integral_for_each(type) {
		genereate_bitop_impl(type, LF_GEN_FUNC_BTR, btr_op);
	}
	/* Generate all btc functions */
	types_integral_for_each(type) {
		genereate_bitop_impl(type, LF_GEN_FUNC_BTC, btc_op);
	}
}

static void generate_op(void)
{
	static const char *module_comment_op =
		"lf_op_gcc.h implements many atomic and lock-free related functions using "
		"only GCC __atomic intrinsics. You should not include this file directly, but "
		"you can if you want to use the GCC intrinsics directly. If you want to let liblf "
		"decide, include lf_op.h instead.";
	lf_gen_header_output("liblf/tools/gen/lf_gen_gcc.c", module_comment_op);
	lf_gen_header_guard(guard_op, true);
	output(includes);

	generate_fences();
	generate_loads();
	generate_stores();
	generate_swaps();
	generate_cas();
	generate_reg_ops();
	generate_bitops();

	lf_gen_header_guard(guard_op, false);
}

static bool is_op_supported(enum lf_gen_func_category func,
			    enum lf_gen_type type)
{
	/* Don't support ops like add, sub, or for pointers */
	if (lf_gen_func_is_integral_only(func) && type == LF_GEN_TYPE_PTR) {
		return false;
	}
	/* I haven't written these, and I'm not sure if I want to using GCC builtins */
	if (func == LF_GEN_FUNC_DCAS || func == LF_GEN_FUNC_DCASX) {
		return false;
	}
	size_t max_size = lf_gen_type_max_sizes[type];
	/* If the max size is 1, 2, or 4 then it is supported on all architectures.
	 * If not, check if its size is "native" to the arch (ptr, size_t, etc.)
	 */
	return max_size < 8 || lf_gen_type_is_native_width(type);
}

static void generate_op_sup(void)
{
	static const char *module_comment_op_sup =
		"lf_op_gcc_sup.h defines macros for each type and operation. If the macro evaluates to "
		"0, that operation is not supported on that type for all supported architectures. "
		"If the macro evaluates to 1, that operation is supported on that type for all supported "
		"architectures. It is important to note that \"supported\" means implemented in a lock-free "
		"manner. For example, DCAS could be emulated on RV32 w/ locks but that is not truly lock-free.";
	lf_gen_header_output("liblf/tools/gen/lf_gen_gcc.c",
			     module_comment_op_sup);
	lf_gen_header_guard(guard_op_sup, true);

	enum lf_gen_func_category func;
	enum lf_gen_type type;
	funcs_for_each(func) {
		types_for_each(type) {
			lf_gen_output_op_sup_macro(type, func,
						   is_op_supported(func, type));
		}
		output("\n");
	}

	lf_gen_header_guard(guard_op_sup, false);
}

int main(int argc, char *argv[])
{
	if (argc < 1) {
		abort();
	} else if (argc == 1) {
		fprintf(stderr, "Usage: %s <op | op_sup>\n", argv[0]);
		return 1;
	}

	if (strcmp(argv[1], "op_sup") == 0) {
		generate_op_sup();
	} else if (strcmp(argv[1], "op") == 0) {
		generate_op();
	} else {
		fprintf(stderr, "ERROR: Unknown argument %s\n", argv[1]);
		fprintf(stderr, "Usage: %s <op | op_sup>\n", argv[0]);
		return 1;
	}
	return 0;
}
