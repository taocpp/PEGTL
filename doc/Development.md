# Development

### C++ Standard

Version 0.x of the PEGTL requires at least C++11.

Version 1.x of the PEGTL requires at least C++11.

Version 2.x of the PEGTL requires at least C++11.

Version 3.x of the PEGTL requires at least C++17.

Version 4.x of the PEGTL requires at least C++17.

Version 5.x of the PEGTL will make the jump to C++20 or even C++23.

 * Add support for C++20 `char8_t` where appropriate.
 * Use C++20 `std::span` in inputs and everywhere else it makes sense.
 * Investigate whether there is anything useful we can do with Ranges in the PEGTL.
 * Use C++20 Concepts instead of all the SFINAE and meta-programming where possible.
 * Give examples for C++20 "lambdas in unevaluated contexts" in conjunction with `tao::pegtl::function`.
 * Keep an open eye for opportunities to use C++20 spaceship operator. Spaceship!
 * Keep an open eye for opportunities to use C++20 defaulted comparison operators.
 * Keep an open eye for opportunities to use C++20 `[[likely]]` and `[[unlikely]]`.
 * Keep an open eye for opportunities to use C++20 `constinit` and `consteval`, and
 * keep an open eye for opportunities to use the extended `constexpr` facilities.
 * keep an open eye for opportunities to use the extended CTAD facilities from C++20.
 * Keep an open eye for opportunities to use class types as non-type template parameters.
 * Replace the hand-crafted endian facilities with C++20 `std::endian` and C++23 `std::byteswap`.
 * Investigate how C++20 and C++23 compile-time facilities can help with compile-time strings.
 * Investigate whether we can use C++20 `std::bit_cast` to improve some of the low-level code.
 * Use C++23 "deducing this" feature to let base class `make_rewind_guard()` return a rewind guard for a derived class.
 * Can we assume the C++17 `charconv` facilities are universally available? Can we do this for 4.x?

### Other Things

 * ~~Build a compile-time facility to convert Unicode code points to UTF8 sequences!~~
   * Done in version 4.0.
 * Investigate whether we are crazy enough to attempt parsing linked lists or trees?

### Buffer Inputs

A couple of things that could be done in the area of buffer inputs.

 * Optional automatic discard.
 * Use the double-mmap ring-buffer to prevent `discard()` having to copy data within the buffer.
 * Debug input and related facilities that detect when data in the input buffer is accessed after being discarded and/or moved by another discard.
 * Investigate the use of ("stackful") coroutines for parsing from a network socket, and
 * investigate whether this can also be used for incremental parsing that keeps everything.

---

This document is part of the [PEGTL](https://github.com/taocpp/PEGTL).

Copyright (c) 2023-2024 Dr. Colin Hirsch and Daniel Frey<br>
Distributed under the Boost Software License, Version 1.0<br>
See accompanying file [LICENSE_1_0.txt](../LICENSE_1_0.txt) or copy at https://www.boost.org/LICENSE_1_0.txt
