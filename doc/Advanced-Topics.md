# Advanced Topics

Advanced topics regarding use (and development) of this library.


## Contents

TODO: Parsing tokens or other objects?

* [Optimizations](#optimizations)
  * [Backtracking](#backtracking)
  * [Whitespace](#whitespace)
* [Development](#development)


## Parsing Objects



## Optimizations

Notes on performance characteristics and optimisation possibilities.

### Backtracking

For performance reasons a grammar should be designed to minimize backtracking.
We will start with a simple example.

```c++
using namespace tao::pegtl;
struct R = sor< seq< A, B >, seq< A, C > > {};  // R = (AB)/(AC)
```

If the input matches `seq< A, C >`, then matching `R` on said input will parse `A` twice (assuming that `B` does not match anything that `C` does).
The first time `A` will match successfully during the unsuccessful attempt to match `seq< A, B >`.
The second time `A` will match the same part of the input successfully again during the successful attempt to match `seq< A, C >`.
The solution is to change the grammar as follows.

```c++
struct R = seq< A, sor< B, C > > {};  // R = A(B/C)
```

Not backtracking over `A` has the additional advantage of not triggering any action attached to `A` twice.

In practice, opportunities to remove superfluous backtracking might not be as obvious as with such a simple rule.
For a more complex example please read the comments ate the beginning of the Lua 5.3 grammar in `src/pegtl/lua53.hpp`.
It shows how to eliminate both left-recursion and superfluous backtracking with multiple rules and recursions.

### Whitespace

Grammars should be designed to minimize redundant multiple parsing of the same whitespace, comments or other padding.

One good way to achieve this is to choose a strategy for whitespace handling and then consistently stick to it.
For example the JSON grammar in `include/tao/pegtl/contrib/json.hpp` consistently has every rule for a "token" consume any following whitespace via the `ws` rule, too.
That way every rule can assume to start matching some "real" input since any whitespace would have already been consumed by the previous one.

### Regarding `at` and `one`

The `at<>`-rule never consumes input, and therefore always uses an input-marker to rewind the input back to where it started, regardless of the match-result.
In the context of optimising our [JSON library](https://github.com/taocpp/json), we noticed that the combination `at< one< ... > >` could be combined into an optimized `at_one< ... >` rule:
Instead of `one< ... >` advancing the input, and `at< one< ... > >` rewinding, the combined rule would omit both the advancing and the rewinding.

Put to the test, the optimized `at_one< '"' >` rule did not show any performance advantage over `at< one< '"' > >`, at least with `-O3`.
Presumably the compiler was smart enough to perform the optimisation by itself.

However with `-O0`, the optimized `at_one< '"' >` was faster by 5-10% in a [JSON library](https://github.com/taocpp/json) micro-benchmark.
As the PEGTL should only be used with optimizations enabled, we removed the `at_one<>` rule, as we try to reduce the number of rules that don't provide a clear benefit.

We still need to test whether the compiler manages to perform the same optimisation in more complex cases.


## Development

Public notes on past, present and future development of the PEGTL.

### C++ Standard

Version 0.x of the PEGTL requires at least C++11.

Version 1.x of the PEGTL requires at least C++11.

Version 2.x of the PEGTL requires at least C++11.

Version 3.x of the PEGTL requires at least C++17.

Version 4.x of the PEGTL requires at least C++17.

Version 5.x of the PEGTL will jump to C++20 or C++23.

### C++ Features

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

### Other Things

 * Investigate whether we are crazy enough to implement parsing non-contiguous memory like linked lists?
 * Investigate whether we are crazy enough to implement parsing non-linear data structures like trees?

### Stream Parsing

A couple of things that *could* be done in the area of stream parsing.

 * Use the double-mapped ring-buffer to prevent `discard()` having to move data.
 * Debug input and related facilities that detect when data in the input buffer is accessed after being discarded or moved (by a discard).
 * Investigate the use of ("stackful") coroutines for parsing from a network socket, and
 * investigate whether this can also be used for stream parsing that keeps everything.

### Development Tools

We use the code coverage facilities of the GCC and Clang compilers to meet our goal of 100% code coverage with unit tests.
Unfortunately the coverage reports are not 100% reliable.

We also try to use `clang-format` to ensure consistent formatting of our source code.
Unfortunately it, too, can get confused and result in incorrectly formatted code.

For example under some circumstances `clang-format` does not understand whether a `&&` is the logical operator or an r-value reference declarator, or whether a `<` is a comparison operator or introduces a template argument list.


---

This page is part of the [PEGTL](https://github.com/taocpp/PEGTL) and its [documentation](README.md).

Copyright (c) 2017-2026 Dr. Colin Hirsch and Daniel Frey<br>
Distributed under the Boost Software License, Version 1.0<br>
See accompanying file [LICENSE_1_0.txt](../LICENSE_1_0.txt) or copy at https://www.boost.org/LICENSE_1_0.txt
