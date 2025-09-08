/* SPDX-License-Identifier: MIT
 * Copyright (c) 2025 Carter Williams
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "lf_gen_common.h"
#include "lf_gen_functions.h"
#include "lf_gen_string.h"
#include "lf_gen_types.h"

static const char *guard_op = "LF_OP_X64_H";
static const char *guard_op_sup = "LF_OP_X64_SUP_H";

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

enum lf_gen_x64_instruction {
	LF_GEN_X64_MOV = 0,
	LF_GEN_X64_XCHG,
	LF_GEN_X64_CMPXCHG,
	LF_GEN_X64_ADD,
	LF_GEN_X64_SUB,
	LF_GEN_X64_XADD,
	LF_GEN_X64_NEG,
	LF_GEN_X64_AND,
	LF_GEN_X64_OR,
	LF_GEN_X64_XOR,
	LF_GEN_X64_BTS,
	LF_GEN_X64_BTR,
	LF_GEN_X64_BTC,
	LF_GEN_X64_COUNT,
};

static const char *lf_gen_x64_instruction_names[] = {
	[LF_GEN_X64_MOV] = "mov",	  [LF_GEN_X64_XCHG] = "xchg",
	[LF_GEN_X64_CMPXCHG] = "cmpxchg", [LF_GEN_X64_ADD] = "add",
	[LF_GEN_X64_SUB] = "sub",	  [LF_GEN_X64_XADD] = "xadd",
	[LF_GEN_X64_NEG] = "neg",	  [LF_GEN_X64_AND] = "and",
	[LF_GEN_X64_OR] = "or",		  [LF_GEN_X64_XOR] = "xor",
	[LF_GEN_X64_BTS] = "bts",	  [LF_GEN_X64_BTR] = "btr",
	[LF_GEN_X64_BTC] = "btc",
};

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

static const size_t *lf_gen_x64_type_to_alignment = lf_gen_x64_type_to_size;

static const enum lf_gen_type lf_gen_x64_type_to_double_width_type[] = {
	[LF_GEN_TYPE_PTR] = LF_GEN_TYPE_COUNT,
	[LF_GEN_TYPE_IPTR] = LF_GEN_TYPE_COUNT,
	[LF_GEN_TYPE_UPTR] = LF_GEN_TYPE_COUNT,
	[LF_GEN_TYPE_CHAR] = LF_GEN_TYPE_U16,
	[LF_GEN_TYPE_UCHAR] = LF_GEN_TYPE_U16,
	[LF_GEN_TYPE_SHORT] = LF_GEN_TYPE_U32,
	[LF_GEN_TYPE_USHORT] = LF_GEN_TYPE_U32,
	[LF_GEN_TYPE_INT] = LF_GEN_TYPE_U64,
	[LF_GEN_TYPE_UINT] = LF_GEN_TYPE_U64,
	[LF_GEN_TYPE_LONG] = LF_GEN_TYPE_COUNT,
	[LF_GEN_TYPE_ULONG] = LF_GEN_TYPE_COUNT,
	[LF_GEN_TYPE_I8] = LF_GEN_TYPE_I16,
	[LF_GEN_TYPE_U8] = LF_GEN_TYPE_U16,
	[LF_GEN_TYPE_I16] = LF_GEN_TYPE_I32,
	[LF_GEN_TYPE_U16] = LF_GEN_TYPE_U32,
	[LF_GEN_TYPE_I32] = LF_GEN_TYPE_I64,
	[LF_GEN_TYPE_U32] = LF_GEN_TYPE_U64,
	[LF_GEN_TYPE_I64] = LF_GEN_TYPE_COUNT,
	[LF_GEN_TYPE_U64] = LF_GEN_TYPE_COUNT,
	[LF_GEN_TYPE_SIZE] = LF_GEN_TYPE_COUNT,
	[LF_GEN_TYPE_NATIVE] = LF_GEN_TYPE_COUNT,
};

static const char lf_gen_x64_size_to_suffix[] = {
	[1] = 'b',
	[2] = 'w',
	[4] = 'l',
	[8] = 'q',
};

static const char *lf_gen_x64_size_to_utype_name[] = {
	[1] = "uint8_t",
	[2] = "uint16_t",
	[4] = "uint32_t",
	[8] = "uint64_t",
};

static bool transform_possible_instruction(const char *word, size_t word_len,
					   char buf[16], enum lf_gen_type type)
{
	enum lf_gen_x64_instruction ins;
	/* Instructions are not this long */
	if (word_len + 1 >= 16) {
		return false;
	}
	for (ins = LF_GEN_X64_MOV; ins < LF_GEN_X64_COUNT; ++ins) {
		const char *cmp = lf_gen_x64_instruction_names[ins];
		bool eq = strncmp(word, cmp, word_len) == 0;
		if (!eq) {
			continue;
		}
		size_t size = lf_gen_x64_type_to_size[type];
		char suffix = lf_gen_x64_size_to_suffix[size];
		memcpy(buf, word, word_len);
		/* bts, btc, and btr do not operate on 1 byte destinations.
		 * We'll use the w suffix instead of b if type is 1 byte.
		 */
		if (ins == LF_GEN_X64_BTS || ins == LF_GEN_X64_BTC ||
		    ins == LF_GEN_X64_BTR) {
			if (size == 1) {
				suffix = lf_gen_x64_size_to_suffix[2];
			}
		}
		buf[word_len] = suffix;
		buf[word_len + 1] = '\0';
		return true;
	}
	return false;
}

