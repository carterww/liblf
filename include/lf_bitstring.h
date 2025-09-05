/* SPDX-License-Identifier: MIT
 * Copyright (c) 2025 Carter Williams
 */

/* This file implements a lock-free bitstring.
 *
 * It is important to note that the caller is responsible for passing
 * valid arguments to the functions; lf_bitstring DOES NOT do any
 * NULL or bounds checking if asserts are not compiled.
 */

#ifndef LF_BITSTRING_H
#define LF_BITSTRING_H

#include <limits.h>
#include <stdbool.h>
#include <string.h>

#include <lf_cc.h>
#include <lf_op.h>
#include <lf_platform.h>

/* All supported platforms use 8-bit bytes */
#define LF_BITSTRING_BYTE_BITS (8)
#define LF_BITSTRING_WORD_SIZE ((unsigned int)sizeof(lf_native_word))
#define LF_BITSTRING_WORD_BITS (LF_BITSTRING_WORD_SIZE * LF_BITSTRING_BYTE_BITS)

/* This is the alignment of lf_bitstring->bits. The buffer you pass to
 * lf_bitstring_init() must have this alignment.
 */
#define LF_BITSTRING_BITS_ALIGNMENT (lf_alignof(lf_native_word))

/* This returns the number of lf_native_words required for lf_bitstring->bits
 * requires for nbits to hold nbits.
 */
#define LF_BITSTRING_REQUIRED_WORDS(nbits) \
	((((nbits) + LF_BITSTRING_WORD_BITS - 1) / LF_BITSTRING_WORD_BITS))

/* This returns the size (in bytes) lf_bitstring->bits requires for nbits. */
#define LF_BITSTRING_REQUIRED_SIZE(nbits) \
	(LF_BITSTRING_REQUIRED_WORDS(nbits) * LF_BITSTRING_WORD_SIZE)

/* lf_bitstring_find_set_and_zero() and lf_bitstring_find_zero_and_set()
 * try to find a bit and atomically change its value. This constant determines
 * the maximum number of times these functions will attempt an operation on a
 * word. This prevents a thread from continually spinning on a contentious word.
 */
#define LF_BITSTRING_WORD_RETRY_MAX (4)

typedef struct lf_bitstring lf_bitstring;

struct lf_bitstring {
	unsigned int nwords;
	lf_native_word *bits;
};

/* Initializes bitstring from a buffer bits of size (bytes) bits_size. It also
 * sets all the bits to bits_val.
 *
 * Returns the number of bits the bitstring can store.
 *
 * This operation is not thread-safe.
 */
LF_ATTR_ALWAYS_INLINE
static unsigned int lf_bitstring_init(lf_bitstring *bitstring, void *bits,
				      unsigned int bits_size, bool bits_val)
{
	size_t size_actual;

	bitstring->nwords = bits_size / LF_BITSTRING_WORD_SIZE;
	bitstring->bits = bits;
	size_actual = bitstring->nwords * LF_BITSTRING_WORD_SIZE;

	memset(bits, bits_val ? 0xff : 0, size_actual);

	return (unsigned int)(size_actual * LF_BITSTRING_BYTE_BITS);
}

/* Resets all the bits in the bitstring to bits_val.
 *
 * This operation is not thread-safe.
 */
LF_ATTR_ALWAYS_INLINE
static void lf_bitstring_reset(lf_bitstring *bitstring, bool bits_val)
{
	memset(bitstring->bits, bits_val ? 0xff : 0,
	       bitstring->nwords * LF_BITSTRING_WORD_SIZE);
}

/* Tests bit at LF_BITSTRING_WORD_BITS * word_index + bit_index in the bitstring.
 *
 * Returns true if the bit was set, false if the bit was zero.
 */
LF_ATTR_ALWAYS_INLINE
static bool lf_bitstring_test_word(lf_bitstring *bitstring,
				   unsigned int word_index,
				   unsigned int bit_index)
{
	lf_native_word word = lf_op_load_native(&bitstring->bits[word_index]);
	return (word & ((lf_native_word)1 << bit_index)) != 0;
}

/* Sets bit at LF_BITSTRING_WORD_BITS * word_index + bit_index in the bitstring.
 *
 * Returns true if the bit was set prior to the operation, false if the
 * bit was zero prior to the operation.
 */
LF_ATTR_ALWAYS_INLINE
static bool lf_bitstring_test_and_set_word(lf_bitstring *bitstring,
					   unsigned int word_index,
					   unsigned int bit_index)
{
	return lf_op_bts_native(&bitstring->bits[word_index], bit_index);
}

/* Complements bit at LF_BITSTRING_WORD_BITS * word_index + bit_index in the
 * bitstring.
 *
 * Returns true if the bit was set prior to the operation, false if the
 * bit was zero prior to the operation.
 */
