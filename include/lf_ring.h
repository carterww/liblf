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

/* lf_ring allows you to use a custom data type that is contained in each
 * slot. If none is provided, a union of generic pointers is stored in
 * the buffer.
 *
 * In order to define your own slot data type, you must do the following
 * before including lf_ring.h:
 * 1. Define LF_RING_CUSTOM_SLOT_DATA.
 * 2. Implement a type named "lf_ring_slot_data"
 * 3. Implement a function named "lf_ring_store_slot_data" that stores
 *    data into slot's data.
 * 4. Implement a function named "lf_ring_load_slot_data" that places
 *    the slot's data into data.
 * You must ensure the store and load functions actually write to memory.
 * Using a normal store like "slot->ptr = data->ptr" is not sufficient
 * because the compiler may optimize this in a way where data->ptr isn't
 * actually written to the slot's memory. Use lf_op.h's load and store
 * primitives.
 *
 * It is also noteworthy that the load and store does not have to be
 * a single load or store operation. You can perform multiple if you
 * are working with structs or arrays.
 *
 * NOTE: Using a custom data type doesn't change the function names.
 * This means you can only use 1 lf_ring.h custom implementation per
 * translation unit. If you use a custom data type, I highly recommend
 * you don't put the lf_ring calls in a header file.
 */
#if !defined(LF_RING_CUSTOM_SLOT_DATA)

typedef union lf_ring_slot_data lf_ring_slot_data;

union lf_ring_slot_data {
	void *ptr;
	uintptr_t uptr;
	intptr_t iptr;
};

LF_ATTR_ALWAYS_INLINE
static void lf_ring_store_slot_data(union lf_ring_slot_data *slot,
				    const union lf_ring_slot_data *data)
{
	lf_op_store_ptr(&slot->ptr, data->ptr);
}

LF_ATTR_ALWAYS_INLINE
static void lf_ring_load_slot_data(union lf_ring_slot_data *slot,
				   union lf_ring_slot_data *data)
{
	data->ptr = lf_op_load_ptr(&slot->ptr);
}

#endif /* !LF_RING_CUSTOM_SLOT_DATA */

/* There are three main implementations:
 * 1. Single-Producer, Single-Consumer (spsc): 1 thread enqueues data and
 *    1 thread dequeues data.
 * 2. Single-Producer, Multiple-Consumer (spmc): 1 thread enqueues data
 *    and 1+ threads dequeues data.
 * 3. Multiple-Producer, Single/Multiple-Consumer (mpsc, mpmc): 1+ thread
 *    enqueues data and 1+ threads dequeues data.
 *
 * Each type has their own functions with spsc, spmc, mpsc, or mpmc in the
 * name. You should use those functions; you should not use spsc functions
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

typedef struct lf_ring_sp_slot lf_ring_spsc_slot;
typedef struct lf_ring_sp_slot lf_ring_spmc_slot;
typedef struct lf_ring_mp_slot lf_ring_mpsc_slot;
typedef struct lf_ring_mp_slot lf_ring_mpmc_slot;

struct lf_ring_sp_slot {
	lf_ring_slot_data data;
};

struct lf_ring_mp_slot {
	size_t seq;
	lf_ring_slot_data data;
};

struct lf_ring_sp {
	size_t head;
	char _pad[LIBLF_CONFIG_CACHELINE - sizeof(size_t)];
	size_t tail;
	size_t mask;
	struct lf_ring_sp_slot *buf;
};

struct lf_ring_mp {
	size_t head;
	char _pad[LIBLF_CONFIG_CACHELINE - sizeof(size_t)];
	size_t tail;
	size_t mask;
	struct lf_ring_mp_slot *buf;
};

/* In the case of mpsc and mpmc, this length calculation is only an estimate
 * because tail and head don't directly correspond to the state of the data
 * in the queue.
 *
 * The tail is incremented before storing the data, and the head is incremented
 * before loading the data. It may be the case that tail == head but none of the
 * consumers have actually loaded the data. Or that tail - head == cap but none
 * of the producers have stored the data.
 */