static void append_inline_asm(string *s, enum lf_gen_type type,
			      const char *body, bool new_line)
{
	char ins_buf[16] = { '\0' };
	size_t bodylen = strlen(body);

	string_append_raw(s, "\t" ASM "(", ASM_LEN + 2);

	/* We need to append suffixes (b, w, l, q) to the instructions and
	 * replace apostrophes with quotes.
	 */
	unsigned long i = 0;
	unsigned long start = 0;
	for (; i < bodylen; ++i) {
		unsigned long word_len;
		bool is_instruction;
		switch (body[i]) {
		case '\'':
			if (i != start) {
				string_append_raw(s, &body[start], i - start);
			}
			/* If next char is also an apostrophe we need to append
			 * literal \n\t and start a new line.
			 */
			if (i + 1 < bodylen && body[i + 1] == '\'') {
				string_append_raw(s, "\\n\\t\"", 5);
				string_append_raw(s, "\n\t\t\t     ", 9);
			} else {
				string_append_raw(s, "\"", 1);
			}
			start = i + 1;
			continue;
		case ' ':
			word_len = i - start;
			if (word_len != 0) {
				is_instruction = transform_possible_instruction(
					&body[start], word_len, ins_buf, type);
				if (!is_instruction) {
					string_append_raw(s, &body[start],
							  word_len);
				} else {
					string_append_raw(s, ins_buf,
							  word_len + 1);
				}
			}
			string_append_raw(s, " ", 1);
			start = i + 1;
			continue;
		case '\n':
			if (i != start) {
				string_append_raw(s, &body[start], i - start);
			}
			/* NOTE(Carter): I pass this through clang-format but might as
			 * well try to make the output as pretty as possible.
			 */
			string_append_raw(s, "\n\t\t\t     ", 9);
			start = i + 1;
			continue;
		default:
			continue;
		}
	}

	if (i != start) {
		string_append_raw(s, &body[start], i - start);
	}

	string_append_raw(s, ");", 2);
	if (new_line) {
		string_append_raw(s, "\n", 1);
	}
}

