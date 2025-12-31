# Contrib and Examples

The PEGTL includes parts that go beyond the core library functionality.
They are included for convenience and/or as examples for how to do things.

Feedback is highly welcome.
The same is true for contributions like grammars that might be generally useful.

For all questions and remarks contact us at **taocpp(at)icemx.net**.

## Contents

* [Contrib](#contrib)
  * [`abnf.hpp`](#abnfhpp)
  * [`abnf2.hpp`](#abnf2hpp)
  * [`alphabet_constants.hpp`](#alphabet_constantshpp)
  * [`alphabet_rules.hpp`](#alphabet_ruleshpp)
  * [`check_count.hpp`](#check_counthpp)
  * [`check_depth.hpp`](#check_depthhpp)
  * [`dispatch.hpp`](#dispatchhpp)
  * [`http.hpp`](#httphpp)
  * [`input_with_depth.hpp`](#input_with_depthhpp)
  * [`input_with_offset.hpp`](#input_with_offsethpp)
  * [`integer.hpp`](#integerhpp)
* [Examples](#examples)

## Contrib

The contrib headers `foo.hpp` reside in `include/tao/pegtl/contrib` and can be included via `<tao/pegtl/contrib/foo.hpp>`.

###### `abnf.hpp`

Core ABNF rules according to [RFC 5234, Appendix B](https://tools.ietf.org/html/rfc5234).

Ready for production use.

###### `abnf2.hpp`

Actual ABNF grammar according to [RFC 5234](https://tools.ietf.org/html/rfc5234), updated by [RFC 7405](https://tools.ietf.org/html/rfc7405).

* Extended with PEG 'and' and 'not' predicates.
* Modified to not allow C++ keywords as rule names.

###### `alphabet_constants.hpp`

Character constants for ASCII letters.

* Total of 52 ASCII character constants.
* In sub-namespace `alphabet`.
* Probably better to use `TAO_PEGTL_STRING()`.

```c++
inline constexpr char a = 'a';
...
```

Trivially ready for production use.

###### `alphabet_rules.hpp`

Rules for single ASCII characters.

* Total of 52 ASCII character rules.
* In sub-namespace `alphabet::rules`.

```c++
struct a : ascii::one< 'a' > {};
...
```

Trivially ready for production use.

###### `check_count.hpp`

An action class with `match()` function that throws an exception when the rule it is attached to consumes more than the allowed number of input objects.

The difference between `check_count` and `limit_count` is that `check_count` checks how much was consumed after the fact, whereas `limit_count` temporarily modifies the input.

```c++
 template< std::size_t Maximum >
 struct check_count
    : maybe_nothing
 { ... };
```

###### `check_depth.hpp`
###### `input_with_depth.hpp`

An action class that throws an exception when the rule(s) it is attached to reach a higher than allowed nesting depth, and:

An input adapter class that adds the required depth counter to the input (thereby not requiring an additional state object).

```c++
 template< std::size_t Maximum >
 struct check_depth
    : maybe_nothing
 { ... };

 template< typename Input >
 class input_with_depth
    : public Input
 {
 public:
    using Input::Input;
    ...
 };
```

For a parsing run with nesting depth check the input has to be changed from whatever `input_class` is otherwise used to `input_with_depth< input_class >`.
Then the `check_depth` action can be strategically attached to an appropriate rule to limit the nesting depth during parsing.

For example in the [JSON](#jsonhpp) grammar the `value` rule should be used to limit the nesting depth as it is the single rule through which the structural recursion is done.

```c++
 using namespace tao::pegtl;

 template< typename Rule >
 struct my_action
    : nothing< Rule >
 {}

 template<>
 struct my_action< json::value >
    : check_depth< 42 >
 {}
```

###### `dispatch.hpp`

An implementation detail of [`record.hpp`](#recordhpp).

A mechanism for attaching actions to multiple rules at once.

###### `http.hpp`

HTTP 1.1 grammar according to [RFC 7230](https://tools.ietf.org/html/rfc7230).

###### `if_then.hpp`

An experiment for something or other...

###### `input_with_depth.hpp`

Used together with [`check_depth.hpp`](#check_depthhpp), see there.

###### `input_with_offset.hpp`

An input adapter class that allows for the reported position to be offset by a fixed value.

###### `integer.hpp`

Rules and and actions related to integer parsing and converting.

Simple rules that are combinations of PEGTL standard rules.

```c++
 struct unsigned_rule_old : plus< digit > {};
 struct unsigned_rule_new : if_then_else< one< '0' >, not_at< digit >, plus< digit > > {};
 struct signed_rule_old : seq< opt< one< '-', '+' > >, plus< digit > > {};
 struct signed_rule_new : seq< opt< one< '-', '+' > >, if_then_else< one< '0' >, not_at< digit >, plus< digit > > > {};
 struct signed_rule_bis : seq< opt< one< '-' > >, if_then_else< one< '0' >, not_at< digit >, plus< digit > > > {};
 struct signed_rule_ter : seq< one< '-', '+' >, if_then_else< one< '0' >, not_at< digit >, plus< digit > > > {};
```

The 'unsigned action' converts the matched digits from `in` into an integer and stores it in `st`.

An exception is thrown on overflow.
Behaviour is undefined if `in` is anything other than a non-empty sequence of ASCII digits.

```c++
 struct unsigned_action
 {
  template< typename ActionInput, typename Unsigned >
  static void apply( const ActionInput& in, Unsigned& st );
 };
```

The 'unsigned rule' is a monolithic version of `unsigned_rule_new`.
It also does not check for overflow, mostly because it doesn't have or know the target data type.

```c++
 struct unsigned_rule;
```

The 'unsigned rule with action' is a combination of `unsigned_rule` and `unsigned_action`.

It is equivalent to, but more efficient than, using `unsigned_rule` with an attached `unsigned_action`;
it is a parsing rule that matches an unsigned integer and also stores the converted integer in the State.

When the States are not just a single mutable reference to an unsigned integer it behaves like `unsigned_rule`.

```c++
 struct unsigned_rule_with_action;
```

The 'maximum action' is similar to 'unsigned_action` but it uses `Maximum`, rather than the maximum value representable by `Unsigned2`, as limit above which to throw an exception for overflow.

```c++
 template< typename Unsigned, Unsigned Maximum >
 struct maximum_action
 {
  template< typename ActionInput, typename Unsigned2 >
  static void apply( const ActionInput& in, Unsigned2& st );
 };
```

The 'maximum rule' behaves like `unsigned_rule` but with a user-supplied maximum.

```c++
 template< typename Unsigned, Unsigned Maximum >
 struct maximum_rule;
```

The 'maximum rule with action` behaves like `unsigned_rule_with_action` with a user-supplied maximum.

```c++
 template< typename Unsigned, Unsigned Maximum >
 struct maximum_rule_with_action;

```

The 'signed action' is similar to `unsigned_action` buf for signed integers.

An exception is thrown on overflow.
Behaviour is undefined if `in` is anything other than a non-empty sequence of ASCII digits with an optional leading sign.

```c++
 struct signed_action
 {
  template< typename ActionInput, typename Signed >
  static void apply( const ActionInput& in, Signed& st );
 };
```

The 'signed rule' behaves like `unsigned_rule` but for signed integers, i.e. ASCII digits with an optional leading sign.

```c++
 struct signed_rule;
```

The 'signed rule with action' behaves like `unsigned_rule_with_action` albeit for signed integers.

```c++
 struct signed_rule_with_action;
```

###### `iri.hpp`

Experimental IRI grammar according to [RFC 3987](https://tools.ietf.org/html/rfc3987).

###### `json.hpp`

JSON grammar according to [RFC 7159](https://tools.ietf.org/html/rfc7159) (UTF-8 only).

###### `json_pointer.hpp`

JSON pointer grammar according to [RFC 6901](https://tools.ietf.org/html/rfc6901) (UTF-8 only).

###### `limit_count.hpp`

An action class with `match()` function that throws an exception when the rule it is attached to consumes more than the allowed number of input objects.

The difference between `check_count` and `limit_count` is that `check_count` checks how much was consumed after the fact, whereas `limit_count` temporarily modifies the input.

```c++
 template< std::size_t Maximum >
 struct limit_count
    : maybe_nothing
 { ... };
```

###### `nested_exceptions.hpp`

Utility functions to work with nested exceptions.
Please consult the header file itself for instructions on how to use.

###### `parse_tree.hpp`
###### `parse_tree_to_dot.hpp`

Documented on the dedicated [Parse Tree](Parse-Tree.md) page.

###### `raw_string.hpp`

Rules for matching Lua-style long or raw string literals.
Please consult the header file itself for instructions on how to use.

###### `record.hpp`

###### `rep_one_min_max.hpp`

An optimized version of `rep_min_max< Min, Max, (ascii::)one< C > >` that is written `(ascii::)rep_one_min_max< Min, Max, C >`.

###### `rep_string.hpp`

An version of `rep< N, (ascii::)string< Cs... > >` that is written `(ascii::)rep_string< N, Cs... >`.

Equivalent to `(ascii::)string< Cs..., Cs..., ... >` with `N` repetitions of `Cs...`.

###### `separated_seq.hpp`

Convenience rule to parse a sequence of rules with a separator.

For example `separated_seq< S, A, B, C, D >` is equivalent to `seq< A, S, B, S, C, S, D >`.

###### `type_to_string.hpp`

Template functions that convert from compile-time strings that are encoded as non-type template parameters to `std::string` and `std::string_view`.

These functions assume that `T` is the instantiation of a class template `foo` declared as follows.

```c++
 template< char... >
 struct foo;
```

The following functions return a string (view) containing the template parameters of `T` as characters.

```c++
 template< typename T >
 [[nodiscard]] std::string type_to_string();

 template< typename T >
 [[nodiscard]] std::string_view type_to_string_view() noexcept;
```

There are also overloads that can be directly called with sequence of characters as template parameters.

```c++
 template< char... Cs >
 [[nodiscard]] std::string type_to_string();

 template< char... Cs >
 [[nodiscard]] std::string_view type_to_string_view() noexcept;
```




###### `<tao/pegtl/contrib/input_with_depth.hpp>`
###### `<tao/pegtl/contrib/limit_depth.hpp>`

* Limit the nesting depth of rules when parsing a grammar to prevent stack overflows.
* Can be applied selectively at specific rules to reduce overhead.
* See `src/test/contrib_limit_depth.cpp`.


###### `unescape.hpp`

Actions for unescaping common kinds of escape sequences.
All actions append the unescaped result to their first and only State argument which must be of type `std::string`.

The `append_all` action does not perform any unescaping.

```c++
 struct append_all
 {
  template< typename ActionInput >
  static void apply( const ActionInput& in, std::string& s )
  {
   s.append( in.string_view() );
  }
 };
```

The `unescape_c` action is for C-style escape sequences like `\n`.

```c++
 template< typename One, char... Rs >
 struct unescape_c
 {
  template< typename ActionInput >
  static void apply( const ActionInput& in, std::string& s );
 };
```

The `unescape_u` action is for Unicode escape sequences like '\u2e78a`.

```c++
 struct unescape_u
 {
  template< typename ActionInput >
  static void apply( const ActionInput& in, std::string& s );
 };
```

The `unescape_j` action is for Unicode escape sequence like in JSON.
The differences to `unescape_u` are that `unescape_j`
* requires exactly four hex digits after the `u` or `U`,
* handles multiple consecutive escape sequences in one go,
* merges consecutive escapes that happen to form a surrogate pair.

```c++
 struct unescape_j
 {
  template< typename ActionInput >
  static void apply( const ActionInput& in, std::string& s );
 };
```



###### `uri.hpp`

Experimental URI grammar according to [RFC 3986](https://tools.ietf.org/html/rfc3986).

## Examples

###### `src/example/abnf2pegtl.cpp`

Reads a file with an [ABNF (RFC 5234)](https://tools.ietf.org/html/rfc5234)-style grammar and converts it into corresponding PEGTL rules in C++. Some extensions and restrictions compared to RFC 5234:

* As we are defining PEGs, the alternations are now ordered (`sor<>`).
* The *and*- and *not*-predicates from PEGs have been added as `&` and `!`, respectively.
* A single LF is also accepted as line ending.
* C++ identifiers are formed by replacing the dashes in rulenames with underscores.
* Reserved identifiers (keywords, ...) are rejected.
* Numerical values must fit into the corresponding C++ data type.

###### `src/example/analyze.cpp`

A small example that provokes the [grammar analysis](Grammar-Analysis.md) to find problems.

###### `src/example/calculator.cpp`

A calculator with all binary operators from the C language that shows

* how to use stack-based actions to perform a calculation on-the-fly during the parsing run, and
* how to build a grammar with a run-time data structure for arbitrary binary operators with arbitrary precedence and associativity.

In addition to the binary operators, round brackets can be used to change the evaluation order. The implementation uses `long` integers as data type for all calculations.

```sh
$ build/src/example/calculator "2 + 3 * -7"  "(2 + 3) * 7"
-19
35
```

In this example the grammar takes a bit of a second place behind the infrastructure for the actions required to actually evaluate the arithmetic expressions.
The basic approach is "shift-reduce", which is very close to a stack machine, which is a model often well suited to PEGTL grammar actions:
Some actions merely push something onto a stack, while other actions apply some functions to the objects on the stack, usually reducing its size.

###### `src/example/chomsky_hierarchy.cpp`

Examples of grammars for regular, context-free, and context-sensitive languages.

###### `src/example/csv1.cpp` and `src/example/csv2.cpp`

Two simple examples for grammars that parse different kinds of CSV-style file formats.

###### `src/example/hello_world.cpp`

Minimal parser-style "hello world" example from the [Getting Started](Getting-Started.md) page.

###### `src/example/indent_aware.cpp`

Shows one approach to implementing an indentation-aware language with a very very small subset of Python.

###### `src/example/json_parse.cpp`

Shows how to use the custom error messages defined in `json_errors.hpp` with the `<tao/pegtl/contrib/json.hpp>` grammar to parse command line arguments as JSON data.

###### `src/example/json_build.cpp`

Extends on `json_parse.cpp` by parsing JSON files into generic JSON data structure.

###### `src/example/json_count.cpp`

Shows how to use a simple custom control to create some parsing statistics while parsing JSON files.

###### `src/example/lua53_parse.cpp`

Parses all files passed on the command line with a slightly experimental grammar that should correspond to the [Lua](http://www.lua.org/) 5.3 lexer and parser.

###### `src/example/modulus_match.cpp`

Shows how to implement a custom parsing rule with the simplified calling convention.

###### `src/example/parse_tree.cpp`

A small example which shows how to create a parse tree for a given grammar using [`<tao/pegtl/contrib/parse_tree.hpp>`](Parse-Tree.md).

The example shows how to choose which rules will produce a parse tree node, which rules will store the content, and how to add additional transformations to the parse tree to transform it into an AST-like structure or to simplify it.

The output is in [DOT](https://en.wikipedia.org/wiki/DOT_(graph_description_language)) format and can be converted into a graph.

```sh
$ build/src/example/parse_tree "(2*a + 3*b) / (4*n)" | dot -Tsvg -o parse_tree.svg
```

The above will generate an SVG file with a graphical representation of the parse tree.

![Parse Tree](Parse-Tree.svg)

###### `src/example/proto3.cpp`

Experimental grammar that parses Protocol Buffers (`.proto3`) files.

###### `src/example/recover.cpp`

See [PEGTL issue 55](https://github.com/taocpp/PEGTL/issues/55) and the source code for a description.

###### `src/example/s_expression.cpp`

Grammar for a toy-version of S-expressions that shows how to include other files during a parsing run.

###### `src/example/sum.cpp`

Simple example that adds a list of comma-separated `double`s read from `std::cin`.

###### `src/example/symbol_table.cpp`

Simple example that shows how to parse with a symbol table.

###### `src/example/unescape.cpp`

Uses the building blocks from `<tao/pegtl/contrib/unescape.hpp>` to show how to actually unescape a string literal with various typical escape sequences.

###### `src/example/uri_trace.cpp`

Shows how to use the included [tracer control](#taopegtlcontribtracerhpp), here together with the URI grammar from `<tao/pegtl/contrib/uri.hpp>`.
Invoked with one or more URIs as command line arguments will attempt to parse the URIs while printing trace information to `std::cerr`.

---

This page is part of the [PEGTL](https://github.com/taocpp/PEGTL) and its [documentation](README.md).

Copyright (c) 2014-2026 Dr. Colin Hirsch and Daniel Frey<br>
Distributed under the Boost Software License, Version 1.0<br>
See accompanying file [LICENSE_1_0.txt](../LICENSE_1_0.txt) or copy at https://www.boost.org/LICENSE_1_0.txt
