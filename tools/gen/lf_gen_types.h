/* SPDX-License-Identifier: MIT
 * Copyright (c) 2025 Carter Williams
 */

#ifndef LF_GEN_TYPES_H
#define LF_GEN_TYPES_H

#include <stdio.h>
#include <string.h>

#include "lf_gen_string.h"

#define types_for_each(type_enum_var)         \
	for (type_enum_var = LF_GEN_TYPE_PTR; \
	     type_enum_var < LF_GEN_TYPE_COUNT; ++type_enum_var)

#define types_integral_for_each(type_enum_var)    \
	for (type_enum_var = LF_GEN_TYPE_PTR + 1; \
	     type_enum_var < LF_GEN_TYPE_COUNT; ++type_enum_var)

enum lf_gen_type {
	/* PTR should always be 0 */
	LF_GEN_TYPE_PTR = 0,
	LF_GEN_TYPE_IPTR,
	LF_GEN_TYPE_UPTR,
	LF_GEN_TYPE_CHAR,
	LF_GEN_TYPE_UCHAR,
	LF_GEN_TYPE_SHORT,
	LF_GEN_TYPE_USHORT,
	LF_GEN_TYPE_INT,
	LF_GEN_TYPE_UINT,
	LF_GEN_TYPE_LONG,
	LF_GEN_TYPE_ULONG,
	LF_GEN_TYPE_I8,
	LF_GEN_TYPE_U8,
	LF_GEN_TYPE_I16,
	LF_GEN_TYPE_U16,
	LF_GEN_TYPE_I32,
	LF_GEN_TYPE_U32,
	LF_GEN_TYPE_I64,
	LF_GEN_TYPE_U64,
	LF_GEN_TYPE_SIZE,
	LF_GEN_TYPE_NATIVE,
	LF_GEN_TYPE_COUNT
};

static const char *lf_gen_type_names[] = {
	[LF_GEN_TYPE_PTR] = "void *",
	[LF_GEN_TYPE_IPTR] = "intptr_t",
	[LF_GEN_TYPE_UPTR] = "uintptr_t",
	[LF_GEN_TYPE_CHAR] = "signed char",
	[LF_GEN_TYPE_UCHAR] = "unsigned char",
	[LF_GEN_TYPE_SHORT] = "short",
	[LF_GEN_TYPE_USHORT] = "unsigned short",
	[LF_GEN_TYPE_INT] = "int",
	[LF_GEN_TYPE_UINT] = "unsigned int",
	[LF_GEN_TYPE_LONG] = "long",
	[LF_GEN_TYPE_ULONG] = "unsigned long",
	[LF_GEN_TYPE_I8] = "int8_t",
	[LF_GEN_TYPE_U8] = "uint8_t",
	[LF_GEN_TYPE_I16] = "int16_t",
	[LF_GEN_TYPE_U16] = "uint16_t",
	[LF_GEN_TYPE_I32] = "int32_t",
	[LF_GEN_TYPE_U32] = "uint32_t",
	[LF_GEN_TYPE_I64] = "int64_t",
	[LF_GEN_TYPE_U64] = "uint64_t",
	[LF_GEN_TYPE_SIZE] = "size_t",
	[LF_GEN_TYPE_NATIVE] = "lf_native_word"
};

static const char *lf_gen_type_name_ptrs[] = {
	[LF_GEN_TYPE_PTR] = "void **",
	[LF_GEN_TYPE_IPTR] = "intptr_t *",
	[LF_GEN_TYPE_UPTR] = "uintptr_t *",
	[LF_GEN_TYPE_CHAR] = "signed char *",
	[LF_GEN_TYPE_UCHAR] = "unsigned char *",
	[LF_GEN_TYPE_SHORT] = "short *",
	[LF_GEN_TYPE_USHORT] = "unsigned short *",
	[LF_GEN_TYPE_INT] = "int *",
	[LF_GEN_TYPE_UINT] = "unsigned int *",
	[LF_GEN_TYPE_LONG] = "long *",
	[LF_GEN_TYPE_ULONG] = "unsigned long *",
	[LF_GEN_TYPE_I8] = "int8_t *",
	[LF_GEN_TYPE_U8] = "uint8_t *",
	[LF_GEN_TYPE_I16] = "int16_t *",
	[LF_GEN_TYPE_U16] = "uint16_t *",
	[LF_GEN_TYPE_I32] = "int32_t *",
	[LF_GEN_TYPE_U32] = "uint32_t *",
	[LF_GEN_TYPE_I64] = "int64_t *",
	[LF_GEN_TYPE_U64] = "uint64_t *",
	[LF_GEN_TYPE_SIZE] = "size_t *",
	[LF_GEN_TYPE_NATIVE] = "lf_native_word *"
};