static void append_inline_asm_fix_ptr_deref(string *s, enum lf_gen_type type,
					    const char *body, bool new_line)
{
	if (type == LF_GEN_TYPE_PTR) {
		size_t len = strlen(body);
		string ptr_impl;
		string_init(&ptr_impl, 128);
		/* This is so lazy of me */
		for (size_t i = 0; i < len - 1; ++i) {
			if (body[i] == '*' && body[i + 1] == 'p') {
				string_append_raw(&ptr_impl, "*(void **)", 10);
				continue;
			} else if (body[i] == '*' && body[i + 1] == 'v') {
				string_append_raw(&ptr_impl, "*(void **)", 10);
				continue;
			}
			string_append_char(&ptr_impl, body[i]);
		}
		string_append_char(&ptr_impl, body[len - 1]);
		append_inline_asm(s, type, ptr_impl.buffer, new_line);
		string_destroy(&ptr_impl);
		return;
	}
	append_inline_asm(s, type, body, new_line);
}

static string generate_load_impl(enum lf_gen_type type)
{
	string s;
	string_init(&s, 128);

	static const char *asm_load_x64 = "'mov %1, %0'\n"
					  ": '=r'(val)\n"
					  ": 'm'(*p)\n"
					  ": 'memory'";

	/* Declare the variable we load into */
	lf_gen_declare_var(&s, type, "val", 1);
	append_inline_asm_fix_ptr_deref(&s, type, asm_load_x64, true);
	lf_gen_return(&s, "val", 1);

	return s;
}

static string generate_store_impl(enum lf_gen_type type)
{
	string s;
	string_init(&s, 128);
	static const char *asm_store_x64 = "'mov %1, %0'\n"
					   ": '=m'(*p)\n"
					   ": 'r'(val)\n"
					   ": 'memory'";

	append_inline_asm_fix_ptr_deref(&s, type, asm_store_x64, false);

	return s;
}

static string generate_swap_impl(enum lf_gen_type type)
{
	string s;
	string_init(&s, 128);
	static const char *asm_swap_x64 = "'lock xchg %0, %1'\n"
					  ": '+r'(val), '+m'(*p)\n"
					  ":\n"
					  ": 'memory'";

	append_inline_asm_fix_ptr_deref(&s, type, asm_swap_x64, true);
	/* Return the val that was swapped with *p */
	lf_gen_return(&s, "val", 1);

	return s;
}

static string generate_cas_impl(enum lf_gen_type type)
{
	string s;
	string_init(&s, 128);
	static const char *asm_cas_x64 =
		"'lock cmpxchg %3, %0'\n"
		": '+m'(*p), '=@ccz'(zf), '+a'(val_old)\n"
		": 'r'(val_new)\n"
		": 'memory', 'cc'";

	/* Declare the zero flag variable */
	lf_gen_declare_var_str_type(&s, "bool", "zf", 1);
	append_inline_asm_fix_ptr_deref(&s, type, asm_cas_x64, true);
	/* Return the zero flag*/
	lf_gen_return(&s, "zf", 1);

	return s;
}

static string generate_casx_impl(enum lf_gen_type type)
{
	string s;
	string_init(&s, 128);

	static const char *asm_casx_x64 =
		"'lock cmpxchg %3, %0'\n"
		": '+m'(*p), '=@ccz'(zf), '+a'(*val_old)\n"
		": 'r'(val_new)\n"
		": 'memory', 'cc'";

	lf_gen_declare_var_str_type(&s, "bool", "zf", 1);
	append_inline_asm_fix_ptr_deref(&s, type, asm_casx_x64, true);
	/* Return the original value of p */
	lf_gen_return(&s, "zf", 1);

	return s;
}

static string generate_faadd_fasub_impl(enum lf_gen_type type,
					enum lf_gen_func_category cat)
{
	string s;
	string_init(&s, 128);

	static const char *asm_faadd_x64 = "'lock xadd %1, %0'\n"
					   ": '+m'(*p), '+r'(val)\n"
					   ":\n"
					   ": 'memory', 'cc'";

