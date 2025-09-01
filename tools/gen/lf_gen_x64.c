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
#include <string.h>

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

enum lf_gen_x64_instruction {
	LF_GEN_X64_MOV = 0,
	LF_GEN_X64_XCHG,
	LF_GEN_X64_CMPXCHG,
	LF_GEN_X64_COUNT,
};

static const char *lf_gen_x64_instruction_names[] = {
	[LF_GEN_X64_MOV] = "mov",
	[LF_GEN_X64_XCHG] = "xchg",
	[LF_GEN_X64_CMPXCHG] = "cmpxchg",
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

static const char lf_gen_x64_size_to_suffix[] = {
	[1] = 'b',
	[2] = 'w',
	[4] = 'l',
	[8] = 'q',
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

	string_append_raw(s, "\t" ASM "(", ASM_LEN + 2);

	/* We need to append suffixes (b, w, l, q) to the instructions and
	 * replace apostrophes with quotes.
	 */
	unsigned long i = 0;
	unsigned long start = 0;
	for (; i < strlen(body); ++i) {
		unsigned long word_len;
		bool is_instruction;
		switch (body[i]) {
		case '\'':
			if (i != start) {
				string_append_raw(s, &body[start], i - start);
			}
			string_append_raw(s, "\"", 1);
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
	append_inline_asm(&s, type, asm_load_x64, true);
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

	append_inline_asm(&s, type, asm_store_x64, false);

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

	append_inline_asm(&s, type, asm_swap_x64, true);
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
	string_append_raw(&s, "\tbool zf;\n", 0);
	append_inline_asm(&s, type, asm_cas_x64, true);
	/* Return the zero flag*/
	lf_gen_return(&s, "zf", 1);

	return s;
}

static string generate_casx_impl(enum lf_gen_type type)
{
	string s;
	string_init(&s, 128);

	static const char *inline_asm_body = "'lock cmpxchg %2, %0'\n"
					     ": '+m'(*p), '+a'(val_old)\n"
					     ": 'r'(val_new)\n"
					     ": 'memory', 'cc'";

	append_inline_asm(&s, type, inline_asm_body, true);
	/* Return the original value of p */
	lf_gen_return(&s, "val_old", 1);

	return s;
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
		"lf_op_cas returns a boolean that indicates if the CAS was successful, and "
		"lf_op_casx returns the original value in p (val_old if successful).\n\n"
		"I just want to make a note of how this works so I don't forget in the future. "
		"cmpxchg uses the a register (rax) as val_old and places p's orignal value in "
		"the a register on failure. This is why we can simply return val_old in casx: "
		"the a register (val_old) will contain the original value on success or failure.";
	lf_gen_module_comment(cas_comment);
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
