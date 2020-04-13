# Grammar Analysis

The PEGTL contains an `analyze()` function that checks a grammar for rules that can enter an infinite loop without consuming input.

## Content

* [Running](#running)
* [Example](#example)
* [Requirements](#requirements)
* [Limitations](#limitations)

## Running

In order to run an analysis on a grammar it is necessary to explicitly include `<tao/pegtl/contrib/analyze.hpp>`.
Then call `tao::pegtl::analyze()` with the top-level grammar rule as template argument.

```c++
#include <tao/pegtl/contrib/analyze.hpp>

const std::size_t issues = tao::pegtl::analyze< my_grammar >();
```

The `analyze()` function prints some information about the found issues to `std::cout` and returns the total number of issues found.
The output can be suppressed by passing `false` as sole function argument.

Analysing a grammar is usually only done while developing and debugging a grammar, or after changing it.

## Example

Regarding the kinds of issues that are detected, consider the following example grammar rules.

```c++
struct bar;

struct foo
   : sor< digit, bar > {};

struct bar
   : plus< foo > {};
```

When attempting to match `bar` against an input where the next character is not a digit the parser immediately goes into an infinite loop between `bar` calling `foo` and then `foo` calling `bar` again.

As shown by the example program `src/example/pegtl/analyze.cpp`, the grammar analysis will correctly detect a cycle without progress in this grammar.

Due to the differences regarding back-tracking and non-deterministic behaviour, this kind of infinite loop is a frequent issue when translating a CFG into a PEG.

## Requirements

The `analyze()` function relies on the [meta data](Meta-Data-and-Visit.md) contained in the rules *and* additional type traits called `analyze_traits`.

Every rule class `R` is first mapped to its implementation as per `R::rule_t` and then converted to a reduced form which is found as `analyze_traits< R, R::rule_t >::reduced`.

The reduced form of a rule uses only the basic combinators `opt`, `seq` and `sor` and captures just enough of the structure of the original rule as is required for the cycle analysis.

Everything else is transformed into either an equivalent form, e.g. `R = star< S >` is reduced to `R = opt< S, R >`, or a semantically reduced form, e.g. `at< S >` is reduced to `opt< S >`.

Further, for every rule class `R`, the list of sub rules as per `analyze_traits< R, R::rule_t >::reduced::subs_t` is used to drive inspection of the complete grammar by allowing the recursive step from a rule to the sub rules of its reduced form.

The reduced forms of the grammar rules are only allowed to use the following combinators and rules.

* `internal::bytes< 1 >` is for rules where "success implies consumption" is true.
* `internal::opt< ... >` is for rules where "success implies consumption" is false; assumes bounded repetition of conjunction of sub-rules.
* `internal::seq< ... >` is for rules where consumption on success depends on non-zero bounded repetition of the conjunction of sub-rules.
* `internal::sor< ... >` is for rules where consumption on success depends on non-zero bounded repetition of the disjunction of sub-rules.

See `include/tao/pegtl/contrib/analyze_traits.hpp` for many examples of how to correctly set up the analyze traits.

For any further reaching questions regarding how to set up the meta data and traits for custom rules please contact the authors at **taocpp(at)icemx.net**.

## Limitations

It has been conjectured, but, given the expressive power of PEGs, not proven, that *all* potential infinite loops are correctly detected.

In practice it appears to catch all cases of left-recursion that are typical for grammars converted from CFGs or other formalisms that gracefully handle left-recursion.

False positives are a theoretical problem in that, while relatively easy to trigger, they are not usually encountered when dealing with real world grammars.

Copyright (c) 2014-2020 Dr. Colin Hirsch and Daniel Frey
