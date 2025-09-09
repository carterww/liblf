# liblf Fence Model
liblf provides a set of fence (memory barrier) functions that offer a lightweight
abstraction over hardware fence instructions. Using these fences is required
because liblf atomic operations in [lf_op.h](../include/lf_op.h) make no guarantees
about memory ordering.

This document serves as an introduction to fences, not an API reference for liblf.
See [fence_api](fence_api.md) for an API reference.

## Why Fences are Required
Imagine there are two threads running in parallel that are operating on a shared
area of memory. For the sake of this thought experiment, assume all operations are
atomic (other thread sees the result of the operation or nothing from it, nothing
in between).

Let's say T1 is writing to two shared variables: "x" and "ready." T2 is loading
"ready" until it equals true. Once it sees that "ready == true," it reads "x" and
does some work with it. The $1,000,000 question: is T2 guaranteed to see the value
T2 wrote to "x" once "ready == true?" The answer is maybe.

```
// T1          |   // T2
x = 40         |   while (!ready) {}
ready = true   |   assert(x == 40)
```

### When is the Answer Always Yes?
T2 is guaranteed to see the new value of "x" after "ready == true" if we are running
the program on an architecture with a memory model named Total Store Order (TSO).

TSO states the following:
1. Loads cannot be reordered after subsequent loads.
2. Loads cannot be reordered after subsequent stores.
3. Stores cannot be reordered after subsequent stores.
4. Stores *can* be reordered after subsequent loads.

From these rules, we can see T2 is guaranteed to see "x == 40" if "ready == true."
T1 stores 40 into "x" then stores true into "ready" and by rule (3), other threads will
observe the stores in that order. And by rule (1), T2 will always load "ready" before
"x." This means T2 will always see "x == 40" if "ready == true."

### When is the Answer Maybe?
TSO is the memory model x86 and x86-64 CPUs use, but it is not the only one. The other
major memory model is the Weak Memory Order (or Relaxed-Memory Order) model (WMO or RMO).
This is the memory model ARM and (most) RISC-V CPUs use, and it is like the Wild West compared
to the TSO memory model. (There is also Partial Memory Order (PSO) but that is not supported
in liblf and very old.)

WMO states the following:
1. Loads *can* be reordered after subsequent loads.
2. Loads *can* be reordered after subsequent stores.
3. Stores *can* be reordered after subsequent stores.
4. Stores *can* be reordered after subsequent loads.

From these rules, it becomes clear why the answer is maybe. WMO *does not* guarantee
T2 will observe T1's stores in program order. Or even that T2 will retire the load instructions
in program order!

NOTE: WMO does guarantee that *dependent* load and store operations cannot be reordered. Imagine
if this got reordered by your CPU:

```
x = 5
x = 10
```

### There's One More Contender?
It turns out the CPU isn't the only entity that can reorder load and store operations.
The compiler can actually reorder loads and stores at compile time in the name of
performance.

Compilers are allowed to reorder operations if it doesn't break the
[as-if rule](https://en.cppreference.com/w/cpp/language/as_if.html). In essence, if
the compiler reorders operations, it must behave "as if" the original code was
executing.

This isn't an issue in a single-threaded program where the compiler can see all
side effects. For example, if I write the following code:
```
x = 0
...
x = 5
```
the compiler can remove the first write to "x" as 0 if x isn't used between
the assignment and reassignment. (And because "x" isn't marked as volatile.)

In multithreaded code, the compiler cannot always observe how Thread A's
operations affect Thread B. Using the last example, maybe "x" is a shared variable
and we may want to actually want to set "x = 0" before "x = 5" for another
thread. This can be solved by using atomic operations in [lf_op.h](../include/lf_op.h)
or by making x volatile.

### Where Do Fences Come In?
Fences allow the programmer to force operations to be in a specific order. In liblf,
there are two classes of fences.
1. Compiler Fence: Instructs the compiler to not reorder operations around the fence.
   Compiler fences *do not* generate any CPU instructions to enforce ordering.
2. Hardware Fence: Instructs the compiler and CPU to not reorder certain types of
   operations around the fence. An ISA usually has multiple fence instructions for
   different types of operations.

liblf provides a set of fence functions that act as an abstraction over the hardware
fences. I will not go over every fence functions here (see [fence_api](fence_api.md)
for that), but I will show how we can fix that previous example.

### Fixing The Example
As we saw previously, the following example will not work on every platform:
```
// T1           |   // T2
x = 40          |   while (!ready) {}
ready = true    |   assert(x == 40)
```
Luckily, we can fix it easily by inserting a fence in each thread's code:
```
// T1              |   // T2
x = 40             |   while (!ready) {}
lf_fence_store()   |   lf_fence_load()
ready = true       |   assert(x == 40)
```

T1's fence ensures the store to "ready" will be visible only after
the store to "x" is. T2's fence ensures the CPU won't execute the
load for "x" before the load for "ready." Both fences also prevent the
compiler from reordering the operations.

NOTE: liblf implements lf_fence_store() and lf_fence_load() differently
depending on the target architecture. For example, these compile to
nothing on x86 and x86-64 and hardware fences on ARM and RISC-V.

This fixes the memory ordering issue, but the compiler may still optimize
the program by caching "x" and "ready" in registers and never updating their
values from memory. To fix that, you need [atomic operations](../include/lf_op.h)
or volatile variables.
