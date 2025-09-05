/* SPDX-License-Identifier: MIT
 * Copyright (c) 2025 Carter Williams
 */

#ifndef LF_RING_H
#define LF_RING_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <lf_cc.h>
#include <lf_op.h>
#include <lf_platform.h>

#define IS_POWER_OF_TWO(x) ((x) && (((x) & ((x) - 1)) == 0))
#define MIN(a, b) ((a) > (b) ? (b) : (a))

/* There are three main implementations:
 * 1. Single-Producer, Single-Consumer (spsc): 1 thread enqueues data and
 *    1 thread dequeues data.
 * 2. Single-Producer, Multiple-Consumer (spmc): 1 thread enqueues data
 *    and 1+ threads dequeues data.
 * 3. Multiple-Producer, Single/Multiple-Consumer (mpsc, mpmc): 1+ thread
 *    enqueues data and 1+ threads dequeues data.
 *
 * Each type has their own functions with a suffix of spsc, spmc, mpsc,
 * or mpmc. You should use those functions; you should not use spsc functions
 * for a spmc queue (even though they have the same underlying type).
 * 
 * Cases (1) and (2) can use the same underlying struct, but case (3)
 * requires each item in the queue to have a sequence number.
 *
 * Case (1) supports enqueue_many() and dequeue_many() functions that
 * use O(1) fences, not O(n). Case (2) supports enqueue_many() but not
 * dequeue_many().
 */
typedef struct lf_ring_sp lf_ring_spsc;
typedef struct lf_ring_sp lf_ring_spmc;
typedef struct lf_ring_mp lf_ring_mpsc;
typedef struct lf_ring_mp lf_ring_mpmc;

typedef struct lf_ring_mp_slot lf_ring_mpsc_slot;
typedef struct lf_ring_mp_slot lf_ring_mpmc_slot;

struct lf_ring_sp {
	size_t head;
	size_t tail;
	size_t mask;
	void **buf;
};

struct lf_ring_mp_slot {
	size_t seq;
	void *data;
};

struct lf_ring_mp {
	size_t head;
	size_t tail;
	size_t mask;
	struct lf_ring_mp_slot *buf;
};

/* Initialize the ring buffer with a buffer of length buffer_len. buffer_len
 * must be a power of two.
 *
 * Retruns true if initialized successfully, false if buffer_len is not a
 * power of 2.
 */
LF_ATTR_ALWAYS_INLINE
static bool lf_ring_init_spsc(lf_ring_spsc *ring, void **buffer,
			      size_t buffer_len)
{
	if (!IS_POWER_OF_TWO(buffer_len)) {
		return false;
	}
	ring->head = 0;
	ring->tail = 0;
	ring->mask = buffer_len - 1;
	ring->buf = buffer;
	return true;
}

/* Returns the number of items in the ring */
LF_ATTR_ALWAYS_INLINE
static size_t lf_ring_len_spsc(lf_ring_spsc *ring)
{
	size_t t, h;
	h = lf_op_load_size(&ring->head);
	t = lf_op_load_size(&ring->tail);
	return t - h;
}

/* Returns the maximum number of items the ring can store */
LF_ATTR_ALWAYS_INLINE
static size_t lf_ring_cap_spsc(lf_ring_spsc *ring)
{
	return ring->mask + 1;
}

/* Place a single item at the tail.
 *
 * Returns true if the item was added successfully, false if the ring
 * is full.
 */
static bool lf_ring_enqueue_spsc(lf_ring_spsc *ring, void *item)
{
	size_t t, h, idx;

	h = lf_op_load_size(&ring->head);
	t = ring->tail;
	if (t - h == ring->mask + 1) {
		return false;
	}
	idx = t & ring->mask;
	lf_op_store_ptr(&ring->buf[idx], item);
	/* Ensure new item visible before tail */
	lf_fence_store();
	/* Producer owns tail, no need for atomic RMW */
	lf_op_store_size(&ring->tail, t + 1);
	return true;
}

/* Take a single items from the head and put its value in item.
 *
 * Returns true if there was an item to dequeue, false if the
 * queue is empty. *item is undefined if false is returned.
 */
static bool lf_ring_dequeue_spsc(lf_ring_spsc *ring, void **item)
{
	size_t t, h, idx;

	h = ring->head;
	t = lf_op_load_size(&ring->tail);
	if (t == h) {
		return false;
	}
	idx = h & ring->mask;
	/* Ensure item will be visible */
	lf_fence_load();
	*item = lf_op_load_ptr(&ring->buf[idx]);
	/* Ensure we load item before incrementing head so
	 * producer can't overwrite idx.
	 */
	lf_fence_load_store();
	/* Consumer owns head, no need for atomic RMW */
	lf_op_store_size(&ring->head, h + 1);
	return true;
}

/* Places at most nitems from items at the tail. If the ring does not have
 * space for nitems, items from indices [0, cap - len) will be added instead.
 *
 * Returns the number of items actually added to the ring.
 *
 * DO NOT use this function for a muliple-producer ring (mpsc, mpmc).
 *
 * Using this function is favorable if you need to add more than 1 item
 * because it only uses 1 fence. If you added nitems through lf_ring_enqueue(),
 * nitems fences would be issued. The only downside is the consumer may
 * see the first new items a bit later.
 */
