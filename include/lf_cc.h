/* SPDX-License-Identifier: MIT
 * Copyright (c) 2025 Carter Williams
 */

#ifndef LF_CC_H
#define LF_CC_H

/* clang also defines __GNUC__ because it's "compatible" but I want to
 * know if it is really gcc.
 */
#if defined(__clang__)
#define LF_CC_CLANG
#elif defined(__GNUC__)
#define LF_CC_GCC
#else
#error Detected an unsupported compiler.
#endif

#if defined(LF_CC_GCC) || defined(LF_CC_CLANG)

#define lf_alignof(t) __alignof__(t)
#define lf_typeof(e) __typeof__(e)
#define lf_expect(expr, expect) __builtin_expect(expr, expect)
#define lf_likely(expr) __builtin_expect(!!(expr), 1)
#define lf_unlikely(expr) __builtin_expect(!!(expr), 0)

#define lf_compiler_barrier() __asm__ __volatile__("" ::: "memory");
#define lf_unreachable() __builtin_unreachable()
#define lf_assume_aligned(p, a) __builtin_assume_aligned(p, a)
#define lf_prefetch(p) __builtin_prefetch(p)

#define LF_ATTR_ALIGNED(a) __attribute__((aligned(a)))
#define LF_ATTR_ALLOC_ALIGNED(arg_p) __attribute__((alloc_align(arg_p)))
#define LF_ATTR_NORETURN __attribute__((noreturn))
#define LF_ATTR_PACKED __attribute__((packed))
#define LF_ATTR_ALWAYS_INLINE __inline__ __attribute__((always_inline))
#define LF_ATTR_COLD __attribute__((cold))
#define LF_ATTR_HOT __attribute__((hot))
#define LF_ATTR_CONST __attribute__((const))
#define LF_ATTR_WEAK __attribute__((weak))
#define LF_ATTR_PUBLIC __attribute__((visibility("default")))
#define LF_ATTR_PRIVATE __attribute__((visibility("hidden")))

#endif /* gcc or clang */

#endif /* LF_CC_H */