	static const char *asm_fasub_x64 = "'neg %1'"
					   "'lock xadd %1, %0'\n"
					   ": '+m'(*p), '+r'(val)\n"
					   ":\n"
					   ": 'memory', 'cc'";
	const char *table[2] = {
		[0] = asm_faadd_x64,
		[LF_GEN_FUNC_FASUB - LF_GEN_FUNC_FAADD] = asm_fasub_x64,
	};
	if (cat == LF_GEN_FUNC_FAADD || cat == LF_GEN_FUNC_FASUB) {
		append_inline_asm(&s, type, table[cat - LF_GEN_FUNC_FAADD],
				  true);
		lf_gen_return(&s, "val", 1);
		return s;
	} else if (cat == LF_GEN_FUNC_FAINC || cat == LF_GEN_FUNC_FADEC) {
		/* Can't use immediate value with xadd so just use underlying
		 * faadd or fasub implementation.
		 */
		string fname = lf_gen_func_get_name(type, cat + 2);
		string_append_raw(&s, "\treturn ", 8);
		string_append(&s, &fname);
		string_append_raw(&s, "(p, 1);", 7);
		string_destroy(&fname);
		return s;
	} else {
		abort();
	}
	return s;
}

static string generate_faand_faor_faxor_impl(enum lf_gen_type type,
					     enum lf_gen_func_category cat)
{
	string s;
	string_init(&s, 256);

	const char *table[3] = {
		[0] = "&",
		[LF_GEN_FUNC_FAOR - LF_GEN_FUNC_FAAND] = "|",
		[LF_GEN_FUNC_FAXOR - LF_GEN_FUNC_FAAND] = "^",
	};
	string load_func = lf_gen_func_get_name(type, LF_GEN_FUNC_LOAD);
	string_append_raw(&load_func, "(p)", 3);

	string cas_func = lf_gen_func_get_name(type, LF_GEN_FUNC_CAS);
	string_append_raw(&cas_func, "(p, old, new)", 13);

	const char *op = table[cat - LF_GEN_FUNC_FAAND];
	string new_set;
	string_init(&new_set, 16);
	string_append_raw(&new_set, "(old ", 5);
	string_append_raw(&new_set, op, 1);
	string_append_raw(&new_set, " val)", 5);

	lf_gen_declare_var(&s, type, "old", 1);
	lf_gen_declare_var(&s, type, "new", 1);
	string_append_raw(&s, "\tdo {\n", 0);
	lf_gen_set_var(&s, "old", load_func.buffer, 2);
	lf_gen_set_var(&s, "new", new_set.buffer, 2);
	string_append_raw(&s, "\t} while (!", 0);
	string_append(&s, &cas_func);
	string_append_raw(&s, ");\n", 0);
	lf_gen_return(&s, "old", 1);

	string_destroy(&load_func);
	string_destroy(&cas_func);
	string_destroy(&new_set);
	return s;
}

static string generate_add_sub_impl(enum lf_gen_type type,
				    enum lf_gen_func_category cat)
{
	string s;
	string_init(&s, 128);

	static const char *asm_inc_x64 = "'lock add $1, %0'\n"
					 ": '+m'(*p)\n"
					 ":\n"
					 ": 'memory', 'cc'";

	static const char *asm_add_x64 = "'lock add %1, %0'\n"
					 ": '+m'(*p)\n"
					 ": 'r'(val)\n"
					 ": 'memory', 'cc'";

	static const char *asm_dec_x64 = "'lock sub $1, %0'\n"
					 ": '+m'(*p)\n"
					 ":\n"
					 ": 'memory', 'cc'";

	static const char *asm_sub_x64 = "'lock sub %1, %0'\n"
					 ": '+m'(*p)\n"
					 ": 'r'(val)\n"
					 ": 'memory', 'cc'";
	const char *table[4] = {
		[0] = asm_inc_x64,
		[LF_GEN_FUNC_DEC - LF_GEN_FUNC_INC] = asm_dec_x64,
		[LF_GEN_FUNC_ADD - LF_GEN_FUNC_INC] = asm_add_x64,
		[LF_GEN_FUNC_SUB - LF_GEN_FUNC_INC] = asm_sub_x64,
	};
	append_inline_asm(&s, type, table[cat - LF_GEN_FUNC_INC], false);
	return s;
}

static string generate_and_or_xor_impl(enum lf_gen_type type,
				       enum lf_gen_func_category cat)
{
	string s;
	string_init(&s, 128);

#define ASM_BOP(ins)              \
	"'lock " ins " %1, %0'\n" \
	": '+m'(*p)\n"            \
	": 'r'(val)\n"            \
	": 'memory', 'cc'"