static size_t lf_ring_enqueue_many_spsc(lf_ring_spsc *ring, void **items,
					size_t nitems)
{
	size_t i, t, h, idx, cap, rem;

	h = lf_op_load_size(&ring->head);
	t = ring->tail;
	cap = ring->mask + 1;
	rem = cap - (t - h);
	nitems = MIN(nitems, rem);
	if (nitems == 0) {
		return 0;
	}
	for (i = 0; i < nitems; i += 1, t += 1) {
		idx = t & ring->mask;
		lf_op_store_ptr(&ring->buf[idx], items[i]);
	}
	lf_fence_store();
	lf_op_store_size(&ring->tail, t);
	return nitems;
}

/* Takes at most nitems from items from the head and places them in items.
 *
 * Returns the number of items actually taken from the ring.
 *
 * DO NOT use this function for a muliple-consumer ring (spmc, mpmc).
 *
 * Using this function is favorable if you can take more than 1 item
 * because it only uses 2 fences. If you retrieved nitems through
 * lf_ring_dequeue(), nitems * 2 fences would be issued. The only
 * downside is the producer may see the first empty slots a bit later.
 */
static size_t lf_ring_dequeue_many_spsc(lf_ring_spsc *ring, void **items,
					size_t nitems)
{
	size_t i, t, h, idx, len;

	h = ring->head;
	t = lf_op_load_size(&ring->tail);
	len = (t - h);
	nitems = MIN(nitems, len);
	if (nitems == 0) {
		return 0;
	}
	lf_fence_load();
	for (i = 0; i < nitems; i += 1, h += 1) {
		idx = h & ring->mask;
		items[i] = lf_op_load_ptr(&ring->buf[idx]);
	}
	lf_fence_load_store();
	lf_op_store_size(&ring->head, h);
	return nitems;
}

/* See spsc comments for more details */
#define lf_ring_init_spmc lf_ring_init_spsc
#define lf_ring_len_spmc lf_ring_len_spsc
#define lf_ring_cap_spmc lf_ring_cap_spsc
#define lf_ring_enqueue_spmc lf_ring_enqueue_spsc
#define lf_ring_enqueue_many_spmc lf_ring_enqueue_many_spsc

/* Same interface as lf_ring_dequeue_spsc with a different implementation
 * to handle contention at the head.
 *
 * The main difference compared to the spsc version is we now require a
 * CAS instead of a simple store to update the head.
 */
static bool lf_ring_dequeue_spmc(lf_ring_spsc *ring, void **item)
{
	size_t t, h, idx;

	do {
		h = lf_op_load_size(&ring->head);
		t = lf_op_load_size(&ring->tail);
		if (t == h) {
			return false;
		}
		idx = h & ring->mask;
		/* Ensure item will be visible */
		lf_fence_load();
		*item = lf_op_load_ptr(&ring->buf[idx]);
		/* Ensure we load item before incrementing head so
		 * producer can't overwrite idx.
		 */
		lf_fence_load_atomic();
		/* If CAS fails it means someone else consumed item.
		 * Try again.
		 */
	} while (!lf_op_cas_size(&ring->head, h, h + 1));
	return true;
}

/* See spsc comments for interface details and mpmc comments for implemenation
 * details.
 */
#define lf_ring_init_mpsc lf_ring_init_mpmc
#define lf_ring_len_mpsc lf_ring_len_mpmc
#define lf_ring_cap_mpsc lf_ring_cap_mpmc
#define lf_ring_enqueue_mpsc lf_ring_enqueue_mpmc

/* Same interface as other dequeue functions with a different implementation
 * to handle producer slot contention.
 *
 * This version is the exact same as the mpmc version, but we replace the
 * CAS on ring->head with a relaxed store. See mpmc version for more comments
 * on implementation.
 */
