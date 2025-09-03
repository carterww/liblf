/* SPDX-License-Identifier: MIT
 * Copyright (c) 2025 Carter Williams
 */

/* This file was automatically generated. DO NOT MODIFY IT DIRECTLY.
 *
 * Date:      2025-09-02 23:01:44
 * Generator: liblf/tools/gen/lf_gen_x64.c
 * Version:   v0.1.0
 *
 * Any changes made to this file will be overwritten when the generator runs
 * again.
 *
 * lf_op_x64.h implements many atomic and lock-free related functions in x86 assembly,
 * specifically x86_64 assembly. You should not include this file directly, but you
 * can if you want to use the x64 functions directly. If you want to let liblf decide,
 * include lf_op.h instead.
 */

#ifndef LF_OP_X64_H
#define LF_OP_X64_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <lf_cc.h>
#include <lf_platform.h>

/* x86_64 uses a memory model called TSO (Total Store Ordering). In essence, all
 * loads and stores are ordered except for store->load. The CPU is free to perform
 * the store after the load in this case. This only applies to the ordering at runtime,
 * however. The compiler can reorder load and store instructions at compile time
 * if there is no fence present.
 * 
 * Each of the lf_fence_*_force() operations implement the underlying fence instruction
 * for that operation. I wouldn't recommend using these unless you really need an
 * mfence, sfence, or lfence for some reason.
 * 
 * It is also noteworthy that lf_fence_full() does not use mfence. Instead, it uses
 * an atomic operation on the stack pointer. This seems very strange, but some people
 * say atomic operations are faster than a mfence as long as the dest is in the L1D.
 * rsp is almost guaranteed to be in the L1D. Atomic operations act as an mfence
 * for all normal memory operations (mfence also serializes non-temporal accesses
 * and instructions like rdtsc).
 */

LF_ATTR_ALWAYS_INLINE
static void lf_fence_cc(void)
{
	__asm__ __volatile__("" ::: "memory");
}

LF_ATTR_ALWAYS_INLINE
static void lf_fence_full_force(void)
{
	__asm__ __volatile__("mfence" ::: "memory");
}

LF_ATTR_ALWAYS_INLINE
static void lf_fence_store_force(void)
{
	__asm__ __volatile__("sfence" ::: "memory");
}

LF_ATTR_ALWAYS_INLINE
static void lf_fence_load_force(void)
{
	__asm__ __volatile__("mfence" ::: "memory");
}

