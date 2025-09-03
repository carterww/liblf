#include <stdbool.h>
#include <stdio.h>

#include <lf_op.h>

int main(void)
{
	/* No fences are used here because all operations are done on counter.
	 * All modern architectures guarantee total ordering of operations
	 * on the same memory location.
	 */
	int counter = 0;
	do {
		/* Increment the counter by 1 atomically */
		lf_op_inc_int(&counter);
	} while (lf_op_load_int(&counter) < 10);
	printf("Counter: %d\n", counter);

	/* Perform an atomic swap on counter and get the previous value */
	int counter_old = lf_op_swap_int(&counter, 0);
	printf("Counter Before: %d, Counter After: %d\n", counter_old,
	       lf_op_load_int(&counter));

	/* Complement the 0th bit, then set it back to 0 */
	bool bit_0_prev = lf_op_btc_int(&counter, 0);
	printf("Bit 0 of counter was %d.\n", bit_0_prev);

	bit_0_prev = lf_op_btr_int(&counter, 0);
	printf("Bit 0 of counter after reset %d.\n", bit_0_prev);

	return 0;
}