	static const char *asm_and_x64 = ASM_BOP("and");
	static const char *asm_or_x64 = ASM_BOP("or");
	static const char *asm_xor_x64 = ASM_BOP("xor");
#undef ASM_BOP
	const char *table[3] = {
		[0] = asm_and_x64,
		[LF_GEN_FUNC_OR - LF_GEN_FUNC_AND] = asm_or_x64,
		[LF_GEN_FUNC_XOR - LF_GEN_FUNC_AND] = asm_xor_x64,
	};
	append_inline_asm(&s, type, table[cat - LF_GEN_FUNC_AND], false);
	return s;
}

static string generate_btops_impl(enum lf_gen_type type,
				  enum lf_gen_func_category cat)
{
	string s;
	string_init(&s, 128);
	size_t type_size = lf_gen_x64_type_to_size[type];
	/* bts, btc, and btr don't work on 1 byte memory locations/
	 * registers. We need to treat the type as 2 bytes.
	 */
	if (type_size == 1) {
		type_size = 2;
	}
	const char *utype_name = lf_gen_x64_size_to_utype_name[type_size];

#define ASM_BOP(ins)                \
	"'lock " ins " %2, %0'\n"   \
	": '+m'(*p), '=@ccc'(cf)\n" \
	": 'r'((^)index)\n"         \
	": 'memory', 'cc'"

	static const char *asm_bts_x64 = ASM_BOP("bts");
	static const char *asm_btc_x64 = ASM_BOP("btc");
	static const char *asm_btr_x64 = ASM_BOP("btr");
#undef ASM_BOP
	const char *table[3] = {
		[0] = asm_bts_x64,
		[LF_GEN_FUNC_BTC - LF_GEN_FUNC_BTS] = asm_btc_x64,
		[LF_GEN_FUNC_BTR - LF_GEN_FUNC_BTS] = asm_btr_x64,
	};

	string impl;
	string_init(&impl, 64);
	const char *asm_templ = table[cat - LF_GEN_FUNC_BTS];
	size_t asm_templ_len = strlen(asm_templ);
	for (size_t i = 0; i < asm_templ_len; ++i) {
		if (asm_templ[i] != '^') {
			string_append_raw(&impl, &asm_templ[i], 1);
			continue;
		}
		string_append_raw(&impl, utype_name, 0);
	}

	/* Delcare variable to hold cf */
	string_append_raw(&s, "\tbool cf;\n", 0);
	append_inline_asm(&s, type, impl.buffer, true);
	lf_gen_return(&s, "cf", 1);
	string_destroy(&impl);
	return s;
}

static string generate_dcas_from_cas_impl(enum lf_gen_type type,
					  enum lf_gen_type type_double_width)
{
	string s;
	(void)type;
	const char *tdw_cas_ns =
		lf_gen_func_category_namespaces[LF_GEN_FUNC_CAS];
	const char *tdw_alias = lf_gen_type_alias[type_double_width];
	string_init(&s, 128);

	string_append_raw(&s, "\treturn ", 8);
	string_append_raw(&s, tdw_cas_ns, 0);
	string_append_raw(&s, tdw_alias, 0);
	string_append_raw(&s, "(&p->scalar, val_old.scalar, val_new.scalar);",
			  0);

	return s;
}

static string generate_dcasx_from_casx_impl(enum lf_gen_type type,
					    enum lf_gen_type type_double_width)
{
	string s;
	const char *tdw_cas_ns =
		lf_gen_func_category_namespaces[LF_GEN_FUNC_CASX];
	const char *tdw_alias = lf_gen_type_alias[type_double_width];
	string union_name = lf_gen_type_dcas_name(type, false);
	string_init(&s, 256);

	string_append_raw(&s, "\t return ", 0);
	string_append_raw(&s, tdw_cas_ns, 0);
	string_append_raw(&s, tdw_alias, 0);
	string_append_raw(&s, "(&p->scalar, &val_old->scalar, val_new.scalar);",
			  0);

