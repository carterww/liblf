# liblf Atomic Operations API
liblf provides a set of functions that implement various atomic operations
(also referred to as operations in liblf). These operations range from simple
loads/stores to more complex read-modify-write (RMW) operations.

## Types
liblf splits its operations into two categories: general and numerical. General operations
are implemented for all types while numerical operations are *only* implemented for
numerical types.

### General Types
Includes all [numerical types](#numerical-types) and the following:
- void \*

### Numerical Types
- intptr_t
- uintptr_t
- signed char[^1]
- unsigned char
- short
- unsigned short
- int
- unsigned int
- long
- unsigned long
- int8_t
- uint8_t
- int16_t
- uint16_t
- int32_t
- uint32_t
- int64_t
- uint64_t
- size_t
- lf_native_word[^2]

[^1]: char is explicitly signed because C99 does not dictate if "char" is signed or
unsigned.
[^2]: This is a custom unsigned type that is the same width as the architecture's
register.

## Functions
These can be found in [lf_op.h](../include/lf_op.h) and the implementation header
files. All of these functions are guaranteed to be atomic.

Each function uses a suffix to denote what type it's operating on. For example,
`lf_op_load_uptr()` loads a `uintptr_t`. The suffixes will be left off the function
names in the following sections.

### Functions On General Types
Includes all [functions on numerical types](#functions-on-numerical-types) and the
following:
- **lf_op_load()**: Loads a value from memory and returns its value.
- **lf_op_store()**: Stores a value into memory.
- **lf_op_swap()**: Stores a value into memory and returns its previous value.
- **lf_op_cas()**: Compares a memory location with an expected value and places
  a new value into memory iff the memory location and expected value are equal.
  Returns true if the values are swapped, false if the memory location does not
  equal the expected value.
- **lf_op_casx()**: Same as `lf_op_cas()` except it takes a pointer to the
  expected value and places the memory location's old value into the expected
  value on failure.
- **lf_op_dcas()**: Same as `lf_op_cas()` except it does an atomic CAS on two
  adjacent memory locations. See [DCAS Discussion](#dcas-discussion) for more
  information.
- **lf_op_dcasx()**: Same as `lf_op_casx()` except it does an atomic CASX on
  adjacent memory locations. See [DCAS Discussion](#dcas-discussion) for more
  information.

### Functions On Numerical Types
- **lf_op_inc()**: Increment a value in memory by 1.
- **lf_op_dec()**: Decrement a value in memory by 1.
- **lf_op_add()**: Add to a value in memory.
- **lf_op_sub()**: Subtract from a value in memory.
- **lf_op_and()**: Perform a bitwise AND on a value in memory.
- **lf_op_or()**: Perform a bitwise OR on a value in memory.
- **lf_op_xor()**: Perform a bitwise XOR on a value in memory.
- **lf_op_fainc()**: Same as `lf_op_inc` except it returns the previous value.[^3]
- **lf_op_fadec()**: Same as `lf_op_dec` except it returns the previous value.
- **lf_op_faadd()**: Same as `lf_op_add` except it returns the previous value.
- **lf_op_fasub()**: Same as `lf_op_sub` except it returns the previous value.
- **lf_op_faand()**: Same as `lf_op_and` except it returns the previous value.
- **lf_op_faor()**: Same as `lf_op_or` except it returns the previous value.
- **lf_op_faxor()**: Same as `lf_op_xor` except it returns the previous value.
- **lf_op_bts()**: Set a single bit of a value in memory and return the bit's previous value.
- **lf_op_btc()**: Complement a single bit of a value in memory and return the bit's previous
  value.
- **lf_op_btr()**: Reset (zero) a single bit of a value in memory and return the bit's previous
  value.

General Note: Using the "fetch-and" variants are generally not free. For example,
`lf_op_faadd` and `lf_op_fasub` are essentially free on x86-64, but the bitwise
variants are not. The bitwise variants decay into a CAS retry loop because x86-64
does not have an instruction for this like XADD.

[^3]: "fa" means "fetch-and."

## Operation Support
liblf provides a [header file (lf_op_sup.h)](../include/lf_op_sup.h) that defines
macros to indicate support for operation-type combos.
[lf_op_gcc_sup.h](../include/lf/gcc/lf_op_gcc_sup.h) defines the *lowest common
denominator* of supported operations across all architectures liblf supports. This means
if [lf_op_gcc_sup.h](../include/lf/gcc/lf_op_gcc_sup.h) says an operation is supported,
you can expect all architectures to support it.

Here are some rules of thumb regarding supported operations:
- Operations on long, unsigned long, int64_t, and uint64_t are not considered "portable"
  because 32 bit architectures usually don't provide instructions to atomically operation
  on 64 bit values.
- DCAS is not portable. See [DCAS Discussion](#dcas-discussion) for more information.

## DCAS Discussion
DCAS is not a widely supported operation. For that reason, I don't include it in the list
of operations that liblf always supports (see [Operation Support](#operation-support)).
You should guard any use of DCAS with `#if LF_OP_DCAS_<type>_SUP` and provide alternative
methods to remain portable.

DCAS also requires the use of types defined by liblf named `lf_dcas_packed_<type>`.
This is for a few reasons:
1. DCAS requires a greater alignment than the regular type (uint64_t must be 16 byte
   aligned). These types are defined with the proper compiler alignment attributes.
2. The `lf_dcas_packed` type is really a union of a scalar of double width and a tuple.
   This allows `lf_op_cas` to be used for small types that don't need to use an instruction
   like `cmpxchg16b`.
