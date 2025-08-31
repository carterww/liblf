/* SPDX-License-Identifier: MIT
 * Copyright (c) 2025 Carter Williams
 */

#ifndef LXEV_DEBUG_H
#define LXEV_DEBUG_H

#include <lf_cc.h>

#define lf_static_assert(expr, message) \
	typedef char lf_static_assert_##message[(expr) ? 1 : -1]

#if defined(LF_COMPILE_ASSERTS) && LF_COMPILE_ASSERTS != 0

#include <stdio.h>
#include <stdlib.h>

static const char *assert_failed_context = "liblf Assertion failed";
static const char *panic_context = "liblf Panic!";

LXEV_ATTR_NORETURN
static void print_and_fail(const char *context, const char *file_name,
			   int line_number)
{
	fprintf(stderr, "[%s:%d] %s\n", file_name, line_number, context);
	abort();
}

#define lf_assert(expr)                                                 \
	do {                                                            \
		if (lf_unlikely(!(expr))) {                             \
			print_and_fail(assert_failed_context, __FILE__, \
				       __LINE__);                       \
		}                                                       \
	} while (0)

#define lf_panic()                                                         \
	do {                                                               \
		print_and_fail(assert_failed_context, __FILE__, __LINE__); \
	} while (0)

#else

#define lf_assert(expr) \
	do {            \
	} while (0)

#define lf_panic() lf_unreachable()

#endif /* LXEV_BUILD_OPT_COMPILE_ASSERTS != 0 */

#endif /* LXEV_DEBUG_H */
