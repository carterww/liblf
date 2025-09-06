#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <lf_op.h>
#include <lf_platform.h>

/* In this example we store a custom type in the ring buffer. See
 * the comments in lf_ring.h for more information on how this works.
 */
#define LF_RING_CUSTOM_SLOT_DATA

struct my_lf_ring_slot_data {
	int x;
	int y;
	int z;
};

/* The following typedef and functions are required if you are using
 * lf_ring.h with a custom type. See the comments in lf_ring.h for
 * more information.
 */
typedef struct my_lf_ring_slot_data lf_ring_slot_data;

LF_ATTR_ALWAYS_INLINE
static void lf_ring_store_slot_data(struct my_lf_ring_slot_data *slot,
				    const struct my_lf_ring_slot_data *data)
{
	lf_op_store_int(&slot->x, data->x);
	lf_op_store_int(&slot->y, data->y);
	lf_op_store_int(&slot->z, data->z);
}

LF_ATTR_ALWAYS_INLINE
static void lf_ring_load_slot_data(struct my_lf_ring_slot_data *slot,
				   struct my_lf_ring_slot_data *data)
{
	data->x = lf_op_load_int(&slot->x);
	data->y = lf_op_load_int(&slot->y);
	data->z = lf_op_load_int(&slot->z);
}

/* Include lf_ring.h after defining custom slot data type
 * and functions.
 */
#include <lf_ring.h>

/* Must be a power of 2 and not 0 */
#define RING_BUFFER_LEN (4)

/* lf_ring.h implements four types of ring buffers:
 * - Single-Producer, Single-Consumer (spsc)
 * - Single-Producer, Multiple-Consumer (spmc)
 * - Multiple-Producer, Single-Consumer (mpsc)
 * - Multiple-Producer, Multiple-Consumer (mpmc)
 *
 * Having a single thread as a producer or consumer comes with
 * optimizations, which is why there are four implementations.
 * Make sure to ONLY use the types/functions associated with that
 * type.
 *
 * Just to get an idea of the optimizations, I ran a simple
 * benchmark that enqueued 4096 items in a loop then dequeued
 * 4096 items in a loop. This occurred in a single thread with
 * no contention on the ring buffer.
 * - spsc: ~8 cycles/enqueue and ~5 cycles/dequeue
 * - mpmc: ~45 cycles/enqueue and ~44 cycles/dequeue.
 *
 * This was a back of the napkin benchmark just to get an idea of
 * the costs associated with choosing a more "capable" ring
 * buffer.
 */
static lf_ring_spsc_slot spsc_ring_buffer[RING_BUFFER_LEN];
static lf_ring_mpmc_slot mpmc_ring_buffer[RING_BUFFER_LEN];

static void spsc_example(void);
static void mpmc_example(void);

int main(void)
{
	printf("SPSC Ring Buffer:\n");
	spsc_example();

	printf("\nMPMC Ring Buffer:\n");
	mpmc_example();
	return 0;
}

static void spsc_example(void)
{
	bool succ;
	size_t nitems;
	lf_ring_spsc spsc_ring;
	lf_ring_slot_data data;
	lf_ring_slot_data datum[RING_BUFFER_LEN];

	/* Create our spsc ring buffer */
	if (!lf_ring_spsc_init(&spsc_ring, spsc_ring_buffer, RING_BUFFER_LEN)) {
		fprintf(stderr, "Ring buffer length %d is not a power of 2!\n",
			RING_BUFFER_LEN);
		abort();
	}

	/* Here we add a single item to the ring buffer */
	data = (lf_ring_slot_data){ .x = 0, .y = 0, .z = 0 };
	succ = lf_ring_spsc_enqueue(&spsc_ring, &data);
	if (!succ) {
		fprintf(stderr, "Failed to add data to spsc ring buffer!\n");
		abort();
	}

	/* spsc ring buffers support enqueue_many and dequeue_many functions.
	 * These only require 1 or 2 fences for the whole operation rather than
	 * 1 or 2 per item added.
	 */
	for (int i = 0; i < RING_BUFFER_LEN; ++i) {
		datum[i].x = i + 1;
		datum[i].y = i + 1;
		datum[i].z = i + 1;
	}
	nitems = lf_ring_spsc_enqueue_many(&spsc_ring, datum, RING_BUFFER_LEN);
	/* We already added one item to the ring so the last item in datum
	 * should've been left off.
	 */
	if (nitems != RING_BUFFER_LEN - 1) {
		fprintf(stderr,
			"spsc_enqueue_many thinks it added more than it could?\n");
		abort();
	}
	printf("Added %zu items to the queue instead of %d because the ring is full\n",
	       nitems, RING_BUFFER_LEN);

	/* Now let's dequeue all of the items */
	nitems = lf_ring_spsc_dequeue_many(&spsc_ring, datum, RING_BUFFER_LEN);
	if (nitems != RING_BUFFER_LEN) {
		fprintf(stderr,
			"spsc_dequeue_many did not dequeue all the items from the ring\n");
		abort();
	}

	for (int i = 0; i < RING_BUFFER_LEN; ++i) {
		printf("Item %d: (%d, %d, %d)\n", i, datum[i].x, datum[i].y,
		       datum[i].z);
	}
}

static void mpmc_example(void)
{
	bool succ;
	int nitems_add;
	lf_ring_mpmc mpmc_ring;
	lf_ring_slot_data data;

	/* Create our spsc ring buffer */
	if (!lf_ring_mpmc_init(&mpmc_ring, mpmc_ring_buffer, RING_BUFFER_LEN)) {
		fprintf(stderr, "Ring buffer length %d is not a power of 2!\n",
			RING_BUFFER_LEN);
		abort();
	}

	/* Try to deuque an item when empty */
	succ = lf_ring_mpmc_dequeue(&mpmc_ring, &data);
	if (succ) {
		fprintf(stderr, "Dequeued an item from an empty queue?\n");
		abort();
	}

	/* Enqueue then immediately dequeue items. */
	nitems_add = RING_BUFFER_LEN * 2 + 1;
	for (int i = 0; i < nitems_add; ++i) {
		data = (lf_ring_slot_data){ .x = i, .y = i, .z = i };
		succ = lf_ring_mpmc_enqueue(&mpmc_ring, &data);
		if (!succ) {
			fprintf(stderr, "Failed to enqueue item %d\n", i);
			abort();
		}
		memset(&data, 0, sizeof(data));

		succ = lf_ring_mpmc_dequeue(&mpmc_ring, &data);
		if (!succ || data.x != i || data.y != i || data.z != i) {
			fprintf(stderr, "Failed to dequeue item %d\n", i);
			abort();
		}
	}

	printf("Successfully enqueued and dequeued %d items. mpmc_ring has a final length of %zu\n",
	       nitems_add, LF_RING_LEN(&mpmc_ring));
}
