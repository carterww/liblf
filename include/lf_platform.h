/* SPDX-License-Identifier: MIT
 * Copyright (c) 2025 Carter Williams
 */

#ifndef LF_PLATFORM_H
#define LF_PLATFORM_H

#include <stdint.h>

/* Figure out the architecture */
#if defined(__x86_64__) || defined(__amd64__)

#define LF_ARCH_X86_64
#define LF_MEM_TSO

#elif defined(__arm__)

#define LF_ARCH_ARM32
#define LF_MEM_WMO

#if defined(__thumb__)
#define LF_ARCH_ARM32_THUMB2
#else
#define LF_ARCH_ARM32_NO_THUMB2
#endif /* __thumb__ */

#elif defined(__aarch64__)

#define LF_ARCH_ARM64
#define LF_MEM_WMO

#elif defined(__x86__) || defined(__i386__)

#define LF_ARCH_X86
#define LF_MEM_TSO

#elif defined(__riscv)

#define LF_ARCH_RISCV
#define LF_MEM_WMO

#if (__riscv_xlen == 64)
#define LF_ARCH_RISCV64
#elif (__riscv_xlen == 32)
#define LF_ARCH_RISCV32
#else
#error RISCV detected but not RV64 or RV32
#endif /* riscv_xlen */

#else
#error Detected an unsupported architecture.
#endif /* arch */

#if defined(LF_ARCH_X86_64) || defined(LF_ARCH_ARM64) || \
	defined(LF_ARCH_RISCV64)
#define LF_ARCH_64_BIT
typedef uint64_t lf_native_word;
#elif defined(LF_ARCH_ARM32) || defined(LF_ARCH_X86) || defined(LF_ARCH_RISCV32)
#define LF_ARCH_32_BIT
typedef uint32_t lf_native_word;
#endif

#endif /* LF_PLATFORM_H */
