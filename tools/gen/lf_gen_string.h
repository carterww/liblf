/* SPDX-License-Identifier: MIT
 * Copyright (c) 2025 Carter Williams
 */

#ifndef LF_GEN_STRING_H
#define LF_GEN_STRING_H

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "lf_debug.h"

typedef struct string string;
struct string {
	char *buffer;
	size_t len;
	size_t cap;
};

static void string_init(string *s, size_t cap)
{
	s->buffer = malloc(cap);
	if (s->buffer == NULL) {
		abort();
	}
	s->len = 0;
	s->cap = cap;
}

static void string_destroy(string *s)
{
	if (s != NULL && s->buffer != NULL) {
		free(s->buffer);
		memset(s, 0, sizeof(*s));
	}
}

static void string_grow(string *s, size_t cap_new_min)
{
	char *buffer_new;
	size_t cap_new = s->cap * 2;
	cap_new = cap_new_min > cap_new ? cap_new_min : cap_new;
	buffer_new = realloc(s->buffer, cap_new);
	if (buffer_new == NULL) {
		abort();
	}
	s->buffer = buffer_new;
	s->cap = cap_new;
}

static void string_append_raw(string *s1, const char *s2, size_t s2_len)
{
	bool success;
	if (s2_len == 0) {
		s2_len = strlen(s2);
	}
	size_t cap_required = s1->len + s2_len + 1;
	if (s1->cap < cap_required) {
		string_grow(s1, cap_required);
	}
	lf_assert(s1->cap >= cap_required);
	memcpy(&s1->buffer[s1->len], s2, s2_len);
	s1->len += s2_len;
	lf_assert(s1->len < s1->cap);
	s1->buffer[s1->len] = '\0';
}

static void string_append(string *s1, const string *s2)
{
	string_append_raw(s1, s2->buffer, s2->len);
}

#endif /* LF_GEN_STRING_H */
