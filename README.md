# liblf
liblf is a C99 library that provides code for lock-free programming.
I previously used [Concurrency Kit](https://github.com/concurrencykit/ck) and
appreciated its API for atomic operations and memory ordering primitives.
However, many of its functions are defined as macros, which make them harder
to work with.

liblf is my attempt to create a replacement that is easier to use in my projects,
leveraging code generation to make the functions readable.

## Operations
liblf provides a set of type-safe atomic operations for almost any standard type you
care about. Currently, I only provide two implementations: one that uses GCC builtins
and another that uses hand-tuned x86_64 ASM. Here are the types that liblf natively
supports:
- void *
- intptr_t, uintptr_t
- signed char, unsigned char
- short, unsigned short
- int, unsigned int
- long, unsigned long
- int8_t, uint8_t
- int16_t, uint16_t
- int32_t, uint32_t
- int64_t, uint64_t
- size_t
- lf_native_word (Guaranteed to be register size for arch)

Functions are generated for each type from a generator program, not macros. This makes
the functions readable and debuggable, but it this makes the files very long
(lf_op_x64.h is ~4,900 lines long).

Here are the atomic operations liblf implements for each type:
- **load**: Load a value from memory.
- **store**: Store a value into memory.
- **swap**: Store a value into memory and returns its previous value.
- **cas** Perform a compare-and-swap and return a boolean indicating
  if it was successful.
- **cax**: Perform a compare-and-swap and return the destination's previous value.
- **dcas**: Similar to cas but performs a double width compare-and-swap (x64 only).
- **dcasx**: Similar to casx but performs a double width compare-and-swap (x64 only).
- **inc**: Increments the value by 1.
- **dec**: Decrements the value by 1.
- **add**: Add to the value.
- **sub**: Subtract from the value.
- **and**, **or**, **xor**: Perform the bitwise operation on the value.
- **fainc**, **fadec**, **faadd**, **fasub**, **faand**, **faor**, **faxor**: Fetch a
  value from memory and do the operation to it (fa stands for fetch-and).
- **bts**: Set a bit to 1 and return the bit's previous value as a bool.
- **btr**: Set a bit to 0 and return the bit's previous value as a bool.
- **btc**: Set a bit to its complement and return the bit's previous value as a bool.

These may seem like a lot and all of them after *inc* can be replaced with a CAS do
while loop, but the point is to implement each efficiently for each architecture. My
goal with this library is to do all atomic operations as efficiently as possible for
all prominent architectures. If that means adding a new function for a new x86 xaddxor
instruction (fetch-and-add-and-xor), so be it.

### Operation Support
liblf is meant to be a low-level, portable library that implements lock-free operations.
To make liblf portable, it provides lf_op_*arch*_sup headers that define macros
indicating whether an operation is supported for a type on *arch*.

lf_op_gcc_sup.h defines the *lowest common denominator* of supported operations across all
supported architectures. If lf_op_gcc_sup.h defines a macro to 1, that operation-type
combo is supported on all supported architectures.

General rules of thumb:
- DCAS is not portable. Technically DCAS could be implemented universally on any
  type of size <= 2 but that is useless.
- Operations on long, unsigned long, int64_t, and uint64_t are not supported. These
  operations are still defined using GCC builtins but aren't guaranteed to be
  lock-free for 32-bit architectures.
- Integer based operations like add, xor, bts, etc. are not supported for void *.

To be 100% portable you should guard lf_op calls with support macros from
lf_op_gcc_sup.h. In practice, unless you need to run on 32-bit systems, you
usually don’t need to worry.
