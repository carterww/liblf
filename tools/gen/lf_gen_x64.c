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

struct instruction {
	char ins[16];
};

static const char *guard_start = "#ifndef LF_OP_X64_H\n"
				 "#define LF_OP_X64_H\n\n";
static const char *guard_end = "#endif /* LF_OP_X64_H */\n";

static const char *includes = "#include <stdbool.h>\n"
			      "#include <stddef.h>\n"
			      "#include <stdint.h>\n\n"
			      "#include <lf_cc.h>\n"
			      "#include <lf_platform.h>\n\n";

#define LF_FENCE_C "\t" ASM "(\"\" ::: \"memory\");"
#define LF_FENCE_M "\t" ASM "(\"mfence\" ::: \"memory\");"
#define LF_FENCE_S "\t" ASM "(\"sfence\" ::: \"memory\");"
#define LF_FENCE_L "\t" ASM "(\"mfence\" ::: \"memory\");"
#define LF_FENCE_M_LOCK "\t" ASM "(\"lock orq $0x0, (%%rsp)\" ::: \"memory\");"

#define LF_FENCE_CC "\tlf_fence_cc();"
#define LF_FENCE_FULL "\tlf_fence_full();"

static const char *lf_gen_func_fence_impl[] = {
	[LF_GEN_FUNC_FENCE_CC] = LF_FENCE_C,
	[LF_GEN_FUNC_FENCE_FULL_FORCE] = LF_FENCE_M,
	[LF_GEN_FUNC_FENCE_STORE_FORCE] = LF_FENCE_S,
	[LF_GEN_FUNC_FENCE_LOAD_FORCE] = LF_FENCE_L,
	[LF_GEN_FUNC_FENCE_FULL] = LF_FENCE_M_LOCK,
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

/* To generate the x64 asm we really only need 4 implements per operation.
 * Each one operates on either bytes, words, long words, or quad words.
 * This array helps us map types to their underlying implementation.
 *
 * To avoid breaking the strict aliasing rule, we can't use lf_op_load_u64()
 * to load a long or something. So we will need to generate inline asm that
 * uses the correct underlying types for each function.
 */
static const size_t lf_gen_x64_type_to_size[] = {
	[LF_GEN_TYPE_PTR] = 8,	  [LF_GEN_TYPE_IPTR] = 8,
	[LF_GEN_TYPE_UPTR] = 8,	  [LF_GEN_TYPE_CHAR] = 1,
	[LF_GEN_TYPE_UCHAR] = 1,  [LF_GEN_TYPE_SHORT] = 2,
	[LF_GEN_TYPE_USHORT] = 2, [LF_GEN_TYPE_INT] = 4,
	[LF_GEN_TYPE_UINT] = 4,	  [LF_GEN_TYPE_LONG] = 8,
	[LF_GEN_TYPE_ULONG] = 8,  [LF_GEN_TYPE_I8] = 1,
	[LF_GEN_TYPE_U8] = 1,	  [LF_GEN_TYPE_I16] = 2,
	[LF_GEN_TYPE_U16] = 2,	  [LF_GEN_TYPE_I32] = 4,
	[LF_GEN_TYPE_U32] = 4,	  [LF_GEN_TYPE_I64] = 8,
	[LF_GEN_TYPE_U64] = 8,	  [LF_GEN_TYPE_SIZE] = 8,
	[LF_GEN_TYPE_NATIVE] = 8,
};

static const char lf_gen_x64_size_to_suffix[] = {
	[1] = 'b',
	[2] = 'w',
	[4] = 'l',
	[8] = 'q',
};

static struct instruction make_instruction(const char *buf,
					   enum lf_gen_type type)
{
	struct instruction ins = { 0 };
	size_t buf_len = strlen(buf);
	size_t size = lf_gen_x64_type_to_size[type];
	char suffix = lf_gen_x64_size_to_suffix[size];
	memcpy(ins.ins, buf, buf_len);
	ins.ins[buf_len] = suffix;
	ins.ins[buf_len + 1] = '\0';
	return ins;
}

static void generate_fences(void)
{
	enum lf_gen_func_fence fence;
	fence_all_for_each(fence) {
		string s = lf_gen_func_fence_define(
			fence, lf_gen_func_fence_impl[fence]);
		output(s.buffer);
		string_destroy(&s);
	}
}

static string generate_load_impl(enum lf_gen_type type)
{
	string s;
	string_init(&s, 128);
	struct instruction ins = make_instruction("mov", type);

	/* Declare the variable */
	lf_gen_declare_var(&s, type, "val", 1);
	/* Inline asm with mov */
	string_append_raw(&s, "\t" ASM "(\"", 0);
	string_append_raw(&s, ins.ins, 4);
	string_append_raw(
		&s, " %1, %0\" : \"=r\"(val) : \"m\"(*p) : \"memory\");\n", 0);
	/* Return the value */
	lf_gen_return(&s, "val", 1);

	return s;
}

static void generate_loads(void)
{
	lf_gen_all_impl_from_func(generate_load_impl, lf_gen_func_load_define);
}

static string generate_store_impl(enum lf_gen_type type)
{
	string s;
	string_init(&s, 128);
	struct instruction ins = make_instruction("mov", type);

	/* Inline asm with mov */
	string_append_raw(&s, "\t" ASM "(\"", 0);
	string_append_raw(&s, ins.ins, 4);
	string_append_raw(
		&s, " %0, %1\" : : \"r\"(val), \"m\"(*p) : \"memory\");", 0);

	return s;
}

static void generate_stores(void)
{
	lf_gen_all_impl_from_func(generate_store_impl,
				  lf_gen_func_store_define);
}

static string generate_swap_impl(enum lf_gen_type type)
{
	string s;
	string_init(&s, 128);
	struct instruction ins = make_instruction("xchg", type);

	/* Inline asm with lock xchg */
	string_append_raw(&s, "\t" ASM "(\"lock ", 0);
	string_append_raw(&s, ins.ins, 5);
	string_append_raw(
		&s, " %0, %1\" : \"+r\"(val), \"+m\"(*p) : : \"memory\");", 0);
	/* Return the value */
	lf_gen_return(&s, "val", 1);

	return s;
}

static void generate_swaps(void)
{
	lf_gen_all_impl_from_func(generate_swap_impl, lf_gen_func_swap_define);
}

static string generate_cas_impl(enum lf_gen_type type)
{
	string s;
	string_init(&s, 128);
	struct instruction ins = make_instruction("cmpxchg", type);

	/* Declare the variable */
	string_append_raw(&s, "\tbool zf;\n", 0);
	/* Inline asm with mov */
	string_append_raw(&s, "\t" ASM "(\"lock ", 0);
	string_append_raw(&s, ins.ins, 8);
	string_append_raw(&s, " %3, %0\" :", 0);
	string_append_raw(&s, " \"+m\"(*p), \"=@ccz\"(zf), \"+a\"(val_old) :", 0);
	string_append_raw(&s, " \"r\"(val_new) :", 0);
	string_append_raw(&s, " \"memory\", \"cc\");\n", 0);
	/* Return the value */
	lf_gen_return(&s, "zf", 1);

	return s;
}

static string generate_casx_impl(enum lf_gen_type type)
{
	string s;
	string_init(&s, 128);
	struct instruction ins = make_instruction("cmpxchg", type);

	/* Declare the variable */
	lf_gen_declare_and_set_var(&s, type, "val_orig", "val_old", 1);
	/* Inline asm with mov */
	string_append_raw(&s, "\t" ASM "(\"lock ", 0);
	string_append_raw(&s, ins.ins, 8);
	string_append_raw(&s, " %2, %0\" :", 0);
	string_append_raw(&s, " \"+m\"(*p),  \"+a\"(val_orig) :", 0);
	string_append_raw(&s, " \"r\"(val_new) :", 0);
	string_append_raw(&s, " \"memory\", \"cc\");\n", 0);
	/* Return the value */
	lf_gen_return(&s, "val_orig", 8);

	return s;
}

static void generate_cas(void)
{
	lf_gen_all_impl_from_func(generate_cas_impl, lf_gen_func_cas_define);
	lf_gen_all_impl_from_func(generate_casx_impl, lf_gen_func_casx_define);
}

int main(void)
{
	static const char *module_comment =
		"lf_op_x64.h implements many atomic and lock-free related functions in "
		"x86 assembly, specifically for x86_64. You should not include this file directly, but "
		"you can if you want to use the x64 functions directly. If you want to let liblf "
		"decide, include lf_op.h instead.";
	lf_gen_header_output("liblf/tools/gen/lf_gen_x64.c", module_comment);
	output(guard_start);
	output(includes);

	generate_fences();
	generate_loads();
	generate_stores();
	generate_swaps();
	generate_cas();

	output(guard_end);
}
