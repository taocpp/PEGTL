# Debug Facilities

Functions and classes to debug grammars, both via their static properties and their run-time behavior.


## Contents

* [Preamble](#preamble)
* [Meta Data](#meta-data)
  * [Rule Type](#rule-type)
  * [Sub Rules](#sub-rules)
  * [Implementation Details](#implementation-details)
* [Parse Trace](#parse-trace)
  * [Trace Traits](#trace-traits)
  * [Rule Selection](#rule-selection)
  * [Trace Action](#trace-action)
* [Rule Coverage](#rule-coverage)
* [Grammar Visit](#grammar-visit)
  * [Grammar Rules](#grammar-rules)
  * [Visit Function](#visit-function)
* [Grammar Print](#grammar-print)
* [Grammar Analysis](#grammar-analysis)
  * [Analyze Function](#analyze-function)
  * [Analyze Traits](#analyze-traits)
  * [Limitations](#limitations)


## Preamble

The [meta data](#meta-data) provides compile-time accessible information about rule classes, most importantly a list of sub-rules.

The [parse trace](#parse-trace) and [rule coverage](#rule-coverage) perform parsing runs with debug information either in the form of raw events or aggregated statistics, respectively.

The [grammar visit](#grammar-visit) uses the [meta data](#meta-data) to call functions for all rules of a grammar, with example functions provided that print them all.

The [grammar analysis](#grammar-analysis) uses the [meta data](#meta-data) and the [analyze traits](#analyze-traits) to check a grammar for cycles without progress, including left recursion.

#### Namespaces

Everything resides in namespace `tao::pegtl` unless noted otherwise.
This default can be changed via the macro `TAO_PEGTL_NAMESPACE` in `include/tao/pegtl/config.hpp`.
As usual the namespace `tao::pegtl` is generally omitted on this page.


## Meta Data

The meta data consists of the [type aliases](Rules-and-Grammars.md#type-aliases) `rule_t` and `subs_t` of every rule.

[True custom rules](Rules-and-Grammars.md#implementing-rules), i.e. rules that implement their own `match()` function, do not need these type aliases for parsing.
They are required for facilities that automatically inspect a grammar, in particular the functions based on [grammar visit](#grammar-visit), [rule coverage](#rule-coverage), [grammar analysis](#grammar-analysis), and the [parse tree](Parse-Tree.md).

The respective "meta data and implementation mapping" sections in the [Rule Reference](Rule-Reference.md) show how `rule_t` and `subs_t` are defined for the rules included with the PEGTL.

> [!NOTE]
> For some rules `R` the type aliases `R::rule_t` and `R::subs_t` are not what might be expected.

### Rule Type

For each rule `R`, the type alias `R::rule_t` is the type of the class that implements the `match()` function used for `R`.

For most rules this is the root of the inheritance hierarchy, which is usually an implementation class in namespace `internal`.

For `R = seq< U, V >` we have `R::rule_t = internal::seq< U, V >`.

For `R = seq<>` we have `R::rule_t = internal::success`.

For `R = opt< U, V >` we have `R::rule_t = internal::opt< internal::seq< U, V > >`.

These examples show how, behind the scenes, rules or special cases of rules are implemented in terms of other rules.

### Sub Rules

For each rule `R`, the type alias `R::subs_t` is a `type_list` with all direct sub-rules of `R` as template parameters.

For example, for `R = seq< U, V >` we have `R::subs_t = type_list< U, V >`.

For `R = seq<>` we have `R::subs_t = empty_list = type_list<>`.

For `R = opt< U, V >` we have `R::subs_t = type_list< internal::seq< U, V > >`.

Note that `subs_t` contains the sub-rules of the implementation mapping, which can be internal rules.
For example, many rules that take multiple sub-rules are implemented by first wrapping these sub-rules in an `internal::seq`.

### Implementation Details

While accessible in the namespace `tao::pegtl`, the [rules and combinators](Rule-Reference.md) included with the PEGTL usually have their actual implementation in the sub-namespace `internal`.
For many rules the header `include/tao/pegtl/rules.hpp` shows how the user-facing rules forward to their `internal` implementations.

This additional level of indirection prevents unintended invocation of user-defined actions due to some PEGTL rules being built from other rules.
For example, if a rule `rep_min` is implemented with a sub-rule `star`, then an action attached to a user-written `star` with the same sub-rules should not also fire for the implementation detail inside `rep_min`.

Therefore it is possible to selectively disable most [control](Control-and-Normal.md) functions, including the `apply` functions that perform action invocation, on a rule-by-rule basis.
More precisely, the action and debug control functions are only invoked for a rule `R` when `Control< R >::enable` is `true`.

The default [normal control](Control-and-Normal.md#normal-control) defines `normal< R >::enable` in terms of `internal::enable_control< R >`, which is `true` by default, but `false` for the implementation rules in namespace `internal`.
This hides implementation details from actions and most control functions during normal parsing.

Some debug facilities deliberately expose these implementation details.
For example, [`complete_trace()`](#parse-trace) shows them, and the [grammar visit](#grammar-visit) follows the exact `subs_t` lists from the meta data.


## Parse Trace

The header `include/tao/pegtl/debug/trace.hpp` contains functions to perform a parsing run with trace information printed to `std::cerr`.

```c++
template< typename TraceTraits,
          typename Rule,
          template< typename... > class Action = nothing,
          template< typename... > class Control = normal,
          typename ParseInput,
          typename... States >
bool generic_trace( ParseInput&&, States&&... );

template< typename Rule,
          template< typename... > class Action = nothing,
          template< typename... > class Control = normal,
          typename ParseInput,
          typename... States >
bool standard_trace( ParseInput&&, States&&... );

template< typename Rule,
          template< typename... > class Action = nothing,
          template< typename... > class Control = normal,
          typename ParseInput,
          typename... States >
bool complete_trace( ParseInput&&, States&&... );

template< typename RuleList,
          typename Rule,
          template< typename... > class Action = nothing,
          template< typename... > class Control = normal,
          typename ParseInput,
          typename... States >
bool include_trace( ParseInput&&, States&&... );

template< typename RuleList,
          typename Rule,
          template< typename... > class Action = nothing,
          template< typename... > class Control = normal,
          typename ParseInput,
          typename... States >
bool exclude_trace( ParseInput&&, States&&... );
```

The trace functions are called like [`parse()`](Inputs-and-Parsing.md#parse-function) and return whether the parsing run succeeded.

The difference between the first two convenience functions is that the *standard* trace only prints trace information for rules `R` where `normal< R >::enable` is `true`.
This excludes the internal rules that are considered implementation details of documented rules.
The *complete* trace includes trace information for all rules.

The include and exclude convenience functions take an additional list of rules as template parameter to select which rules to print trace information for.
See [below](#rule-selection) for details.

The trace output contains the current input position, numbered rule starts, `success` and `failure` events, rewind events, global-failure events `raise`, `raise_nested` and `unwind`, and action events `apply` and `apply0`.
The exact rule names in the output depend on the compiler and the demangler.

```c++
#include <tao/pegtl.hpp>
#include <tao/pegtl/debug/trace.hpp>
#include <tao/pegtl/example/json.hpp>

namespace pegtl = tao::pegtl;

using grammar = pegtl::seq< pegtl::json::text, pegtl::eof >;

int main( int argc, char** argv )
{
   if( argc != 2 ) {
      return 1;
   }

   pegtl::text_file_input< pegtl::scan::lf > in( argv[ 1 ] );
   return pegtl::standard_trace< grammar >( in ) ? 0 : 1;
}
```

The full JSON trace example can be found in `src/example/json_trace.cpp`.

### Trace Traits

The trace output is configured through a traits class.
The default traits are `standard_trace_traits` and `complete_trace_traits`, both based on `trace_traits`.
The traits are defined in `include/tao/pegtl/debug/trace_traits.hpp`; the lower-level support classes are in `include/tao/pegtl/debug/trace_state.hpp` and `include/tao/pegtl/debug/trace_struct.hpp`.

```c++
template< bool HideInternal = true,
          bool UseColor = true,
          bool PrintSourceLine = false,
          std::size_t IndentIncrement = 2,
          std::size_t InitialIndent = 8 >
struct trace_traits;

using standard_trace_traits = trace_traits< true >;
using complete_trace_traits = trace_traits< false >;
```

The first template argument controls whether `normal< R >::enable` is used to hide internal rules.
The second template argument controls whether ANSI color escape sequences are emitted.
The third template argument defines the `print_source_line` member of the traits class.
The last two arguments control the indentation of nested rule invocations.

A custom trace can be run by passing a custom traits type to `generic_trace`.

```c++
using plain_trace_traits = pegtl::trace_traits< true, false >;

pegtl::generic_trace< plain_trace_traits, grammar >( in );
```

### Rule Selection

Two additional traits classes give control over which rules to include in the trace.

```c++
template< typename RuleList,
          bool UseColor = true,
          bool PrintSourceLine = false,
          std::size_t IndentIncrement = 2,
          std::size_t InitialIndent = 8 >
struct include_trace_traits;

template< typename RuleList,
          bool HideInternal = true,
          bool UseColor = true,
          bool PrintSourceLine = false,
          std::size_t IndentIncrement = 2,
          std::size_t InitialIndent = 8 >
struct exclude_trace_traits;
```

Both take a list of rules in the form of a [`tao::pegtl::type_list`](../include/tao/pegtl/type_list.hpp) as first template parameter.
The include traits only print trace information for rules in the provided rule list, the exclude traits for all rules except the listed ones.

Attention, only the exclude traits take the `HideInternal` parameter to choose whether internal rules are excluded, too.

### Trace Action

The header `include/tao/pegtl/debug/trace_action.hpp` contains an action adapter for tracing only a selected part of a grammar during an otherwise normal parsing run.

```c++
template< typename TraceTraits >
struct trace_action;

using trace_standard = trace_action< standard_trace_traits >;
using trace_complete = trace_action< complete_trace_traits >;

template< typename RuleList >
using trace_include = trace_action< include_trace_traits< RuleList > >;
template< typename RuleList >
using trace_exclude = trace_action< exclude_trace_traits< RuleList > >;
```

Use one of these actions as base class for an action specialization for the rule where tracing should start.

```c++
struct inner : seq< one< 'a' >, sor< one< 'b' >, one< 'c' >, inner > > {};
struct outer : seq< one< 'x' >, inner, one< 'y' > > {};

template< typename Rule >
struct action {};

template<>
struct action< inner >
   : trace_standard {};

text_view_input< scan::lf > in( "xaacy" );
parse< outer, action >( in );
```

The full test in `src/test/debug_trace_3.cpp` shows this pattern in context.


## Rule Coverage

The header `include/tao/pegtl/debug/coverage.hpp` contains everything for a parsing run to collect statistics about

* how often each rule was attempted to match,
* what the outcome of each match attempt was,
* which immediate sub-rules were attempted to match how often,
* how often rewind guards were created and needed.

To perform a parsing run that collects statistics the function `coverage()` is used instead of the usual [`parse()`](Inputs-and-Parsing.md#parse-function).

```c++
template< typename Rule,
          template< typename... > class Action = nothing,
          template< typename... > class Control = normal,
          typename ParseInput,
          typename... States >
bool coverage( ParseInput&& in, coverage_result& result, States&&... st );
```

The first function argument to `coverage()` is the input, and the second is a default-constructed object of type `coverage_result` that will receive the statistics.
The function returns whether the parsing run succeeded.

Behind the scenes `coverage()` uses [grammar visit](#grammar-visit) to initialize the `coverage_result` with all rules found in the grammar, and [`rewind_state_control`](Control-Reference.md#rewind_state_control) to call control functions on an internal coverage state.

The companion header `include/tao/pegtl/debug/print_coverage.hpp` contains everything needed to format the coverage result in a human-readable form.
After including it the `coverage_result` can be printed to `std::cout`, or any other `std::ostream`, via the usual `operator<<`.

```c++
#include <tao/pegtl/debug/coverage.hpp>
#include <tao/pegtl/debug/print_coverage.hpp>

coverage_result result;
const bool success = coverage< grammar >( in, result );

std::cout << result;
```

The coverage result can also be inspected directly.

```c++
struct coverage_info
{
   std::size_t start = 0;
   std::size_t success = 0;
   std::size_t failure = 0;
   std::size_t unwind = 0;
   std::size_t raise = 0;
   std::size_t raise_nested = 0;

   [[nodiscard]] explicit operator bool() const noexcept;
};

struct rewind_coverage_info
{
   std::size_t prep_rewind = 0;
   std::size_t will_rewind = 0;
   std::size_t wont_rewind = 0;
};

struct coverage_entry
   : coverage_info
{
   std::map< std::string_view, coverage_info > branches;
};

struct coverage_result
{
   std::map< std::string_view, coverage_entry > coverage;
   std::map< std::string_view, rewind_coverage_info > rewind;
};
```

The `coverage` map contains an entry for every rule found via `subs_t`, including rules that were not reached in this particular parsing run and therefore have zero counters.
The `coverage_info` inherited by `coverage_entry` counts events for the rule itself, while the `branches` map counts the same events for each of its immediate sub-rules.

The `rewind` map counts rewind guard events for rules that created rewind guards.

The source files in `src/example/` that end with `_coverage.cpp` show how to generate and print the coverage statistics.
After compiling the examples their executables can be found in `build/bin/example/` and run to see their respective outputs.


## Grammar Visit

The header `include/tao/pegtl/debug/visit.hpp` uses the [meta data](#meta-data) to find all rules of a grammar and call a visitor function for every one of them.

### Grammar Rules

The type alias `grammar_rules_t< Grammar >` is a `type_list` containing each rule and sub-rule found in `Grammar` exactly once.

```c++
template< typename Grammar >
using grammar_rules_t = /* type_list with all rules found in Grammar */;
```

The variable template `rule_in_grammar_v< Rule, Grammar >` is a shortcut to check whether any given `Rule` occurs anywhere in `Grammar`.

```c++
template< typename Rule, typename Grammar >
inline constexpr bool rule_in_grammar_v = type_list_contains_v< Rule, grammar_rules_t< Grammar > >;
```

Both facilities follow `subs_t` recursively.
This means they see the grammar as represented by the meta data and therefore include the implementation rules that appear in `subs_t`.

### Visit Function

Given a grammar `Grammar` and a class template `Func`, the visit function calls `Func< Rule >::visit( args... )` for every `Rule` in the type list `grammar_rules_t< Grammar >`.

```c++
template< typename Grammar, template< typename... > class Func, typename... Args >
void visit( Args&&... args );
```

All arguments given to `visit()` are passed on to every `Func< Rule >::visit()` call.
The order in which the distinct rules of a grammar are visited is unspecified and should not be relied upon.

```c++
template< typename Rule >
struct visitor
{
   static void visit( std::vector< std::string >& names )
   {
      names.emplace_back( demangle< Rule >() );
   }
};

std::vector< std::string > names;
visit< grammar, visitor >( names );
```

The source file `src/test/debug_visit.cpp` contains a small example.


## Grammar Print

The header `include/tao/pegtl/debug/print.hpp` contains two functions that use the [visit function](#visit-function) to print all rules of a grammar.

```c++
template< typename Grammar >
void print_names( std::ostream& );

template< typename Grammar >
void print_debug( std::ostream& );
```

The function `print_names()` prints only the names of all rules.
The function `print_debug()` also prints every rule's `rule_t` and all individual sub-rules from `subs_t`.

The source files in `src/example/` that end with `_print_debug.cpp` or `_print_names.cpp` show how to use these functions.
After compiling the examples their executables can be found in `build/bin/example/` and run without command line arguments to see their respective outputs.


## Grammar Analysis

The grammar analysis checks a grammar for rules that can enter an infinite loop without making progress, i.e. without consuming input.
Such loops are most often the result of [left recursion](https://en.wikipedia.org/wiki/Left_recursion), or of careless nesting of rules like `star< opt< ... > >`.

Due to the differences in backtracking and deterministic semantics, this kind of infinite loop is a frequent issue when translating a [context-free grammar](https://en.wikipedia.org/wiki/Context-free_grammar) into a PEG.

Analyzing a grammar is usually only done while developing and debugging a grammar, or after changing it.
It is often best practice to create a separate dedicated program that does nothing else but run the grammar analysis, thus keeping this development and debug aid out of the main application.

### Analyze Function

In order to run an analysis on a grammar it is necessary to explicitly include `include/tao/pegtl/debug/analyze.hpp`.
Then call `analyze()` with the top-level grammar rule as template argument.

```c++
template< typename Grammar >
[[nodiscard]] std::size_t analyze( int verbose = 1 );
```

The return value is the number of potential problems found.
A return value of zero means that no problem was found.

The number itself is not very informative because the same problem can be found multiple times, e.g. when a loop without progress consists of multiple rules.

When `verbose` is non-negative the analysis prints warnings to `std::cerr`.
When `verbose` is positive the warnings also include the transformed rules involved in the potential cycle.
Passing `-1` suppresses this output.

```c++
#include <iostream>

#include <tao/pegtl.hpp>
#include <tao/pegtl/debug/analyze.hpp>
#include <tao/pegtl/example/json.hpp>

namespace pegtl = tao::pegtl;

using grammar = pegtl::seq< pegtl::json::text, pegtl::eof >;

int main()
{
   if( pegtl::analyze< grammar >() != 0 ) {
      std::cerr << "cycles without progress detected!" << std::endl;
      return 1;
   }
   return 0;
}
```

The full JSON analysis example can be found in `src/example/json_analyze.cpp`.

Regarding the kinds of issues that are detected, consider the following example rules.

```c++
struct bar;

struct foo
   : sor< digit, bar > {};

struct bar
   : plus< foo > {};
```

When attempting to match `bar` against an input where the next character is not a digit, the parser immediately goes into an infinite loop between `bar` calling `foo`, and `foo` calling `bar`.

As shown by the example program `src/example/analyze.cpp`, the grammar analysis correctly detects a cycle without progress in this grammar.

### Analyze Traits

The `analyze()` function operates on an abstract form of the grammar that is "sufficiently equivalent" to the original grammar regarding the possibility of infinite cycles without progress.

This abstract form is obtained via specialisations of the `analyze_traits<>` class template from `include/tao/pegtl/debug/analyze_traits.hpp`.
Every specialisation needs to have exactly one of the following as public (direct or indirect) base class.

```c++
template< typename... Rules >
struct analyze_any_traits;

template< typename... Rules >
struct analyze_opt_traits;

template< typename... Rules >
struct analyze_seq_traits;

template< typename... Rules >
struct analyze_sor_traits;
```

The base class is chosen as follows.

1. `analyze_any_traits<>` is used for rules that always consume input when they succeed.
2. `analyze_opt_traits<>` is used for rules that can succeed without consuming input.
3. `analyze_seq_traits<>` is used for rules that, regarding their match behavior, are equivalent to `seq<>`.
4. `analyze_sor_traits<>` is used for rules that, regarding their match behavior, are equivalent to `sor<>`.

If a rule calls sub-rules, these sub-rules need to be passed as template parameters to the chosen base class.

Specialisations of `analyze_traits<>` are implemented for all grammar rule classes included with the PEGTL.
This support automatically extends to custom rules built "the usual way" via public inheritance of, or combinations of, rules included with the PEGTL.

For [true custom rules](Rules-and-Grammars.md#implementing-rules), i.e. rules that implement their own `match()` function, the following steps need to be taken for them to work with the grammar analysis.

1. The rule needs a [`rule_t`](#rule-type), usually a type alias for the grammar rule itself.
2. The rule needs a [`subs_t`](#sub-rules), usually `empty_list` for a rule that does not call other rules.
3. There needs to be a specialisation of `analyze_traits<>` for the custom rule's `rule_t`.

Assuming a custom rule like the following

```c++
struct my_rule
{
   using rule_t = my_rule;
   using subs_t = tao::pegtl::empty_list;

   template< typename ParseInput >
   static bool match( ParseInput& in )
   {
      if( !in.empty() ) {
         in.template consume< my_rule >( 1 );
         return true;
      }
      return false;
   }
};
```

the analyze traits need to be set up as follows.

```c++
namespace tao::pegtl
{
   template< typename Name >
   struct analyze_traits< Name, my_rule >
      : analyze_any_traits<>  // In this case "any" traits are the right ones.
   {};
}
```

The first template parameter `Name` is required by the analyze traits of some rules in order to facilitate their transcription in terms of the basic combinators `seq`, `sor` and `opt`.

For example `R = plus< T >` is equivalent, for the analysis, to `seq< T, opt< R > >`, and the corresponding specialisation of the analyze traits is as follows.

```c++
template< typename Name, typename... Rules >
struct analyze_traits< Name, internal::plus< Rules... > >
   : analyze_traits< Name, typename internal::seq< Rules..., internal::opt< Name > >::rule_t >
{};
```

Note how the specialisation is for `internal::plus`, rather than `plus`, since that is used as the `rule_t` type alias.
The indirection of using `Rule::rule_t` rather than `Rule` itself to drive the grammar analysis greatly reduces the number of classes that need to define `rule_t`, and consequently reduces the number of required `analyze_traits<>` specialisations.

The analyze traits have the task of simplifying the grammar in order to keep the core analysis algorithm as simple as possible.
Please consult `include/tao/pegtl/debug/analyze_traits.hpp` as reference for setting up analyze traits for more complex rules, it contains specializations for all rules included with the PEGTL[^1].

### Limitations

The grammar analysis is a conservative static analysis of an abstract form of the grammar.
It does not execute actions, inspect semantic state, or reason about arbitrary code in custom rules.

False positives are possible, and easy to provoke, because the analysis only tracks whether a rule can be shown to consume input when it succeeds, however no false positives have yet been observed on real-world grammars.

In practice the grammar analysis has proven useful in that it catches all cases of left recursion and similar iteration or repetition without progress that are typical for grammars converted from context-free grammars (or other formalisms that can handle left recursion).


---

This page is part of the [PEGTL](https://github.com/taocpp/PEGTL) and its [documentation](README.md).

Copyright (c) 2017-2026 Dr. Colin Hirsch and Daniel Frey<br>
Distributed under the Boost Software License, Version 1.0<br>
See accompanying file [LICENSE_1_0.txt](../LICENSE_1_0.txt) or copy at https://www.boost.org/LICENSE_1_0.txt

[^1:] The [stream parsing](Stream-Parsing.md) rules, as well as the [extras](Extra-Reference.md) and [examples](Example-Reference.md), have their respective traits specializations separately.