	string_destroy(&union_name);
	return s;
}

static string generate_dcas_impl(enum lf_gen_type type)
{
	enum lf_gen_type type_double_width =
		lf_gen_x64_type_to_double_width_type[type];
	/* Use a normal CAS if double width type natively exists */
	if (type_double_width != LF_GEN_TYPE_COUNT) {
		return generate_dcas_from_cas_impl(type, type_double_width);
	}
	static const char *asm_dcas16b_x64 =
		"'lock cmpxchg16b %0'\n"
		": '+m'(*p->tuple), '=@ccz'(zf),\n"
		"  '+a'(val_old.tuple[0]),\n"
		"  '+d'(val_old.tuple[1])\n"
		": 'b'(val_new.tuple[0]), 'c'(val_new.tuple[1])\n"
		": 'memory', 'cc'";
	/* Use lock cmpxchg16b */
	string s;
	string_init(&s, 256);
	lf_gen_declare_var_str_type(&s, "bool", "zf", 1);
	append_inline_asm(&s, type, asm_dcas16b_x64, true);
	lf_gen_return(&s, "zf", 1);
	return s;
}

static string generate_dcasx_impl(enum lf_gen_type type)
{
	enum lf_gen_type type_double_width =
		lf_gen_x64_type_to_double_width_type[type];
	/* Use a normal CAS if double width type natively exists */
	if (type_double_width != LF_GEN_TYPE_COUNT) {
		return generate_dcasx_from_casx_impl(type, type_double_width);
	}
	static const char *asm_dcasx16b_x64 =
		"'lock cmpxchg16b %0'\n"
		": '+m'(*p->tuple), '=@ccz'(zf),\n"
		"  '+a'(val_old->tuple[0]),\n"
		"  '+d'(val_old->tuple[1])\n"
		": 'b'(val_new.tuple[0]), 'c'(val_new.tuple[1])\n"
		": 'memory', 'cc'";
	/* Use lock cmpxchg16b */
	string s;
	string_init(&s, 256);
	lf_gen_declare_var_str_type(&s, "bool", "zf", 1);
	append_inline_asm(&s, type, asm_dcasx16b_x64, true);
	lf_gen_return(&s, "zf", 1);
	return s;
}

static void generate_dcas_packed_types(void)
{
	static const char *dcas_packed_types_comment =
		"I was (and still am) unsure what the best approach to DCAS's interface is. I grappled between "
		"uint32_t *, uint32_t[2], and a union of uint64_t & uint32_t[2] (using dcas_u32 as an example). "
		"In the end, I decide on the union approach for a few reasons:\n"
		"- It bakes alignment into the type. You can't accidentally pass a uint32_t *\n"
		"  that isn't 8 byte aligned.\n"
		"- It makes returning the original value from casx simpler.\n"
		"- Using a union allows us to following strict aliasing rules without using\n"
		"  the __may_alias__ builtins.";

	string comment = lf_gen_format_comment(dcas_packed_types_comment, 0, 8,
					       false, false);
	output(comment.buffer);
	string_destroy(&comment);

	enum lf_gen_type type;
	types_for_each(type) {
		string typedef_line = lf_gen_type_dcas_typedef(type);
		output(typedef_line.buffer);
		string_destroy(&typedef_line);
	}
	output("\n");

	types_for_each(type) {
		enum lf_gen_type type_double_width =
			lf_gen_x64_type_to_double_width_type[type];
		size_t type_double_alignment =
			lf_gen_x64_type_to_alignment[type] * 2;
		string type_definition = lf_gen_type_dcas_define(
			type, type_double_width, type_double_alignment);
		output(type_definition.buffer);
		string_destroy(&type_definition);
	}
	output("\n");
}

