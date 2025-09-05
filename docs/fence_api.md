# liblf Fence API
liblf provides a set of fence functions that enforce memory ordering on
loads, stores, and atomic RMW operations. liblf's fences API is closer to
the hardware's memory model than C++11's memory model, but it still offers
a small abstraction over native fence instructions.

This API was heavily inspired by [ck's](https://github.com/concurrencykit/ck)
fence API.

## Functions
These can be found in (lf_op.h)[../include/lf_op.h] and the implementation
header files.
- **lf_fence_cc()**: Prevents compiler from reordering operations.
- **lf_fence_full()**: Always emits a fence that orders all temporal memory operations.
  (Does not order instructions like rdtsc or non-temporal memory accesses.)
- **lf_fence_store()**: Order store->store.
- **lf_fence_load()**: Order load->load.
- **lf_fence_store_load()**: Order store->load.
- **lf_fence_load_store()**: Order load->store.
- **lf_fence_atomic()**: Order atomic->atomic (atomic refers to a RMW write operation
  from lf_op.h like lf_op_sub_u8).
- **lf_fence_atomic_load()**: Order atomic->load.
- **lf_fence_load_atomic()**: Order load->atomic.
- **lf_fence_atomic_store()**: Order atomic->store.
- **lf_fence_store_atomic()**: Order store->atomic.
- **lf_fence_full_force()**: Always emits a fence that orders all operations.
- **lf_fence_store_force()**: Always emits a store->store fence.
- **lf_fence_load_force()**: Always emits a load->load fence.

## Architecture Support
liblf currently supports architectures with Total Store Order (TSO) and
Weak Memory Order (WMO or RMO) memory models. The following architectures
have native fence support:
- x86-64

The following architectures use GCC __atomic builtins to implement fences:
- x86
- ARM32
- ARM64
- RISCV32
- RISCV64
