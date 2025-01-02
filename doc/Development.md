# Development

Notes on past, present and future development.
Public parts of the development roadmap.


## C++ Standard

Version 0.x of the PEGTL requires at least C++11.

Version 1.x of the PEGTL requires at least C++11.

Version 2.x of the PEGTL requires at least C++11.

Version 3.x of the PEGTL requires at least C++17.

Version 4.x of the PEGTL requires at least C++17.

Version 5.x of the PEGTL will jump to C++20 or C++23.


## C++ Features

 * Add support for C++20 `char8_t` where appropriate.
 * Use C++20 `std::span` in inputs and everywhere else it makes sense.
 * Investigate whether there is anything useful we can do with Ranges in the PEGTL.
 * Use C++20 Concepts instead of all the SFINAE and meta-programming where possible.
 * Give examples for C++20 "lambdas in unevaluated contexts" in conjunction with `tao::pegtl::function`.
 * ~~Keep an open eye for opportunities to use C++20 spaceship operator. Spaceship!~~
 * Keep an open eye for opportunities to use C++20 defaulted comparison operators.
 * Keep an open eye for opportunities to use C++20 `[[likely]]` and `[[unlikely]]`.
 * Keep an open eye for opportunities to use C++20 `constinit` and `consteval`, and
 * keep an open eye for opportunities to use the extended `constexpr` facilities.
 * For example make the bulk() and size() functions in the peek classes consteval!
 * Keep an open eye for opportunities to use the extended CTAD facilities from C++20.
 * Keep an open eye for opportunities to use class types as non-type template parameters.
 * Replace the hand-crafted endian facilities with C++20 `std::endian` and C++23 `std::byteswap`.
 * Investigate how C++20 and C++23 compile-time facilities can help with compile-time strings.
 * Investigate whether we can use C++20 `std::bit_cast` to improve some of the low-level code.
 * Use C++23 "deducing this" feature to let base class `make_rewind_guard()` return a rewind guard for a derived class.
 * When can we assume the C++17 `charconv` facilities are universally available?


## Other Things

 * ~~Build a compile-time facility to convert Unicode code points to UTF8 sequences!~~
   * Done in version 4.0.
 * Investigate whether we are crazy enough to implement parsing linked lists or trees?


## Buffer Inputs

A couple of things that *could* be done in the area of buffer inputs.

 * Optional automatic discard.
 * Use the double-mapped ring-buffer to prevent `discard()` having to copy data within the buffer.
 * Debug input and related facilities that detect when data in the input buffer is accessed after being discarded or moved (by a discard).
 * Investigate the use of ("stackful") coroutines for parsing from a network socket, and
 * investigate whether this can also be used for incremental parsing that keeps everything.


## Development Tools

We use the code coverage facilities of the GCC and Clang compilers to meet our goal of 100% code coverage with unit tests.
Unfortunately the coverage reports are not 100% reliable.

We also try to use `clang-format` to ensure consistent formatting of our source code.
Unfortunately it, too, can get confused and result in incorrectly formatted code.

For example under some circumstances `clang-format` does not understand whether a `&&` is the logical operator or an r-value reference declarator, or whether a `<` is a comparison operator or introduces a template argument list.

---

This document is part of the [PEGTL](https://github.com/taocpp/PEGTL).

Copyright (c) 2023-2025 Dr. Colin Hirsch and Daniel Frey<br>
Distributed under the Boost Software License, Version 1.0<br>
See accompanying file [LICENSE_1_0.txt](../LICENSE_1_0.txt) or copy at https://www.boost.org/LICENSE_1_0.txt
