#include <stdbool.h>
#include <stdio.h>

#include <lf_bitstring.h>
#include <lf_op.h>
#include <lf_platform.h>

#define NBITS (512)

/* lf_bitstring uses lf_native_word as the underlying type to store bits, 
 * which matches the native register size for maximum efficiency.
 *
 * lf_bitstring_init() can accept any buffer, so you do not need to declare
 * a dedicated array like below. If you use a generic buffer, make sure:
 *   1. The buffer is aligned to LF_BITSTRING_BITS_ALIGNMENT.
 *   2. The buffer is at least LF_BITSTRING_REQUIRED_SIZE(nbits) bytes.
 */
static lf_native_word bitstring_bits[LF_BITSTRING_REQUIRED_WORDS(NBITS)];

lf_bitstring *bitstring_global = NULL;

int main(void)
{
	lf_bitstring bitstring;

	/* Initialize the bitstring by setting bits to 0 and returns the
	 * max bits it can store.
	 */
	unsigned int nbits_max = lf_bitstring_init(&bitstring, bitstring_bits,
						   sizeof(bitstring_bits), 0);
	if (nbits_max < NBITS) {
		fprintf(stderr, "lf_bitstring_init() is broken.\n");
		return 1;
	}
	printf("Created a bitstring with %d bits\n", NBITS);

	/* lf_bitstring_init() just sets some variables in bitstring. Before
	 * we make it visible through the global pointer, use a fence to ensure
	 * stores to bitstring will be seen first.
	 */
	lf_fence_store();
	lf_op_store_ptr(&bitstring_global, &bitstring);

	/* Atomically set each bit */
	for (unsigned int i = 0; i < NBITS; ++i) {
		bool bit_prev_val = lf_bitstring_test_and_set(&bitstring, i);
		if (bit_prev_val) {
			fprintf(stderr, "Bit %u was not 0\n", i);
			return 1;
		}
	}

	printf("Set %d bits to 1\n", NBITS);

	/* Atomically zero each bit */
	for (unsigned int i = 0; i < NBITS; ++i) {
		bool bit_prev_val = lf_bitstring_test_and_zero(&bitstring, i);
		if (!bit_prev_val) {
			fprintf(stderr, "Bit %u was not 1\n", i);
			return 1;
		}
	}

	printf("Set %d bits to 0\n", NBITS);

	return 0;
}
