/* SPDX-License-Identifier: MIT
 * Copyright (c) 2025 Carter Williams
 */

#include <lf_cc.h>
#include <lf_platform.h>

#if defined(LF_CC_GCC) || defined(LF_CC_CLANG)

/* Only x86_64 supported natively for now :( */
#if defined(LF_ARCH_X86_64)
#include <lf/gcc/lf_op_x64.h>
#else
#include <lf/gcc/lf_op_gcc.h>
#endif /* arch */

#else
#error liblf requires GCC or Clang because it uses inline asm and gcc builtins
#endif /* CC */
