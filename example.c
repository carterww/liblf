#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>

#include <lf_op.h>
#include <lf_op_sup.h>

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

/* This example shows how to use DCASX. I guard the use of DCASX because it is
 * not a portable operation in liblf.
 */
#if LF_OP_DCASX_U64_SUP

static lf_dcas_packed_u64 pu64 = { .tuple = { 512, 4096 } };
static char *buf_global = NULL;

static void dcasx_example(void)
{
	bool success;
	char buf_global_new[16];

	lf_dcas_packed_u64 old = { .tuple = { 512, 4096 } };
	lf_dcas_packed_u64 new = { .tuple = { 1024, 2048 } };

	lf_dcas_packed_u64 orig = lf_op_dcasx_u64(&pu64, old, new, &success);
	/* A fence is required here if we want to guarantee the load is done
	 * only after the DCAS is visible since these operate on independent
	 * variables.
	 */
	lf_fence_atomic_load();
	lf_op_store_ptr((void **)&buf_global, buf_global_new);

	if (success) {
		printf("Successfully swapped (%lu, %lu) with (%lu, %lu)\n",
		       old.tuple[0], old.tuple[1], new.tuple[0], new.tuple[1]);
	} else {
		printf("BUG: Failed to swap. pu64 was (%lu, %lu) not (%lu, %lu)\n",
		       orig.tuple[0], orig.tuple[1], old.tuple[0],
		       old.tuple[1]);
	}
	printf("New global buffer pointer after atomic dcas: %p\n",
	       lf_op_load_ptr(&buf_global));
}

#endif /* LF_OP_DCASX_U64_SUP */
