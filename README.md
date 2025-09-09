# liblf
liblf is a C99 library that provides code for lock-free programming.
I previously used [Concurrency Kit](https://github.com/concurrencykit/ck) and
appreciated its API for atomic operations and memory ordering primitives.
However, many of its functions are defined as macros, which make them harder
to work with.

liblf is my attempt to create a replacement that is easier to use in my projects,
leveraging code generation to make the functions readable.