static void generate_fences(void)
{
	enum lf_gen_func_fence fence;
	static const char *fences_comment =
		"x86_64 uses a memory model called TSO (Total Store Ordering). In essence, all "
		"loads and stores are ordered except for store->load. The CPU is free to perform "
		"the store after the load in this case. This only applies to the ordering at runtime, "
		"however. The compiler can reorder load and store instructions at compile time if "
		"there is no fence present.\n\n"
		"Each of the lf_fence_*_force() operations implement the underlying fence instruction "
		"for that operation. I wouldn't recommend using these unless you really need an mfence, "
		"sfence, or lfence for some reason.\n\n"
		"It is also noteworthy that lf_fence_full() does not use mfence. Instead, it uses an "
		"atomic operation on the stack pointer. This seems very strange, but some people say "
		"atomic operations are faster than a mfence as long as the dest is in the L1D. rsp is "
		"almost guaranteed to be in the L1D. Atomic operations act as an mfence for all normal "
		"memory operations (mfence also serializes non-temporal accesses and instructions like "
		"rdtsc).";
	lf_gen_module_comment(fences_comment);
	fence_all_for_each(fence) {
		string s = lf_gen_func_fence_define(
			fence, lf_gen_func_fence_impl[fence]);
		output(s.buffer);
		string_destroy(&s);
	}
}

static void generate_loads(void)
{
	lf_gen_all_impl_from_func(generate_load_impl, lf_gen_func_load_define);
}

static void generate_stores(void)
{
	lf_gen_all_impl_from_func(generate_store_impl,
				  lf_gen_func_store_define);
}

static void generate_swaps(void)
{
	lf_gen_all_impl_from_func(generate_swap_impl, lf_gen_func_swap_define);
}

static void generate_cas(void)
{
	const char *cas_comment =
		"lf_op_cas and lf_op_cax use the cmpxchg instruction to perform an atomic CAS. "
		"Both functions return a boolean that indicate if the CAS was successful. "
		"The only difference is lf_op_casx takes a pointer to val_old and loads p's "
		"actual value into val_old on failure.\n\n"
		"I just want to make a note of how this works so I don't forget in the future. "
		"cmpxchg uses the a register (rax) as val_old and places p's orignal value in "
		"the a register on failure. This is why we can simply return val_old in casx: "
		"the a register (val_old) will contain the original value on success or failure.";
	lf_gen_module_comment(cas_comment);
	lf_gen_all_impl_from_func(generate_cas_impl, lf_gen_func_cas_define);
	lf_gen_all_impl_from_func(generate_casx_impl, lf_gen_func_casx_define);
}

static void generate_faadds_fasubs(void)
{
	lf_gen_integral_impl_from_func(LF_GEN_FUNC_FAADD,
				       generate_faadd_fasub_impl,
				       lf_gen_func_faop_define);
	lf_gen_integral_impl_from_func(LF_GEN_FUNC_FASUB,
				       generate_faadd_fasub_impl,
				       lf_gen_func_faop_define);
	lf_gen_integral_impl_from_func(LF_GEN_FUNC_FAINC,
				       generate_faadd_fasub_impl,
				       lf_gen_func_fainc_fadec_define);
	lf_gen_integral_impl_from_func(LF_GEN_FUNC_FADEC,
				       generate_faadd_fasub_impl,
				       lf_gen_func_fainc_fadec_define);
}

static void generate_faand_faor_faxor(void)
{
	static const char *fabitops_comment =
		"Unfortunately x86_64 does not have an XADD like instruction for bitwise "
		"AND, OR, or XOR. Therefore, we have to implement the fetch-and versions "
		"of AND, OR, and XOR as CAS loops. Consider using the BTS, BTR, and BTC "
		"functions if you only need to modify a single bit.";
	lf_gen_module_comment(fabitops_comment);
	lf_gen_integral_impl_from_func(LF_GEN_FUNC_FAAND,
				       generate_faand_faor_faxor_impl,
				       lf_gen_func_faop_define);
	lf_gen_integral_impl_from_func(LF_GEN_FUNC_FAOR,
				       generate_faand_faor_faxor_impl,
				       lf_gen_func_faop_define);
	lf_gen_integral_impl_from_func(LF_GEN_FUNC_FAXOR,
				       generate_faand_faor_faxor_impl,
				       lf_gen_func_faop_define);
}

