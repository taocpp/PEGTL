# Extras and Examples

The reference documentation for the included example grammars and programs.
Feedback and contributions welcome.
For all questions and remarks contact us at **taocpp(at)icemx.net**.


## Contents

* [Extra Headers](#extra-headers)
  * [`charconv.hpp`](#charconvhpp)
  * [`parse_tree.hpp`](#parse_treehpp)
  * [`unescape.hpp`](#unscapehpp)
* [Example Grammars](#example-grammars)
  * [`abnf_abnf.hpp`](#abnf_abnfhpp)
  * [`abnf_core.hpp`](#anbf_corehpp)
  * [`escaped.hpp`](#escapedhpp)
  * [`fp.hpp`](#fphpp)
  * [`http.hpp`](#httphpp)
  * [`integer.hpp`](#integerhpp)
  * [`iri.hpp`](#irihpp)
  * [`json.hpp`](#jsonhpp)
  * [`json_points.hpp`](#json_pointer.hpp)
  * [`lua53.hpp`](#lua53hpp)
  * [`proto3.hpp`](#proto3hpp)
  * [`semver2.hpp`](#semver2hpp)
  * [`uri.hpp`](#urihpp)
* [Example Programs](#example-programs)
  * [`abnf2pegtl.cpp`](#abnf2pegtlcpp)
  * [`abnf_record.cpp`](#abnf_recordcpp)
  * [`analyze.cpp`](#analyzecpp)
  * [`behaviour.cpp`](#behaviourcpp)
  * [`calculator.cpp`](#calculatorcpp)
  * [`chomsky_hierarchy.cpp`](#chomsky_hierarchycpp)
  * [`csv_1.cpp`](#csv_1cpp)
  * [`csv_2.cpp`](#csv_2cpp)
  * [`dispatch.cpp`](#dispatchcpp)
  * [`dynamic_match.cpp`](#dynamic_matchcpp)
  * [`expression.cpp`](#expressioncpp)
  * [`hello_world.cpp`](#hello_worldcpp)
  * [`indent_aware.cpp`](#indent_awarecpp)
  * [`iri_struct.cpp`](#iri_structcpp)
  * [`json_analyze.cpp`](#json_analyzecpp)
  * [`json_ast.cpp`](#json_astcpp)
  * [`json_build.cpp`](#json_buildcpp)
  * [`json_count.cpp`](#json_countcpp)
  * [`json_coverage.cpp`](#json_coveragecpp)
  * [`json_parse.cpp`](#json_parsecpp)
  * [`json_print_debug.cpp`](#json_print_debugcpp)
  * [`json_print_names.cpp`](#json_print_namescpp)
  * [`json_record.cpp`](#json_recordcpp)
  * [`json_stream.cpp`](#json_streamcpp)
  * [`json_tokenize.cpp`](#json_tokenizecpp)
  * [`json_trace.cpp`](#json_tracecpp)
  * [`lua53_analyze.cpp`](#lua53_analyzecpp)
  * [`lua53_parse.cpp`](#lua53_parsecpp)
  * [`modulus_match.cpp`](#modulus_matchcpp)
  * [`parse_tree.cpp`](#parse_treecpp)
  * [`parse_tree_user_state.cpp`](#parse_tree_user_statecpp)
  * [`proto3_analyze.cpp`](#proto3_analyzecpp)
  * [`proto3_parse.cpp`](#proto3_parsecpp)
  * [`recover.cpp`](#recovercpp)
  * [`s_expression.cpp`](#s_expressioncpp)
  * [`semver2_parse.cpp`](#semver2_parsecpp)
  * [`sum.cpp`](#sumcpp)
  * [`symbol_table.cpp`](#symbol_tablecpp)
  * [`token_input_1.cpp`](#token_input_1cpp)
  * [`token_input_2.cpp`](#token_input_2cpp)
  * [`unescape.cpp`](#unescapecpp)
  * [`uri_print_debug.cpp`](#uri_print_debugcpp)
  * [`uri_print_names.cpp`](#uri_print_namescpp)
  * [`uri_struct.cpp`](#uri_structcpp)
  * [`uri_trace.cpp`](#uri_tracecpp)


## Extra Headers

The extra headers can be found in `include/tao/pegtl/extra/`.

###### `charconv.hpp`

Provides string-to-integer conversion based on [`std::from_chars()`]](https://en.cppreference.com/w/cpp/utility/from_chars.html) from the standard [`<charconv>`](https://en.cppreference.com/w/cpp/header/charconv.html) header.
The classes defined in this header implement both the `match()` function to serve as rule **and** the `apply()` function to serve as action.

Conversion classes for decimal and hexadecimal are provided, other bases can be selected via the appropriate template parameter.

```c++
template< typename Integral, std::uint8_t Base = 10 > struct from_chars_throws;
template< typename Integral, std::uint8_t Base = 10 > struct from_chars_nothrow;

template< typename Integral > struct from_xchars_throws : from_chars_throws< Integral, 16 > {};
template< typename Integral > struct from_xchars_nothrow : from_chars_nothrow< Integral, 16 > {};
```

The `Base` parameter is passed to `std::from_chars()` as fourth function argument unchanged.
It can be anything between 2 and 36, inclusive.

Remember that `std::from_chars()` does **not**  allow a leading `+` and does **not** recognize the `0x` prefix for hexadecimal numbers.

In both roles any additional state arguments are ignored, and `bool` is returned (as is mandatory for `match()` but optional for `apply()`).

The `_throws` vs. `_nothrow_` suffix chooses between failing locally, i.e. returning `false`, when `std::from_chars()` returns `std::errc::result_out_of_range`, or failing globally, i.e. throwing an exception derived from `tao::pegtl::parse_error`.



 1. Success is when a portion of the parse input, or, as action, the whole action input can be parsed to an integer by `std::from_chars()`. The functions return `true`.
 2. Failure to match is when the parse input does not start with an integer, or the action input is not wholly an integer, respectively. The functions return `false`.
 3. Overflow is when the integer being parsed does not fit into the target type. In this case the `_throws` or `nothrow` suffix comes into play and chooses whether to return `false` or throw a `parse_error`.

When `Integral` is `void` both `apply()` and `match()` use the type of their first state argument 
Further, `match()` ignores the `apply_mode` and the first state argument must always be of mutable integral type.


###### `parse_tree.hpp`

Given its scope and complexity [the parse tree has a dedicated page](Parse-Tree.md).

###### `unescape.hpp`

A mostly just repackaged iteration of the previous unescape header which can now be found in `deprecated/unescape.hpp`.

It defines individual action classes for common unescape operations as well as an action class template that attaches these actions to corresponding grammar rules.

All actions output to their first state (second argument) which must be a mutable `std::string`.

```c++
struct all_of_these_actions
{
   template< typename ActionInput >
   static void apply( const ActionInput& in, std::string& out );
};
```

`unescape_append` is for non-escaped characters that need to just be copied from the input unchanged by calling `out.append( in.string_view() );`

`unescape_char<>` is for single character escape sequences like `\n`.
It takes any number of template parameters of type `char` and has to be attached to a rule of type `(ascii::)one<>` with the same number of template parameters.

During unescape the n-th template parameter of the `one` is mapped to the n-th template parameter of `unescape_char`.

`unescape_byte` is for bytes represented by two hex digits and must be attached to a rule that matches exactly that.

`unescape_unicode<>` is for hexadecimal Unicode code points, excluding surrogates.
The template parameter of type `std::size_t` must match the fixed (!) number of hex digits the rule it is attached to matches, usually 4 or 8.

`json_unescape_unicode` is for sequences of hexadecimal UTF-16 code points with 4 hex digits each.
It unescapes each individual code point while checking for and merging consecutive surrogate pairs as required by RFC 8259.

> [!WARNING]
> The actions do **not** double-check that the matched portion of the input they are invoked with has the expected size and data!

The action template `unescape<>` is set up with the usual default

```c++
template< typename Rule > unescape< Rule > : nothing< Rule > {};
```

It has specializations for some general rules that it might be used with and for some more specific rules taken from `include/tao/pegtl/example/escaped.hpp`.
Please check out `include/tao/pegtl/extra/unescape.hpp` to see which individual unescape action is attached to which rule by `unescape<>`.


## Example Grammars

The example grammars reside in `include/tao/pegtl/example/`.

###### `abnf_abnf.hpp`

Grammar for ABNF rules according to Section 4 of [RFC 5234](https://tools.ietf.org/html/rfc5234) as updated by [RFC 7405](https://tools.ietf.org/html/rfc7405).

 * Extended with PEG 'and' and 'not' predicates.
 * Modified to not allow C++ keywords as rule names.

###### `abnf_core.hpp`

Rules for the ABNF core rules according to Appendix B.1 of [RFC 5234, Appendix B](https://tools.ietf.org/html/rfc5234).

###### `escaped.hpp`

Rules for escape sequences in C and JSON strings ready for [the unescape actions](#unescape).

###### `fp.hpp`

A grammar for the textual representation of floating point numbers, suitable for `std::stod()` (without locale support).

###### `http.hpp`

HTTP 1.1 grammar according to [RFC 7230](https://tools.ietf.org/html/rfc7230).

###### `integer.hpp`

Various rules for the textual representation of integer values; the old `include/tao/pegtl/contrib/integer.hpp` is now `include/tao/pegtl/deprecated/integer.hpp`.

###### `iri.hpp`

IRI grammar according to [RFC 3987](https://tools.ietf.org/html/rfc3987).

###### `json.hpp`

JSON grammar according to [RFC 7159](https://tools.ietf.org/html/rfc7159) (UTF-8 only).

###### `json_pointer.hpp`

JSON pointer grammar according to [RFC 6901](https://tools.ietf.org/html/rfc6901) (UTF-8 only).

###### `lua53.hpp`

Grammar for the [Lua](https://lua.org) [5.3](https://lua.org/manual/5.3/) scripting language that combines lexer and parser.

###### `proto3.hpp`

Grammar for [Protocol Buffers (Proto3)](https://developers.google.com/protocol-buffers/docs/reference/proto3-spec).

###### `semver2.hpp`

Grammar for [SemVer Versions 2.0.0](https://semver.org/#backusnaur-form-grammar-for-valid-semver-versions).

###### `uri.hpp`

URI grammar according to [RFC 3986](https://tools.ietf.org/html/rfc3986).


## Example Programs

The example programs can be found in `src/example/`.

TODO: Complete list:

###### `abnf2pegtl.cpp`

Reads a file with an [ABNF (RFC 5234)](https://tools.ietf.org/html/rfc5234)-style grammar and converts it into corresponding C++ PEGTL rules.
Some extensions and restrictions compared to RFC 5234:

 * As we are defining PEGs, the alternations are now ordered (`sor<>`).
 * The *and*- and *not*-predicates from PEGs have been added as `&` and `!`, respectively.
 * A single LF is also accepted as line ending.
 * C++ identifiers are formed by replacing the dashes in rulenames with underscores.
 * Reserved identifiers (keywords, ...) are rejected.
 * Numerical values must fit into the corresponding C++ data type.

###### `abnf_record.cpp`

###### `analyze.cpp`

A small example that provokes the [grammar analysis](Grammar-Analysis.md) to find problems.

###### `calculator.cpp`

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

###### `chomsky_hierarchy.cpp`

Examples of grammars for regular, context-free, and context-sensitive languages.

###### `csv1.cpp` and `src/example/csv2.cpp`

Two simple examples for grammars that parse different kinds of CSV-style file formats.

###### `hello_world.cpp`

Minimal parser-style "hello world" example from the [Introduction](Introduction.md) page.

###### `indent_aware.cpp`

Shows one approach to implementing an indentation-aware language with a very very small subset of Python.

###### `json_parse.cpp`

Shows how to use the custom error messages defined in `json_errors.hpp` with the `<tao/pegtl/contrib/json.hpp>` grammar to parse command line arguments as JSON data.

###### `json_build.cpp`

Extends on `json_parse.cpp` by parsing JSON files into generic JSON data structure.

###### `json_count.cpp`

Shows how to use a simple custom control to create some parsing statistics while parsing JSON files.

###### `lua53_parse.cpp`

Parses all files passed on the command line with a slightly experimental grammar that should correspond to the [Lua](http://www.lua.org/) 5.3 lexer and parser.

###### `modulus_match.cpp`

Shows how to [implement a parsing rule from scratch](Rules-and-Grammars.md#implementing-rules), in this case using the [simplified calling convention](Rules-and-Grammars.md#simple-match).

###### `parse_tree.cpp`

A small example which shows how to create a parse tree for a given grammar using [`<tao/pegtl/contrib/parse_tree.hpp>`](Parse-Tree.md).

The example shows how to choose which rules will produce a parse tree node, which rules will store the content, and how to add additional transformations to the parse tree to transform it into an AST-like structure or to simplify it.

The output is in [DOT](https://en.wikipedia.org/wiki/DOT_(graph_description_language)) format and can be converted into a graph.

```sh
$ build/src/example/parse_tree "(2*a + 3*b) / (4*n)" | dot -Tsvg -o parse_tree.svg
```

The above will generate an SVG file with a graphical representation of the parse tree.

![Parse Tree](Parse-Tree.svg)

###### `proto3.cpp`

Experimental grammar that parses Protocol Buffers (`.proto3`) files.

###### `recover.cpp`

An experiment in recovering from parse failures, see [PEGTL issue 55](https://github.com/taocpp/PEGTL/issues/55) and the source code for a description.

###### `s_expression.cpp`

Defines a simplified S-expression grammar TODO
Parses its command line arguments.

###### `semver2_parse.cpp`

Shows the [SemVer Version grammar](#semver2hpp) in action.
Parses its command line arguments.

###### `sum.cpp`

Shows how to add comma-separated lists of floating-point numbers taken from `std::cin`.

```
$ echo "1, 2, 3.14159, 42e3" | build/bin/example/sum
Give me a comma separated list of numbers.
The numbers are added using the PEGTL.
Type [q or Q] to quit

parsing OK; sum = 42006.14159
```

###### `symbol_table.cpp`

Shows how to parse and store integers in a simple symbol table.
Each symbol needs to be defined before it is assigned to.
Uses the command line arguments as files to parse.

```
$ cat /tmp/ramdisk/symbol_table.txt
def foo;
def bar;
foo = 42;
bar = 23;
$ build/bin/example/symbol_table /tmp/ramdisk/symbol_table.txt
bar = 23
foo = 42
```

###### `token_input_1.cpp`

Shows how to parse a sequence of tokens, rather than the usual sequence of `char`, where each token consist of an enum and a string.

###### `token_input_2.cpp`

Shows how to parse a sequence of tokens, rather than the usual sequence of `char`, where each token consist of an enum and a string.

###### `unescape.cpp`

Uses the building blocks from `<tao/pegtl/contrib/unescape.hpp>` to show how to actually unescape a string literal with various typical escape sequences.
Parses its command line arguments.

```
$ build/bin/example/unescape 'X\x22Y' 'X\u0022Y' 'X\"Y'
argv[ 1 ] = X"Y
argv[ 2 ] = X"Y
argv[ 3 ] = X"Y
```

###### `uri_print_debug.cpp`

Shows how to use `print_debug()` from `include/tao/pegtl/debug/print.hpp` to print all rules of the [URI grammar](#urihpp).

###### `uri_print_names.cpp`

Shows how to use `print_names()` from `include/tao/pegtl/debug/print.hpp` to print all rules of the [URI grammar](#urihpp).

###### `uri_struct.cpp`

Shows how to use the [URI grammar](#urihpp) to parse a [URI](https://en.wikipedia.org/wiki/Uniform_Resource_Identifier) into a data structure.
Parses its command line arguments.

###### `uri_trace.cpp`

Shows how to use `complete_trace` from `include/tao/pegtl/debug.trace.hpp` to parse a [URI](https://en.wikipedia.org/wiki/Uniform_Resource_Identifier) with a complete trace.
Parses its command line arguments.


---

This page is part of the [PEGTL](https://github.com/taocpp/PEGTL) and its [documentation](README.md).

Copyright (c) 2014-2026 Dr. Colin Hirsch and Daniel Frey<br>
Distributed under the Boost Software License, Version 1.0<br>
See accompanying file [LICENSE_1_0.txt](../LICENSE_1_0.txt) or copy at https://www.boost.org/LICENSE_1_0.txt