LF_ATTR_ALWAYS_INLINE
static bool lf_bitstring_test_and_complement_word(lf_bitstring *bitstring,
						  unsigned int word_index,
						  unsigned int bit_index)
{
	return lf_op_btc_native(&bitstring->bits[word_index], bit_index);
}

/* Zeros bit at LF_BITSTRING_WORD_BITS * word_index + bit_index in the
 * bitstring.
 *
 * Returns true if the bit was set prior to the operation, false if the
 * bit was zero prior to the operation.
 */
LF_ATTR_ALWAYS_INLINE
static bool lf_bitstring_test_and_zero_word(lf_bitstring *bitstring,
					    unsigned int word_index,
					    unsigned int bit_index)
{
	return lf_op_btr_native(&bitstring->bits[word_index], bit_index);
}

/* Helper macros to convert an absolute bit index to an offset into the
 * array and an offset into the word.
 */
#define LF_BS_OFF(index) ((index) / LF_BITSTRING_WORD_BITS)
#define LF_BS_OFF_INTO(index) ((index) & (LF_BITSTRING_WORD_BITS - 1))

/* Tests bit at index in the bitstring. This function differs from
 * lf_bitstring_test_word() because it takes the bit's absolute index
 * into the bitstring.
 *
 * Returns true if the bit was set, false if the bit was zero.
 */
LF_ATTR_ALWAYS_INLINE
static bool lf_bitstring_test(lf_bitstring *bitstring, unsigned int index)
{
	unsigned int word_index = LF_BS_OFF(index);
	unsigned int bit_index = LF_BS_OFF_INTO(index);
	return lf_bitstring_test_word(bitstring, word_index, bit_index);
}

/* Sets bit at index in the bitstring. This function differs from
 * lf_bitstring_test_and_set_word() because it takes the bit's absolute index
 * into the bitstring.
 *
 * Returns true if the bit was set prior to the operation, false if the
 * bit was zero prior to the operation.
 */
LF_ATTR_ALWAYS_INLINE
static bool lf_bitstring_test_and_set(lf_bitstring *bitstring,
				      unsigned int index)
{
	unsigned int word_index = LF_BS_OFF(index);
	unsigned int bit_index = LF_BS_OFF_INTO(index);
	return lf_bitstring_test_and_set_word(bitstring, word_index, bit_index);
}

/* Complements bit at index in the bitstring. This function differs from
 * lf_bitstring_test_and_complement_word() because it takes the bit's
 * absolute index into the bitstring.
 *
 * Returns true if the bit was set prior to the operation, false if the
 * bit was zero prior to the operation.
 */
LF_ATTR_ALWAYS_INLINE
static bool lf_bitstring_test_and_complement(lf_bitstring *bitstring,
					     unsigned int index)
{
	unsigned int word_index = LF_BS_OFF(index);
	unsigned int bit_index = LF_BS_OFF_INTO(index);
	return lf_bitstring_test_and_complement_word(bitstring, word_index,
						     bit_index);
}

/* Zeros bit at index in the bitstring. This function differs from
 * lf_bitstring_test_and_zero_word() because it takes the bit's
 * absolute index into the bitstring.
 *
 * Returns true if the bit was set prior to the operation, false if the
 * bit was zero prior to the operation.
 */
LF_ATTR_ALWAYS_INLINE
static bool lf_bitstring_test_and_zero(lf_bitstring *bitstring,
				       unsigned int index)
{
	unsigned int word_index = LF_BS_OFF(index);
	unsigned int bit_index = LF_BS_OFF_INTO(index);
	return lf_bitstring_test_and_zero_word(bitstring, word_index,
					       bit_index);
}

#undef LF_BS_OFF
#undef LF_BS_OFF_INTO

/* Internal function used to find the index of the next set bit in
 * the bitstring. It is not defined if the search starts from the
 * end or start of the word. Some architectures don't implement
 * trailing zero count.
 *
 * word CANNOT be zero.
 */
LF_ATTR_ALWAYS_INLINE
static unsigned int lf_bitstring_find_set_word(lf_native_word word)
{
	lf_native_word idx;
#if defined(LF_ARCH_X86_64)
	__asm__ __volatile__("tzcnt %1, %0" : "=r"(idx) : "rm"(word) : "cc");
#elif defined(LF_ARCH_ARM64) || defined(LF_ARCH_ARM)
	__asm__ __volatile__("clz %0, %1" : "=r"(idx) : "r"(word) :);
	idx = LF_BITSTRING_WORD_BITS - idx - 1;
#else
	idx = (lf_native_word)__builtin_ctzl(word);
	/* NOTE(Carter): Just in case long is 64 bits and lf_native_word is
	 * somehow 32 bits.
	 */
	idx = idx > LF_BITSTRING_WORD_BITS ? LF_BITSTRING_WORD_BITS : idx;
#endif
	return (unsigned int)idx;
}