static void generate_adds_subs(void)
{
	lf_gen_integral_impl_from_func(LF_GEN_FUNC_ADD, generate_add_sub_impl,
				       lf_gen_func_op_define);
	lf_gen_integral_impl_from_func(LF_GEN_FUNC_SUB, generate_add_sub_impl,
				       lf_gen_func_op_define);
	lf_gen_integral_impl_from_func(LF_GEN_FUNC_INC, generate_add_sub_impl,
				       lf_gen_func_inc_dec_define);
	lf_gen_integral_impl_from_func(LF_GEN_FUNC_DEC, generate_add_sub_impl,
				       lf_gen_func_inc_dec_define);
}

static void generate_and_or_xor(void)
{
	lf_gen_integral_impl_from_func(LF_GEN_FUNC_AND,
				       generate_and_or_xor_impl,
				       lf_gen_func_op_define);
	lf_gen_integral_impl_from_func(LF_GEN_FUNC_OR, generate_and_or_xor_impl,
				       lf_gen_func_op_define);
	lf_gen_integral_impl_from_func(LF_GEN_FUNC_XOR,
				       generate_and_or_xor_impl,
				       lf_gen_func_op_define);
}

static void generate_btops(void)
{
	lf_gen_integral_impl_from_func(LF_GEN_FUNC_BTS, generate_btops_impl,
				       lf_gen_func_bitop_define);
	lf_gen_integral_impl_from_func(LF_GEN_FUNC_BTC, generate_btops_impl,
				       lf_gen_func_bitop_define);
	lf_gen_integral_impl_from_func(LF_GEN_FUNC_BTR, generate_btops_impl,
				       lf_gen_func_bitop_define);
}

static void generate_dcas(void)
{
	generate_dcas_packed_types();
	lf_gen_all_impl_from_func(generate_dcas_impl, lf_gen_func_dcas_define);
	lf_gen_all_impl_from_func(generate_dcasx_impl,
				  lf_gen_func_dcasx_define);
}

static void generate_op(void)
{
	static const char *module_comment_op =
		"lf_op_x64.h implements many atomic and lock-free related functions in "
		"x86 assembly, specifically x86_64 assembly. You should not include this file directly, but "
		"you can if you want to use the x64 functions directly. If you want to let liblf "
		"decide, include lf_op.h instead.";
	lf_gen_header_output("liblf/tools/gen/lf_gen_x64.c", module_comment_op);
	lf_gen_header_guard(guard_op, true);
	output(includes);

	/* Normal stuff */
	generate_fences();
	generate_loads();
	generate_stores();
	generate_swaps();
	generate_cas();
	generate_faadds_fasubs();
	generate_faand_faor_faxor();
	generate_adds_subs();
	generate_and_or_xor();
	generate_btops();

	/* Double width CAS */
	generate_dcas();

	lf_gen_header_guard(guard_op, false);
}

static void generate_op_sup(void)
{
	static const char *module_comment_op_sup =
		"lf_op_x64_sup.h defines macros for each type and operation. If the macro evaluates to "
		"0, that operation is not supported on that type for x64. If the macro evaluates to 1, that "
		"operation is supported on that type for x64. This header is important if you plan to target "
		"multiple architectures because not all architectures support operations on all types. For exmaple, "
		"x64 defines dcas on uint64_t but x86 does not. You should "
		"use these macros if you suspect some architectures you suppport will not implement certain operations.";
	lf_gen_header_output("liblf/tools/gen/lf_gen_x64.c",
			     module_comment_op_sup);
	lf_gen_header_guard(guard_op_sup, true);

	enum lf_gen_func_category func;
	enum lf_gen_type type;
	funcs_for_each(func) {
		bool integral_only_func = lf_gen_func_is_integral_only(func);
		types_for_each(type) {
			bool supported = !integral_only_func ||
					 type != LF_GEN_TYPE_PTR;
			lf_gen_output_op_sup_macro(type, func, supported);
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