LF_ATTR_ALWAYS_INLINE
static void lf_fence_full(void)
{
	__asm__ __volatile__("lock orq $0x0, (%%rsp)" ::: "memory");
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
	lf_fence_full();
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

LF_ATTR_ALWAYS_INLINE
static void *lf_op_load_ptr(void *p)
{
	void *val;
	__asm__ __volatile__("movq %1, %0"
			     : "=r"(val)
			     : "m"(*(void **)p)
			     : "memory");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static intptr_t lf_op_load_iptr(intptr_t *p)
{
	intptr_t val;
	__asm__ __volatile__("movq %1, %0" : "=r"(val) : "m"(*p) : "memory");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static uintptr_t lf_op_load_uptr(uintptr_t *p)
{
	uintptr_t val;
	__asm__ __volatile__("movq %1, %0" : "=r"(val) : "m"(*p) : "memory");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static signed char lf_op_load_char(signed char *p)
{
	signed char val;
	__asm__ __volatile__("movb %1, %0" : "=r"(val) : "m"(*p) : "memory");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static unsigned char lf_op_load_uchar(unsigned char *p)
{
	unsigned char val;
	__asm__ __volatile__("movb %1, %0" : "=r"(val) : "m"(*p) : "memory");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static short lf_op_load_short(short *p)
{
	short val;
	__asm__ __volatile__("movw %1, %0" : "=r"(val) : "m"(*p) : "memory");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static unsigned short lf_op_load_ushort(unsigned short *p)
{
	unsigned short val;
	__asm__ __volatile__("movw %1, %0" : "=r"(val) : "m"(*p) : "memory");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static int lf_op_load_int(int *p)
{
	int val;
	__asm__ __volatile__("movl %1, %0" : "=r"(val) : "m"(*p) : "memory");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static unsigned int lf_op_load_uint(unsigned int *p)
{
	unsigned int val;
	__asm__ __volatile__("movl %1, %0" : "=r"(val) : "m"(*p) : "memory");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static long lf_op_load_long(long *p)
{
	long val;
	__asm__ __volatile__("movq %1, %0" : "=r"(val) : "m"(*p) : "memory");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static unsigned long lf_op_load_ulong(unsigned long *p)
{
	unsigned long val;
	__asm__ __volatile__("movq %1, %0" : "=r"(val) : "m"(*p) : "memory");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static int8_t lf_op_load_i8(int8_t *p)
{
	int8_t val;
	__asm__ __volatile__("movb %1, %0" : "=r"(val) : "m"(*p) : "memory");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static uint8_t lf_op_load_u8(uint8_t *p)
{
	uint8_t val;
	__asm__ __volatile__("movb %1, %0" : "=r"(val) : "m"(*p) : "memory");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static int16_t lf_op_load_i16(int16_t *p)
{
	int16_t val;
	__asm__ __volatile__("movw %1, %0" : "=r"(val) : "m"(*p) : "memory");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static uint16_t lf_op_load_u16(uint16_t *p)
{
	uint16_t val;
	__asm__ __volatile__("movw %1, %0" : "=r"(val) : "m"(*p) : "memory");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static int32_t lf_op_load_i32(int32_t *p)
{
	int32_t val;
	__asm__ __volatile__("movl %1, %0" : "=r"(val) : "m"(*p) : "memory");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static uint32_t lf_op_load_u32(uint32_t *p)
{
	uint32_t val;
	__asm__ __volatile__("movl %1, %0" : "=r"(val) : "m"(*p) : "memory");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static int64_t lf_op_load_i64(int64_t *p)
{
	int64_t val;
	__asm__ __volatile__("movq %1, %0" : "=r"(val) : "m"(*p) : "memory");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static uint64_t lf_op_load_u64(uint64_t *p)
{
	uint64_t val;
	__asm__ __volatile__("movq %1, %0" : "=r"(val) : "m"(*p) : "memory");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static size_t lf_op_load_size(size_t *p)
{
	size_t val;
	__asm__ __volatile__("movq %1, %0" : "=r"(val) : "m"(*p) : "memory");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static lf_native_word lf_op_load_native(lf_native_word *p)
{
	lf_native_word val;
	__asm__ __volatile__("movq %1, %0" : "=r"(val) : "m"(*p) : "memory");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_store_ptr(void *p, void *val)
{
	__asm__ __volatile__("movq %1, %0"
			     : "=m"(*(void **)p)
			     : "r"(val)
			     : "memory");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_store_iptr(intptr_t *p, intptr_t val)
{
	__asm__ __volatile__("movq %1, %0" : "=m"(*p) : "r"(val) : "memory");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_store_uptr(uintptr_t *p, uintptr_t val)
{
	__asm__ __volatile__("movq %1, %0" : "=m"(*p) : "r"(val) : "memory");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_store_char(signed char *p, signed char val)
{
	__asm__ __volatile__("movb %1, %0" : "=m"(*p) : "r"(val) : "memory");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_store_uchar(unsigned char *p, unsigned char val)
{
	__asm__ __volatile__("movb %1, %0" : "=m"(*p) : "r"(val) : "memory");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_store_short(short *p, short val)
{
	__asm__ __volatile__("movw %1, %0" : "=m"(*p) : "r"(val) : "memory");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_store_ushort(unsigned short *p, unsigned short val)
{
	__asm__ __volatile__("movw %1, %0" : "=m"(*p) : "r"(val) : "memory");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_store_int(int *p, int val)
{
	__asm__ __volatile__("movl %1, %0" : "=m"(*p) : "r"(val) : "memory");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_store_uint(unsigned int *p, unsigned int val)
{
	__asm__ __volatile__("movl %1, %0" : "=m"(*p) : "r"(val) : "memory");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_store_long(long *p, long val)
{
	__asm__ __volatile__("movq %1, %0" : "=m"(*p) : "r"(val) : "memory");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_store_ulong(unsigned long *p, unsigned long val)
{
	__asm__ __volatile__("movq %1, %0" : "=m"(*p) : "r"(val) : "memory");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_store_i8(int8_t *p, int8_t val)
{
	__asm__ __volatile__("movb %1, %0" : "=m"(*p) : "r"(val) : "memory");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_store_u8(uint8_t *p, uint8_t val)
{
	__asm__ __volatile__("movb %1, %0" : "=m"(*p) : "r"(val) : "memory");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_store_i16(int16_t *p, int16_t val)
{
	__asm__ __volatile__("movw %1, %0" : "=m"(*p) : "r"(val) : "memory");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_store_u16(uint16_t *p, uint16_t val)
{
	__asm__ __volatile__("movw %1, %0" : "=m"(*p) : "r"(val) : "memory");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_store_i32(int32_t *p, int32_t val)
{
	__asm__ __volatile__("movl %1, %0" : "=m"(*p) : "r"(val) : "memory");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_store_u32(uint32_t *p, uint32_t val)
{
	__asm__ __volatile__("movl %1, %0" : "=m"(*p) : "r"(val) : "memory");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_store_i64(int64_t *p, int64_t val)
{
	__asm__ __volatile__("movq %1, %0" : "=m"(*p) : "r"(val) : "memory");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_store_u64(uint64_t *p, uint64_t val)
{
	__asm__ __volatile__("movq %1, %0" : "=m"(*p) : "r"(val) : "memory");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_store_size(size_t *p, size_t val)
{
	__asm__ __volatile__("movq %1, %0" : "=m"(*p) : "r"(val) : "memory");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_store_native(lf_native_word *p, lf_native_word val)
{
	__asm__ __volatile__("movq %1, %0" : "=m"(*p) : "r"(val) : "memory");
}

LF_ATTR_ALWAYS_INLINE
static void *lf_op_swap_ptr(void *p, void *val)
{
	__asm__ __volatile__("lock xchgq %0, %1"
			     : "+r"(val), "+m"(*(void **)p)
			     :
			     : "memory");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static intptr_t lf_op_swap_iptr(intptr_t *p, intptr_t val)
{
	__asm__ __volatile__("lock xchgq %0, %1"
			     : "+r"(val), "+m"(*p)
			     :
			     : "memory");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static uintptr_t lf_op_swap_uptr(uintptr_t *p, uintptr_t val)
{
	__asm__ __volatile__("lock xchgq %0, %1"
			     : "+r"(val), "+m"(*p)
			     :
			     : "memory");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static signed char lf_op_swap_char(signed char *p, signed char val)
{
	__asm__ __volatile__("lock xchgb %0, %1"
			     : "+r"(val), "+m"(*p)
			     :
			     : "memory");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static unsigned char lf_op_swap_uchar(unsigned char *p, unsigned char val)
{
	__asm__ __volatile__("lock xchgb %0, %1"
			     : "+r"(val), "+m"(*p)
			     :
			     : "memory");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static short lf_op_swap_short(short *p, short val)
{
	__asm__ __volatile__("lock xchgw %0, %1"
			     : "+r"(val), "+m"(*p)
			     :
			     : "memory");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static unsigned short lf_op_swap_ushort(unsigned short *p, unsigned short val)
{
	__asm__ __volatile__("lock xchgw %0, %1"
			     : "+r"(val), "+m"(*p)
			     :
			     : "memory");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static int lf_op_swap_int(int *p, int val)
{
	__asm__ __volatile__("lock xchgl %0, %1"
			     : "+r"(val), "+m"(*p)
			     :
			     : "memory");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static unsigned int lf_op_swap_uint(unsigned int *p, unsigned int val)
{
	__asm__ __volatile__("lock xchgl %0, %1"
			     : "+r"(val), "+m"(*p)
			     :
			     : "memory");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static long lf_op_swap_long(long *p, long val)
{
	__asm__ __volatile__("lock xchgq %0, %1"
			     : "+r"(val), "+m"(*p)
			     :
			     : "memory");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static unsigned long lf_op_swap_ulong(unsigned long *p, unsigned long val)
{
	__asm__ __volatile__("lock xchgq %0, %1"
			     : "+r"(val), "+m"(*p)
			     :
			     : "memory");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static int8_t lf_op_swap_i8(int8_t *p, int8_t val)
{
	__asm__ __volatile__("lock xchgb %0, %1"
			     : "+r"(val), "+m"(*p)
			     :
			     : "memory");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static uint8_t lf_op_swap_u8(uint8_t *p, uint8_t val)
{
	__asm__ __volatile__("lock xchgb %0, %1"
			     : "+r"(val), "+m"(*p)
			     :
			     : "memory");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static int16_t lf_op_swap_i16(int16_t *p, int16_t val)
{
	__asm__ __volatile__("lock xchgw %0, %1"
			     : "+r"(val), "+m"(*p)
			     :
			     : "memory");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static uint16_t lf_op_swap_u16(uint16_t *p, uint16_t val)
{
	__asm__ __volatile__("lock xchgw %0, %1"
			     : "+r"(val), "+m"(*p)
			     :
			     : "memory");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static int32_t lf_op_swap_i32(int32_t *p, int32_t val)
{
	__asm__ __volatile__("lock xchgl %0, %1"
			     : "+r"(val), "+m"(*p)
			     :
			     : "memory");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static uint32_t lf_op_swap_u32(uint32_t *p, uint32_t val)
{
	__asm__ __volatile__("lock xchgl %0, %1"
			     : "+r"(val), "+m"(*p)
			     :
			     : "memory");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static int64_t lf_op_swap_i64(int64_t *p, int64_t val)
{
	__asm__ __volatile__("lock xchgq %0, %1"
			     : "+r"(val), "+m"(*p)
			     :
			     : "memory");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static uint64_t lf_op_swap_u64(uint64_t *p, uint64_t val)
{
	__asm__ __volatile__("lock xchgq %0, %1"
			     : "+r"(val), "+m"(*p)
			     :
			     : "memory");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static size_t lf_op_swap_size(size_t *p, size_t val)
{
	__asm__ __volatile__("lock xchgq %0, %1"
			     : "+r"(val), "+m"(*p)
			     :
			     : "memory");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static lf_native_word lf_op_swap_native(lf_native_word *p, lf_native_word val)
{
	__asm__ __volatile__("lock xchgq %0, %1"
			     : "+r"(val), "+m"(*p)
			     :
			     : "memory");
	return val;
}

/* lf_op_cas and lf_op_cax use the cmpxchg instruction to perform an atomic CAS.
 * lf_op_cas returns a boolean that indicates if the CAS was successful, and lf_op_casx
 * returns the original value in p (val_old if successful).
 * 
 * I just want to make a note of how this works so I don't forget in the future.
 * cmpxchg uses the a register (rax) as val_old and places p's orignal value in the
 * a register on failure. This is why we can simply return val_old in casx: the a
 * register (val_old) will contain the original value on success or failure.
 */

LF_ATTR_ALWAYS_INLINE
static bool lf_op_cas_ptr(void *p, void *val_old, void *val_new)
{
	bool zf;
	__asm__ __volatile__("lock cmpxchgq %3, %0"
			     : "+m"(*(void **)p), "=@ccz"(zf), "+a"(val_old)
			     : "r"(val_new)
			     : "memory", "cc");
	return zf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_cas_iptr(intptr_t *p, intptr_t val_old, intptr_t val_new)
{
	bool zf;
	__asm__ __volatile__("lock cmpxchgq %3, %0"
			     : "+m"(*p), "=@ccz"(zf), "+a"(val_old)
			     : "r"(val_new)
			     : "memory", "cc");
	return zf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_cas_uptr(uintptr_t *p, uintptr_t val_old, uintptr_t val_new)
{
	bool zf;
	__asm__ __volatile__("lock cmpxchgq %3, %0"
			     : "+m"(*p), "=@ccz"(zf), "+a"(val_old)
			     : "r"(val_new)
			     : "memory", "cc");
	return zf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_cas_char(signed char *p, signed char val_old,
			   signed char val_new)
{
	bool zf;
	__asm__ __volatile__("lock cmpxchgb %3, %0"
			     : "+m"(*p), "=@ccz"(zf), "+a"(val_old)
			     : "r"(val_new)
			     : "memory", "cc");
	return zf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_cas_uchar(unsigned char *p, unsigned char val_old,
			    unsigned char val_new)
{
	bool zf;
	__asm__ __volatile__("lock cmpxchgb %3, %0"
			     : "+m"(*p), "=@ccz"(zf), "+a"(val_old)
			     : "r"(val_new)
			     : "memory", "cc");
	return zf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_cas_short(short *p, short val_old, short val_new)
{
	bool zf;
	__asm__ __volatile__("lock cmpxchgw %3, %0"
			     : "+m"(*p), "=@ccz"(zf), "+a"(val_old)
			     : "r"(val_new)
			     : "memory", "cc");
	return zf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_cas_ushort(unsigned short *p, unsigned short val_old,
			     unsigned short val_new)
{
	bool zf;
	__asm__ __volatile__("lock cmpxchgw %3, %0"
			     : "+m"(*p), "=@ccz"(zf), "+a"(val_old)
			     : "r"(val_new)
			     : "memory", "cc");
	return zf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_cas_int(int *p, int val_old, int val_new)
{
	bool zf;
	__asm__ __volatile__("lock cmpxchgl %3, %0"
			     : "+m"(*p), "=@ccz"(zf), "+a"(val_old)
			     : "r"(val_new)
			     : "memory", "cc");
	return zf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_cas_uint(unsigned int *p, unsigned int val_old,
			   unsigned int val_new)
{
	bool zf;
	__asm__ __volatile__("lock cmpxchgl %3, %0"
			     : "+m"(*p), "=@ccz"(zf), "+a"(val_old)
			     : "r"(val_new)
			     : "memory", "cc");
	return zf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_cas_long(long *p, long val_old, long val_new)
{
	bool zf;
	__asm__ __volatile__("lock cmpxchgq %3, %0"
			     : "+m"(*p), "=@ccz"(zf), "+a"(val_old)
			     : "r"(val_new)
			     : "memory", "cc");
	return zf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_cas_ulong(unsigned long *p, unsigned long val_old,
			    unsigned long val_new)
{
	bool zf;
	__asm__ __volatile__("lock cmpxchgq %3, %0"
			     : "+m"(*p), "=@ccz"(zf), "+a"(val_old)
			     : "r"(val_new)
			     : "memory", "cc");
	return zf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_cas_i8(int8_t *p, int8_t val_old, int8_t val_new)
{
	bool zf;
	__asm__ __volatile__("lock cmpxchgb %3, %0"
			     : "+m"(*p), "=@ccz"(zf), "+a"(val_old)
			     : "r"(val_new)
			     : "memory", "cc");
	return zf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_cas_u8(uint8_t *p, uint8_t val_old, uint8_t val_new)
{
	bool zf;
	__asm__ __volatile__("lock cmpxchgb %3, %0"
			     : "+m"(*p), "=@ccz"(zf), "+a"(val_old)
			     : "r"(val_new)
			     : "memory", "cc");
	return zf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_cas_i16(int16_t *p, int16_t val_old, int16_t val_new)
{
	bool zf;
	__asm__ __volatile__("lock cmpxchgw %3, %0"
			     : "+m"(*p), "=@ccz"(zf), "+a"(val_old)
			     : "r"(val_new)
			     : "memory", "cc");
	return zf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_cas_u16(uint16_t *p, uint16_t val_old, uint16_t val_new)
{
	bool zf;
	__asm__ __volatile__("lock cmpxchgw %3, %0"
			     : "+m"(*p), "=@ccz"(zf), "+a"(val_old)
			     : "r"(val_new)
			     : "memory", "cc");
	return zf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_cas_i32(int32_t *p, int32_t val_old, int32_t val_new)
{
	bool zf;
	__asm__ __volatile__("lock cmpxchgl %3, %0"
			     : "+m"(*p), "=@ccz"(zf), "+a"(val_old)
			     : "r"(val_new)
			     : "memory", "cc");
	return zf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_cas_u32(uint32_t *p, uint32_t val_old, uint32_t val_new)
{
	bool zf;
	__asm__ __volatile__("lock cmpxchgl %3, %0"
			     : "+m"(*p), "=@ccz"(zf), "+a"(val_old)
			     : "r"(val_new)
			     : "memory", "cc");
	return zf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_cas_i64(int64_t *p, int64_t val_old, int64_t val_new)
{
	bool zf;
	__asm__ __volatile__("lock cmpxchgq %3, %0"
			     : "+m"(*p), "=@ccz"(zf), "+a"(val_old)
			     : "r"(val_new)
			     : "memory", "cc");
	return zf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_cas_u64(uint64_t *p, uint64_t val_old, uint64_t val_new)
{
	bool zf;
	__asm__ __volatile__("lock cmpxchgq %3, %0"
			     : "+m"(*p), "=@ccz"(zf), "+a"(val_old)
			     : "r"(val_new)
			     : "memory", "cc");
	return zf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_cas_size(size_t *p, size_t val_old, size_t val_new)
{
	bool zf;
	__asm__ __volatile__("lock cmpxchgq %3, %0"
			     : "+m"(*p), "=@ccz"(zf), "+a"(val_old)
			     : "r"(val_new)
			     : "memory", "cc");
	return zf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_cas_native(lf_native_word *p, lf_native_word val_old,
			     lf_native_word val_new)
{
	bool zf;
	__asm__ __volatile__("lock cmpxchgq %3, %0"
			     : "+m"(*p), "=@ccz"(zf), "+a"(val_old)
			     : "r"(val_new)
			     : "memory", "cc");
	return zf;
}

LF_ATTR_ALWAYS_INLINE
static void *lf_op_casx_ptr(void *p, void *val_old, void *val_new)
{
	__asm__ __volatile__("lock cmpxchgq %2, %0"
			     : "+m"(*(void **)p), "+a"(val_old)
			     : "r"(val_new)
			     : "memory", "cc");
	return val_old;
}

LF_ATTR_ALWAYS_INLINE
static intptr_t lf_op_casx_iptr(intptr_t *p, intptr_t val_old, intptr_t val_new)
{
	__asm__ __volatile__("lock cmpxchgq %2, %0"
			     : "+m"(*p), "+a"(val_old)
			     : "r"(val_new)
			     : "memory", "cc");
	return val_old;
}

LF_ATTR_ALWAYS_INLINE
static uintptr_t lf_op_casx_uptr(uintptr_t *p, uintptr_t val_old,
				 uintptr_t val_new)
{
	__asm__ __volatile__("lock cmpxchgq %2, %0"
			     : "+m"(*p), "+a"(val_old)
			     : "r"(val_new)
			     : "memory", "cc");
	return val_old;
}

LF_ATTR_ALWAYS_INLINE
static signed char lf_op_casx_char(signed char *p, signed char val_old,
				   signed char val_new)
{
	__asm__ __volatile__("lock cmpxchgb %2, %0"
			     : "+m"(*p), "+a"(val_old)
			     : "r"(val_new)
			     : "memory", "cc");
	return val_old;
}

LF_ATTR_ALWAYS_INLINE
static unsigned char lf_op_casx_uchar(unsigned char *p, unsigned char val_old,
				      unsigned char val_new)
{
	__asm__ __volatile__("lock cmpxchgb %2, %0"
			     : "+m"(*p), "+a"(val_old)
			     : "r"(val_new)
			     : "memory", "cc");
	return val_old;
}

LF_ATTR_ALWAYS_INLINE
static short lf_op_casx_short(short *p, short val_old, short val_new)
{
	__asm__ __volatile__("lock cmpxchgw %2, %0"
			     : "+m"(*p), "+a"(val_old)
			     : "r"(val_new)
			     : "memory", "cc");
	return val_old;
}

LF_ATTR_ALWAYS_INLINE
static unsigned short lf_op_casx_ushort(unsigned short *p,
					unsigned short val_old,
					unsigned short val_new)
{
	__asm__ __volatile__("lock cmpxchgw %2, %0"
			     : "+m"(*p), "+a"(val_old)
			     : "r"(val_new)
			     : "memory", "cc");
	return val_old;
}

LF_ATTR_ALWAYS_INLINE
static int lf_op_casx_int(int *p, int val_old, int val_new)
{
	__asm__ __volatile__("lock cmpxchgl %2, %0"
			     : "+m"(*p), "+a"(val_old)
			     : "r"(val_new)
			     : "memory", "cc");
	return val_old;
}

LF_ATTR_ALWAYS_INLINE
static unsigned int lf_op_casx_uint(unsigned int *p, unsigned int val_old,
				    unsigned int val_new)
{
	__asm__ __volatile__("lock cmpxchgl %2, %0"
			     : "+m"(*p), "+a"(val_old)
			     : "r"(val_new)
			     : "memory", "cc");
	return val_old;
}

LF_ATTR_ALWAYS_INLINE
static long lf_op_casx_long(long *p, long val_old, long val_new)
{
	__asm__ __volatile__("lock cmpxchgq %2, %0"
			     : "+m"(*p), "+a"(val_old)
			     : "r"(val_new)
			     : "memory", "cc");
	return val_old;
}

LF_ATTR_ALWAYS_INLINE
static unsigned long lf_op_casx_ulong(unsigned long *p, unsigned long val_old,
				      unsigned long val_new)
{
	__asm__ __volatile__("lock cmpxchgq %2, %0"
			     : "+m"(*p), "+a"(val_old)
			     : "r"(val_new)
			     : "memory", "cc");
	return val_old;
}

LF_ATTR_ALWAYS_INLINE
static int8_t lf_op_casx_i8(int8_t *p, int8_t val_old, int8_t val_new)
{
	__asm__ __volatile__("lock cmpxchgb %2, %0"
			     : "+m"(*p), "+a"(val_old)
			     : "r"(val_new)
			     : "memory", "cc");
	return val_old;
}

LF_ATTR_ALWAYS_INLINE
static uint8_t lf_op_casx_u8(uint8_t *p, uint8_t val_old, uint8_t val_new)
{
	__asm__ __volatile__("lock cmpxchgb %2, %0"
			     : "+m"(*p), "+a"(val_old)
			     : "r"(val_new)
			     : "memory", "cc");
	return val_old;
}

LF_ATTR_ALWAYS_INLINE
static int16_t lf_op_casx_i16(int16_t *p, int16_t val_old, int16_t val_new)
{
	__asm__ __volatile__("lock cmpxchgw %2, %0"
			     : "+m"(*p), "+a"(val_old)
			     : "r"(val_new)
			     : "memory", "cc");
	return val_old;
}

LF_ATTR_ALWAYS_INLINE
static uint16_t lf_op_casx_u16(uint16_t *p, uint16_t val_old, uint16_t val_new)
{
	__asm__ __volatile__("lock cmpxchgw %2, %0"
			     : "+m"(*p), "+a"(val_old)
			     : "r"(val_new)
			     : "memory", "cc");
	return val_old;
}

LF_ATTR_ALWAYS_INLINE
static int32_t lf_op_casx_i32(int32_t *p, int32_t val_old, int32_t val_new)
{
	__asm__ __volatile__("lock cmpxchgl %2, %0"
			     : "+m"(*p), "+a"(val_old)
			     : "r"(val_new)
			     : "memory", "cc");
	return val_old;
}

LF_ATTR_ALWAYS_INLINE
static uint32_t lf_op_casx_u32(uint32_t *p, uint32_t val_old, uint32_t val_new)
{
	__asm__ __volatile__("lock cmpxchgl %2, %0"
			     : "+m"(*p), "+a"(val_old)
			     : "r"(val_new)
			     : "memory", "cc");
	return val_old;
}

LF_ATTR_ALWAYS_INLINE
static int64_t lf_op_casx_i64(int64_t *p, int64_t val_old, int64_t val_new)
{
	__asm__ __volatile__("lock cmpxchgq %2, %0"
			     : "+m"(*p), "+a"(val_old)
			     : "r"(val_new)
			     : "memory", "cc");
	return val_old;
}

LF_ATTR_ALWAYS_INLINE
static uint64_t lf_op_casx_u64(uint64_t *p, uint64_t val_old, uint64_t val_new)
{
	__asm__ __volatile__("lock cmpxchgq %2, %0"
			     : "+m"(*p), "+a"(val_old)
			     : "r"(val_new)
			     : "memory", "cc");
	return val_old;
}

LF_ATTR_ALWAYS_INLINE
static size_t lf_op_casx_size(size_t *p, size_t val_old, size_t val_new)
{
	__asm__ __volatile__("lock cmpxchgq %2, %0"
			     : "+m"(*p), "+a"(val_old)
			     : "r"(val_new)
			     : "memory", "cc");
	return val_old;
}

LF_ATTR_ALWAYS_INLINE
static lf_native_word lf_op_casx_native(lf_native_word *p,
					lf_native_word val_old,
					lf_native_word val_new)
{
	__asm__ __volatile__("lock cmpxchgq %2, %0"
			     : "+m"(*p), "+a"(val_old)
			     : "r"(val_new)
			     : "memory", "cc");
	return val_old;
}

LF_ATTR_ALWAYS_INLINE
static intptr_t lf_op_faadd_iptr(intptr_t *p, intptr_t val)
{
	__asm__ __volatile__("lock xaddq %1, %0"
			     : "+m"(*p), "+r"(val)
			     :
			     : "memory", "cc");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static uintptr_t lf_op_faadd_uptr(uintptr_t *p, uintptr_t val)
{
	__asm__ __volatile__("lock xaddq %1, %0"
			     : "+m"(*p), "+r"(val)
			     :
			     : "memory", "cc");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static signed char lf_op_faadd_char(signed char *p, signed char val)
{
	__asm__ __volatile__("lock xaddb %1, %0"
			     : "+m"(*p), "+r"(val)
			     :
			     : "memory", "cc");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static unsigned char lf_op_faadd_uchar(unsigned char *p, unsigned char val)
{
	__asm__ __volatile__("lock xaddb %1, %0"
			     : "+m"(*p), "+r"(val)
			     :
			     : "memory", "cc");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static short lf_op_faadd_short(short *p, short val)
{
	__asm__ __volatile__("lock xaddw %1, %0"
			     : "+m"(*p), "+r"(val)
			     :
			     : "memory", "cc");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static unsigned short lf_op_faadd_ushort(unsigned short *p, unsigned short val)
{
	__asm__ __volatile__("lock xaddw %1, %0"
			     : "+m"(*p), "+r"(val)
			     :
			     : "memory", "cc");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static int lf_op_faadd_int(int *p, int val)
{
	__asm__ __volatile__("lock xaddl %1, %0"
			     : "+m"(*p), "+r"(val)
			     :
			     : "memory", "cc");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static unsigned int lf_op_faadd_uint(unsigned int *p, unsigned int val)
{
	__asm__ __volatile__("lock xaddl %1, %0"
			     : "+m"(*p), "+r"(val)
			     :
			     : "memory", "cc");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static long lf_op_faadd_long(long *p, long val)
{
	__asm__ __volatile__("lock xaddq %1, %0"
			     : "+m"(*p), "+r"(val)
			     :
			     : "memory", "cc");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static unsigned long lf_op_faadd_ulong(unsigned long *p, unsigned long val)
{
	__asm__ __volatile__("lock xaddq %1, %0"
			     : "+m"(*p), "+r"(val)
			     :
			     : "memory", "cc");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static int8_t lf_op_faadd_i8(int8_t *p, int8_t val)
{
	__asm__ __volatile__("lock xaddb %1, %0"
			     : "+m"(*p), "+r"(val)
			     :
			     : "memory", "cc");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static uint8_t lf_op_faadd_u8(uint8_t *p, uint8_t val)
{
	__asm__ __volatile__("lock xaddb %1, %0"
			     : "+m"(*p), "+r"(val)
			     :
			     : "memory", "cc");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static int16_t lf_op_faadd_i16(int16_t *p, int16_t val)
{
	__asm__ __volatile__("lock xaddw %1, %0"
			     : "+m"(*p), "+r"(val)
			     :
			     : "memory", "cc");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static uint16_t lf_op_faadd_u16(uint16_t *p, uint16_t val)
{
	__asm__ __volatile__("lock xaddw %1, %0"
			     : "+m"(*p), "+r"(val)
			     :
			     : "memory", "cc");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static int32_t lf_op_faadd_i32(int32_t *p, int32_t val)
{
	__asm__ __volatile__("lock xaddl %1, %0"
			     : "+m"(*p), "+r"(val)
			     :
			     : "memory", "cc");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static uint32_t lf_op_faadd_u32(uint32_t *p, uint32_t val)
{
	__asm__ __volatile__("lock xaddl %1, %0"
			     : "+m"(*p), "+r"(val)
			     :
			     : "memory", "cc");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static int64_t lf_op_faadd_i64(int64_t *p, int64_t val)
{
	__asm__ __volatile__("lock xaddq %1, %0"
			     : "+m"(*p), "+r"(val)
			     :
			     : "memory", "cc");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static uint64_t lf_op_faadd_u64(uint64_t *p, uint64_t val)
{
	__asm__ __volatile__("lock xaddq %1, %0"
			     : "+m"(*p), "+r"(val)
			     :
			     : "memory", "cc");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static size_t lf_op_faadd_size(size_t *p, size_t val)
{
	__asm__ __volatile__("lock xaddq %1, %0"
			     : "+m"(*p), "+r"(val)
			     :
			     : "memory", "cc");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static lf_native_word lf_op_faadd_native(lf_native_word *p, lf_native_word val)
{
	__asm__ __volatile__("lock xaddq %1, %0"
			     : "+m"(*p), "+r"(val)
			     :
			     : "memory", "cc");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static intptr_t lf_op_fasub_iptr(intptr_t *p, intptr_t val)
{
	__asm__ __volatile__("negq %1\n\t"
			     "lock xaddq %1, %0"
			     : "+m"(*p), "+r"(val)
			     :
			     : "memory", "cc");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static uintptr_t lf_op_fasub_uptr(uintptr_t *p, uintptr_t val)
{
	__asm__ __volatile__("negq %1\n\t"
			     "lock xaddq %1, %0"
			     : "+m"(*p), "+r"(val)
			     :
			     : "memory", "cc");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static signed char lf_op_fasub_char(signed char *p, signed char val)
{
	__asm__ __volatile__("negb %1\n\t"
			     "lock xaddb %1, %0"
			     : "+m"(*p), "+r"(val)
			     :
			     : "memory", "cc");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static unsigned char lf_op_fasub_uchar(unsigned char *p, unsigned char val)
{
	__asm__ __volatile__("negb %1\n\t"
			     "lock xaddb %1, %0"
			     : "+m"(*p), "+r"(val)
			     :
			     : "memory", "cc");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static short lf_op_fasub_short(short *p, short val)
{
	__asm__ __volatile__("negw %1\n\t"
			     "lock xaddw %1, %0"
			     : "+m"(*p), "+r"(val)
			     :
			     : "memory", "cc");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static unsigned short lf_op_fasub_ushort(unsigned short *p, unsigned short val)
{
	__asm__ __volatile__("negw %1\n\t"
			     "lock xaddw %1, %0"
			     : "+m"(*p), "+r"(val)
			     :
			     : "memory", "cc");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static int lf_op_fasub_int(int *p, int val)
{
	__asm__ __volatile__("negl %1\n\t"
			     "lock xaddl %1, %0"
			     : "+m"(*p), "+r"(val)
			     :
			     : "memory", "cc");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static unsigned int lf_op_fasub_uint(unsigned int *p, unsigned int val)
{
	__asm__ __volatile__("negl %1\n\t"
			     "lock xaddl %1, %0"
			     : "+m"(*p), "+r"(val)
			     :
			     : "memory", "cc");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static long lf_op_fasub_long(long *p, long val)
{
	__asm__ __volatile__("negq %1\n\t"
			     "lock xaddq %1, %0"
			     : "+m"(*p), "+r"(val)
			     :
			     : "memory", "cc");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static unsigned long lf_op_fasub_ulong(unsigned long *p, unsigned long val)
{
	__asm__ __volatile__("negq %1\n\t"
			     "lock xaddq %1, %0"
			     : "+m"(*p), "+r"(val)
			     :
			     : "memory", "cc");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static int8_t lf_op_fasub_i8(int8_t *p, int8_t val)
{
	__asm__ __volatile__("negb %1\n\t"
			     "lock xaddb %1, %0"
			     : "+m"(*p), "+r"(val)
			     :
			     : "memory", "cc");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static uint8_t lf_op_fasub_u8(uint8_t *p, uint8_t val)
{
	__asm__ __volatile__("negb %1\n\t"
			     "lock xaddb %1, %0"
			     : "+m"(*p), "+r"(val)
			     :
			     : "memory", "cc");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static int16_t lf_op_fasub_i16(int16_t *p, int16_t val)
{
	__asm__ __volatile__("negw %1\n\t"
			     "lock xaddw %1, %0"
			     : "+m"(*p), "+r"(val)
			     :
			     : "memory", "cc");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static uint16_t lf_op_fasub_u16(uint16_t *p, uint16_t val)
{
	__asm__ __volatile__("negw %1\n\t"
			     "lock xaddw %1, %0"
			     : "+m"(*p), "+r"(val)
			     :
			     : "memory", "cc");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static int32_t lf_op_fasub_i32(int32_t *p, int32_t val)
{
	__asm__ __volatile__("negl %1\n\t"
			     "lock xaddl %1, %0"
			     : "+m"(*p), "+r"(val)
			     :
			     : "memory", "cc");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static uint32_t lf_op_fasub_u32(uint32_t *p, uint32_t val)
{
	__asm__ __volatile__("negl %1\n\t"
			     "lock xaddl %1, %0"
			     : "+m"(*p), "+r"(val)
			     :
			     : "memory", "cc");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static int64_t lf_op_fasub_i64(int64_t *p, int64_t val)
{
	__asm__ __volatile__("negq %1\n\t"
			     "lock xaddq %1, %0"
			     : "+m"(*p), "+r"(val)
			     :
			     : "memory", "cc");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static uint64_t lf_op_fasub_u64(uint64_t *p, uint64_t val)
{
	__asm__ __volatile__("negq %1\n\t"
			     "lock xaddq %1, %0"
			     : "+m"(*p), "+r"(val)
			     :
			     : "memory", "cc");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static size_t lf_op_fasub_size(size_t *p, size_t val)
{
	__asm__ __volatile__("negq %1\n\t"
			     "lock xaddq %1, %0"
			     : "+m"(*p), "+r"(val)
			     :
			     : "memory", "cc");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static lf_native_word lf_op_fasub_native(lf_native_word *p, lf_native_word val)
{
	__asm__ __volatile__("negq %1\n\t"
			     "lock xaddq %1, %0"
			     : "+m"(*p), "+r"(val)
			     :
			     : "memory", "cc");
	return val;
}

LF_ATTR_ALWAYS_INLINE
static intptr_t lf_op_fainc_iptr(intptr_t *p)
{
	return lf_op_faadd_iptr(p, 1);
}

LF_ATTR_ALWAYS_INLINE
static uintptr_t lf_op_fainc_uptr(uintptr_t *p)
{
	return lf_op_faadd_uptr(p, 1);
}

LF_ATTR_ALWAYS_INLINE
static signed char lf_op_fainc_char(signed char *p)
{
	return lf_op_faadd_char(p, 1);
}

LF_ATTR_ALWAYS_INLINE
static unsigned char lf_op_fainc_uchar(unsigned char *p)
{
	return lf_op_faadd_uchar(p, 1);
}

LF_ATTR_ALWAYS_INLINE
static short lf_op_fainc_short(short *p)
{
	return lf_op_faadd_short(p, 1);
}

LF_ATTR_ALWAYS_INLINE
static unsigned short lf_op_fainc_ushort(unsigned short *p)
{
	return lf_op_faadd_ushort(p, 1);
}

LF_ATTR_ALWAYS_INLINE
static int lf_op_fainc_int(int *p)
{
	return lf_op_faadd_int(p, 1);
}

LF_ATTR_ALWAYS_INLINE
static unsigned int lf_op_fainc_uint(unsigned int *p)
{
	return lf_op_faadd_uint(p, 1);
}

LF_ATTR_ALWAYS_INLINE
static long lf_op_fainc_long(long *p)
{
	return lf_op_faadd_long(p, 1);
}

LF_ATTR_ALWAYS_INLINE
static unsigned long lf_op_fainc_ulong(unsigned long *p)
{
	return lf_op_faadd_ulong(p, 1);
}

LF_ATTR_ALWAYS_INLINE
static int8_t lf_op_fainc_i8(int8_t *p)
{
	return lf_op_faadd_i8(p, 1);
}

LF_ATTR_ALWAYS_INLINE
static uint8_t lf_op_fainc_u8(uint8_t *p)
{
	return lf_op_faadd_u8(p, 1);
}

LF_ATTR_ALWAYS_INLINE
static int16_t lf_op_fainc_i16(int16_t *p)
{
	return lf_op_faadd_i16(p, 1);
}

LF_ATTR_ALWAYS_INLINE
static uint16_t lf_op_fainc_u16(uint16_t *p)
{
	return lf_op_faadd_u16(p, 1);
}

LF_ATTR_ALWAYS_INLINE
static int32_t lf_op_fainc_i32(int32_t *p)
{
	return lf_op_faadd_i32(p, 1);
}

LF_ATTR_ALWAYS_INLINE
static uint32_t lf_op_fainc_u32(uint32_t *p)
{
	return lf_op_faadd_u32(p, 1);
}

LF_ATTR_ALWAYS_INLINE
static int64_t lf_op_fainc_i64(int64_t *p)
{
	return lf_op_faadd_i64(p, 1);
}

LF_ATTR_ALWAYS_INLINE
static uint64_t lf_op_fainc_u64(uint64_t *p)
{
	return lf_op_faadd_u64(p, 1);
}

LF_ATTR_ALWAYS_INLINE
static size_t lf_op_fainc_size(size_t *p)
{
	return lf_op_faadd_size(p, 1);
}

LF_ATTR_ALWAYS_INLINE
static lf_native_word lf_op_fainc_native(lf_native_word *p)
{
	return lf_op_faadd_native(p, 1);
}

LF_ATTR_ALWAYS_INLINE
static intptr_t lf_op_fadec_iptr(intptr_t *p)
{
	return lf_op_fasub_iptr(p, 1);
}

LF_ATTR_ALWAYS_INLINE
static uintptr_t lf_op_fadec_uptr(uintptr_t *p)
{
	return lf_op_fasub_uptr(p, 1);
}

LF_ATTR_ALWAYS_INLINE
static signed char lf_op_fadec_char(signed char *p)
{
	return lf_op_fasub_char(p, 1);
}

LF_ATTR_ALWAYS_INLINE
static unsigned char lf_op_fadec_uchar(unsigned char *p)
{
	return lf_op_fasub_uchar(p, 1);
}

LF_ATTR_ALWAYS_INLINE
static short lf_op_fadec_short(short *p)
{
	return lf_op_fasub_short(p, 1);
}

LF_ATTR_ALWAYS_INLINE
static unsigned short lf_op_fadec_ushort(unsigned short *p)
{
	return lf_op_fasub_ushort(p, 1);
}

LF_ATTR_ALWAYS_INLINE
static int lf_op_fadec_int(int *p)
{
	return lf_op_fasub_int(p, 1);
}

LF_ATTR_ALWAYS_INLINE
static unsigned int lf_op_fadec_uint(unsigned int *p)
{
	return lf_op_fasub_uint(p, 1);
}

LF_ATTR_ALWAYS_INLINE
static long lf_op_fadec_long(long *p)
{
	return lf_op_fasub_long(p, 1);
}

LF_ATTR_ALWAYS_INLINE
static unsigned long lf_op_fadec_ulong(unsigned long *p)
{
	return lf_op_fasub_ulong(p, 1);
}

LF_ATTR_ALWAYS_INLINE
static int8_t lf_op_fadec_i8(int8_t *p)
{
	return lf_op_fasub_i8(p, 1);
}

LF_ATTR_ALWAYS_INLINE
static uint8_t lf_op_fadec_u8(uint8_t *p)
{
	return lf_op_fasub_u8(p, 1);
}

LF_ATTR_ALWAYS_INLINE
static int16_t lf_op_fadec_i16(int16_t *p)
{
	return lf_op_fasub_i16(p, 1);
}

LF_ATTR_ALWAYS_INLINE
static uint16_t lf_op_fadec_u16(uint16_t *p)
{
	return lf_op_fasub_u16(p, 1);
}

LF_ATTR_ALWAYS_INLINE
static int32_t lf_op_fadec_i32(int32_t *p)
{
	return lf_op_fasub_i32(p, 1);
}

LF_ATTR_ALWAYS_INLINE
static uint32_t lf_op_fadec_u32(uint32_t *p)
{
	return lf_op_fasub_u32(p, 1);
}

LF_ATTR_ALWAYS_INLINE
static int64_t lf_op_fadec_i64(int64_t *p)
{
	return lf_op_fasub_i64(p, 1);
}

LF_ATTR_ALWAYS_INLINE
static uint64_t lf_op_fadec_u64(uint64_t *p)
{
	return lf_op_fasub_u64(p, 1);
}

LF_ATTR_ALWAYS_INLINE
static size_t lf_op_fadec_size(size_t *p)
{
	return lf_op_fasub_size(p, 1);
}

LF_ATTR_ALWAYS_INLINE
static lf_native_word lf_op_fadec_native(lf_native_word *p)
{
	return lf_op_fasub_native(p, 1);
}

/* Unfortunately x86_64 does not have an XADD like instruction for bitwise AND, OR,
 * or XOR. Therefore, we have to implement the fetch-and versions of AND, OR, and
 * XOR as CAS loops. Consider using the BTS, BTR, and BTC functions if you only need
 * to modify a single bit.
 */

LF_ATTR_ALWAYS_INLINE
static intptr_t lf_op_faand_iptr(intptr_t *p, intptr_t val)
{
	intptr_t old;
	intptr_t new;
	do {
		old = lf_op_load_iptr(p);
		new = (old & val);
	} while (!lf_op_cas_iptr(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static uintptr_t lf_op_faand_uptr(uintptr_t *p, uintptr_t val)
{
	uintptr_t old;
	uintptr_t new;
	do {
		old = lf_op_load_uptr(p);
		new = (old & val);
	} while (!lf_op_cas_uptr(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static signed char lf_op_faand_char(signed char *p, signed char val)
{
	signed char old;
	signed char new;
	do {
		old = lf_op_load_char(p);
		new = (old & val);
	} while (!lf_op_cas_char(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static unsigned char lf_op_faand_uchar(unsigned char *p, unsigned char val)
{
	unsigned char old;
	unsigned char new;
	do {
		old = lf_op_load_uchar(p);
		new = (old & val);
	} while (!lf_op_cas_uchar(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static short lf_op_faand_short(short *p, short val)
{
	short old;
	short new;
	do {
		old = lf_op_load_short(p);
		new = (old & val);
	} while (!lf_op_cas_short(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static unsigned short lf_op_faand_ushort(unsigned short *p, unsigned short val)
{
	unsigned short old;
	unsigned short new;
	do {
		old = lf_op_load_ushort(p);
		new = (old & val);
	} while (!lf_op_cas_ushort(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static int lf_op_faand_int(int *p, int val)
{
	int old;
	int new;
	do {
		old = lf_op_load_int(p);
		new = (old & val);
	} while (!lf_op_cas_int(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static unsigned int lf_op_faand_uint(unsigned int *p, unsigned int val)
{
	unsigned int old;
	unsigned int new;
	do {
		old = lf_op_load_uint(p);
		new = (old & val);
	} while (!lf_op_cas_uint(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static long lf_op_faand_long(long *p, long val)
{
	long old;
	long new;
	do {
		old = lf_op_load_long(p);
		new = (old & val);
	} while (!lf_op_cas_long(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static unsigned long lf_op_faand_ulong(unsigned long *p, unsigned long val)
{
	unsigned long old;
	unsigned long new;
	do {
		old = lf_op_load_ulong(p);
		new = (old & val);
	} while (!lf_op_cas_ulong(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static int8_t lf_op_faand_i8(int8_t *p, int8_t val)
{
	int8_t old;
	int8_t new;
	do {
		old = lf_op_load_i8(p);
		new = (old & val);
	} while (!lf_op_cas_i8(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static uint8_t lf_op_faand_u8(uint8_t *p, uint8_t val)
{
	uint8_t old;
	uint8_t new;
	do {
		old = lf_op_load_u8(p);
		new = (old & val);
	} while (!lf_op_cas_u8(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static int16_t lf_op_faand_i16(int16_t *p, int16_t val)
{
	int16_t old;
	int16_t new;
	do {
		old = lf_op_load_i16(p);
		new = (old & val);
	} while (!lf_op_cas_i16(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static uint16_t lf_op_faand_u16(uint16_t *p, uint16_t val)
{
	uint16_t old;
	uint16_t new;
	do {
		old = lf_op_load_u16(p);
		new = (old & val);
	} while (!lf_op_cas_u16(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static int32_t lf_op_faand_i32(int32_t *p, int32_t val)
{
	int32_t old;
	int32_t new;
	do {
		old = lf_op_load_i32(p);
		new = (old & val);
	} while (!lf_op_cas_i32(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static uint32_t lf_op_faand_u32(uint32_t *p, uint32_t val)
{
	uint32_t old;
	uint32_t new;
	do {
		old = lf_op_load_u32(p);
		new = (old & val);
	} while (!lf_op_cas_u32(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static int64_t lf_op_faand_i64(int64_t *p, int64_t val)
{
	int64_t old;
	int64_t new;
	do {
		old = lf_op_load_i64(p);
		new = (old & val);
	} while (!lf_op_cas_i64(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static uint64_t lf_op_faand_u64(uint64_t *p, uint64_t val)
{
	uint64_t old;
	uint64_t new;
	do {
		old = lf_op_load_u64(p);
		new = (old & val);
	} while (!lf_op_cas_u64(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static size_t lf_op_faand_size(size_t *p, size_t val)
{
	size_t old;
	size_t new;
	do {
		old = lf_op_load_size(p);
		new = (old & val);
	} while (!lf_op_cas_size(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static lf_native_word lf_op_faand_native(lf_native_word *p, lf_native_word val)
{
	lf_native_word old;
	lf_native_word new;
	do {
		old = lf_op_load_native(p);
		new = (old & val);
	} while (!lf_op_cas_native(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static intptr_t lf_op_faor_iptr(intptr_t *p, intptr_t val)
{
	intptr_t old;
	intptr_t new;
	do {
		old = lf_op_load_iptr(p);
		new = (old | val);
	} while (!lf_op_cas_iptr(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static uintptr_t lf_op_faor_uptr(uintptr_t *p, uintptr_t val)
{
	uintptr_t old;
	uintptr_t new;
	do {
		old = lf_op_load_uptr(p);
		new = (old | val);
	} while (!lf_op_cas_uptr(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static signed char lf_op_faor_char(signed char *p, signed char val)
{
	signed char old;
	signed char new;
	do {
		old = lf_op_load_char(p);
		new = (old | val);
	} while (!lf_op_cas_char(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static unsigned char lf_op_faor_uchar(unsigned char *p, unsigned char val)
{
	unsigned char old;
	unsigned char new;
	do {
		old = lf_op_load_uchar(p);
		new = (old | val);
	} while (!lf_op_cas_uchar(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static short lf_op_faor_short(short *p, short val)
{
	short old;
	short new;
	do {
		old = lf_op_load_short(p);
		new = (old | val);
	} while (!lf_op_cas_short(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static unsigned short lf_op_faor_ushort(unsigned short *p, unsigned short val)
{
	unsigned short old;
	unsigned short new;
	do {
		old = lf_op_load_ushort(p);
		new = (old | val);
	} while (!lf_op_cas_ushort(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static int lf_op_faor_int(int *p, int val)
{
	int old;
	int new;
	do {
		old = lf_op_load_int(p);
		new = (old | val);
	} while (!lf_op_cas_int(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static unsigned int lf_op_faor_uint(unsigned int *p, unsigned int val)
{
	unsigned int old;
	unsigned int new;
	do {
		old = lf_op_load_uint(p);
		new = (old | val);
	} while (!lf_op_cas_uint(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static long lf_op_faor_long(long *p, long val)
{
	long old;
	long new;
	do {
		old = lf_op_load_long(p);
		new = (old | val);
	} while (!lf_op_cas_long(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static unsigned long lf_op_faor_ulong(unsigned long *p, unsigned long val)
{
	unsigned long old;
	unsigned long new;
	do {
		old = lf_op_load_ulong(p);
		new = (old | val);
	} while (!lf_op_cas_ulong(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static int8_t lf_op_faor_i8(int8_t *p, int8_t val)
{
	int8_t old;
	int8_t new;
	do {
		old = lf_op_load_i8(p);
		new = (old | val);
	} while (!lf_op_cas_i8(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static uint8_t lf_op_faor_u8(uint8_t *p, uint8_t val)
{
	uint8_t old;
	uint8_t new;
	do {
		old = lf_op_load_u8(p);
		new = (old | val);
	} while (!lf_op_cas_u8(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static int16_t lf_op_faor_i16(int16_t *p, int16_t val)
{
	int16_t old;
	int16_t new;
	do {
		old = lf_op_load_i16(p);
		new = (old | val);
	} while (!lf_op_cas_i16(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static uint16_t lf_op_faor_u16(uint16_t *p, uint16_t val)
{
	uint16_t old;
	uint16_t new;
	do {
		old = lf_op_load_u16(p);
		new = (old | val);
	} while (!lf_op_cas_u16(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static int32_t lf_op_faor_i32(int32_t *p, int32_t val)
{
	int32_t old;
	int32_t new;
	do {
		old = lf_op_load_i32(p);
		new = (old | val);
	} while (!lf_op_cas_i32(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static uint32_t lf_op_faor_u32(uint32_t *p, uint32_t val)
{
	uint32_t old;
	uint32_t new;
	do {
		old = lf_op_load_u32(p);
		new = (old | val);
	} while (!lf_op_cas_u32(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static int64_t lf_op_faor_i64(int64_t *p, int64_t val)
{
	int64_t old;
	int64_t new;
	do {
		old = lf_op_load_i64(p);
		new = (old | val);
	} while (!lf_op_cas_i64(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static uint64_t lf_op_faor_u64(uint64_t *p, uint64_t val)
{
	uint64_t old;
	uint64_t new;
	do {
		old = lf_op_load_u64(p);
		new = (old | val);
	} while (!lf_op_cas_u64(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static size_t lf_op_faor_size(size_t *p, size_t val)
{
	size_t old;
	size_t new;
	do {
		old = lf_op_load_size(p);
		new = (old | val);
	} while (!lf_op_cas_size(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static lf_native_word lf_op_faor_native(lf_native_word *p, lf_native_word val)
{
	lf_native_word old;
	lf_native_word new;
	do {
		old = lf_op_load_native(p);
		new = (old | val);
	} while (!lf_op_cas_native(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static intptr_t lf_op_faxor_iptr(intptr_t *p, intptr_t val)
{
	intptr_t old;
	intptr_t new;
	do {
		old = lf_op_load_iptr(p);
		new = (old ^ val);
	} while (!lf_op_cas_iptr(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static uintptr_t lf_op_faxor_uptr(uintptr_t *p, uintptr_t val)
{
	uintptr_t old;
	uintptr_t new;
	do {
		old = lf_op_load_uptr(p);
		new = (old ^ val);
	} while (!lf_op_cas_uptr(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static signed char lf_op_faxor_char(signed char *p, signed char val)
{
	signed char old;
	signed char new;
	do {
		old = lf_op_load_char(p);
		new = (old ^ val);
	} while (!lf_op_cas_char(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static unsigned char lf_op_faxor_uchar(unsigned char *p, unsigned char val)
{
	unsigned char old;
	unsigned char new;
	do {
		old = lf_op_load_uchar(p);
		new = (old ^ val);
	} while (!lf_op_cas_uchar(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static short lf_op_faxor_short(short *p, short val)
{
	short old;
	short new;
	do {
		old = lf_op_load_short(p);
		new = (old ^ val);
	} while (!lf_op_cas_short(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static unsigned short lf_op_faxor_ushort(unsigned short *p, unsigned short val)
{
	unsigned short old;
	unsigned short new;
	do {
		old = lf_op_load_ushort(p);
		new = (old ^ val);
	} while (!lf_op_cas_ushort(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static int lf_op_faxor_int(int *p, int val)
{
	int old;
	int new;
	do {
		old = lf_op_load_int(p);
		new = (old ^ val);
	} while (!lf_op_cas_int(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static unsigned int lf_op_faxor_uint(unsigned int *p, unsigned int val)
{
	unsigned int old;
	unsigned int new;
	do {
		old = lf_op_load_uint(p);
		new = (old ^ val);
	} while (!lf_op_cas_uint(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static long lf_op_faxor_long(long *p, long val)
{
	long old;
	long new;
	do {
		old = lf_op_load_long(p);
		new = (old ^ val);
	} while (!lf_op_cas_long(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static unsigned long lf_op_faxor_ulong(unsigned long *p, unsigned long val)
{
	unsigned long old;
	unsigned long new;
	do {
		old = lf_op_load_ulong(p);
		new = (old ^ val);
	} while (!lf_op_cas_ulong(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static int8_t lf_op_faxor_i8(int8_t *p, int8_t val)
{
	int8_t old;
	int8_t new;
	do {
		old = lf_op_load_i8(p);
		new = (old ^ val);
	} while (!lf_op_cas_i8(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static uint8_t lf_op_faxor_u8(uint8_t *p, uint8_t val)
{
	uint8_t old;
	uint8_t new;
	do {
		old = lf_op_load_u8(p);
		new = (old ^ val);
	} while (!lf_op_cas_u8(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static int16_t lf_op_faxor_i16(int16_t *p, int16_t val)
{
	int16_t old;
	int16_t new;
	do {
		old = lf_op_load_i16(p);
		new = (old ^ val);
	} while (!lf_op_cas_i16(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static uint16_t lf_op_faxor_u16(uint16_t *p, uint16_t val)
{
	uint16_t old;
	uint16_t new;
	do {
		old = lf_op_load_u16(p);
		new = (old ^ val);
	} while (!lf_op_cas_u16(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static int32_t lf_op_faxor_i32(int32_t *p, int32_t val)
{
	int32_t old;
	int32_t new;
	do {
		old = lf_op_load_i32(p);
		new = (old ^ val);
	} while (!lf_op_cas_i32(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static uint32_t lf_op_faxor_u32(uint32_t *p, uint32_t val)
{
	uint32_t old;
	uint32_t new;
	do {
		old = lf_op_load_u32(p);
		new = (old ^ val);
	} while (!lf_op_cas_u32(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static int64_t lf_op_faxor_i64(int64_t *p, int64_t val)
{
	int64_t old;
	int64_t new;
	do {
		old = lf_op_load_i64(p);
		new = (old ^ val);
	} while (!lf_op_cas_i64(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static uint64_t lf_op_faxor_u64(uint64_t *p, uint64_t val)
{
	uint64_t old;
	uint64_t new;
	do {
		old = lf_op_load_u64(p);
		new = (old ^ val);
	} while (!lf_op_cas_u64(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static size_t lf_op_faxor_size(size_t *p, size_t val)
{
	size_t old;
	size_t new;
	do {
		old = lf_op_load_size(p);
		new = (old ^ val);
	} while (!lf_op_cas_size(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static lf_native_word lf_op_faxor_native(lf_native_word *p, lf_native_word val)
{
	lf_native_word old;
	lf_native_word new;
	do {
		old = lf_op_load_native(p);
		new = (old ^ val);
	} while (!lf_op_cas_native(p, old, new));
	return old;
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_add_iptr(intptr_t *p, intptr_t val)
{
	__asm__ __volatile__("lock addq %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_add_uptr(uintptr_t *p, uintptr_t val)
{
	__asm__ __volatile__("lock addq %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_add_char(signed char *p, signed char val)
{
	__asm__ __volatile__("lock addb %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_add_uchar(unsigned char *p, unsigned char val)
{
	__asm__ __volatile__("lock addb %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_add_short(short *p, short val)
{
	__asm__ __volatile__("lock addw %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_add_ushort(unsigned short *p, unsigned short val)
{
	__asm__ __volatile__("lock addw %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_add_int(int *p, int val)
{
	__asm__ __volatile__("lock addl %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_add_uint(unsigned int *p, unsigned int val)
{
	__asm__ __volatile__("lock addl %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_add_long(long *p, long val)
{
	__asm__ __volatile__("lock addq %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_add_ulong(unsigned long *p, unsigned long val)
{
	__asm__ __volatile__("lock addq %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_add_i8(int8_t *p, int8_t val)
{
	__asm__ __volatile__("lock addb %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_add_u8(uint8_t *p, uint8_t val)
{
	__asm__ __volatile__("lock addb %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_add_i16(int16_t *p, int16_t val)
{
	__asm__ __volatile__("lock addw %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_add_u16(uint16_t *p, uint16_t val)
{
	__asm__ __volatile__("lock addw %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_add_i32(int32_t *p, int32_t val)
{
	__asm__ __volatile__("lock addl %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_add_u32(uint32_t *p, uint32_t val)
{
	__asm__ __volatile__("lock addl %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_add_i64(int64_t *p, int64_t val)
{
	__asm__ __volatile__("lock addq %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_add_u64(uint64_t *p, uint64_t val)
{
	__asm__ __volatile__("lock addq %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_add_size(size_t *p, size_t val)
{
	__asm__ __volatile__("lock addq %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_add_native(lf_native_word *p, lf_native_word val)
{
	__asm__ __volatile__("lock addq %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_sub_iptr(intptr_t *p, intptr_t val)
{
	__asm__ __volatile__("lock subq %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_sub_uptr(uintptr_t *p, uintptr_t val)
{
	__asm__ __volatile__("lock subq %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_sub_char(signed char *p, signed char val)
{
	__asm__ __volatile__("lock subb %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_sub_uchar(unsigned char *p, unsigned char val)
{
	__asm__ __volatile__("lock subb %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_sub_short(short *p, short val)
{
	__asm__ __volatile__("lock subw %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_sub_ushort(unsigned short *p, unsigned short val)
{
	__asm__ __volatile__("lock subw %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_sub_int(int *p, int val)
{
	__asm__ __volatile__("lock subl %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_sub_uint(unsigned int *p, unsigned int val)
{
	__asm__ __volatile__("lock subl %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_sub_long(long *p, long val)
{
	__asm__ __volatile__("lock subq %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_sub_ulong(unsigned long *p, unsigned long val)
{
	__asm__ __volatile__("lock subq %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_sub_i8(int8_t *p, int8_t val)
{
	__asm__ __volatile__("lock subb %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_sub_u8(uint8_t *p, uint8_t val)
{
	__asm__ __volatile__("lock subb %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_sub_i16(int16_t *p, int16_t val)
{
	__asm__ __volatile__("lock subw %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_sub_u16(uint16_t *p, uint16_t val)
{
	__asm__ __volatile__("lock subw %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_sub_i32(int32_t *p, int32_t val)
{
	__asm__ __volatile__("lock subl %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_sub_u32(uint32_t *p, uint32_t val)
{
	__asm__ __volatile__("lock subl %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_sub_i64(int64_t *p, int64_t val)
{
	__asm__ __volatile__("lock subq %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_sub_u64(uint64_t *p, uint64_t val)
{
	__asm__ __volatile__("lock subq %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_sub_size(size_t *p, size_t val)
{
	__asm__ __volatile__("lock subq %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_sub_native(lf_native_word *p, lf_native_word val)
{
	__asm__ __volatile__("lock subq %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_inc_iptr(intptr_t *p)
{
	__asm__ __volatile__("lock addq $1, %0" : "+m"(*p) : : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_inc_uptr(uintptr_t *p)
{
	__asm__ __volatile__("lock addq $1, %0" : "+m"(*p) : : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_inc_char(signed char *p)
{
	__asm__ __volatile__("lock addb $1, %0" : "+m"(*p) : : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_inc_uchar(unsigned char *p)
{
	__asm__ __volatile__("lock addb $1, %0" : "+m"(*p) : : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_inc_short(short *p)
{
	__asm__ __volatile__("lock addw $1, %0" : "+m"(*p) : : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_inc_ushort(unsigned short *p)
{
	__asm__ __volatile__("lock addw $1, %0" : "+m"(*p) : : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_inc_int(int *p)
{
	__asm__ __volatile__("lock addl $1, %0" : "+m"(*p) : : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_inc_uint(unsigned int *p)
{
	__asm__ __volatile__("lock addl $1, %0" : "+m"(*p) : : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_inc_long(long *p)
{
	__asm__ __volatile__("lock addq $1, %0" : "+m"(*p) : : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_inc_ulong(unsigned long *p)
{
	__asm__ __volatile__("lock addq $1, %0" : "+m"(*p) : : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_inc_i8(int8_t *p)
{
	__asm__ __volatile__("lock addb $1, %0" : "+m"(*p) : : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_inc_u8(uint8_t *p)
{
	__asm__ __volatile__("lock addb $1, %0" : "+m"(*p) : : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_inc_i16(int16_t *p)
{
	__asm__ __volatile__("lock addw $1, %0" : "+m"(*p) : : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_inc_u16(uint16_t *p)
{
	__asm__ __volatile__("lock addw $1, %0" : "+m"(*p) : : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_inc_i32(int32_t *p)
{
	__asm__ __volatile__("lock addl $1, %0" : "+m"(*p) : : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_inc_u32(uint32_t *p)
{
	__asm__ __volatile__("lock addl $1, %0" : "+m"(*p) : : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_inc_i64(int64_t *p)
{
	__asm__ __volatile__("lock addq $1, %0" : "+m"(*p) : : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_inc_u64(uint64_t *p)
{
	__asm__ __volatile__("lock addq $1, %0" : "+m"(*p) : : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_inc_size(size_t *p)
{
	__asm__ __volatile__("lock addq $1, %0" : "+m"(*p) : : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_inc_native(lf_native_word *p)
{
	__asm__ __volatile__("lock addq $1, %0" : "+m"(*p) : : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_dec_iptr(intptr_t *p)
{
	__asm__ __volatile__("lock subq $1, %0" : "+m"(*p) : : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_dec_uptr(uintptr_t *p)
{
	__asm__ __volatile__("lock subq $1, %0" : "+m"(*p) : : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_dec_char(signed char *p)
{
	__asm__ __volatile__("lock subb $1, %0" : "+m"(*p) : : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_dec_uchar(unsigned char *p)
{
	__asm__ __volatile__("lock subb $1, %0" : "+m"(*p) : : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_dec_short(short *p)
{
	__asm__ __volatile__("lock subw $1, %0" : "+m"(*p) : : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_dec_ushort(unsigned short *p)
{
	__asm__ __volatile__("lock subw $1, %0" : "+m"(*p) : : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_dec_int(int *p)
{
	__asm__ __volatile__("lock subl $1, %0" : "+m"(*p) : : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_dec_uint(unsigned int *p)
{
	__asm__ __volatile__("lock subl $1, %0" : "+m"(*p) : : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_dec_long(long *p)
{
	__asm__ __volatile__("lock subq $1, %0" : "+m"(*p) : : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_dec_ulong(unsigned long *p)
{
	__asm__ __volatile__("lock subq $1, %0" : "+m"(*p) : : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_dec_i8(int8_t *p)
{
	__asm__ __volatile__("lock subb $1, %0" : "+m"(*p) : : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_dec_u8(uint8_t *p)
{
	__asm__ __volatile__("lock subb $1, %0" : "+m"(*p) : : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_dec_i16(int16_t *p)
{
	__asm__ __volatile__("lock subw $1, %0" : "+m"(*p) : : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_dec_u16(uint16_t *p)
{
	__asm__ __volatile__("lock subw $1, %0" : "+m"(*p) : : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_dec_i32(int32_t *p)
{
	__asm__ __volatile__("lock subl $1, %0" : "+m"(*p) : : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_dec_u32(uint32_t *p)
{
	__asm__ __volatile__("lock subl $1, %0" : "+m"(*p) : : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_dec_i64(int64_t *p)
{
	__asm__ __volatile__("lock subq $1, %0" : "+m"(*p) : : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_dec_u64(uint64_t *p)
{
	__asm__ __volatile__("lock subq $1, %0" : "+m"(*p) : : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_dec_size(size_t *p)
{
	__asm__ __volatile__("lock subq $1, %0" : "+m"(*p) : : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_dec_native(lf_native_word *p)
{
	__asm__ __volatile__("lock subq $1, %0" : "+m"(*p) : : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_and_iptr(intptr_t *p, intptr_t val)
{
	__asm__ __volatile__("lock andq %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_and_uptr(uintptr_t *p, uintptr_t val)
{
	__asm__ __volatile__("lock andq %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_and_char(signed char *p, signed char val)
{
	__asm__ __volatile__("lock andb %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_and_uchar(unsigned char *p, unsigned char val)
{
	__asm__ __volatile__("lock andb %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_and_short(short *p, short val)
{
	__asm__ __volatile__("lock andw %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_and_ushort(unsigned short *p, unsigned short val)
{
	__asm__ __volatile__("lock andw %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_and_int(int *p, int val)
{
	__asm__ __volatile__("lock andl %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_and_uint(unsigned int *p, unsigned int val)
{
	__asm__ __volatile__("lock andl %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_and_long(long *p, long val)
{
	__asm__ __volatile__("lock andq %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_and_ulong(unsigned long *p, unsigned long val)
{
	__asm__ __volatile__("lock andq %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_and_i8(int8_t *p, int8_t val)
{
	__asm__ __volatile__("lock andb %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_and_u8(uint8_t *p, uint8_t val)
{
	__asm__ __volatile__("lock andb %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_and_i16(int16_t *p, int16_t val)
{
	__asm__ __volatile__("lock andw %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_and_u16(uint16_t *p, uint16_t val)
{
	__asm__ __volatile__("lock andw %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_and_i32(int32_t *p, int32_t val)
{
	__asm__ __volatile__("lock andl %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_and_u32(uint32_t *p, uint32_t val)
{
	__asm__ __volatile__("lock andl %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_and_i64(int64_t *p, int64_t val)
{
	__asm__ __volatile__("lock andq %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_and_u64(uint64_t *p, uint64_t val)
{
	__asm__ __volatile__("lock andq %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_and_size(size_t *p, size_t val)
{
	__asm__ __volatile__("lock andq %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_and_native(lf_native_word *p, lf_native_word val)
{
	__asm__ __volatile__("lock andq %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_or_iptr(intptr_t *p, intptr_t val)
{
	__asm__ __volatile__("lock orq %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_or_uptr(uintptr_t *p, uintptr_t val)
{
	__asm__ __volatile__("lock orq %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_or_char(signed char *p, signed char val)
{
	__asm__ __volatile__("lock orb %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_or_uchar(unsigned char *p, unsigned char val)
{
	__asm__ __volatile__("lock orb %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_or_short(short *p, short val)
{
	__asm__ __volatile__("lock orw %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_or_ushort(unsigned short *p, unsigned short val)
{
	__asm__ __volatile__("lock orw %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_or_int(int *p, int val)
{
	__asm__ __volatile__("lock orl %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_or_uint(unsigned int *p, unsigned int val)
{
	__asm__ __volatile__("lock orl %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_or_long(long *p, long val)
{
	__asm__ __volatile__("lock orq %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_or_ulong(unsigned long *p, unsigned long val)
{
	__asm__ __volatile__("lock orq %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_or_i8(int8_t *p, int8_t val)
{
	__asm__ __volatile__("lock orb %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_or_u8(uint8_t *p, uint8_t val)
{
	__asm__ __volatile__("lock orb %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_or_i16(int16_t *p, int16_t val)
{
	__asm__ __volatile__("lock orw %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_or_u16(uint16_t *p, uint16_t val)
{
	__asm__ __volatile__("lock orw %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_or_i32(int32_t *p, int32_t val)
{
	__asm__ __volatile__("lock orl %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_or_u32(uint32_t *p, uint32_t val)
{
	__asm__ __volatile__("lock orl %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_or_i64(int64_t *p, int64_t val)
{
	__asm__ __volatile__("lock orq %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_or_u64(uint64_t *p, uint64_t val)
{
	__asm__ __volatile__("lock orq %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_or_size(size_t *p, size_t val)
{
	__asm__ __volatile__("lock orq %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_or_native(lf_native_word *p, lf_native_word val)
{
	__asm__ __volatile__("lock orq %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_xor_iptr(intptr_t *p, intptr_t val)
{
	__asm__ __volatile__("lock xorq %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_xor_uptr(uintptr_t *p, uintptr_t val)
{
	__asm__ __volatile__("lock xorq %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_xor_char(signed char *p, signed char val)
{
	__asm__ __volatile__("lock xorb %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_xor_uchar(unsigned char *p, unsigned char val)
{
	__asm__ __volatile__("lock xorb %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_xor_short(short *p, short val)
{
	__asm__ __volatile__("lock xorw %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_xor_ushort(unsigned short *p, unsigned short val)
{
	__asm__ __volatile__("lock xorw %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_xor_int(int *p, int val)
{
	__asm__ __volatile__("lock xorl %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_xor_uint(unsigned int *p, unsigned int val)
{
	__asm__ __volatile__("lock xorl %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_xor_long(long *p, long val)
{
	__asm__ __volatile__("lock xorq %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_xor_ulong(unsigned long *p, unsigned long val)
{
	__asm__ __volatile__("lock xorq %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_xor_i8(int8_t *p, int8_t val)
{
	__asm__ __volatile__("lock xorb %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_xor_u8(uint8_t *p, uint8_t val)
{
	__asm__ __volatile__("lock xorb %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_xor_i16(int16_t *p, int16_t val)
{
	__asm__ __volatile__("lock xorw %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_xor_u16(uint16_t *p, uint16_t val)
{
	__asm__ __volatile__("lock xorw %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_xor_i32(int32_t *p, int32_t val)
{
	__asm__ __volatile__("lock xorl %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_xor_u32(uint32_t *p, uint32_t val)
{
	__asm__ __volatile__("lock xorl %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_xor_i64(int64_t *p, int64_t val)
{
	__asm__ __volatile__("lock xorq %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_xor_u64(uint64_t *p, uint64_t val)
{
	__asm__ __volatile__("lock xorq %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_xor_size(size_t *p, size_t val)
{
	__asm__ __volatile__("lock xorq %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static void lf_op_xor_native(lf_native_word *p, lf_native_word val)
{
	__asm__ __volatile__("lock xorq %1, %0"
			     : "+m"(*p)
			     : "r"(val)
			     : "memory", "cc");
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_bts_iptr(intptr_t *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btsq %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint64_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_bts_uptr(uintptr_t *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btsq %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint64_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_bts_char(signed char *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btsw %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint16_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_bts_uchar(unsigned char *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btsw %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint16_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_bts_short(short *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btsw %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint16_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_bts_ushort(unsigned short *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btsw %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint16_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_bts_int(int *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btsl %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint32_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_bts_uint(unsigned int *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btsl %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint32_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_bts_long(long *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btsq %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint64_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_bts_ulong(unsigned long *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btsq %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint64_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_bts_i8(int8_t *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btsw %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint16_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_bts_u8(uint8_t *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btsw %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint16_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_bts_i16(int16_t *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btsw %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint16_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_bts_u16(uint16_t *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btsw %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint16_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_bts_i32(int32_t *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btsl %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint32_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_bts_u32(uint32_t *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btsl %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint32_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_bts_i64(int64_t *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btsq %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint64_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_bts_u64(uint64_t *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btsq %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint64_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_bts_size(size_t *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btsq %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint64_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_bts_native(lf_native_word *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btsq %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint64_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btc_iptr(intptr_t *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btcq %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint64_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btc_uptr(uintptr_t *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btcq %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint64_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btc_char(signed char *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btcw %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint16_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btc_uchar(unsigned char *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btcw %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint16_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btc_short(short *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btcw %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint16_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btc_ushort(unsigned short *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btcw %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint16_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btc_int(int *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btcl %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint32_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btc_uint(unsigned int *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btcl %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint32_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btc_long(long *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btcq %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint64_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btc_ulong(unsigned long *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btcq %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint64_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btc_i8(int8_t *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btcw %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint16_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btc_u8(uint8_t *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btcw %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint16_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btc_i16(int16_t *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btcw %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint16_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btc_u16(uint16_t *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btcw %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint16_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btc_i32(int32_t *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btcl %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint32_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btc_u32(uint32_t *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btcl %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint32_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btc_i64(int64_t *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btcq %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint64_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btc_u64(uint64_t *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btcq %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint64_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btc_size(size_t *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btcq %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint64_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btc_native(lf_native_word *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btcq %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint64_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btr_iptr(intptr_t *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btrq %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint64_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btr_uptr(uintptr_t *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btrq %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint64_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btr_char(signed char *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btrw %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint16_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btr_uchar(unsigned char *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btrw %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint16_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btr_short(short *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btrw %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint16_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btr_ushort(unsigned short *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btrw %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint16_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btr_int(int *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btrl %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint32_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btr_uint(unsigned int *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btrl %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint32_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btr_long(long *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btrq %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint64_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btr_ulong(unsigned long *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btrq %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint64_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btr_i8(int8_t *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btrw %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint16_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btr_u8(uint8_t *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btrw %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint16_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btr_i16(int16_t *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btrw %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint16_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btr_u16(uint16_t *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btrw %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint16_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btr_i32(int32_t *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btrl %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint32_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btr_u32(uint32_t *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btrl %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint32_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btr_i64(int64_t *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btrq %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint64_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btr_u64(uint64_t *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btrq %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint64_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btr_size(size_t *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btrq %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint64_t)index)
			     : "memory", "cc");
	return cf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_btr_native(lf_native_word *p, unsigned int index)
{
	bool cf;
	__asm__ __volatile__("lock btrq %2, %0"
			     : "+m"(*p), "=@ccc"(cf)
			     : "r"((uint64_t)index)
			     : "memory", "cc");
	return cf;
}

/* I was (and still am) unsure what the best approach to DCAS's interface is. I grappled
 * between uint32_t *, uint32_t[2], and a union of uint64_t & uint32_t[2] (using
 * dcas_u32 as an example). In the end, I decide on the union approach for a few
 * reasons:
 * - It bakes alignment into the type. You can't accidentally pass a uint32_t *
 *   that isn't 8 byte aligned.
 * - It makes returning the original value from casx simpler.
 * - Using a union allows us to following strict aliasing rules without using
 *   the __may_alias__ builtins.
 */
typedef union lf_dcas_packed_ptr lf_dcas_packed_ptr;
typedef union lf_dcas_packed_iptr lf_dcas_packed_iptr;
typedef union lf_dcas_packed_uptr lf_dcas_packed_uptr;
typedef union lf_dcas_packed_char lf_dcas_packed_char;
typedef union lf_dcas_packed_uchar lf_dcas_packed_uchar;
typedef union lf_dcas_packed_short lf_dcas_packed_short;
typedef union lf_dcas_packed_ushort lf_dcas_packed_ushort;
typedef union lf_dcas_packed_int lf_dcas_packed_int;
typedef union lf_dcas_packed_uint lf_dcas_packed_uint;
typedef union lf_dcas_packed_long lf_dcas_packed_long;
typedef union lf_dcas_packed_ulong lf_dcas_packed_ulong;
typedef union lf_dcas_packed_i8 lf_dcas_packed_i8;
typedef union lf_dcas_packed_u8 lf_dcas_packed_u8;
typedef union lf_dcas_packed_i16 lf_dcas_packed_i16;
typedef union lf_dcas_packed_u16 lf_dcas_packed_u16;
typedef union lf_dcas_packed_i32 lf_dcas_packed_i32;
typedef union lf_dcas_packed_u32 lf_dcas_packed_u32;
typedef union lf_dcas_packed_i64 lf_dcas_packed_i64;
typedef union lf_dcas_packed_u64 lf_dcas_packed_u64;
typedef union lf_dcas_packed_size lf_dcas_packed_size;
typedef union lf_dcas_packed_native lf_dcas_packed_native;

union lf_dcas_packed_ptr {
	void *tuple[2];
} LF_ATTR_ALIGNED(16);
union lf_dcas_packed_iptr {
	intptr_t tuple[2];
} LF_ATTR_ALIGNED(16);
union lf_dcas_packed_uptr {
	uintptr_t tuple[2];
} LF_ATTR_ALIGNED(16);
union lf_dcas_packed_char {
	uint16_t scalar;
	signed char tuple[2];
} LF_ATTR_ALIGNED(2);
union lf_dcas_packed_uchar {
	uint16_t scalar;
	unsigned char tuple[2];
} LF_ATTR_ALIGNED(2);
union lf_dcas_packed_short {
	uint32_t scalar;
	short tuple[2];
} LF_ATTR_ALIGNED(4);
union lf_dcas_packed_ushort {
	uint32_t scalar;
	unsigned short tuple[2];
} LF_ATTR_ALIGNED(4);
union lf_dcas_packed_int {
	uint64_t scalar;
	int tuple[2];
} LF_ATTR_ALIGNED(8);
union lf_dcas_packed_uint {
	uint64_t scalar;
	unsigned int tuple[2];
} LF_ATTR_ALIGNED(8);
union lf_dcas_packed_long {
	long tuple[2];
} LF_ATTR_ALIGNED(16);
union lf_dcas_packed_ulong {
	unsigned long tuple[2];
} LF_ATTR_ALIGNED(16);
union lf_dcas_packed_i8 {
	int16_t scalar;
	int8_t tuple[2];
} LF_ATTR_ALIGNED(2);
union lf_dcas_packed_u8 {
	uint16_t scalar;
	uint8_t tuple[2];
} LF_ATTR_ALIGNED(2);
union lf_dcas_packed_i16 {
	int32_t scalar;
	int16_t tuple[2];
} LF_ATTR_ALIGNED(4);
union lf_dcas_packed_u16 {
	uint32_t scalar;
	uint16_t tuple[2];
} LF_ATTR_ALIGNED(4);
union lf_dcas_packed_i32 {
	int64_t scalar;
	int32_t tuple[2];
} LF_ATTR_ALIGNED(8);
union lf_dcas_packed_u32 {
	uint64_t scalar;
	uint32_t tuple[2];
} LF_ATTR_ALIGNED(8);
union lf_dcas_packed_i64 {
	int64_t tuple[2];
} LF_ATTR_ALIGNED(16);
union lf_dcas_packed_u64 {
	uint64_t tuple[2];
} LF_ATTR_ALIGNED(16);
union lf_dcas_packed_size {
	size_t tuple[2];
} LF_ATTR_ALIGNED(16);
union lf_dcas_packed_native {
	lf_native_word tuple[2];
} LF_ATTR_ALIGNED(16);

LF_ATTR_ALWAYS_INLINE
static bool lf_op_dcas_ptr(lf_dcas_packed_ptr *p, lf_dcas_packed_ptr val_old,
			   lf_dcas_packed_ptr val_new)
{
	bool zf;
	__asm__ __volatile__("lock cmpxchg16b %0"
			     : "+m"(*p->tuple), "=@ccz"(zf),
			       "+a"(val_old.tuple[0]), "+d"(val_old.tuple[1])
			     : "b"(val_new.tuple[0]), "c"(val_new.tuple[1])
			     : "memory", "cc");
	return zf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_dcas_iptr(lf_dcas_packed_iptr *p, lf_dcas_packed_iptr val_old,
			    lf_dcas_packed_iptr val_new)
{
	bool zf;
	__asm__ __volatile__("lock cmpxchg16b %0"
			     : "+m"(*p->tuple), "=@ccz"(zf),
			       "+a"(val_old.tuple[0]), "+d"(val_old.tuple[1])
			     : "b"(val_new.tuple[0]), "c"(val_new.tuple[1])
			     : "memory", "cc");
	return zf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_dcas_uptr(lf_dcas_packed_uptr *p, lf_dcas_packed_uptr val_old,
			    lf_dcas_packed_uptr val_new)
{
	bool zf;
	__asm__ __volatile__("lock cmpxchg16b %0"
			     : "+m"(*p->tuple), "=@ccz"(zf),
			       "+a"(val_old.tuple[0]), "+d"(val_old.tuple[1])
			     : "b"(val_new.tuple[0]), "c"(val_new.tuple[1])
			     : "memory", "cc");
	return zf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_dcas_char(lf_dcas_packed_char *p, lf_dcas_packed_char val_old,
			    lf_dcas_packed_char val_new)
{
	return lf_op_cas_u16(&p->scalar, val_old.scalar, val_new.scalar);
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_dcas_uchar(lf_dcas_packed_uchar *p,
			     lf_dcas_packed_uchar val_old,
			     lf_dcas_packed_uchar val_new)
{
	return lf_op_cas_u16(&p->scalar, val_old.scalar, val_new.scalar);
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_dcas_short(lf_dcas_packed_short *p,
			     lf_dcas_packed_short val_old,
			     lf_dcas_packed_short val_new)
{
	return lf_op_cas_u32(&p->scalar, val_old.scalar, val_new.scalar);
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_dcas_ushort(lf_dcas_packed_ushort *p,
			      lf_dcas_packed_ushort val_old,
			      lf_dcas_packed_ushort val_new)
{
	return lf_op_cas_u32(&p->scalar, val_old.scalar, val_new.scalar);
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_dcas_int(lf_dcas_packed_int *p, lf_dcas_packed_int val_old,
			   lf_dcas_packed_int val_new)
{
	return lf_op_cas_u64(&p->scalar, val_old.scalar, val_new.scalar);
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_dcas_uint(lf_dcas_packed_uint *p, lf_dcas_packed_uint val_old,
			    lf_dcas_packed_uint val_new)
{
	return lf_op_cas_u64(&p->scalar, val_old.scalar, val_new.scalar);
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_dcas_long(lf_dcas_packed_long *p, lf_dcas_packed_long val_old,
			    lf_dcas_packed_long val_new)
{
	bool zf;
	__asm__ __volatile__("lock cmpxchg16b %0"
			     : "+m"(*p->tuple), "=@ccz"(zf),
			       "+a"(val_old.tuple[0]), "+d"(val_old.tuple[1])
			     : "b"(val_new.tuple[0]), "c"(val_new.tuple[1])
			     : "memory", "cc");
	return zf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_dcas_ulong(lf_dcas_packed_ulong *p,
			     lf_dcas_packed_ulong val_old,
			     lf_dcas_packed_ulong val_new)
{
	bool zf;
	__asm__ __volatile__("lock cmpxchg16b %0"
			     : "+m"(*p->tuple), "=@ccz"(zf),
			       "+a"(val_old.tuple[0]), "+d"(val_old.tuple[1])
			     : "b"(val_new.tuple[0]), "c"(val_new.tuple[1])
			     : "memory", "cc");
	return zf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_dcas_i8(lf_dcas_packed_i8 *p, lf_dcas_packed_i8 val_old,
			  lf_dcas_packed_i8 val_new)
{
	return lf_op_cas_i16(&p->scalar, val_old.scalar, val_new.scalar);
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_dcas_u8(lf_dcas_packed_u8 *p, lf_dcas_packed_u8 val_old,
			  lf_dcas_packed_u8 val_new)
{
	return lf_op_cas_u16(&p->scalar, val_old.scalar, val_new.scalar);
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_dcas_i16(lf_dcas_packed_i16 *p, lf_dcas_packed_i16 val_old,
			   lf_dcas_packed_i16 val_new)
{
	return lf_op_cas_i32(&p->scalar, val_old.scalar, val_new.scalar);
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_dcas_u16(lf_dcas_packed_u16 *p, lf_dcas_packed_u16 val_old,
			   lf_dcas_packed_u16 val_new)
{
	return lf_op_cas_u32(&p->scalar, val_old.scalar, val_new.scalar);
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_dcas_i32(lf_dcas_packed_i32 *p, lf_dcas_packed_i32 val_old,
			   lf_dcas_packed_i32 val_new)
{
	return lf_op_cas_i64(&p->scalar, val_old.scalar, val_new.scalar);
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_dcas_u32(lf_dcas_packed_u32 *p, lf_dcas_packed_u32 val_old,
			   lf_dcas_packed_u32 val_new)
{
	return lf_op_cas_u64(&p->scalar, val_old.scalar, val_new.scalar);
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_dcas_i64(lf_dcas_packed_i64 *p, lf_dcas_packed_i64 val_old,
			   lf_dcas_packed_i64 val_new)
{
	bool zf;
	__asm__ __volatile__("lock cmpxchg16b %0"
			     : "+m"(*p->tuple), "=@ccz"(zf),
			       "+a"(val_old.tuple[0]), "+d"(val_old.tuple[1])
			     : "b"(val_new.tuple[0]), "c"(val_new.tuple[1])
			     : "memory", "cc");
	return zf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_dcas_u64(lf_dcas_packed_u64 *p, lf_dcas_packed_u64 val_old,
			   lf_dcas_packed_u64 val_new)
{
	bool zf;
	__asm__ __volatile__("lock cmpxchg16b %0"
			     : "+m"(*p->tuple), "=@ccz"(zf),
			       "+a"(val_old.tuple[0]), "+d"(val_old.tuple[1])
			     : "b"(val_new.tuple[0]), "c"(val_new.tuple[1])
			     : "memory", "cc");
	return zf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_dcas_size(lf_dcas_packed_size *p, lf_dcas_packed_size val_old,
			    lf_dcas_packed_size val_new)
{
	bool zf;
	__asm__ __volatile__("lock cmpxchg16b %0"
			     : "+m"(*p->tuple), "=@ccz"(zf),
			       "+a"(val_old.tuple[0]), "+d"(val_old.tuple[1])
			     : "b"(val_new.tuple[0]), "c"(val_new.tuple[1])
			     : "memory", "cc");
	return zf;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_op_dcas_native(lf_dcas_packed_native *p,
			      lf_dcas_packed_native val_old,
			      lf_dcas_packed_native val_new)
{
	bool zf;
	__asm__ __volatile__("lock cmpxchg16b %0"
			     : "+m"(*p->tuple), "=@ccz"(zf),
			       "+a"(val_old.tuple[0]), "+d"(val_old.tuple[1])
			     : "b"(val_new.tuple[0]), "c"(val_new.tuple[1])
			     : "memory", "cc");
	return zf;
}

LF_ATTR_ALWAYS_INLINE
static lf_dcas_packed_ptr lf_op_dcasx_ptr(lf_dcas_packed_ptr *p,
					  lf_dcas_packed_ptr val_old,
					  lf_dcas_packed_ptr val_new,
					  bool *success)
{
	__asm__ __volatile__("lock cmpxchg16b %0"
			     : "+m"(*p->tuple), "=@ccz"(*success),
			       "+a"(val_old.tuple[0]), "+d"(val_old.tuple[1])
			     : "b"(val_new.tuple[0]), "c"(val_new.tuple[1])
			     : "memory", "cc");
	return val_old;
}

LF_ATTR_ALWAYS_INLINE
static lf_dcas_packed_iptr lf_op_dcasx_iptr(lf_dcas_packed_iptr *p,
					    lf_dcas_packed_iptr val_old,
					    lf_dcas_packed_iptr val_new,
					    bool *success)
{
	__asm__ __volatile__("lock cmpxchg16b %0"
			     : "+m"(*p->tuple), "=@ccz"(*success),
			       "+a"(val_old.tuple[0]), "+d"(val_old.tuple[1])
			     : "b"(val_new.tuple[0]), "c"(val_new.tuple[1])
			     : "memory", "cc");
	return val_old;
}

LF_ATTR_ALWAYS_INLINE
static lf_dcas_packed_uptr lf_op_dcasx_uptr(lf_dcas_packed_uptr *p,
					    lf_dcas_packed_uptr val_old,
					    lf_dcas_packed_uptr val_new,
					    bool *success)
{
	__asm__ __volatile__("lock cmpxchg16b %0"
			     : "+m"(*p->tuple), "=@ccz"(*success),
			       "+a"(val_old.tuple[0]), "+d"(val_old.tuple[1])
			     : "b"(val_new.tuple[0]), "c"(val_new.tuple[1])
			     : "memory", "cc");
	return val_old;
}

LF_ATTR_ALWAYS_INLINE
static lf_dcas_packed_char lf_op_dcasx_char(lf_dcas_packed_char *p,
					    lf_dcas_packed_char val_old,
					    lf_dcas_packed_char val_new,
					    bool *success)
{
	lf_dcas_packed_char val_orig;
	val_orig.scalar =
		lf_op_casx_u16(&p->scalar, val_old.scalar, val_new.scalar);
	*success = val_orig.scalar == val_old.scalar;
	return val_orig;
}

LF_ATTR_ALWAYS_INLINE
static lf_dcas_packed_uchar lf_op_dcasx_uchar(lf_dcas_packed_uchar *p,
					      lf_dcas_packed_uchar val_old,
					      lf_dcas_packed_uchar val_new,
					      bool *success)
{
	lf_dcas_packed_uchar val_orig;
	val_orig.scalar =
		lf_op_casx_u16(&p->scalar, val_old.scalar, val_new.scalar);
	*success = val_orig.scalar == val_old.scalar;
	return val_orig;
}

LF_ATTR_ALWAYS_INLINE
static lf_dcas_packed_short lf_op_dcasx_short(lf_dcas_packed_short *p,
					      lf_dcas_packed_short val_old,
					      lf_dcas_packed_short val_new,
					      bool *success)
{
	lf_dcas_packed_short val_orig;
	val_orig.scalar =
		lf_op_casx_u32(&p->scalar, val_old.scalar, val_new.scalar);
	*success = val_orig.scalar == val_old.scalar;
	return val_orig;
}

LF_ATTR_ALWAYS_INLINE
static lf_dcas_packed_ushort lf_op_dcasx_ushort(lf_dcas_packed_ushort *p,
						lf_dcas_packed_ushort val_old,
						lf_dcas_packed_ushort val_new,
						bool *success)
{
	lf_dcas_packed_ushort val_orig;
	val_orig.scalar =
		lf_op_casx_u32(&p->scalar, val_old.scalar, val_new.scalar);
	*success = val_orig.scalar == val_old.scalar;
	return val_orig;
}

LF_ATTR_ALWAYS_INLINE
static lf_dcas_packed_int lf_op_dcasx_int(lf_dcas_packed_int *p,
					  lf_dcas_packed_int val_old,
					  lf_dcas_packed_int val_new,
					  bool *success)
{
	lf_dcas_packed_int val_orig;
	val_orig.scalar =
		lf_op_casx_u64(&p->scalar, val_old.scalar, val_new.scalar);
	*success = val_orig.scalar == val_old.scalar;
	return val_orig;
}

LF_ATTR_ALWAYS_INLINE
static lf_dcas_packed_uint lf_op_dcasx_uint(lf_dcas_packed_uint *p,
					    lf_dcas_packed_uint val_old,
					    lf_dcas_packed_uint val_new,
					    bool *success)
{
	lf_dcas_packed_uint val_orig;
	val_orig.scalar =
		lf_op_casx_u64(&p->scalar, val_old.scalar, val_new.scalar);
	*success = val_orig.scalar == val_old.scalar;
	return val_orig;
}

LF_ATTR_ALWAYS_INLINE
static lf_dcas_packed_long lf_op_dcasx_long(lf_dcas_packed_long *p,
					    lf_dcas_packed_long val_old,
					    lf_dcas_packed_long val_new,
					    bool *success)
{
	__asm__ __volatile__("lock cmpxchg16b %0"
			     : "+m"(*p->tuple), "=@ccz"(*success),
			       "+a"(val_old.tuple[0]), "+d"(val_old.tuple[1])
			     : "b"(val_new.tuple[0]), "c"(val_new.tuple[1])
			     : "memory", "cc");
	return val_old;
}

LF_ATTR_ALWAYS_INLINE
static lf_dcas_packed_ulong lf_op_dcasx_ulong(lf_dcas_packed_ulong *p,
					      lf_dcas_packed_ulong val_old,
					      lf_dcas_packed_ulong val_new,
					      bool *success)
{
	__asm__ __volatile__("lock cmpxchg16b %0"
			     : "+m"(*p->tuple), "=@ccz"(*success),
			       "+a"(val_old.tuple[0]), "+d"(val_old.tuple[1])
			     : "b"(val_new.tuple[0]), "c"(val_new.tuple[1])
			     : "memory", "cc");
	return val_old;
}

LF_ATTR_ALWAYS_INLINE
static lf_dcas_packed_i8 lf_op_dcasx_i8(lf_dcas_packed_i8 *p,
					lf_dcas_packed_i8 val_old,
					lf_dcas_packed_i8 val_new,
					bool *success)
{
	lf_dcas_packed_i8 val_orig;
	val_orig.scalar =
		lf_op_casx_i16(&p->scalar, val_old.scalar, val_new.scalar);
	*success = val_orig.scalar == val_old.scalar;
	return val_orig;
}

LF_ATTR_ALWAYS_INLINE
static lf_dcas_packed_u8 lf_op_dcasx_u8(lf_dcas_packed_u8 *p,
					lf_dcas_packed_u8 val_old,
					lf_dcas_packed_u8 val_new,
					bool *success)
{
	lf_dcas_packed_u8 val_orig;
	val_orig.scalar =
		lf_op_casx_u16(&p->scalar, val_old.scalar, val_new.scalar);
	*success = val_orig.scalar == val_old.scalar;
	return val_orig;
}

LF_ATTR_ALWAYS_INLINE
static lf_dcas_packed_i16 lf_op_dcasx_i16(lf_dcas_packed_i16 *p,
					  lf_dcas_packed_i16 val_old,
					  lf_dcas_packed_i16 val_new,
					  bool *success)
{
	lf_dcas_packed_i16 val_orig;
	val_orig.scalar =
		lf_op_casx_i32(&p->scalar, val_old.scalar, val_new.scalar);
	*success = val_orig.scalar == val_old.scalar;
	return val_orig;
}

LF_ATTR_ALWAYS_INLINE
static lf_dcas_packed_u16 lf_op_dcasx_u16(lf_dcas_packed_u16 *p,
					  lf_dcas_packed_u16 val_old,
					  lf_dcas_packed_u16 val_new,
					  bool *success)
{
	lf_dcas_packed_u16 val_orig;
	val_orig.scalar =
		lf_op_casx_u32(&p->scalar, val_old.scalar, val_new.scalar);
	*success = val_orig.scalar == val_old.scalar;
	return val_orig;
}

LF_ATTR_ALWAYS_INLINE
static lf_dcas_packed_i32 lf_op_dcasx_i32(lf_dcas_packed_i32 *p,
					  lf_dcas_packed_i32 val_old,
					  lf_dcas_packed_i32 val_new,
					  bool *success)
{
	lf_dcas_packed_i32 val_orig;
	val_orig.scalar =
		lf_op_casx_i64(&p->scalar, val_old.scalar, val_new.scalar);
	*success = val_orig.scalar == val_old.scalar;
	return val_orig;
}

LF_ATTR_ALWAYS_INLINE
static lf_dcas_packed_u32 lf_op_dcasx_u32(lf_dcas_packed_u32 *p,
					  lf_dcas_packed_u32 val_old,
					  lf_dcas_packed_u32 val_new,
					  bool *success)
{
	lf_dcas_packed_u32 val_orig;
	val_orig.scalar =
		lf_op_casx_u64(&p->scalar, val_old.scalar, val_new.scalar);
	*success = val_orig.scalar == val_old.scalar;
	return val_orig;
}

LF_ATTR_ALWAYS_INLINE
static lf_dcas_packed_i64 lf_op_dcasx_i64(lf_dcas_packed_i64 *p,
					  lf_dcas_packed_i64 val_old,
					  lf_dcas_packed_i64 val_new,
					  bool *success)
{
	__asm__ __volatile__("lock cmpxchg16b %0"
			     : "+m"(*p->tuple), "=@ccz"(*success),
			       "+a"(val_old.tuple[0]), "+d"(val_old.tuple[1])
			     : "b"(val_new.tuple[0]), "c"(val_new.tuple[1])
			     : "memory", "cc");
	return val_old;
}

LF_ATTR_ALWAYS_INLINE
static lf_dcas_packed_u64 lf_op_dcasx_u64(lf_dcas_packed_u64 *p,
					  lf_dcas_packed_u64 val_old,
					  lf_dcas_packed_u64 val_new,
					  bool *success)
{
	__asm__ __volatile__("lock cmpxchg16b %0"
			     : "+m"(*p->tuple), "=@ccz"(*success),
			       "+a"(val_old.tuple[0]), "+d"(val_old.tuple[1])
			     : "b"(val_new.tuple[0]), "c"(val_new.tuple[1])
			     : "memory", "cc");
	return val_old;
}

LF_ATTR_ALWAYS_INLINE
static lf_dcas_packed_size lf_op_dcasx_size(lf_dcas_packed_size *p,
					    lf_dcas_packed_size val_old,
					    lf_dcas_packed_size val_new,
					    bool *success)
{
	__asm__ __volatile__("lock cmpxchg16b %0"
			     : "+m"(*p->tuple), "=@ccz"(*success),
			       "+a"(val_old.tuple[0]), "+d"(val_old.tuple[1])
			     : "b"(val_new.tuple[0]), "c"(val_new.tuple[1])
			     : "memory", "cc");
	return val_old;
}

LF_ATTR_ALWAYS_INLINE
static lf_dcas_packed_native lf_op_dcasx_native(lf_dcas_packed_native *p,
						lf_dcas_packed_native val_old,
						lf_dcas_packed_native val_new,
						bool *success)
{
	__asm__ __volatile__("lock cmpxchg16b %0"
			     : "+m"(*p->tuple), "=@ccz"(*success),
			       "+a"(val_old.tuple[0]), "+d"(val_old.tuple[1])
			     : "b"(val_new.tuple[0]), "c"(val_new.tuple[1])
			     : "memory", "cc");
	return val_old;
}

#endif /* LF_OP_X64_H */
