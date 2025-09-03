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

#else

static void dcasx_example(void)
{
	printf("liblf doesn't implement dcasx on u64s for your architecture :(\n");
}

#endif /* LF_OP_DCASX_U64_SUP */

int main(void)
{
	dcasx_example();
}