#define LF_RING_LEN(ring_ptr)                 \
	(lf_op_load_size(&(ring_ptr)->tail) - \
	 lf_op_load_size(&(ring_ptr)->head))

#define LF_RING_CAP(ring_ptr) ((ring_ptr)->mask + 1)

#define IS_POWER_OF_TWO(x) ((x) && (((x) & ((x) - 1)) == 0))
#define MIN(a, b) ((a) > (b) ? (b) : (a))

/* Initialize the ring buffer with a buffer of length buffer_len. buffer_len
 * must be a power of two.
 *
 * Retruns true if initialized successfully, false if buffer_len is not a
 * power of 2.
 */
LF_ATTR_ALWAYS_INLINE
static bool lf_ring_spsc_init(lf_ring_spsc *ring, lf_ring_spsc_slot *buffer,
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

/* Place a single item at the tail.
 *
 * Returns true if the item was added successfully, false if the ring
 * is full.
 */
static bool lf_ring_spsc_enqueue(lf_ring_spsc *ring,
				 const lf_ring_slot_data *item)
{
	size_t t, h, idx;

	h = lf_op_load_size(&ring->head);
	t = ring->tail;
	if (t - h == ring->mask + 1) {
		return false;
	}
	idx = t & ring->mask;
	lf_ring_store_slot_data(&ring->buf[idx].data, item);
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
static bool lf_ring_spsc_dequeue(lf_ring_spsc *ring, lf_ring_slot_data *item)
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
	lf_ring_load_slot_data(&ring->buf[idx].data, item);
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
static size_t lf_ring_spsc_enqueue_many(lf_ring_spsc *ring,
					const lf_ring_slot_data *items,
					size_t nitems)
{
	size_t i, t, h, idx, rem;

	h = lf_op_load_size(&ring->head);
	t = ring->tail;
	rem = LF_RING_CAP(ring) - (t - h);
	nitems = MIN(nitems, rem);
	if (nitems == 0) {
		return 0;
	}
	for (i = 0; i < nitems; i += 1, t += 1) {
		idx = t & ring->mask;
		lf_ring_store_slot_data(&ring->buf[idx].data, &items[i]);
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
static size_t lf_ring_spsc_dequeue_many(lf_ring_spsc *ring,
					lf_ring_slot_data *items, size_t nitems)
{
	size_t i, t, h, idx, len;

	h = ring->head;
	t = lf_op_load_size(&ring->tail);
	len = t - h;
	nitems = MIN(nitems, len);
	if (nitems == 0) {
		return 0;
	}
	lf_fence_load();
	for (i = 0; i < nitems; i += 1, h += 1) {
		idx = h & ring->mask;
		lf_ring_load_slot_data(&ring->buf[idx].data, &items[i]);
	}
	lf_fence_load_store();
	lf_op_store_size(&ring->head, h);
	return nitems;
}

/* See spsc comments for more details on spmc functions that use spsc impl */

LF_ATTR_ALWAYS_INLINE
static bool lf_ring_spmc_init(lf_ring_spsc *ring, lf_ring_spsc_slot *buffer,
			      size_t buffer_len)
{
	return lf_ring_spsc_init(ring, buffer, buffer_len);
}

static bool lf_ring_spmc_enqueue(lf_ring_spsc *ring,
				 const lf_ring_slot_data *item)
{
	return lf_ring_spsc_enqueue(ring, item);
}

/* Same interface as lf_ring_dequeue_spsc with a different implementation
 * to handle contention at the head.
 *
 * The main difference compared to the spsc version is we now require a
 * CAS instead of a simple store to update the head.
 */
static bool lf_ring_spmc_dequeue(lf_ring_spsc *ring, lf_ring_slot_data *item)
{
	size_t t, h, idx;

	h = lf_op_load_size(&ring->head);
	do {
		t = lf_op_load_size(&ring->tail);
		if (t == h) {
			return false;
		}
		idx = h & ring->mask;
		/* Ensure item will be visible */
		lf_fence_load();
		lf_ring_load_slot_data(&ring->buf[idx].data, item);
		/* Ensure we load item before incrementing head so
		 * producer can't overwrite idx.
		 */
		lf_fence_load_atomic();
		/* If CAS fails it means someone else consumed item.
		 * Try again.
		 */
	} while (!lf_op_casx_size(&ring->head, &h, h + 1));
	return true;
}

static size_t lf_ring_spmc_enqueue_many(lf_ring_spsc *ring,
					const lf_ring_slot_data *items,
					size_t nitems)
{
	return lf_ring_spsc_enqueue_many(ring, items, nitems);
}

LF_ATTR_ALWAYS_INLINE
static bool lf_ring_mpsc_init(lf_ring_mpmc *ring, lf_ring_mpmc_slot *buffer,
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

/* The mpsc and mpmc implementations were derived from Dmitry Vyukov's
 * implementation at
 * https://www.1024cores.net/home/lock-free-algorithms/queues/bounded-mpmc-queue.
 */

/* Same interface as other enqueue functions with a different implementation
 * to handle data races.
 */
static bool lf_ring_mpsc_enqueue(lf_ring_mpmc *ring,
				 const lf_ring_slot_data *item)
{
	size_t t, seq, idx;
	intptr_t diff_signed;
	lf_ring_mpmc_slot *slot;

	t = lf_op_load_size(&ring->tail);
	while (true) {
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
			if (lf_op_casx_size(&ring->tail, &t, t + 1)) {
				break;
			}
		} else if (diff_signed < 0) {
			return false;
		}
	}

	lf_ring_store_slot_data(&slot->data, item);
	/* Ensure data is seen before seq number */
	lf_fence_store();
	/* Incrementing the sequence number tells the consumer the data
	 * is posted.
	 */
	lf_op_store_size(&slot->seq, t + 1);

	return true;
}

/* Same interface as other dequeue functions with a different implementation
 * to handle producer slot contention.
 *
 * This version is the exact same as the mpmc version, but we replace the
 * CAS on ring->head with a relaxed store. See mpmc version for more comments
 * on implementation.
 */
static bool lf_ring_mpsc_dequeue(lf_ring_mpmc *ring, lf_ring_slot_data *item)
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
			break;
		} else if (diff_signed < 0) {
			return false;
		}
	}
	lf_fence_load();

	lf_ring_load_slot_data(&slot->data, item);
	lf_fence_load_store();
	lf_op_store_size(&slot->seq, h_next + ring->mask);
	return true;
}

LF_ATTR_ALWAYS_INLINE
static bool lf_ring_mpmc_init(lf_ring_mpmc *ring, lf_ring_mpmc_slot *buffer,
			      size_t buffer_len)
{
	return lf_ring_mpsc_init(ring, buffer, buffer_len);
}

static bool lf_ring_mpmc_enqueue(lf_ring_mpmc *ring,
				 const lf_ring_slot_data *item)
{
	return lf_ring_mpsc_enqueue(ring, item);
}

static bool lf_ring_mpmc_dequeue(lf_ring_mpmc *ring, lf_ring_slot_data *item)
{
	size_t h, h_next, seq, idx;
	intptr_t diff_signed;
	lf_ring_mpmc_slot *slot;

	h = lf_op_load_size(&ring->head);
	while (true) {
		h_next = h + 1;
		idx = h & ring->mask;
		slot = &ring->buf[idx];
		seq = lf_op_load_size(&slot->seq);

		diff_signed = (intptr_t)seq - (intptr_t)h_next;
		if (diff_signed == 0) {
			if (lf_op_casx_size(&ring->head, &h, h_next)) {
				break;
			}
		} else if (diff_signed < 0) {
			return false;
		}
	}
	lf_fence_load();

	lf_ring_load_slot_data(&slot->data, item);
	lf_fence_load_store();
	lf_op_store_size(&slot->seq, h_next + ring->mask);
	return true;
}

#undef IS_POWER_OF_TWO
#undef MIN

#endif /* LF_RING_H */
