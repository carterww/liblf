#include <stdbool.h>
#include <stdio.h>

#include <lf_op.h>

#define RING_LEN (4)

static unsigned int head = 0;
static unsigned int tail = 0;
static void *ring[RING_LEN];

static void thread_consumer(void)
{
	unsigned int h, t, idx;
	void *item;
	while (true) {
		/* These can be loaded in any order so we don't put a lf_fence_load()
		 * between them.
		 */
		h = lf_op_load_uint(&head);
		t = lf_op_load_uint(&tail);
		if (t == h) {
			break;
		}
		idx = h & (RING_LEN - 1);
		/* This fence is required because we need to ensure the previous
		 * load of tail and the load of ring[idx] are ordered correctly.
		 * Without this fence, we could possibly see the new tail without
		 * seeing the new item.
		 */
		lf_fence_load();
		item = lf_op_load_ptr(&ring[idx]);
		/* This fence is also required to ensure the item is actually consumed
		 * prior to incrementing head. Once we increment head, we are signaling
		 * a new slot is empty. This means it could technically overwrite ring[idx].
		 */
		lf_fence_load_atomic();
		lf_op_inc_uint(&head);

		printf("Consumer: got %p at %u\n", item, idx);
	}
}

static void thread_producer(void)
{
	unsigned int t, h, idx;
	void *item;
	while (true) {
		/* These can be loaded in any order so we don't put a lf_fence_load()
		 * between them.
		 */
		t = lf_op_load_uint(&tail);
		h = lf_op_load_uint(&head);
		if (t - h == RING_LEN) {
			break;
		}
		item = (void *)(size_t)(t - h + 1);
		idx = t & (RING_LEN - 1);
		lf_op_store_ptr(&ring[idx], item);
		/* This fence is required because we need the contents of
		 * ring[idx] to be visible *before* the tail counter. If the tail
		 * increment becomes visible first, the consumer may consume a
		 * stale or non-existent item.
		 */
		lf_fence_store_atomic();
		lf_op_inc_uint(&tail);

		printf("Producer: put %p at %u\n", item, idx);
	}
}

int main(void)
{
	/* Imagine these are running concurrently for the purposes of the
	 * example. In this case there are no issues because they are run
	 * one after the other.
	 *
	 * This is also a single producer single consumer ring buffer. That
	 * means the producer is the only thread that can write to tail and
	 * the consumer is the only thread that can write to head.
	 */
	int switch_count = 0;
	while (switch_count < 2) {
		thread_producer();
		thread_consumer();
		switch_count += 1;
	}
	return 0;
}
