# Debug Facilities

Functions and classes to debug grammars, both via their static properties and their run-time behavior.


## Contents

* [Preamble](#preamble)
* [Meta Data](#meta-data)
* [Parse Trace](#parse-trace)
* [Rule Coverage](#rule-coverage)
* [Grammar Visit](#grammar-visit)
  * [Grammar Rules](#grammar-rules)
  * [Visit Function](#visit-function)
* [Grammar Print](#grammar-print)
* [Grammar Analysis](#grammar-analysis)


## Preamble

The [meta data](#meta-data) provides compile-time accessible information about rule classes, most importantly a list of sub-rules.

The [parse trace](#parse-trace) and [rule coverage](#rule-coverage) perform parsing runs with debug information either in the form of raw events or aggregated, respectively.

The [grammar visit](#grammar-visit) uses the [meta data](#meta-data) to call functions for all rules of a grammar with example functions provided that print them all.

The [grammar analysis](#grammar-analysis) is a function that checks a grammar for occurrences of infinite cycles without progress including left recursion.

#### Namespaces

Everything resides in namespace `tao::pegtl` unless noted otherwise.
This default can be changed via the macro `TAO_PEGTL_NAMESPACE` in `include/tao/pegtl/config.hpp`.
As usual the namespace `tao::pegtl` is generally omitted on this page.


## Meta Data

The meta data provides compile-time accessible information about rule classes, most importantly a list of sub-rules.
This meta data consists of the [type aliases](Rules-and-Grammars.md#type-aliases) `rule_t` and `subs_t` of every rule.

> [!NOTE]
> For some rules `R` the type aliases `R::rule_t` and `R::subs_t` are not what might be naively expected.

For `R = seq< U, V >` the meta data type aliases are both as expected, i.e. `R::rule_t` is `internal::seq< U, V >` and `R::subs_t` is `type_list< U, V >.

For `R = seq<>` the meta data is an unexpected `R::rule_t` of `internal::success` but the expected `R::subs_t` of `empty_list`, i.e. `type_list<>`.

For `R = opt< U, V >` we obtain an `R::rule_t` of `internal::partial< internal::seq< U, V > >` and consequently an `R::subs_t` of `type_list< internal::seq< U, V > >`.

These examples show how, behind the scenes, rules or special cases of rules are implemented in terms of other rules.

The respective "meta data and implementation mapping" sections in the [Rule Reference](Rule-Reference.md) show how `rule_t` and `subs_t` are defined for all rules.


## Parse Trace

The header `include/tao/pegtl/debug/trace.hpp` contains functions to perform a parsing run with trace information printed to `std::cerr`.

```c++
template< typename Rule,
          template< typename... > class Action = nothing,
          template< typename... > class Control = normal,
          typename ParseInput,
          typename... States >
bool standard_trace( ParseInput&& , States&&... );

template< typename Rule,
          template< typename... > class Action = nothing,
          template< typename... > class Control = normal,
          typename ParseInput,
          typename... States >
bool complete_trace( ParseInput&& , States&&... );
```

The difference between the two functions is that the *standard* trace only prints trace information for rules `R` where `normal< R >::enable` is `true`.
This excludes all internal rules that are considered implementation details of documented rules.
The *complete* trace ignores the `enable` flag and includes trace information for all rules.

# TODO -> Integrate below into above

A fundamental tool for grammar development is a *tracer* that prints every step of a parsing run.
It shows exactly which rule was attempted to match where, and what the result was.
The PEGTL provides a tracer that serves both as practical debug tool and as example of the modular nature of the core library that allows for such facilities to be added.

```c++
#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/trace.hpp>

// This example uses the included JSON grammar
#include <tao/pegtl/contrib/json.hpp>

namespace pegtl = tao::pegtl;

using grammar = pegtl::must< pegtl::json::text, pegtl::eof >;

int main( int argc, char** argv )
{
   if( argc != 2 ) return 1;

   pegtl::argv_input in( argv, 1 );
   pegtl::standard_trace< grammar >( in );

   return 0;
}
```

In the example above, each command line parameter is parsed as a JSON string and a trace is printed to `stderr`.

The full example can be found in `src/pegtl/json_trace.cpp`.
For more information see `tao/pegtl/contrib/trace.hpp`.


## Rule Coverage // TODO -- and see below

The header `include/tao/pegtl/debug/coverage.hpp` contains everything for a parsing run to collect statistics about

* how often each rule was attempted to match,
* what the outcome of each match attempt was,
* which sub-rules were attempted to match how often,
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

The first argument to `coverage` needs to be a default-constructed object of type `coverage_result`.
Behind the scenes `coverage()` uses [`rewind_state_control`](Control-Reference.md#rewind_state_control) to call control functions on the `coverage_result`.

The companion header `/include/tao/pegtl/debug/print_coverage.hpp` contains everything needed to format the coverage result in a human-readable form.
After including it the `coverage_result` can be printed to `std::cout` - or any other `std::ostream` - via the usual `operator<<`.

For manual inspection of the coverage result please find the details in the header file `include/tao/pegtl/debug/coverage.hpp` as they should be self explanatory.

The source files in `src/example/` that end with `_coverage.cpp` show how to generate and print the coverage statistics and after compiling everything their executables can be found in `build/bin/example/` and run to see their respective outputs.


## Rule Coverage // TODO -- and see above

The function `coverage()` from `include/tao/pegtl/contrib/coverage.hpp` is very similar to the `parse()` function.
It is called like `parse()`, with the some of the same template parameters and all of the same function arguments, however it returns an object of type `coverage_result` instead of a `bool`.

```c++
template< typename Rule,
          template< typename... > class Action = nothing,
          template< typename... > class Control = normal,
          typename ParseInput,
          typename... States >
coverage_result coverage( ParseInput& in,
                          States&&... st );
```

After a parsing run, the `coverage_result` indicates whether the run was a success or a failure, and contains "rule coverage" and "branch coverage" information.

The "rule coverage" shows how often each rule was attempted to match, and how many of these attempts were a success or a -- local or global -- failure.

The "branch coverage" consists in the matching information also being recorded for each immediate sub-rule of every rule; in the case of an `sor<>` this shows how often each sub-rule was taken, hence the name.

The coverage information in the `coverage_result` can either be inspected and processed or printed manually, or the `ostream` output `operator<<` from `include/tao/pegtl/contrib/print_coverage.hpp` can be used.
The operator formats the output as JSON.

```c++
std::ostream operator<<( std::ostream&, const coverage_result& );
```

The coverage information in the `coverage_result` is defined as follows.
The `coverage_info` is used in two places, as part of the `coverage_entry` for each rule, and as value in the `branches` map for each immediate sub-rule.

```c++
struct coverage_info
{
   std::size_t start = 0;  // How often a rule was attempted to match.
   std::size_t success = 0;  // How many attempts were a success (true).
   std::size_t failure = 0;  // How many attempts were a local failure (false).
   std::size_t unwind = 0;  // How many attempts were aborted due to an exception (thrown here or elsewhere).
   std::size_t raise = 0;  // How many attempts were a global failure (exception thrown at this rule).
};

struct coverage_entry
   : coverage_info  // The coverage_info for each rule.
{
   std::map< std::string_view, coverage_info > branches;  // The coverage_info for each immediate sub-rule.
};

struct coverage_result
{
   std::string_view grammar;  // Name of the top-level grammar rule.
   std::string source;  // From the input.

   std::map< std::string_view, coverage_entry > map;  // The coverage_entry for each rule.
   bool result;  // Whether the parsing run was a success.
};
```

As usual, unless otherwise indicated, all functions and data structure are in the namespace `tao::pegtl`.



## Grammar Visit

The header `include/tao/pegtl/debug/visit.hpp` uses the [meta data](#meta-data) to find and visit all rules of a grammar.

### Grammar Rules

The type alias `grammar_rules_t< Grammar >` is a `type_list` containing each rule and sub-rule found in `Grammar` exactly once.

```c++
template< typename Grammar >
using grammar_rules_t = type_list< all, rules, in, Grammar >;
```

The variable template `rule_in_grammar_v< Rule, Grammar >` is a shortcut to check whether any given `Rule` occurs anywhere in `Grammar`.

```c++
template< typename Rule, typename Grammar >
inline constexpr bool rule_in_grammar_v = type_list_contains_v< Rule, grammar_rules_t< Grammar > >;
```

### Visit Function

Given a grammar `Grammar` and a type `Func` the `visit()` function calls `Func::visit< Rule >( std::forward< Args >( args )... )` for every `Rule` in the type list `grammar_rules_t< Grammar >`.

```c++
template< typename Grammar, template< typename... > class Func, typename... Args >
void visit( Args&&... args );
```

The order in which `Func::visit()` is called for the distinct rules of a grammar is undefined and should not be relied upon.

// TODO: Merge

The `visit()` function uses `subs_t` to recursively find all sub-rules of a grammar and call a function for each of them.

1. The first, explicit, template parameter of `visit()` is the starting rule of the grammar that is to be inspected.

2. The second, explicit, template parameter of `visit()` is a class template `F` where, for every sub-rule `R`, `visit()` will call `F< R >::visit()`.

All arguments given to `visit()` will be passed on to all `F< R >::visit()` calls.

The header `include/tao/pegtl/contrib/visit_rt.hpp` contains a drop-in replacement for `visit()` called `visit_rt()` that uses a run-time data structure, rather than compile-time type lists, to make sure the visitor is called only once for ever
y grammar rule.
This can be a advantageous when working with large grammars since it reduces the template instantiation depth by shifting some of the work from compile time to run time.
Unlike `visit()`, `visit_rt()` returns the number of rules visited.


## Grammar Print

The header `include/tao/peglt/debug/print.hpp` contains two functions that use the [visit function](#visit-function) to print all rules of a grammar.

```c++
template< typename Grammar >
void print_names( std::ostream& );

template< typename Grammar >
void print_debug( std::ostream& );
```

The function `print_names()` prints only the names of all rules, the function `print_debug()` also prints all rules' `rule_t` and all individual sub-rules.

The source files in `src/example/` that end with `_print_debug.cpp` or `print_names.cpp` show how to use these functions and after compiling everything their executables can be found in `build/bin/example/` and run without command line arguments to see their respective outputs.


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
3. `analyze_seq_traits<>` is used for rules that, regarding their match behavior, are equivalent to `seq<>`.
4. `analyze_sor_traits<>` is used for rules that, regarding their match behavior, are equivalent to `sor<>`.

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



# TODO

Each rule has several type aliases that allow for automatic inspection of a grammar and all of its rules for multiple purposes.
Note that true custom rules, i.e. rules that implement custom `match()` functions, do **not** need to define these type aliases for parsing.
They are only required to support functions based on `visit()` and the [grammar analysis](Grammar-Analysis.md).

## Internals

While accessible in the namespace `tao::pegtl`, the [rules and combinators](Rule-Reference.md) included with the PEGTL all have their actual implementation in the sub-namespace `internal`.
For example the header `include/tao/pegtl/rules.hpp` shows how the user-facing rules are nothing more than forwarders to their `internal` implementation.

The original motivation for this additional level of indirection was to prevent uninteded invocation of user-defined actions due to some PEGTL rules being built from exisiting rules instead of having a dedicated implementation.
For example consider `rep_min` from `include/tao/pegtl/internal/rep_min.hpp`.

```c++
template< unsigned Min, typename Rule, typename... Rules >
using rep_min = seq< rep< Min, Rule, Rules... >, star< Rule, Rules... > >;
```

The expansion of `rep_min` uses the sub-rule `star< Rule, Rules... >`.
If a grammar were to contain both `rep_min` and `star` with the same sub-rules, and an action were provided for `star` with these exact sub-rules, then the action would not only be called for the explicit occurrences of `star` in the grammar bu
t *also* for the corresponding sub-rule of `rep_min`.

The action invocation for the sub-rule of `rep_min` is considered surprising and undesirable because it exposes implementation details to the user, forces her to deal with them, and breaks her grammar if the implementation were to change.

Therefore it is possible to selectively disable most of the [control](Control-and-Normal.md) functions, including the `apply`-functions that perform action invocation, on a rule-by-rule basis.
More precisely, the action and debug control functions are only invoked for a rule `R` when `control< R >::enable` is `true`.

```c++
template< typename Rule >
struct normal
{
   static constexpr bool enable = internal::enable_control< Rule >;

// ...
};
```

The default control class template `normal` defines `normal< R >::enable` in terms of `internal::enable_control< R >` which is `true` by default, but `false` for all rules in sub-namespace `internal`, thereby hiding all invocations of `internal
` rules from all actions and most of the control class functions.

The facilities documented on this page however do **not** hide the implementation details since, while debugging a grammar or a parsing run, it is essential to have a complete picture.

## Rule Type

For each rule `R`, the type alias `R::rule_t` is defined as the type of the class that actually implements the `match()` function.
This is usually the root of the inheritance hierarchy.

Note that `R::rule_t` can be completely different from `R`.
For example while `seq< R >::rule_t` is `internal::seq< R >`, due to `seq<>` being not only equivalent to `success`, but also implemented in terms of it, `seq<>::rule_t` is actually `internal::success`.

In each rule's implementation mapping section the [rule reference](Rule-Reference.md) shows how `rule_t` is defined depending on the template parameters.

## Sub Rules

For each rule `R`, the type alias `R::subs_t` is an instantiation of `type_list` with all the direct sub-rules of `R` as template parameters.

For example `seq< R, S >::subs_t` is `type_list< R, S >` and `alpha::subs_t` is `type_list<>`.

Note that for many rules with sub-rules the corresponding `subs_t` is not as might be expected.
For example `enable< R, S >::subs_t` is `type_list< internal::seq< R, S > >` instead of the probably expected `type_list< R, S >`.

Please again consult the [Rule Reference](Rule-Reference.md) (or the source) for how `subs_t` is defined for all included rules.



---

This page is part of the [PEGTL](https://github.com/taocpp/PEGTL) and its [documentation](README.md).

Copyright (c) 2017-2026 Dr. Colin Hirsch and Daniel Frey<br>
Distributed under the Boost Software License, Version 1.0<br>
See accompanying file [LICENSE_1_0.txt](../LICENSE_1_0.txt) or copy at https://www.boost.org/LICENSE_1_0.txt
