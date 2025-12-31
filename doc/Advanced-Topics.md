# Advanced Topics

Advanced topics regarding use (and development) of this library.


## Contents

TODO: Parsing tokens or other objects?

* [Grammar Analysis](#grammar-analysis)
  * [How To](#how-to)
  * [Example](#example)
  * [Requirements](#requirements)
  * [Limitations](#limitations)
* [Optimizations](#optimizations)
  * [Backtracking](#backtracking)
  * [Whitespace](#whitespace)
* [Development](#development)


## Grammar Analysis // TODO -- and see below

Every grammar must be free of cycles that make no progress, i.e. it must not contain unbounded recursive or iterative rules that do not consume any input, as such grammar might enter an infinite loop.
One common pattern for these kinds of problematic grammars is the "dreaded" [left recursion](https://en.wikipedia.org/wiki/Left_recursion) that, while not a problem for less deterministic formalisms like CFGs, must be avoided with PEGs in order
 to prevent aforementioned infinite loops.

The PEGTL provides a [grammar analysis](Grammar-Analysis.md) that searches the rules of a grammar for cycles that make no progress.
While this could probably be implemented with compile-time meta-programming the implementation in the PEGTL avoids excessive compile times by doing everything at run-time.
It is best practice to create a separate dedicated program that does nothing else but run the grammar analysis, thus keeping this development and debug aid out of the main application.

```c++
#include <tao/pegtl.hpp>
#include <tao/pegtl/analyze.hpp>

// For this example we use the included JSON grammar.

#include <tao/pegtl/contrib/json.hpp>

namespace pegtl = tao::pegtl;

using grammar = pegtl::must< pegtl::json::text, pegtl::eof >;

int main()
{
   if( pegtl::analyze< grammar >() != 0 ) {
      std::cerr << "Cycles without progress detected!" << std::endl;
      return 1;
   }
   return 0;
}
```
The full example can be found in `src/pegtl/json_analyze.cpp`.
For more information see [Grammar Analysis](Grammar-Analysis.md).

## Grammar Analysis // TODO -- and see above

The PEGTL contains an `analyze()` function that checks a grammar for rules that can enter an infinite loop without consuming input.
Such loops are most often the result of [left recursion](https://en.wikipedia.org/wiki/Left_recursion) or of careless nesting of rules like `star< star< ... > >`.

### How To

In order to run an analysis on a grammar it is necessary to explicitly include `<tao/pegtl/debug/analyze.hpp>`.
Then call `tao::pegtl::analyze()` with the top-level grammar rule as template argument.

```c++
#include <tao/pegtl/debug/analyze.hpp>

const std::size_t issues = tao::pegtl::analyze< my_grammar >();
```

The `analyze()` function prints some information about the found issues to `std::cout` and returns the total number of issues found.
The output can be suppressed by passing `-1` as sole function argument, or be extended to give some information about the issues when called with `1`.

Analysing a grammar is usually only done while developing and debugging a grammar, or after changing it.

### Example

Regarding the kinds of issues that are detected, consider the following example rules.

```c++
struct bar;

struct foo
   : sor< digit, bar > {};

struct bar
   : plus< foo > {};
```

When attempting to match `bar` against an input where the next character is not a digit the parser immediately goes into an infinite loop between `bar` calling `foo` then `foo` calling `bar`.

As shown by the example program `src/pegtl/analyze.cpp`, the grammar analysis will correctly detect a cycle without progress in this grammar.

Due to the differences in back-tracking and (non-)deterministic semantics, this kind of infinite loop is a frequent issue when translating a CFG into a PEG.

### Requirements

The `analyze()` function operates on an abstract form of the grammar that is mostly equivalent to the original grammar regarding the possibility of infinite cycles without progress.

This abstract form is obtained via specialisations of the `analyze_traits<>` class template which each must have exactly one of `analyze_any_traits`, `analyze_opt_traits`, `analyze_seq_traits` and `analyze_sor_traits` as public (direct or indirect) base class.

Specialisations of the `analyze_traits<>` class template are appropriately implemented for all grammar rule classes included with the PEGTL.
This support automatically extends to all custom rules built "the usual way" via public inheritance of (combinations and specialisations of) rules included with the PEGTL.

For true custom rules, i.e. rules that implement their own `match()` function, the following steps need to be taken for them to work with the grammar analysis.

1. The rule needs a [`rule_t`](Meta-Data-and-Visit.md#rule-type) that, usually for true custom rules, is a type alias for the grammar rule itself.
2. There needs to be a specialisation of the `analyze_traits<>` for the custom rule's `rule_t`, with an additional first template parameter:

Assuming a custom rule like the following

```c++
struct my_rule
{
   using rule_t = my_rule;
   using subs_t = tao::pegtl::empty_list;  // tao/pegtl/type_list.hpp

   template< typename Input >
   bool match( Input& in )
   {
      // Do something and...
      return // ...return whether it matched.
   }
};
```

the analyze traits need to be set up as

```c++
template< typename Name >
struct tao::pegtl::analyze_traits< Name, my_rule >
   : analyze_any_traits<>  // Or one of the other 3 base classes, as appropriate.
{};
```

where the base class is chosen as follows.

1. `analyze_any_traits<>` is used for rules that always consume when they succeed.
2. `analyze_opt_traits<>` is used for rules that (can also) succeed without consuming.
3. `analyze_seq_traits<>` is used for rules that, regarding their match behaviour, are equivalent to `seq<>`.
4. `analyze_sor_traits<>` is used for rules that, regarding their match behaviour, are equivalent to `sor<>`.

If `my_rule` has rules, that it calls upon as sub-rules, as template parameters, these need to be passed as template parameters to the chosen base class.

Note that the first template parameter `Name` is required by the analyse traits of some rules in order to facilitate their transcription in terms of the basic combinators `seq`, `sor` and `opt`.

For example `R = plus< T >` is equivalent to `seq< T, opt< R > >`, and the corresponding specialisation of the analyse traits is as follows.

```c++
   template< typename Name, typename... Rules >
   struct analyze_traits< Name, internal::plus< Rules... > >
      : analyze_traits< Name, typename seq< Rules..., opt< Name > >::rule_t >
   {};
```

Note how the specialisation is for `internal::plus`, rather than `plus`, since that is used as the `rule_t` type alias.
And remember that the convention is for precisely the classes that actually implement `match()` to define `rule_t`.
The indirection of using `Rule::rule_t` rather than `Rule` itself to drive the grammar analysis greatly reduces the number of classes that need to define `rule_t`, and consequently greatly reduces the number of required `analyze_traits` specialisations.

Note further how `Name` is required to transform the implicitly iterative rule `plus` into an explicitly recursive form that only uses `seq` and `opt`.
The analyse traits have the task of simplifying the grammar in order to keep the core analysis algorithm as simple as possible.

Please consult `include/tao/pegtl/debug/analyze_traits_impl.hpp` for many examples of how to correctly set up analyse traits for more complex rules, in particular for rules that do not directly fall into one of the aforementioned four categories.

For any further reaching questions  how to set up the traits for custom rules please contact the authors at **taocpp(at)icemx.net**.

### Limitations

It has been conjectured, but, given the expressive power of PEGs, not proven, that *all* potential infinite loops are detected by the grammar analysis.

In practice it has proven to catch all cases of left-recursion that are typical for grammars converted from CFGs or other formalisms that can handle left-recursion.

False positives are possible and relatively easy to trigger intentionally, however we, the authors of the PEGTL, have never encountered one when dealing with real-world grammars.


## Optimizations

Notes on performance characteristics and optimisation possibilities.

### Backtracking

For performance reasons a grammar should be designed to minimise backtracking.
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

Grammars should be designed to minimise redundant multiple parsing of the same whitespace, comments or other padding.

One good way to achieve this is to choose a strategy for whitespace handling and then consistently stick to it.
For example the JSON grammar in `include/tao/pegtl/contrib/json.hpp` consistently has every rule for a "token" consume any following whitespace via the `ws` rule, too.
That way every rule can assume to start matching some "real" input since any whitespace would have already been consumed by the previous one.

### Regarding `at` and `one`

The `at<>`-rule never consumes input, and therefore always uses an input-marker to rewind the input back to where it started, regardless of the match-result.
In the context of optimising our [JSON library](https://github.com/taocpp/json), we noticed that the combination `at< one< ... > >` could be combined into an optimised `at_one< ... >` rule:
Instead of `one< ... >` advancing the input, and `at< one< ... > >` rewinding, the combined rule would omit both the advancing and the rewinding.

Put to the test, the optimised `at_one< '"' >` rule did not show any performance advantage over `at< one< '"' > >`, at least with `-O3`.
Presumably the compiler was smart enough to perform the optimisation by itself.

However with `-O0`, the optimised `at_one< '"' >` was faster by 5-10% in a [JSON library](https://github.com/taocpp/json) micro-benchmark.
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
