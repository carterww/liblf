/* SPDX-License-Identifier: MIT
 * Copyright (c) 2025 Carter Williams
 */

/* This file was automatically generated. DO NOT MODIFY IT DIRECTLY.
 *
 * Date:      2025-08-31 22:29:05
 * Generator: liblf/tools/gen/lf_gen_gcc.c
 * Version:   v0.1.0
 *
 * Any changes made to this file will be overwritten when the generator runs
 * again.
 *
 * lf_op_gcc.h implements many atomic and lock-free related functions using only
 * GCC __atomic intrinsics. You should not include this file directly, but you can
 * if you want to use the GCC intrinsics directly. If you want to let liblf decide,
 * include lf_op.h instead.
 */

#ifndef LF_OP_GCC_H
#define LF_OP_GCC_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <lf_cc.h>
#include <lf_platform.h>

LF_ATTR_ALWAYS_INLINE
static void lf_fence_cc(void)
{
	__asm__ __volatile__("" ::: "memory");
}

LF_ATTR_ALWAYS_INLINE
static void lf_fence_full_force(void)
{
	__atomic_thread_fence(__ATOMIC_SEQ_CST);
}

LF_ATTR_ALWAYS_INLINE
static void lf_fence_store_force(void)
{
	__atomic_thread_fence(__ATOMIC_RELEASE);
}

LF_ATTR_ALWAYS_INLINE
static void lf_fence_load_force(void)
{
	__atomic_thread_fence(__ATOMIC_ACQUIRE);
}

#if defined(LF_MEM_TSO)

LF_ATTR_ALWAYS_INLINE
static void lf_fence_full(void)
{
	lf_fence_full_force();
}

LF_ATTR_ALWAYS_INLINE
static void lf_fence_store(void)
{
	lf_fence_cc();
}

LF_ATTR_ALWAYS_INLINE
static void lf_fence_load(void)
{
	lf_fence_cc();
}

LF_ATTR_ALWAYS_INLINE
static void lf_fence_store_load(void)
{
	lf_fence_full_force();
}

LF_ATTR_ALWAYS_INLINE
static void lf_fence_load_store(void)
{
	lf_fence_cc();
}

LF_ATTR_ALWAYS_INLINE
static void lf_fence_atomic(void)
{
	lf_fence_cc();
}

LF_ATTR_ALWAYS_INLINE
static void lf_fence_atomic_load(void)
{
	lf_fence_cc();
}

LF_ATTR_ALWAYS_INLINE
static void lf_fence_load_atomic(void)
{
	lf_fence_cc();
}

LF_ATTR_ALWAYS_INLINE
static void lf_fence_atomic_store(void)
{
	lf_fence_cc();
}

LF_ATTR_ALWAYS_INLINE
static void lf_fence_store_atomic(void)
{
	lf_fence_cc();
}

#elif defined(LF_MEM_WMO)

LF_ATTR_ALWAYS_INLINE
static void lf_fence_full(void)
{
	lf_fence_full_force();
}

LF_ATTR_ALWAYS_INLINE
static void lf_fence_store(void)
{
	lf_fence_store_force();
}

LF_ATTR_ALWAYS_INLINE
static void lf_fence_load(void)
{
	lf_fence_load_force();
}

LF_ATTR_ALWAYS_INLINE
static void lf_fence_store_load(void)
{
	lf_fence_full_force();
}

LF_ATTR_ALWAYS_INLINE
static void lf_fence_load_store(void)
{
	lf_fence_full_force();
}

LF_ATTR_ALWAYS_INLINE
static void lf_fence_atomic(void)
{
	lf_fence_full_force();
}

LF_ATTR_ALWAYS_INLINE
static void lf_fence_atomic_load(void)
{
	lf_fence_full_force();
}

LF_ATTR_ALWAYS_INLINE
static void lf_fence_load_atomic(void)
{
	lf_fence_full_force();
}

LF_ATTR_ALWAYS_INLINE
static void lf_fence_atomic_store(void)
{
	lf_fence_full_force();
}

LF_ATTR_ALWAYS_INLINE
static void lf_fence_store_atomic(void)
{
	lf_fence_full_force();
}

#else
#error Unsupported memory model detected
#endif /* LF_MEM_TSO or LF_MEM_WMO */

