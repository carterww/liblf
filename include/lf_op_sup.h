/* SPDX-License-Identifier: MIT
 * Copyright (c) 2025 Carter Williams
 */

#ifndef LF_OP_SUP_H
#define LF_OP_SUP_H

#include <lf_platform.h>

/* Only x86_64 supported natively for now :( */
#if defined(LF_ARCH_X86_64)
#include <lf/gcc/lf_op_x64_sup.h>
#else
#include <lf/gcc/lf_op_gcc_sup.h>
#endif /* arch */

#endif /* LF_OP_SUP_H */