static const char *lf_gen_type_alias[] = {
	[LF_GEN_TYPE_PTR] = "ptr",	 [LF_GEN_TYPE_IPTR] = "iptr",
	[LF_GEN_TYPE_UPTR] = "uptr",	 [LF_GEN_TYPE_CHAR] = "char",
	[LF_GEN_TYPE_UCHAR] = "uchar",	 [LF_GEN_TYPE_SHORT] = "short",
	[LF_GEN_TYPE_USHORT] = "ushort", [LF_GEN_TYPE_INT] = "int",
	[LF_GEN_TYPE_UINT] = "uint",	 [LF_GEN_TYPE_LONG] = "long",
	[LF_GEN_TYPE_ULONG] = "ulong",	 [LF_GEN_TYPE_I8] = "i8",
	[LF_GEN_TYPE_U8] = "u8",	 [LF_GEN_TYPE_I16] = "i16",
	[LF_GEN_TYPE_U16] = "u16",	 [LF_GEN_TYPE_I32] = "i32",
	[LF_GEN_TYPE_U32] = "u32",	 [LF_GEN_TYPE_I64] = "i64",
	[LF_GEN_TYPE_U64] = "u64",	 [LF_GEN_TYPE_SIZE] = "size",
	[LF_GEN_TYPE_NATIVE] = "native"
};

/* Min sizes of types across all supported architectures */
static const size_t lf_gen_type_min_sizes[] = {
	[LF_GEN_TYPE_PTR] = 4,	  [LF_GEN_TYPE_IPTR] = 4,
	[LF_GEN_TYPE_UPTR] = 4,	  [LF_GEN_TYPE_CHAR] = 1,
	[LF_GEN_TYPE_UCHAR] = 1,  [LF_GEN_TYPE_SHORT] = 2,
	[LF_GEN_TYPE_USHORT] = 2, [LF_GEN_TYPE_INT] = 2,
	[LF_GEN_TYPE_UINT] = 2,	  [LF_GEN_TYPE_LONG] = 4,
	[LF_GEN_TYPE_ULONG] = 4,  [LF_GEN_TYPE_I8] = 1,
	[LF_GEN_TYPE_U8] = 1,	  [LF_GEN_TYPE_I16] = 2,
	[LF_GEN_TYPE_U16] = 2,	  [LF_GEN_TYPE_I32] = 4,
	[LF_GEN_TYPE_U32] = 4,	  [LF_GEN_TYPE_I64] = 8,
	[LF_GEN_TYPE_U64] = 8,	  [LF_GEN_TYPE_SIZE] = 4,
	[LF_GEN_TYPE_NATIVE] = 4
};

/* Max sizes of types across all supported architectures */
static const size_t lf_gen_type_max_sizes[] = {
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
	[LF_GEN_TYPE_NATIVE] = 8
};

static bool lf_gen_type_is_native_width(enum lf_gen_type type)
{
	return type == LF_GEN_TYPE_PTR || type == LF_GEN_TYPE_IPTR ||
	       type == LF_GEN_TYPE_UPTR || type == LF_GEN_TYPE_SIZE ||
	       type == LF_GEN_TYPE_NATIVE;
}

static string lf_gen_type_dcas_name(enum lf_gen_type type, bool ptr)
{
	static const char *dcas_type_namespace = "lf_dcas_packed_";
	string s;
	const char *alias = lf_gen_type_alias[type];
	size_t alen = strlen(alias);
	size_t nlen = strlen(dcas_type_namespace);
	string_init(&s, alen + nlen + 3);
	string_append_raw(&s, dcas_type_namespace, nlen);
	string_append_raw(&s, alias, alen);
	if (ptr) {
		string_append_raw(&s, " *", 2);
	}
	return s;
}

static string lf_gen_type_dcas_typedef(enum lf_gen_type type)
{
	string s;
	string name = lf_gen_type_dcas_name(type, false);
	string_init(&s, 20 + name.len * 2);
	string_append_raw(&s, "typedef union ", 14);
	string_append(&s, &name);
	string_append_raw(&s, " ", 1);
	string_append(&s, &name);
	string_append_raw(&s, ";\n", 2);
	string_destroy(&name);
	return s;
}

static string lf_gen_type_dcas_define(enum lf_gen_type type,
				      enum lf_gen_type type_double_width,
				      size_t type_double_alignment)
{
	string s;
	char buf[128] = { 0 };
	string union_name = lf_gen_type_dcas_name(type, false);
	const char *raw_type_name = lf_gen_type_names[type];
	string_init(&s, 128);

	snprintf(buf, 128, "union %s {\n", union_name.buffer);
	string_append_raw(&s, buf, 0);

	if (type_double_width != LF_GEN_TYPE_COUNT) {
		snprintf(buf, 128, "\t%s%sscalar;\n",
			 lf_gen_type_names[type_double_width],
			 type_double_width == LF_GEN_TYPE_PTR ? "" : " ");
		string_append_raw(&s, buf, 0);
	}
	snprintf(buf, 128, "\t%s%stuple[2];\n", raw_type_name,
		 type_double_width == LF_GEN_TYPE_PTR ? "" : " ");
	string_append_raw(&s, buf, 0);
	snprintf(buf, 128, "} LF_ATTR_ALIGNED(%zu);\n", type_double_alignment);
	string_append_raw(&s, buf, 0);

	string_destroy(&union_name);
	return s;
}

#endif /* LF_GEN_TYPES_H */
