# liblf
liblf is a C99 library that provides code for lock-free programming.
I previously used [Concurrency Kit](https://github.com/concurrencykit/ck) and
appreciated its API for atomic operations and memory ordering primitives.
However, many of its functions are defined as macros, which make them harder
to work with.

liblf is my attempt to create a replacement that is easier to use in my projects,
leveraging code generation to make the functions readable.

## Getting Started
liblf provides a set of [examples](./examples) that show how to use the library.
There is also an incomplete set of [documentation](./docs).

If you still have questions, you can peruse the source code for comments or email
me at <carter@carterwilliams.cc>.

## Table of Contents
### Documentation
- [Fences Introduction](./docs/fence_intro.md)
- [Fences API](./docs/fence_api.md)
- [Atomic Operations API](./docs/atomic_api.md)

### Examples
- [Basic](./examples/example_basic.c)
- [Bitstring](./examples/example_bitstring.c)
- [DCAS](./examples/example_dcasx.c)
- [Ring](./examples/example_ring.c)

### External Resources
- [Article About Memory Ordering](https://dev.to/kprotty/understanding-atomics-and-memory-ordering-2mom)
- [A Better Lock-Free Library](https://github.com/concurrencykit/ck)
- [MPMC Ring Buffer Implementation](https://www.1024cores.net/home/lock-free-algorithms/queues/bounded-mpmc-queue)
- [Intel Developer's Manual](https://software.intel.com/en-us/download/intel-64-and-ia-32-architectures-sdm-combined-volumes-1-2a-2b-2c-2d-3a-3b-3c-3d-and-4)