LF_ATTR_ALWAYS_INLINE
static void *lf_op_load_ptr(void **p)
{
	return __atomic_load_n(p, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static intptr_t lf_op_load_iptr(intptr_t *p)
{
	return __atomic_load_n(p, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static uintptr_t lf_op_load_uptr(uintptr_t *p)
{
	return __atomic_load_n(p, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static signed char lf_op_load_char(signed char *p)
{
	return __atomic_load_n(p, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static unsigned char lf_op_load_uchar(unsigned char *p)
{
	return __atomic_load_n(p, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static short lf_op_load_short(short *p)
{
	return __atomic_load_n(p, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static unsigned short lf_op_load_ushort(unsigned short *p)
{
	return __atomic_load_n(p, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static int lf_op_load_int(int *p)
{
	return __atomic_load_n(p, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static unsigned int lf_op_load_uint(unsigned int *p)
{
	return __atomic_load_n(p, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static long lf_op_load_long(long *p)
{
	return __atomic_load_n(p, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static unsigned long lf_op_load_ulong(unsigned long *p)
{
	return __atomic_load_n(p, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static int8_t lf_op_load_i8(int8_t *p)
{
	return __atomic_load_n(p, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static uint8_t lf_op_load_u8(uint8_t *p)
{
	return __atomic_load_n(p, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static int16_t lf_op_load_i16(int16_t *p)
{
	return __atomic_load_n(p, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static uint16_t lf_op_load_u16(uint16_t *p)
{
	return __atomic_load_n(p, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static int32_t lf_op_load_i32(int32_t *p)
{
	return __atomic_load_n(p, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static uint32_t lf_op_load_u32(uint32_t *p)
{
	return __atomic_load_n(p, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static int64_t lf_op_load_i64(int64_t *p)
{
	return __atomic_load_n(p, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static uint64_t lf_op_load_u64(uint64_t *p)
{
	return __atomic_load_n(p, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static size_t lf_op_load_size(size_t *p)
{
	return __atomic_load_n(p, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static lf_native_word lf_op_load_native(lf_native_word *p)
{
	return __atomic_load_n(p, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_store_ptr(void **p, void *val)
{
	__atomic_store_n(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_store_iptr(intptr_t *p, intptr_t val)
{
	__atomic_store_n(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_store_uptr(uintptr_t *p, uintptr_t val)
{
	__atomic_store_n(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_store_char(signed char *p, signed char val)
{
	__atomic_store_n(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_store_uchar(unsigned char *p, unsigned char val)
{
	__atomic_store_n(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_store_short(short *p, short val)
{
	__atomic_store_n(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_store_ushort(unsigned short *p, unsigned short val)
{
	__atomic_store_n(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_store_int(int *p, int val)
{
	__atomic_store_n(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_store_uint(unsigned int *p, unsigned int val)
{
	__atomic_store_n(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_store_long(long *p, long val)
{
	__atomic_store_n(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_store_ulong(unsigned long *p, unsigned long val)
{
	__atomic_store_n(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_store_i8(int8_t *p, int8_t val)
{
	__atomic_store_n(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_store_u8(uint8_t *p, uint8_t val)
{
	__atomic_store_n(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_store_i16(int16_t *p, int16_t val)
{
	__atomic_store_n(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_store_u16(uint16_t *p, uint16_t val)
{
	__atomic_store_n(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_store_i32(int32_t *p, int32_t val)
{
	__atomic_store_n(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_store_u32(uint32_t *p, uint32_t val)
{
	__atomic_store_n(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_store_i64(int64_t *p, int64_t val)
{
	__atomic_store_n(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_store_u64(uint64_t *p, uint64_t val)
{
	__atomic_store_n(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_store_size(size_t *p, size_t val)
{
	__atomic_store_n(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_store_native(lf_native_word *p, lf_native_word val)
{
	__atomic_store_n(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static void *lf_op_swap_ptr(void **p, void *val)
{
	return __atomic_exchange_n(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static intptr_t lf_op_swap_iptr(intptr_t *p, intptr_t val)
{
	return __atomic_exchange_n(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static uintptr_t lf_op_swap_uptr(uintptr_t *p, uintptr_t val)
{
	return __atomic_exchange_n(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static signed char lf_op_swap_char(signed char *p, signed char val)
{
	return __atomic_exchange_n(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static unsigned char lf_op_swap_uchar(unsigned char *p, unsigned char val)
{
	return __atomic_exchange_n(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static short lf_op_swap_short(short *p, short val)
{
	return __atomic_exchange_n(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static unsigned short lf_op_swap_ushort(unsigned short *p, unsigned short val)
{
	return __atomic_exchange_n(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static int lf_op_swap_int(int *p, int val)
{
	return __atomic_exchange_n(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static unsigned int lf_op_swap_uint(unsigned int *p, unsigned int val)
{
	return __atomic_exchange_n(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static long lf_op_swap_long(long *p, long val)
{
	return __atomic_exchange_n(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static unsigned long lf_op_swap_ulong(unsigned long *p, unsigned long val)
{
	return __atomic_exchange_n(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static int8_t lf_op_swap_i8(int8_t *p, int8_t val)
{
	return __atomic_exchange_n(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static uint8_t lf_op_swap_u8(uint8_t *p, uint8_t val)
{
	return __atomic_exchange_n(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static int16_t lf_op_swap_i16(int16_t *p, int16_t val)
{
	return __atomic_exchange_n(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static uint16_t lf_op_swap_u16(uint16_t *p, uint16_t val)
{
	return __atomic_exchange_n(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static int32_t lf_op_swap_i32(int32_t *p, int32_t val)
{
	return __atomic_exchange_n(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static uint32_t lf_op_swap_u32(uint32_t *p, uint32_t val)
{
	return __atomic_exchange_n(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static int64_t lf_op_swap_i64(int64_t *p, int64_t val)
{
	return __atomic_exchange_n(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static uint64_t lf_op_swap_u64(uint64_t *p, uint64_t val)
{
	return __atomic_exchange_n(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static size_t lf_op_swap_size(size_t *p, size_t val)
{
	return __atomic_exchange_n(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static lf_native_word lf_op_swap_native(lf_native_word *p, lf_native_word val)
{
	return __atomic_exchange_n(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_cas_ptr(void **p, void *val_old, void *val_new)
{
	return __atomic_compare_exchange_n(p, &val_old, val_new, true,
					   __ATOMIC_RELAXED, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_cas_iptr(intptr_t *p, intptr_t val_old, intptr_t val_new)
{
	return __atomic_compare_exchange_n(p, &val_old, val_new, true,
					   __ATOMIC_RELAXED, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_cas_uptr(uintptr_t *p, uintptr_t val_old, uintptr_t val_new)
{
	return __atomic_compare_exchange_n(p, &val_old, val_new, true,
					   __ATOMIC_RELAXED, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_cas_char(signed char *p, signed char val_old,
			   signed char val_new)
{
	return __atomic_compare_exchange_n(p, &val_old, val_new, true,
					   __ATOMIC_RELAXED, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_cas_uchar(unsigned char *p, unsigned char val_old,
			    unsigned char val_new)
{
	return __atomic_compare_exchange_n(p, &val_old, val_new, true,
					   __ATOMIC_RELAXED, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_cas_short(short *p, short val_old, short val_new)
{
	return __atomic_compare_exchange_n(p, &val_old, val_new, true,
					   __ATOMIC_RELAXED, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_cas_ushort(unsigned short *p, unsigned short val_old,
			     unsigned short val_new)
{
	return __atomic_compare_exchange_n(p, &val_old, val_new, true,
					   __ATOMIC_RELAXED, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_cas_int(int *p, int val_old, int val_new)
{
	return __atomic_compare_exchange_n(p, &val_old, val_new, true,
					   __ATOMIC_RELAXED, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_cas_uint(unsigned int *p, unsigned int val_old,
			   unsigned int val_new)
{
	return __atomic_compare_exchange_n(p, &val_old, val_new, true,
					   __ATOMIC_RELAXED, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_cas_long(long *p, long val_old, long val_new)
{
	return __atomic_compare_exchange_n(p, &val_old, val_new, true,
					   __ATOMIC_RELAXED, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_cas_ulong(unsigned long *p, unsigned long val_old,
			    unsigned long val_new)
{
	return __atomic_compare_exchange_n(p, &val_old, val_new, true,
					   __ATOMIC_RELAXED, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_cas_i8(int8_t *p, int8_t val_old, int8_t val_new)
{
	return __atomic_compare_exchange_n(p, &val_old, val_new, true,
					   __ATOMIC_RELAXED, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_cas_u8(uint8_t *p, uint8_t val_old, uint8_t val_new)
{
	return __atomic_compare_exchange_n(p, &val_old, val_new, true,
					   __ATOMIC_RELAXED, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_cas_i16(int16_t *p, int16_t val_old, int16_t val_new)
{
	return __atomic_compare_exchange_n(p, &val_old, val_new, true,
					   __ATOMIC_RELAXED, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_cas_u16(uint16_t *p, uint16_t val_old, uint16_t val_new)
{
	return __atomic_compare_exchange_n(p, &val_old, val_new, true,
					   __ATOMIC_RELAXED, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_cas_i32(int32_t *p, int32_t val_old, int32_t val_new)
{
	return __atomic_compare_exchange_n(p, &val_old, val_new, true,
					   __ATOMIC_RELAXED, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_cas_u32(uint32_t *p, uint32_t val_old, uint32_t val_new)
{
	return __atomic_compare_exchange_n(p, &val_old, val_new, true,
					   __ATOMIC_RELAXED, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_cas_i64(int64_t *p, int64_t val_old, int64_t val_new)
{
	return __atomic_compare_exchange_n(p, &val_old, val_new, true,
					   __ATOMIC_RELAXED, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_cas_u64(uint64_t *p, uint64_t val_old, uint64_t val_new)
{
	return __atomic_compare_exchange_n(p, &val_old, val_new, true,
					   __ATOMIC_RELAXED, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_cas_size(size_t *p, size_t val_old, size_t val_new)
{
	return __atomic_compare_exchange_n(p, &val_old, val_new, true,
					   __ATOMIC_RELAXED, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_cas_native(lf_native_word *p, lf_native_word val_old,
			     lf_native_word val_new)
{
	return __atomic_compare_exchange_n(p, &val_old, val_new, true,
					   __ATOMIC_RELAXED, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static void *lf_op_casx_ptr(void **p, void *val_old, void *val_new)
{
	(void)__atomic_compare_exchange_n(p, &val_old, val_new, true,
					  __ATOMIC_RELAXED, __ATOMIC_RELAXED);
	return val_old;
}

LF_ATTR_ALWAYS_INLINE
static intptr_t lf_op_casx_iptr(intptr_t *p, intptr_t val_old, intptr_t val_new)
{
	(void)__atomic_compare_exchange_n(p, &val_old, val_new, true,
					  __ATOMIC_RELAXED, __ATOMIC_RELAXED);
	return val_old;
}

LF_ATTR_ALWAYS_INLINE
static uintptr_t lf_op_casx_uptr(uintptr_t *p, uintptr_t val_old,
				 uintptr_t val_new)
{
	(void)__atomic_compare_exchange_n(p, &val_old, val_new, true,
					  __ATOMIC_RELAXED, __ATOMIC_RELAXED);
	return val_old;
}

LF_ATTR_ALWAYS_INLINE
static signed char lf_op_casx_char(signed char *p, signed char val_old,
				   signed char val_new)
{
	(void)__atomic_compare_exchange_n(p, &val_old, val_new, true,
					  __ATOMIC_RELAXED, __ATOMIC_RELAXED);
	return val_old;
}

LF_ATTR_ALWAYS_INLINE
static unsigned char lf_op_casx_uchar(unsigned char *p, unsigned char val_old,
				      unsigned char val_new)
{
	(void)__atomic_compare_exchange_n(p, &val_old, val_new, true,
					  __ATOMIC_RELAXED, __ATOMIC_RELAXED);
	return val_old;
}

LF_ATTR_ALWAYS_INLINE
static short lf_op_casx_short(short *p, short val_old, short val_new)
{
	(void)__atomic_compare_exchange_n(p, &val_old, val_new, true,
					  __ATOMIC_RELAXED, __ATOMIC_RELAXED);
	return val_old;
}

LF_ATTR_ALWAYS_INLINE
static unsigned short lf_op_casx_ushort(unsigned short *p,
					unsigned short val_old,
					unsigned short val_new)
{
	(void)__atomic_compare_exchange_n(p, &val_old, val_new, true,
					  __ATOMIC_RELAXED, __ATOMIC_RELAXED);
	return val_old;
}

LF_ATTR_ALWAYS_INLINE
static int lf_op_casx_int(int *p, int val_old, int val_new)
{
	(void)__atomic_compare_exchange_n(p, &val_old, val_new, true,
					  __ATOMIC_RELAXED, __ATOMIC_RELAXED);
	return val_old;
}

LF_ATTR_ALWAYS_INLINE
static unsigned int lf_op_casx_uint(unsigned int *p, unsigned int val_old,
				    unsigned int val_new)
{
	(void)__atomic_compare_exchange_n(p, &val_old, val_new, true,
					  __ATOMIC_RELAXED, __ATOMIC_RELAXED);
	return val_old;
}

LF_ATTR_ALWAYS_INLINE
static long lf_op_casx_long(long *p, long val_old, long val_new)
{
	(void)__atomic_compare_exchange_n(p, &val_old, val_new, true,
					  __ATOMIC_RELAXED, __ATOMIC_RELAXED);
	return val_old;
}

LF_ATTR_ALWAYS_INLINE
static unsigned long lf_op_casx_ulong(unsigned long *p, unsigned long val_old,
				      unsigned long val_new)
{
	(void)__atomic_compare_exchange_n(p, &val_old, val_new, true,
					  __ATOMIC_RELAXED, __ATOMIC_RELAXED);
	return val_old;
}

LF_ATTR_ALWAYS_INLINE
static int8_t lf_op_casx_i8(int8_t *p, int8_t val_old, int8_t val_new)
{
	(void)__atomic_compare_exchange_n(p, &val_old, val_new, true,
					  __ATOMIC_RELAXED, __ATOMIC_RELAXED);
	return val_old;
}

LF_ATTR_ALWAYS_INLINE
static uint8_t lf_op_casx_u8(uint8_t *p, uint8_t val_old, uint8_t val_new)
{
	(void)__atomic_compare_exchange_n(p, &val_old, val_new, true,
					  __ATOMIC_RELAXED, __ATOMIC_RELAXED);
	return val_old;
}

LF_ATTR_ALWAYS_INLINE
static int16_t lf_op_casx_i16(int16_t *p, int16_t val_old, int16_t val_new)
{
	(void)__atomic_compare_exchange_n(p, &val_old, val_new, true,
					  __ATOMIC_RELAXED, __ATOMIC_RELAXED);
	return val_old;
}

LF_ATTR_ALWAYS_INLINE
static uint16_t lf_op_casx_u16(uint16_t *p, uint16_t val_old, uint16_t val_new)
{
	(void)__atomic_compare_exchange_n(p, &val_old, val_new, true,
					  __ATOMIC_RELAXED, __ATOMIC_RELAXED);
	return val_old;
}

LF_ATTR_ALWAYS_INLINE
static int32_t lf_op_casx_i32(int32_t *p, int32_t val_old, int32_t val_new)
{
	(void)__atomic_compare_exchange_n(p, &val_old, val_new, true,
					  __ATOMIC_RELAXED, __ATOMIC_RELAXED);
	return val_old;
}

LF_ATTR_ALWAYS_INLINE
static uint32_t lf_op_casx_u32(uint32_t *p, uint32_t val_old, uint32_t val_new)
{
	(void)__atomic_compare_exchange_n(p, &val_old, val_new, true,
					  __ATOMIC_RELAXED, __ATOMIC_RELAXED);
	return val_old;
}

LF_ATTR_ALWAYS_INLINE
static int64_t lf_op_casx_i64(int64_t *p, int64_t val_old, int64_t val_new)
{
	(void)__atomic_compare_exchange_n(p, &val_old, val_new, true,
					  __ATOMIC_RELAXED, __ATOMIC_RELAXED);
	return val_old;
}

LF_ATTR_ALWAYS_INLINE
static uint64_t lf_op_casx_u64(uint64_t *p, uint64_t val_old, uint64_t val_new)
{
	(void)__atomic_compare_exchange_n(p, &val_old, val_new, true,
					  __ATOMIC_RELAXED, __ATOMIC_RELAXED);
	return val_old;
}

LF_ATTR_ALWAYS_INLINE
static size_t lf_op_casx_size(size_t *p, size_t val_old, size_t val_new)
{
	(void)__atomic_compare_exchange_n(p, &val_old, val_new, true,
					  __ATOMIC_RELAXED, __ATOMIC_RELAXED);
	return val_old;
}

LF_ATTR_ALWAYS_INLINE
static lf_native_word lf_op_casx_native(lf_native_word *p,
					lf_native_word val_old,
					lf_native_word val_new)
{
	(void)__atomic_compare_exchange_n(p, &val_old, val_new, true,
					  __ATOMIC_RELAXED, __ATOMIC_RELAXED);
	return val_old;
}

LF_ATTR_ALWAYS_INLINE
static intptr_t lf_op_fainc_iptr(intptr_t *p)
{
	return __atomic_fetch_add(p, 1, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static uintptr_t lf_op_fainc_uptr(uintptr_t *p)
{
	return __atomic_fetch_add(p, 1, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static signed char lf_op_fainc_char(signed char *p)
{
	return __atomic_fetch_add(p, 1, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static unsigned char lf_op_fainc_uchar(unsigned char *p)
{
	return __atomic_fetch_add(p, 1, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static short lf_op_fainc_short(short *p)
{
	return __atomic_fetch_add(p, 1, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static unsigned short lf_op_fainc_ushort(unsigned short *p)
{
	return __atomic_fetch_add(p, 1, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static int lf_op_fainc_int(int *p)
{
	return __atomic_fetch_add(p, 1, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static unsigned int lf_op_fainc_uint(unsigned int *p)
{
	return __atomic_fetch_add(p, 1, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static long lf_op_fainc_long(long *p)
{
	return __atomic_fetch_add(p, 1, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static unsigned long lf_op_fainc_ulong(unsigned long *p)
{
	return __atomic_fetch_add(p, 1, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static int8_t lf_op_fainc_i8(int8_t *p)
{
	return __atomic_fetch_add(p, 1, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static uint8_t lf_op_fainc_u8(uint8_t *p)
{
	return __atomic_fetch_add(p, 1, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static int16_t lf_op_fainc_i16(int16_t *p)
{
	return __atomic_fetch_add(p, 1, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static uint16_t lf_op_fainc_u16(uint16_t *p)
{
	return __atomic_fetch_add(p, 1, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static int32_t lf_op_fainc_i32(int32_t *p)
{
	return __atomic_fetch_add(p, 1, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static uint32_t lf_op_fainc_u32(uint32_t *p)
{
	return __atomic_fetch_add(p, 1, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static int64_t lf_op_fainc_i64(int64_t *p)
{
	return __atomic_fetch_add(p, 1, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static uint64_t lf_op_fainc_u64(uint64_t *p)
{
	return __atomic_fetch_add(p, 1, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static size_t lf_op_fainc_size(size_t *p)
{
	return __atomic_fetch_add(p, 1, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static lf_native_word lf_op_fainc_native(lf_native_word *p)
{
	return __atomic_fetch_add(p, 1, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static intptr_t lf_op_fadec_iptr(intptr_t *p)
{
	return __atomic_fetch_sub(p, 1, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static uintptr_t lf_op_fadec_uptr(uintptr_t *p)
{
	return __atomic_fetch_sub(p, 1, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static signed char lf_op_fadec_char(signed char *p)
{
	return __atomic_fetch_sub(p, 1, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static unsigned char lf_op_fadec_uchar(unsigned char *p)
{
	return __atomic_fetch_sub(p, 1, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static short lf_op_fadec_short(short *p)
{
	return __atomic_fetch_sub(p, 1, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static unsigned short lf_op_fadec_ushort(unsigned short *p)
{
	return __atomic_fetch_sub(p, 1, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static int lf_op_fadec_int(int *p)
{
	return __atomic_fetch_sub(p, 1, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static unsigned int lf_op_fadec_uint(unsigned int *p)
{
	return __atomic_fetch_sub(p, 1, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static long lf_op_fadec_long(long *p)
{
	return __atomic_fetch_sub(p, 1, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static unsigned long lf_op_fadec_ulong(unsigned long *p)
{
	return __atomic_fetch_sub(p, 1, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static int8_t lf_op_fadec_i8(int8_t *p)
{
	return __atomic_fetch_sub(p, 1, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static uint8_t lf_op_fadec_u8(uint8_t *p)
{
	return __atomic_fetch_sub(p, 1, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static int16_t lf_op_fadec_i16(int16_t *p)
{
	return __atomic_fetch_sub(p, 1, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static uint16_t lf_op_fadec_u16(uint16_t *p)
{
	return __atomic_fetch_sub(p, 1, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static int32_t lf_op_fadec_i32(int32_t *p)
{
	return __atomic_fetch_sub(p, 1, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static uint32_t lf_op_fadec_u32(uint32_t *p)
{
	return __atomic_fetch_sub(p, 1, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static int64_t lf_op_fadec_i64(int64_t *p)
{
	return __atomic_fetch_sub(p, 1, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static uint64_t lf_op_fadec_u64(uint64_t *p)
{
	return __atomic_fetch_sub(p, 1, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static size_t lf_op_fadec_size(size_t *p)
{
	return __atomic_fetch_sub(p, 1, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static lf_native_word lf_op_fadec_native(lf_native_word *p)
{
	return __atomic_fetch_sub(p, 1, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static intptr_t lf_op_faadd_iptr(intptr_t *p, intptr_t val)
{
	return __atomic_fetch_add(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static uintptr_t lf_op_faadd_uptr(uintptr_t *p, uintptr_t val)
{
	return __atomic_fetch_add(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static signed char lf_op_faadd_char(signed char *p, signed char val)
{
	return __atomic_fetch_add(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static unsigned char lf_op_faadd_uchar(unsigned char *p, unsigned char val)
{
	return __atomic_fetch_add(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static short lf_op_faadd_short(short *p, short val)
{
	return __atomic_fetch_add(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static unsigned short lf_op_faadd_ushort(unsigned short *p, unsigned short val)
{
	return __atomic_fetch_add(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static int lf_op_faadd_int(int *p, int val)
{
	return __atomic_fetch_add(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static unsigned int lf_op_faadd_uint(unsigned int *p, unsigned int val)
{
	return __atomic_fetch_add(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static long lf_op_faadd_long(long *p, long val)
{
	return __atomic_fetch_add(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static unsigned long lf_op_faadd_ulong(unsigned long *p, unsigned long val)
{
	return __atomic_fetch_add(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static int8_t lf_op_faadd_i8(int8_t *p, int8_t val)
{
	return __atomic_fetch_add(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static uint8_t lf_op_faadd_u8(uint8_t *p, uint8_t val)
{
	return __atomic_fetch_add(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static int16_t lf_op_faadd_i16(int16_t *p, int16_t val)
{
	return __atomic_fetch_add(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static uint16_t lf_op_faadd_u16(uint16_t *p, uint16_t val)
{
	return __atomic_fetch_add(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static int32_t lf_op_faadd_i32(int32_t *p, int32_t val)
{
	return __atomic_fetch_add(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static uint32_t lf_op_faadd_u32(uint32_t *p, uint32_t val)
{
	return __atomic_fetch_add(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static int64_t lf_op_faadd_i64(int64_t *p, int64_t val)
{
	return __atomic_fetch_add(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static uint64_t lf_op_faadd_u64(uint64_t *p, uint64_t val)
{
	return __atomic_fetch_add(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static size_t lf_op_faadd_size(size_t *p, size_t val)
{
	return __atomic_fetch_add(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static lf_native_word lf_op_faadd_native(lf_native_word *p, lf_native_word val)
{
	return __atomic_fetch_add(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static intptr_t lf_op_fasub_iptr(intptr_t *p, intptr_t val)
{
	return __atomic_fetch_sub(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static uintptr_t lf_op_fasub_uptr(uintptr_t *p, uintptr_t val)
{
	return __atomic_fetch_sub(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static signed char lf_op_fasub_char(signed char *p, signed char val)
{
	return __atomic_fetch_sub(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static unsigned char lf_op_fasub_uchar(unsigned char *p, unsigned char val)
{
	return __atomic_fetch_sub(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static short lf_op_fasub_short(short *p, short val)
{
	return __atomic_fetch_sub(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static unsigned short lf_op_fasub_ushort(unsigned short *p, unsigned short val)
{
	return __atomic_fetch_sub(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static int lf_op_fasub_int(int *p, int val)
{
	return __atomic_fetch_sub(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static unsigned int lf_op_fasub_uint(unsigned int *p, unsigned int val)
{
	return __atomic_fetch_sub(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static long lf_op_fasub_long(long *p, long val)
{
	return __atomic_fetch_sub(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static unsigned long lf_op_fasub_ulong(unsigned long *p, unsigned long val)
{
	return __atomic_fetch_sub(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static int8_t lf_op_fasub_i8(int8_t *p, int8_t val)
{
	return __atomic_fetch_sub(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static uint8_t lf_op_fasub_u8(uint8_t *p, uint8_t val)
{
	return __atomic_fetch_sub(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static int16_t lf_op_fasub_i16(int16_t *p, int16_t val)
{
	return __atomic_fetch_sub(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static uint16_t lf_op_fasub_u16(uint16_t *p, uint16_t val)
{
	return __atomic_fetch_sub(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static int32_t lf_op_fasub_i32(int32_t *p, int32_t val)
{
	return __atomic_fetch_sub(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static uint32_t lf_op_fasub_u32(uint32_t *p, uint32_t val)
{
	return __atomic_fetch_sub(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static int64_t lf_op_fasub_i64(int64_t *p, int64_t val)
{
	return __atomic_fetch_sub(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static uint64_t lf_op_fasub_u64(uint64_t *p, uint64_t val)
{
	return __atomic_fetch_sub(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static size_t lf_op_fasub_size(size_t *p, size_t val)
{
	return __atomic_fetch_sub(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static lf_native_word lf_op_fasub_native(lf_native_word *p, lf_native_word val)
{
	return __atomic_fetch_sub(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static intptr_t lf_op_faand_iptr(intptr_t *p, intptr_t val)
{
	return __atomic_fetch_and(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static uintptr_t lf_op_faand_uptr(uintptr_t *p, uintptr_t val)
{
	return __atomic_fetch_and(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static signed char lf_op_faand_char(signed char *p, signed char val)
{
	return __atomic_fetch_and(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static unsigned char lf_op_faand_uchar(unsigned char *p, unsigned char val)
{
	return __atomic_fetch_and(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static short lf_op_faand_short(short *p, short val)
{
	return __atomic_fetch_and(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static unsigned short lf_op_faand_ushort(unsigned short *p, unsigned short val)
{
	return __atomic_fetch_and(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static int lf_op_faand_int(int *p, int val)
{
	return __atomic_fetch_and(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static unsigned int lf_op_faand_uint(unsigned int *p, unsigned int val)
{
	return __atomic_fetch_and(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static long lf_op_faand_long(long *p, long val)
{
	return __atomic_fetch_and(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static unsigned long lf_op_faand_ulong(unsigned long *p, unsigned long val)
{
	return __atomic_fetch_and(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static int8_t lf_op_faand_i8(int8_t *p, int8_t val)
{
	return __atomic_fetch_and(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static uint8_t lf_op_faand_u8(uint8_t *p, uint8_t val)
{
	return __atomic_fetch_and(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static int16_t lf_op_faand_i16(int16_t *p, int16_t val)
{
	return __atomic_fetch_and(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static uint16_t lf_op_faand_u16(uint16_t *p, uint16_t val)
{
	return __atomic_fetch_and(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static int32_t lf_op_faand_i32(int32_t *p, int32_t val)
{
	return __atomic_fetch_and(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static uint32_t lf_op_faand_u32(uint32_t *p, uint32_t val)
{
	return __atomic_fetch_and(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static int64_t lf_op_faand_i64(int64_t *p, int64_t val)
{
	return __atomic_fetch_and(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static uint64_t lf_op_faand_u64(uint64_t *p, uint64_t val)
{
	return __atomic_fetch_and(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static size_t lf_op_faand_size(size_t *p, size_t val)
{
	return __atomic_fetch_and(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static lf_native_word lf_op_faand_native(lf_native_word *p, lf_native_word val)
{
	return __atomic_fetch_and(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static intptr_t lf_op_faor_iptr(intptr_t *p, intptr_t val)
{
	return __atomic_fetch_or(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static uintptr_t lf_op_faor_uptr(uintptr_t *p, uintptr_t val)
{
	return __atomic_fetch_or(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static signed char lf_op_faor_char(signed char *p, signed char val)
{
	return __atomic_fetch_or(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static unsigned char lf_op_faor_uchar(unsigned char *p, unsigned char val)
{
	return __atomic_fetch_or(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static short lf_op_faor_short(short *p, short val)
{
	return __atomic_fetch_or(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static unsigned short lf_op_faor_ushort(unsigned short *p, unsigned short val)
{
	return __atomic_fetch_or(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static int lf_op_faor_int(int *p, int val)
{
	return __atomic_fetch_or(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static unsigned int lf_op_faor_uint(unsigned int *p, unsigned int val)
{
	return __atomic_fetch_or(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static long lf_op_faor_long(long *p, long val)
{
	return __atomic_fetch_or(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static unsigned long lf_op_faor_ulong(unsigned long *p, unsigned long val)
{
	return __atomic_fetch_or(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static int8_t lf_op_faor_i8(int8_t *p, int8_t val)
{
	return __atomic_fetch_or(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static uint8_t lf_op_faor_u8(uint8_t *p, uint8_t val)
{
	return __atomic_fetch_or(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static int16_t lf_op_faor_i16(int16_t *p, int16_t val)
{
	return __atomic_fetch_or(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static uint16_t lf_op_faor_u16(uint16_t *p, uint16_t val)
{
	return __atomic_fetch_or(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static int32_t lf_op_faor_i32(int32_t *p, int32_t val)
{
	return __atomic_fetch_or(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static uint32_t lf_op_faor_u32(uint32_t *p, uint32_t val)
{
	return __atomic_fetch_or(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static int64_t lf_op_faor_i64(int64_t *p, int64_t val)
{
	return __atomic_fetch_or(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static uint64_t lf_op_faor_u64(uint64_t *p, uint64_t val)
{
	return __atomic_fetch_or(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static size_t lf_op_faor_size(size_t *p, size_t val)
{
	return __atomic_fetch_or(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static lf_native_word lf_op_faor_native(lf_native_word *p, lf_native_word val)
{
	return __atomic_fetch_or(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static intptr_t lf_op_faxor_iptr(intptr_t *p, intptr_t val)
{
	return __atomic_fetch_xor(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static uintptr_t lf_op_faxor_uptr(uintptr_t *p, uintptr_t val)
{
	return __atomic_fetch_xor(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static signed char lf_op_faxor_char(signed char *p, signed char val)
{
	return __atomic_fetch_xor(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static unsigned char lf_op_faxor_uchar(unsigned char *p, unsigned char val)
{
	return __atomic_fetch_xor(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static short lf_op_faxor_short(short *p, short val)
{
	return __atomic_fetch_xor(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static unsigned short lf_op_faxor_ushort(unsigned short *p, unsigned short val)
{
	return __atomic_fetch_xor(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static int lf_op_faxor_int(int *p, int val)
{
	return __atomic_fetch_xor(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static unsigned int lf_op_faxor_uint(unsigned int *p, unsigned int val)
{
	return __atomic_fetch_xor(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static long lf_op_faxor_long(long *p, long val)
{
	return __atomic_fetch_xor(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static unsigned long lf_op_faxor_ulong(unsigned long *p, unsigned long val)
{
	return __atomic_fetch_xor(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static int8_t lf_op_faxor_i8(int8_t *p, int8_t val)
{
	return __atomic_fetch_xor(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static uint8_t lf_op_faxor_u8(uint8_t *p, uint8_t val)
{
	return __atomic_fetch_xor(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static int16_t lf_op_faxor_i16(int16_t *p, int16_t val)
{
	return __atomic_fetch_xor(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static uint16_t lf_op_faxor_u16(uint16_t *p, uint16_t val)
{
	return __atomic_fetch_xor(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static int32_t lf_op_faxor_i32(int32_t *p, int32_t val)
{
	return __atomic_fetch_xor(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static uint32_t lf_op_faxor_u32(uint32_t *p, uint32_t val)
{
	return __atomic_fetch_xor(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static int64_t lf_op_faxor_i64(int64_t *p, int64_t val)
{
	return __atomic_fetch_xor(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static uint64_t lf_op_faxor_u64(uint64_t *p, uint64_t val)
{
	return __atomic_fetch_xor(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static size_t lf_op_faxor_size(size_t *p, size_t val)
{
	return __atomic_fetch_xor(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static lf_native_word lf_op_faxor_native(lf_native_word *p, lf_native_word val)
{
	return __atomic_fetch_xor(p, val, __ATOMIC_RELAXED);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_inc_iptr(intptr_t *p)
{
	(void)lf_op_fainc_iptr(p);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_inc_uptr(uintptr_t *p)
{
	(void)lf_op_fainc_uptr(p);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_inc_char(signed char *p)
{
	(void)lf_op_fainc_char(p);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_inc_uchar(unsigned char *p)
{
	(void)lf_op_fainc_uchar(p);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_inc_short(short *p)
{
	(void)lf_op_fainc_short(p);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_inc_ushort(unsigned short *p)
{
	(void)lf_op_fainc_ushort(p);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_inc_int(int *p)
{
	(void)lf_op_fainc_int(p);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_inc_uint(unsigned int *p)
{
	(void)lf_op_fainc_uint(p);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_inc_long(long *p)
{
	(void)lf_op_fainc_long(p);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_inc_ulong(unsigned long *p)
{
	(void)lf_op_fainc_ulong(p);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_inc_i8(int8_t *p)
{
	(void)lf_op_fainc_i8(p);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_inc_u8(uint8_t *p)
{
	(void)lf_op_fainc_u8(p);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_inc_i16(int16_t *p)
{
	(void)lf_op_fainc_i16(p);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_inc_u16(uint16_t *p)
{
	(void)lf_op_fainc_u16(p);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_inc_i32(int32_t *p)
{
	(void)lf_op_fainc_i32(p);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_inc_u32(uint32_t *p)
{
	(void)lf_op_fainc_u32(p);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_inc_i64(int64_t *p)
{
	(void)lf_op_fainc_i64(p);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_inc_u64(uint64_t *p)
{
	(void)lf_op_fainc_u64(p);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_inc_size(size_t *p)
{
	(void)lf_op_fainc_size(p);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_inc_native(lf_native_word *p)
{
	(void)lf_op_fainc_native(p);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_dec_iptr(intptr_t *p)
{
	(void)lf_op_fadec_iptr(p);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_dec_uptr(uintptr_t *p)
{
	(void)lf_op_fadec_uptr(p);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_dec_char(signed char *p)
{
	(void)lf_op_fadec_char(p);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_dec_uchar(unsigned char *p)
{
	(void)lf_op_fadec_uchar(p);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_dec_short(short *p)
{
	(void)lf_op_fadec_short(p);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_dec_ushort(unsigned short *p)
{
	(void)lf_op_fadec_ushort(p);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_dec_int(int *p)
{
	(void)lf_op_fadec_int(p);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_dec_uint(unsigned int *p)
{
	(void)lf_op_fadec_uint(p);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_dec_long(long *p)
{
	(void)lf_op_fadec_long(p);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_dec_ulong(unsigned long *p)
{
	(void)lf_op_fadec_ulong(p);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_dec_i8(int8_t *p)
{
	(void)lf_op_fadec_i8(p);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_dec_u8(uint8_t *p)
{
	(void)lf_op_fadec_u8(p);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_dec_i16(int16_t *p)
{
	(void)lf_op_fadec_i16(p);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_dec_u16(uint16_t *p)
{
	(void)lf_op_fadec_u16(p);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_dec_i32(int32_t *p)
{
	(void)lf_op_fadec_i32(p);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_dec_u32(uint32_t *p)
{
	(void)lf_op_fadec_u32(p);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_dec_i64(int64_t *p)
{
	(void)lf_op_fadec_i64(p);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_dec_u64(uint64_t *p)
{
	(void)lf_op_fadec_u64(p);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_dec_size(size_t *p)
{
	(void)lf_op_fadec_size(p);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_dec_native(lf_native_word *p)
{
	(void)lf_op_fadec_native(p);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_add_iptr(intptr_t *p, intptr_t val)
{
	(void)lf_op_faadd_iptr(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_add_uptr(uintptr_t *p, uintptr_t val)
{
	(void)lf_op_faadd_uptr(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_add_char(signed char *p, signed char val)
{
	(void)lf_op_faadd_char(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_add_uchar(unsigned char *p, unsigned char val)
{
	(void)lf_op_faadd_uchar(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_add_short(short *p, short val)
{
	(void)lf_op_faadd_short(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_add_ushort(unsigned short *p, unsigned short val)
{
	(void)lf_op_faadd_ushort(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_add_int(int *p, int val)
{
	(void)lf_op_faadd_int(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_add_uint(unsigned int *p, unsigned int val)
{
	(void)lf_op_faadd_uint(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_add_long(long *p, long val)
{
	(void)lf_op_faadd_long(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_add_ulong(unsigned long *p, unsigned long val)
{
	(void)lf_op_faadd_ulong(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_add_i8(int8_t *p, int8_t val)
{
	(void)lf_op_faadd_i8(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_add_u8(uint8_t *p, uint8_t val)
{
	(void)lf_op_faadd_u8(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_add_i16(int16_t *p, int16_t val)
{
	(void)lf_op_faadd_i16(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_add_u16(uint16_t *p, uint16_t val)
{
	(void)lf_op_faadd_u16(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_add_i32(int32_t *p, int32_t val)
{
	(void)lf_op_faadd_i32(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_add_u32(uint32_t *p, uint32_t val)
{
	(void)lf_op_faadd_u32(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_add_i64(int64_t *p, int64_t val)
{
	(void)lf_op_faadd_i64(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_add_u64(uint64_t *p, uint64_t val)
{
	(void)lf_op_faadd_u64(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_add_size(size_t *p, size_t val)
{
	(void)lf_op_faadd_size(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_add_native(lf_native_word *p, lf_native_word val)
{
	(void)lf_op_faadd_native(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_sub_iptr(intptr_t *p, intptr_t val)
{
	(void)lf_op_fasub_iptr(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_sub_uptr(uintptr_t *p, uintptr_t val)
{
	(void)lf_op_fasub_uptr(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_sub_char(signed char *p, signed char val)
{
	(void)lf_op_fasub_char(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_sub_uchar(unsigned char *p, unsigned char val)
{
	(void)lf_op_fasub_uchar(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_sub_short(short *p, short val)
{
	(void)lf_op_fasub_short(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_sub_ushort(unsigned short *p, unsigned short val)
{
	(void)lf_op_fasub_ushort(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_sub_int(int *p, int val)
{
	(void)lf_op_fasub_int(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_sub_uint(unsigned int *p, unsigned int val)
{
	(void)lf_op_fasub_uint(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_sub_long(long *p, long val)
{
	(void)lf_op_fasub_long(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_sub_ulong(unsigned long *p, unsigned long val)
{
	(void)lf_op_fasub_ulong(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_sub_i8(int8_t *p, int8_t val)
{
	(void)lf_op_fasub_i8(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_sub_u8(uint8_t *p, uint8_t val)
{
	(void)lf_op_fasub_u8(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_sub_i16(int16_t *p, int16_t val)
{
	(void)lf_op_fasub_i16(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_sub_u16(uint16_t *p, uint16_t val)
{
	(void)lf_op_fasub_u16(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_sub_i32(int32_t *p, int32_t val)
{
	(void)lf_op_fasub_i32(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_sub_u32(uint32_t *p, uint32_t val)
{
	(void)lf_op_fasub_u32(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_sub_i64(int64_t *p, int64_t val)
{
	(void)lf_op_fasub_i64(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_sub_u64(uint64_t *p, uint64_t val)
{
	(void)lf_op_fasub_u64(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_sub_size(size_t *p, size_t val)
{
	(void)lf_op_fasub_size(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_sub_native(lf_native_word *p, lf_native_word val)
{
	(void)lf_op_fasub_native(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_and_iptr(intptr_t *p, intptr_t val)
{
	(void)lf_op_faand_iptr(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_and_uptr(uintptr_t *p, uintptr_t val)
{
	(void)lf_op_faand_uptr(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_and_char(signed char *p, signed char val)
{
	(void)lf_op_faand_char(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_and_uchar(unsigned char *p, unsigned char val)
{
	(void)lf_op_faand_uchar(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_and_short(short *p, short val)
{
	(void)lf_op_faand_short(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_and_ushort(unsigned short *p, unsigned short val)
{
	(void)lf_op_faand_ushort(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_and_int(int *p, int val)
{
	(void)lf_op_faand_int(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_and_uint(unsigned int *p, unsigned int val)
{
	(void)lf_op_faand_uint(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_and_long(long *p, long val)
{
	(void)lf_op_faand_long(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_and_ulong(unsigned long *p, unsigned long val)
{
	(void)lf_op_faand_ulong(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_and_i8(int8_t *p, int8_t val)
{
	(void)lf_op_faand_i8(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_and_u8(uint8_t *p, uint8_t val)
{
	(void)lf_op_faand_u8(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_and_i16(int16_t *p, int16_t val)
{
	(void)lf_op_faand_i16(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_and_u16(uint16_t *p, uint16_t val)
{
	(void)lf_op_faand_u16(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_and_i32(int32_t *p, int32_t val)
{
	(void)lf_op_faand_i32(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_and_u32(uint32_t *p, uint32_t val)
{
	(void)lf_op_faand_u32(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_and_i64(int64_t *p, int64_t val)
{
	(void)lf_op_faand_i64(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_and_u64(uint64_t *p, uint64_t val)
{
	(void)lf_op_faand_u64(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_and_size(size_t *p, size_t val)
{
	(void)lf_op_faand_size(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_and_native(lf_native_word *p, lf_native_word val)
{
	(void)lf_op_faand_native(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_or_iptr(intptr_t *p, intptr_t val)
{
	(void)lf_op_faor_iptr(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_or_uptr(uintptr_t *p, uintptr_t val)
{
	(void)lf_op_faor_uptr(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_or_char(signed char *p, signed char val)
{
	(void)lf_op_faor_char(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_or_uchar(unsigned char *p, unsigned char val)
{
	(void)lf_op_faor_uchar(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_or_short(short *p, short val)
{
	(void)lf_op_faor_short(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_or_ushort(unsigned short *p, unsigned short val)
{
	(void)lf_op_faor_ushort(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_or_int(int *p, int val)
{
	(void)lf_op_faor_int(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_or_uint(unsigned int *p, unsigned int val)
{
	(void)lf_op_faor_uint(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_or_long(long *p, long val)
{
	(void)lf_op_faor_long(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_or_ulong(unsigned long *p, unsigned long val)
{
	(void)lf_op_faor_ulong(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_or_i8(int8_t *p, int8_t val)
{
	(void)lf_op_faor_i8(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_or_u8(uint8_t *p, uint8_t val)
{
	(void)lf_op_faor_u8(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_or_i16(int16_t *p, int16_t val)
{
	(void)lf_op_faor_i16(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_or_u16(uint16_t *p, uint16_t val)
{
	(void)lf_op_faor_u16(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_or_i32(int32_t *p, int32_t val)
{
	(void)lf_op_faor_i32(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_or_u32(uint32_t *p, uint32_t val)
{
	(void)lf_op_faor_u32(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_or_i64(int64_t *p, int64_t val)
{
	(void)lf_op_faor_i64(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_or_u64(uint64_t *p, uint64_t val)
{
	(void)lf_op_faor_u64(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_or_size(size_t *p, size_t val)
{
	(void)lf_op_faor_size(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_or_native(lf_native_word *p, lf_native_word val)
{
	(void)lf_op_faor_native(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_xor_iptr(intptr_t *p, intptr_t val)
{
	(void)lf_op_faxor_iptr(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_xor_uptr(uintptr_t *p, uintptr_t val)
{
	(void)lf_op_faxor_uptr(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_xor_char(signed char *p, signed char val)
{
	(void)lf_op_faxor_char(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_xor_uchar(unsigned char *p, unsigned char val)
{
	(void)lf_op_faxor_uchar(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_xor_short(short *p, short val)
{
	(void)lf_op_faxor_short(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_xor_ushort(unsigned short *p, unsigned short val)
{
	(void)lf_op_faxor_ushort(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_xor_int(int *p, int val)
{
	(void)lf_op_faxor_int(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_xor_uint(unsigned int *p, unsigned int val)
{
	(void)lf_op_faxor_uint(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_xor_long(long *p, long val)
{
	(void)lf_op_faxor_long(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_xor_ulong(unsigned long *p, unsigned long val)
{
	(void)lf_op_faxor_ulong(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_xor_i8(int8_t *p, int8_t val)
{
	(void)lf_op_faxor_i8(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_xor_u8(uint8_t *p, uint8_t val)
{
	(void)lf_op_faxor_u8(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_xor_i16(int16_t *p, int16_t val)
{
	(void)lf_op_faxor_i16(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_xor_u16(uint16_t *p, uint16_t val)
{
	(void)lf_op_faxor_u16(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_xor_i32(int32_t *p, int32_t val)
{
	(void)lf_op_faxor_i32(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_xor_u32(uint32_t *p, uint32_t val)
{
	(void)lf_op_faxor_u32(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_xor_i64(int64_t *p, int64_t val)
{
	(void)lf_op_faxor_i64(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_xor_u64(uint64_t *p, uint64_t val)
{
	(void)lf_op_faxor_u64(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_xor_size(size_t *p, size_t val)
{
	(void)lf_op_faxor_size(p, val);
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_xor_native(lf_native_word *p, lf_native_word val)
{
	(void)lf_op_faxor_native(p, val);
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_bts_iptr(intptr_t *p, unsigned int index)
{
	intptr_t mask = (intptr_t)((intptr_t)1 << index);
	intptr_t old = __atomic_fetch_or(p, mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_bts_uptr(uintptr_t *p, unsigned int index)
{
	uintptr_t mask = (uintptr_t)((uintptr_t)1 << index);
	uintptr_t old = __atomic_fetch_or(p, mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_bts_char(signed char *p, unsigned int index)
{
	signed char mask = (signed char)((signed char)1 << index);
	signed char old = __atomic_fetch_or(p, mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_bts_uchar(unsigned char *p, unsigned int index)
{
	unsigned char mask = (unsigned char)((unsigned char)1 << index);
	unsigned char old = __atomic_fetch_or(p, mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_bts_short(short *p, unsigned int index)
{
	short mask = (short)((short)1 << index);
	short old = __atomic_fetch_or(p, mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_bts_ushort(unsigned short *p, unsigned int index)
{
	unsigned short mask = (unsigned short)((unsigned short)1 << index);
	unsigned short old = __atomic_fetch_or(p, mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_bts_int(int *p, unsigned int index)
{
	int mask = (int)((int)1 << index);
	int old = __atomic_fetch_or(p, mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_bts_uint(unsigned int *p, unsigned int index)
{
	unsigned int mask = (unsigned int)((unsigned int)1 << index);
	unsigned int old = __atomic_fetch_or(p, mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_bts_long(long *p, unsigned int index)
{
	long mask = (long)((long)1 << index);
	long old = __atomic_fetch_or(p, mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_bts_ulong(unsigned long *p, unsigned int index)
{
	unsigned long mask = (unsigned long)((unsigned long)1 << index);
	unsigned long old = __atomic_fetch_or(p, mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_bts_i8(int8_t *p, unsigned int index)
{
	int8_t mask = (int8_t)((int8_t)1 << index);
	int8_t old = __atomic_fetch_or(p, mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_bts_u8(uint8_t *p, unsigned int index)
{
	uint8_t mask = (uint8_t)((uint8_t)1 << index);
	uint8_t old = __atomic_fetch_or(p, mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_bts_i16(int16_t *p, unsigned int index)
{
	int16_t mask = (int16_t)((int16_t)1 << index);
	int16_t old = __atomic_fetch_or(p, mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_bts_u16(uint16_t *p, unsigned int index)
{
	uint16_t mask = (uint16_t)((uint16_t)1 << index);
	uint16_t old = __atomic_fetch_or(p, mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_bts_i32(int32_t *p, unsigned int index)
{
	int32_t mask = (int32_t)((int32_t)1 << index);
	int32_t old = __atomic_fetch_or(p, mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_bts_u32(uint32_t *p, unsigned int index)
{
	uint32_t mask = (uint32_t)((uint32_t)1 << index);
	uint32_t old = __atomic_fetch_or(p, mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_bts_i64(int64_t *p, unsigned int index)
{
	int64_t mask = (int64_t)((int64_t)1 << index);
	int64_t old = __atomic_fetch_or(p, mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_bts_u64(uint64_t *p, unsigned int index)
{
	uint64_t mask = (uint64_t)((uint64_t)1 << index);
	uint64_t old = __atomic_fetch_or(p, mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_bts_size(size_t *p, unsigned int index)
{
	size_t mask = (size_t)((size_t)1 << index);
	size_t old = __atomic_fetch_or(p, mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_bts_native(lf_native_word *p, unsigned int index)
{
	lf_native_word mask = (lf_native_word)((lf_native_word)1 << index);
	lf_native_word old = __atomic_fetch_or(p, mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btr_iptr(intptr_t *p, unsigned int index)
{
	intptr_t mask = (intptr_t)((intptr_t)1 << index);
	intptr_t old = __atomic_fetch_and(p, ~mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btr_uptr(uintptr_t *p, unsigned int index)
{
	uintptr_t mask = (uintptr_t)((uintptr_t)1 << index);
	uintptr_t old = __atomic_fetch_and(p, ~mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btr_char(signed char *p, unsigned int index)
{
	signed char mask = (signed char)((signed char)1 << index);
	signed char old = __atomic_fetch_and(p, ~mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btr_uchar(unsigned char *p, unsigned int index)
{
	unsigned char mask = (unsigned char)((unsigned char)1 << index);
	unsigned char old = __atomic_fetch_and(p, ~mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btr_short(short *p, unsigned int index)
{
	short mask = (short)((short)1 << index);
	short old = __atomic_fetch_and(p, ~mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btr_ushort(unsigned short *p, unsigned int index)
{
	unsigned short mask = (unsigned short)((unsigned short)1 << index);
	unsigned short old = __atomic_fetch_and(p, ~mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btr_int(int *p, unsigned int index)
{
	int mask = (int)((int)1 << index);
	int old = __atomic_fetch_and(p, ~mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btr_uint(unsigned int *p, unsigned int index)
{
	unsigned int mask = (unsigned int)((unsigned int)1 << index);
	unsigned int old = __atomic_fetch_and(p, ~mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btr_long(long *p, unsigned int index)
{
	long mask = (long)((long)1 << index);
	long old = __atomic_fetch_and(p, ~mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btr_ulong(unsigned long *p, unsigned int index)
{
	unsigned long mask = (unsigned long)((unsigned long)1 << index);
	unsigned long old = __atomic_fetch_and(p, ~mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btr_i8(int8_t *p, unsigned int index)
{
	int8_t mask = (int8_t)((int8_t)1 << index);
	int8_t old = __atomic_fetch_and(p, ~mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btr_u8(uint8_t *p, unsigned int index)
{
	uint8_t mask = (uint8_t)((uint8_t)1 << index);
	uint8_t old = __atomic_fetch_and(p, ~mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btr_i16(int16_t *p, unsigned int index)
{
	int16_t mask = (int16_t)((int16_t)1 << index);
	int16_t old = __atomic_fetch_and(p, ~mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btr_u16(uint16_t *p, unsigned int index)
{
	uint16_t mask = (uint16_t)((uint16_t)1 << index);
	uint16_t old = __atomic_fetch_and(p, ~mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btr_i32(int32_t *p, unsigned int index)
{
	int32_t mask = (int32_t)((int32_t)1 << index);
	int32_t old = __atomic_fetch_and(p, ~mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btr_u32(uint32_t *p, unsigned int index)
{
	uint32_t mask = (uint32_t)((uint32_t)1 << index);
	uint32_t old = __atomic_fetch_and(p, ~mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btr_i64(int64_t *p, unsigned int index)
{
	int64_t mask = (int64_t)((int64_t)1 << index);
	int64_t old = __atomic_fetch_and(p, ~mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btr_u64(uint64_t *p, unsigned int index)
{
	uint64_t mask = (uint64_t)((uint64_t)1 << index);
	uint64_t old = __atomic_fetch_and(p, ~mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btr_size(size_t *p, unsigned int index)
{
	size_t mask = (size_t)((size_t)1 << index);
	size_t old = __atomic_fetch_and(p, ~mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btr_native(lf_native_word *p, unsigned int index)
{
	lf_native_word mask = (lf_native_word)((lf_native_word)1 << index);
	lf_native_word old = __atomic_fetch_and(p, ~mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btc_iptr(intptr_t *p, unsigned int index)
{
	intptr_t mask = (intptr_t)((intptr_t)1 << index);
	intptr_t old = __atomic_fetch_xor(p, mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btc_uptr(uintptr_t *p, unsigned int index)
{
	uintptr_t mask = (uintptr_t)((uintptr_t)1 << index);
	uintptr_t old = __atomic_fetch_xor(p, mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btc_char(signed char *p, unsigned int index)
{
	signed char mask = (signed char)((signed char)1 << index);
	signed char old = __atomic_fetch_xor(p, mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btc_uchar(unsigned char *p, unsigned int index)
{
	unsigned char mask = (unsigned char)((unsigned char)1 << index);
	unsigned char old = __atomic_fetch_xor(p, mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btc_short(short *p, unsigned int index)
{
	short mask = (short)((short)1 << index);
	short old = __atomic_fetch_xor(p, mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btc_ushort(unsigned short *p, unsigned int index)
{
	unsigned short mask = (unsigned short)((unsigned short)1 << index);
	unsigned short old = __atomic_fetch_xor(p, mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btc_int(int *p, unsigned int index)
{
	int mask = (int)((int)1 << index);
	int old = __atomic_fetch_xor(p, mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btc_uint(unsigned int *p, unsigned int index)
{
	unsigned int mask = (unsigned int)((unsigned int)1 << index);
	unsigned int old = __atomic_fetch_xor(p, mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btc_long(long *p, unsigned int index)
{
	long mask = (long)((long)1 << index);
	long old = __atomic_fetch_xor(p, mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btc_ulong(unsigned long *p, unsigned int index)
{
	unsigned long mask = (unsigned long)((unsigned long)1 << index);
	unsigned long old = __atomic_fetch_xor(p, mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btc_i8(int8_t *p, unsigned int index)
{
	int8_t mask = (int8_t)((int8_t)1 << index);
	int8_t old = __atomic_fetch_xor(p, mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btc_u8(uint8_t *p, unsigned int index)
{
	uint8_t mask = (uint8_t)((uint8_t)1 << index);
	uint8_t old = __atomic_fetch_xor(p, mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btc_i16(int16_t *p, unsigned int index)
{
	int16_t mask = (int16_t)((int16_t)1 << index);
	int16_t old = __atomic_fetch_xor(p, mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btc_u16(uint16_t *p, unsigned int index)
{
	uint16_t mask = (uint16_t)((uint16_t)1 << index);
	uint16_t old = __atomic_fetch_xor(p, mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btc_i32(int32_t *p, unsigned int index)
{
	int32_t mask = (int32_t)((int32_t)1 << index);
	int32_t old = __atomic_fetch_xor(p, mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btc_u32(uint32_t *p, unsigned int index)
{
	uint32_t mask = (uint32_t)((uint32_t)1 << index);
	uint32_t old = __atomic_fetch_xor(p, mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btc_i64(int64_t *p, unsigned int index)
{
	int64_t mask = (int64_t)((int64_t)1 << index);
	int64_t old = __atomic_fetch_xor(p, mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btc_u64(uint64_t *p, unsigned int index)
{
	uint64_t mask = (uint64_t)((uint64_t)1 << index);
	uint64_t old = __atomic_fetch_xor(p, mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btc_size(size_t *p, unsigned int index)
{
	size_t mask = (size_t)((size_t)1 << index);
	size_t old = __atomic_fetch_xor(p, mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btc_native(lf_native_word *p, unsigned int index)
{
	lf_native_word mask = (lf_native_word)((lf_native_word)1 << index);
	lf_native_word old = __atomic_fetch_xor(p, mask, __ATOMIC_RELAXED);
	return (old & mask) != 0;
}

#endif /* LF_OP_GCC_H */
