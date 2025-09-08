#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>

#include <lf_op.h>
#include <lf_op_sup.h>

/* This example shows how to use DCASX. I guard the use of DCASX because it is
 * not a portable operation in liblf.
 */
#if LF_OP_DCASX_U64_SUP

static char *buf_global = NULL;

static void dcasx_example(void)
{
	bool success;
	char buf_global_new[16];

	lf_dcas_packed_u64 val = { .tuple = { 512, 4096 } };
	lf_dcas_packed_u64 old = { .tuple = { 512, 4096 } };
	lf_dcas_packed_u64 new = { .tuple = { 1024, 2048 } };

	/* dcasx compares val.tuple with old.tuple and atomically sets
	 * val.tuple = new.tuple iff val.tuple == old.tuple. If not, dcasx
	 * places val.tuple's actual value into old.tuple
	 */
	success = lf_op_dcasx_u64(&val, &old, new);
	/* A fence is required here if we want to guarantee the store is done
	 * only after the DCAS is visible since these operate on independent
	 * variables.
	 */
	lf_fence_atomic_store();
	lf_op_store_ptr((void **)&buf_global, buf_global_new);

	if (success) {
		printf("Successfully swapped (%lu, %lu) with (%lu, %lu)\n",
		       old.tuple[0], old.tuple[1], new.tuple[0], new.tuple[1]);
	} else {
		printf("BUG: Failed to swap. val's original value was (%lu, %lu)\n",
		       old.tuple[0], old.tuple[1]);
	}
	printf("New global buffer pointer after atomic dcas: %p\n",
	       lf_op_load_ptr(&buf_global));

	/* dcas even works on smaller types */
	lf_dcas_packed_char valb = { .tuple = { 'a', 'b' } };
	lf_dcas_packed_char oldb = { .tuple = { 'a', 'b' } };
	lf_dcas_packed_char newb = { .tuple = { 'y', 'z' } };
	success = lf_op_dcas_char(&valb, oldb, newb);
	if (success) {
		printf("Successfully swapped (%c, %c) with (%c, %c)\n",
		       oldb.tuple[0], oldb.tuple[1], newb.tuple[0],
		       newb.tuple[1]);
	} else {
		printf("BUG: Failed to swap!\n");
	}
}

#else

static void dcasx_example(void)
{
	printf("liblf doesn't implement dcasx on u64s for your architecture :(\n");
}

#endif /* LF_OP_DCASX_U64_SUP */

int main(void)
{
	dcasx_example();
	return 0;
}