static bool lf_ring_dequeue_mpsc(lf_ring_mpmc *ring, void **item)
{
	size_t h, h_next, seq, idx;
	intptr_t diff_signed;
	lf_ring_mpmc_slot *slot;

	while (true) {
		h = lf_op_load_size(&ring->head);
		h_next = h + 1;
		idx = h & ring->mask;
		slot = &ring->buf[idx];
		seq = lf_op_load_size(&slot->seq);

		diff_signed = (intptr_t)seq - (intptr_t)h_next;
		if (diff_signed == 0) {
			/* Perform a relaxed store on head instead of CAS since no
			 * other consumers can write to it.
			 */
			lf_op_store_size(&ring->head, h_next);
		} else if (diff_signed > 0) {
			continue;
		} else {
			return false;
		}
	}
	lf_fence_load();

	*item = lf_op_load_ptr(&slot->data);
	lf_fence_load_store();
	lf_op_store_size(&slot->seq, h_next + ring->mask);
	return true;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_ring_init_mpmc(lf_ring_mpmc *ring, lf_ring_mpmc_slot *buffer,
			      size_t buffer_len)
{
	size_t i;

	if (!IS_POWER_OF_TWO(buffer_len)) {
		return false;
	}
	ring->head = 0;
	ring->tail = 0;
	ring->mask = buffer_len - 1;
	ring->buf = buffer;

	/* Initial sequence number should equal the slot's index to indicate
	 * emptiness.
	 */
	for (i = 0; i < buffer_len; ++i) {
		buffer[i].seq = i;
	}
	return true;
}

/* In the case of mpsc and mpmc, this length calculation is only an estimate
 * because tail and head don't directly correspond to the state of the data
 * in the queue.
 *
 * The tail is incremented before storing the data, and the head is incremented
 * before loading the data. It may be the case that tail == head but none of the
 * consumers have actually loaded the data. Or that tail - head == cap but none
 * of the producers have stored the data.
 */
LF_ATTR_ALWAYS_INLINE
static size_t lf_ring_len_mpmc(lf_ring_mpmc *ring)
{
	size_t t, h;
	h = lf_op_load_size(&ring->head);
	t = lf_op_load_size(&ring->tail);
	return t - h;
}

LF_ATTR_ALWAYS_INLINE
static size_t lf_ring_cap_mpmc(lf_ring_mpmc *ring)
{
	return ring->mask + 1;
}

/* The mpmc implementations were derived from Dmitry Vyukov's implementation
 * (https://www.1024cores.net/home/lock-free-algorithms/queues/bounded-mpmc-queue).
 */

/* Same interface as other enqueue functions with a different implementation
 * to handle data races.
 */
static bool lf_ring_enqueue_mpmc(lf_ring_mpmc *ring, void *item)
{
	size_t t, seq, idx;
	intptr_t diff_signed;
	lf_ring_mpmc_slot *slot;

	while (true) {
		t = lf_op_load_size(&ring->tail);
		idx = t & ring->mask;
		slot = &ring->buf[idx];
		seq = lf_op_load_size(&slot->seq);

		/* Compare sequence number and tail as signed integers to take
		 * wrap around into account.
		 */
		diff_signed = (intptr_t)seq - (intptr_t)t;

		/* If seq == tail then slot is empty, try to increment tail to
		 * "claim" slot. Even though we increment the tail, the consumer
		 * won't assume there's data because seq != t + 1 yet.
		 *
		 * If seq > tail then consumer marked slot as free or another
		 * producer already incremented seq (possible since there's no
		 * atomic_store fence between tail and seq update).
		 *
		 * If seq < tail then consumer still hasn't dequeued the data
		 * from the slot and ring buffer is full.
		 */
		if (diff_signed == 0) {
			if (lf_op_cas_size(&ring->tail, t, t + 1)) {
				break;
			}
		} else if (diff_signed > 0) {
			continue;
		} else {
			return false;
		}
	}

	lf_op_store_ptr(&slot->data, item);
	/* Ensure data is seen before seq number */
	lf_fence_store();
	/* Incrementing the sequence number tells the consumer the data
	 * is posted.
	 */
	lf_op_store_size(&slot->seq, t + 1);

	return true;
}

/* Same interface as other dequeue functions with a different implementation
 * to handle data races.
 */
static bool lf_ring_dequeue_mpmc(lf_ring_mpmc *ring, void **item)
{
	size_t h, h_next, seq, idx;
	intptr_t diff_signed;
	lf_ring_mpmc_slot *slot;

	while (true) {
		h = lf_op_load_size(&ring->head);
		h_next = h + 1;
		idx = h & ring->mask;
		slot = &ring->buf[idx];
		seq = lf_op_load_size(&slot->seq);

		/* Compare sequence number and head as signed integers to take
		 * wrap around into account.
		 *
		 * We use head + 1 because the consumer expects seq == slot_index
		 * to indicate the slot is empty and seq == slot_index + 1 to indicate
		 * there is data (see enqueue_mpmc).
		 */
		diff_signed = (intptr_t)seq - (intptr_t)h_next;

		/* These cases are largely identical to the enqueue_mpmc ones with
		 * a slightly different meaning.
		 */
		if (diff_signed == 0) {
			if (lf_op_cas_size(&ring->head, h, h_next)) {
				break;
			}
		} else if (diff_signed > 0) {
			continue;
		} else {
			return false;
		}
	}
	/* Order with respect to previous loads to ensure we see most
	 * recent data. (Might not be necessary)
	 */
	lf_fence_load();

	*item = lf_op_load_ptr(&slot->data);
	/* Ensure data is actually loaded before seq number is
	 * updated.
	 */
	lf_fence_load_store();
	/* Add cap to slot->seq to indicate slot is free.
	 *
	 * h = slot->seq, h_next = h + 1, cap = ring->mask + 1
	 *
	 * Using h_next seems weird but I used h + 1 3 times so
	 * might as well store that and use it.
	 */
	lf_op_store_size(&slot->seq, h_next + ring->mask);
	return true;
}

#undef IS_POWER_OF_TWO
#undef MIN

#endif /* LF_RING_H */
