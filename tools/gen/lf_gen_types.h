/* SPDX-License-Identifier: MIT
 * Copyright (c) 2025 Carter Williams
 */

#ifndef LF_GEN_TYPES_H
#define LF_GEN_TYPES_H

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

#endif /* LF_GEN_TYPES_H */
