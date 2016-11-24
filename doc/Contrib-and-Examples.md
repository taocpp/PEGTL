# Contrib and Examples

The PEGTL includes several parts that go beyond the core library functionality.

They are included both for convenience and to show how certain things can be done with the PEGTL.

## Notice

All feedback is highly welcome, in particular whether more sub-rules to serve as attachment points for actions are required.

Similarly, if you have written a grammar with the PEGTL that might be generally useful, you are welcome to contribute it for inclusion in future versions.

For all questions and remarks contact us at **pegtl (at) colin-hirsch.net**.

## Contents

* [Contrib](#contrib)
* [Examples](#examples)

## Contrib

###### `pegtl/contrib/abnf.hh`

* Core ABNF rules according to [RFC 5234, Appendix B](https://tools.ietf.org/html/rfc5234).
* Ready for production use.

###### `pegtl/contrib/alphabet.hh`

* Constants for ASCII letters.
* Shortens `string<'f','o','o'>` to `string<f,o,o>`.
* Ready for production use.

###### `pegtl/contrib/changes.hh`

* Control class components for [external switching style](Switching-Style.md).
* Ready for production use but might be changed in the future.

###### `pegtl/contrib/http.hh`

* HTTP 1.1 grammar according to [RFC 7230](https://tools.ietf.org/html/rfc7230).
* This is still experimental.

###### `pegtl/contrib/json.hh`

* JSON grammar according to [RFC 7159](https://tools.ietf.org/html/rfc7159) (for UTF-8 encoded JSON only).
* Ready for production use.

###### `pegtl/contrib/raw_string.hh`

* Grammar rules to parse Lua-style long (or raw) string literals.
* Ready for production use.

###### `pegtl/contrib/unescape.hh`

This file does not contain a grammar, but rather:

* Utility functions frequently needed to unescape escape-sequences.
* Action classes that perform unescaping of escape-sequences.

###### `pegtl/contrib/uri.hh`

* URI grammar according to [RFC 3986](https://tools.ietf.org/html/rfc3986).
* This is still experimental.

## Examples

###### `examples/abnf2pegtl.cc`

Reads a file with a grammar based on [ABNF (RFC 5234)](https://tools.ietf.org/html/rfc5234) and converts it into corresponding PEGTL rule definitions in C++. Some extensions and restrictions against RFC 5234 exist:

* As we are defining PEGs, the alternations are now ordered (`sor<>`)
* The *and*- and *not*-predicates from PEGs have been added as `&` and `!` respectively
* All line endings (CR, LF, CRLF) are accepted
* C++ identifier are formed by replacing the minus(es) of the rulename with underscore(s)
* Reserved identifiers (keywords, ...) are rejected
* When using numerical values, they must be within the range of the corresponding C++ data type

###### `examples/calculator.cc`

A calculator with all binary operators from the C language that shows

* how to use stack-based actions to perform a calculation on-the-fly during the parsing run, and
* how to build a grammar with a run-time data structure for arbitrary binary operators with arbitrary precedence and associativity.

###### `examples/csv1.cc` and `examples/csv2.cc`

Two simple examples for grammars that parse some kind of CSV-style file formats.

###### `examples/hello_world.cc`

Minimal parser-style "hello world" example.

###### `examples/json_parse.cc`

Shows how to use the custom error messages defined in `json_errors.hh` with the `pegtl/contrib/json.hh` grammar to parse command line arguments as JSON data.

###### `examples/json_build_one.cc`

Extends on `json_parse.cc` by parsing JSON files into generic JSON data structures using the "[switching style](Switching-Style.md)" of managing the actions and states.

###### `examples/json_build_two.cc`

Extends on `json_parse.cc` by parsing JSON files into generic JSON data structures using a single monolithic state and action for the entire parsing run.

###### `examples/lua53_parse.cc`

Parses all files passed on the command line with a slightly experimental grammar that should correspond to the [Lua](http://www.lua.org/) 5.3 lexer and parser.

###### `examples/mmap_test.cc`

Simple `cat` replacement to test the included `mmap(2)`-based file reader on platforms that support it.

###### `examples/modulus_match.cc`

Shows how to implement a simple custom parsing rule with the simplified calling convention.

###### `examples/read_test.cc`

Simple `cat` replacement to test the included `fread(3)`-based file reader.

###### `examples/s_expression.cc`

Grammar for a toy-version of S-expressions that shows how to include other files during a parsing run.

###### `examples/unescape.cc`

Uses the building blocks from `pegtl/contrib/unescape.hh` to show how to actually unescape a string literal with various typical escape sequences.

###### `examples/uri_trace.cc`

Shows how to use the included tracer control, here together with the URI grammar from `pegtl/contrib/uri.hh`.

Copyright (c) 2014-2016 Dr. Colin Hirsch and Daniel Frey