/* IMPORTANT NOTE:
 *
 * The next set of functions search through the bitstring from lower memory
 * words to higher memory words. Since this is a lock-free bitstring, it
 * is possible that another thread sets a bit in a word that was already
 * checked. This could lead to a function returning UINT_MAX to indicate
 * no set or zero bit was found even though there truly is.
 *
 * You may want to do more than one pass over the bitstring if the bitstring
 * is changing frequently.
 */

/* Looks for a set bit in the bitstring starting at word word_index_start.
 *
 * Return the index of a set bit, UINT_MAX otherwise.
 */
LF_ATTR_ALWAYS_INLINE
static unsigned int lf_bitstring_next_set(lf_bitstring *bitstring,
					  unsigned int word_index_start)
{
	lf_native_word word;
	unsigned int bit_index;

	for (unsigned int i = word_index_start; i < bitstring->nwords; ++i) {
		word = lf_op_load_native(&bitstring->bits[i]);
		if (word == 0) {
			continue;
		}
		bit_index = lf_bitstring_find_set_word(word);
		return (i * LF_BITSTRING_WORD_BITS) + bit_index;
	}
	return UINT_MAX;
}

/* Looks for a zero bit in the bitstring starting at word word_index_start.
 *
 * Return the index of a set bit, UINT_MAX otherwise.
 */
LF_ATTR_ALWAYS_INLINE
static unsigned int lf_bitstring_next_zero(lf_bitstring *bitstring,
					   unsigned int word_index_start)
{
	lf_native_word word;
	unsigned int bit_index;

	for (unsigned int i = word_index_start; i < bitstring->nwords; ++i) {
		word = ~lf_op_load_native(&bitstring->bits[i]);
		if (word == 0) {
			continue;
		}
		bit_index = lf_bitstring_find_set_word(word);
		return (i * LF_BITSTRING_WORD_BITS) + bit_index;
	}
	return UINT_MAX;
}

/* Looks for a set bit in the bitstring starting the first word.
 *
 * Return the index of a set bit, UINT_MAX otherwise.
 */
LF_ATTR_ALWAYS_INLINE
static unsigned int lf_bitstring_find_set(lf_bitstring *bitstring)
{
	return lf_bitstring_next_set(bitstring, 0);
}

/* Looks for a zero bit in the bitstring starting the first word.
 *
 * Return the index of a set bit, UINT_MAX otherwise.
 */
LF_ATTR_ALWAYS_INLINE
static unsigned int lf_bitstring_find_zero(lf_bitstring *bitstring)
{
	return lf_bitstring_next_zero(bitstring, 0);
}

/* Attemps to find a set bit and atomically set it to zero.
 *
 * Returns the index of the zeroed bit if successful, UINT_MAX if no bit
 * could be zeroed.
 */
LF_ATTR_ALWAYS_INLINE
static unsigned int lf_bitstring_find_set_and_zero(lf_bitstring *bitstring)
{
	bool previous;
	lf_native_word word;
	unsigned int bit_index;

	for (unsigned int i = 0; i < bitstring->nwords; ++i) {
		unsigned int try_count = 0;

		while (1) {
			word = lf_op_load_native(&bitstring->bits[i]);
			if (word == 0) {
				break;
			}
			bit_index = lf_bitstring_find_set_word(word);
			previous = lf_bitstring_test_and_zero_word(bitstring, i,
								   bit_index);
			/* Ensure bit was still 1 by the time we zero it */
			if (previous) {
				return (i * LF_BITSTRING_WORD_BITS) + bit_index;
			}
			/* Someone else reset the bit already */
			if (++try_count > LF_BITSTRING_WORD_RETRY_MAX) {
				break;
			}
		}
	}
	return UINT_MAX;
}

/* Attemps to find a zero bit and atomically set it.
 *
 * Returns the index of the set bit if successful, UINT_MAX if no bit
 * could be set.
 */
LF_ATTR_ALWAYS_INLINE
static unsigned int lf_bitstring_find_zero_and_set(lf_bitstring *bitstring)
{
	bool previous;
	lf_native_word word;
	unsigned int bit_index;

	for (unsigned int i = 0; i < bitstring->nwords; ++i) {
		unsigned int try_count = 0;

		while (1) {
			word = ~lf_op_load_native(&bitstring->bits[i]);
			if (word == 0) {
				break;
			}
			bit_index = lf_bitstring_find_set_word(word);
			previous = lf_bitstring_test_and_set_word(bitstring, i,
								  bit_index);
			/* Ensure bit was still 0 by the time we set it */
			if (!previous) {
				return (i * LF_BITSTRING_WORD_BITS) + bit_index;
			}
			/* Someone else set the bit already */
			if (++try_count > LF_BITSTRING_WORD_RETRY_MAX) {
				break;
			}
		}
	}
	return UINT_MAX;
}

#endif /* LF_BITSTRING_H */
