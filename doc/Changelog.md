# Changelog

#### Next

Released in the future

* Added support for different EOL-styles.
* Added new combinator class `minus< M, S >`.
* Added support for custom incremental input readers.
* Added support for parsing C streams, i.e. `std::FILE *`.
* Added support for parsing C++ streams, i.e. `std::istream`.
* Added incremental input support rule `discard`.
* Added the byte position to input classes and `position_info`.
* Added `apply` and `if_apply` rules for in-grammar direct actions.
* Removed the `begin` member from class `position_info`.
* Refactored the `input` class into multiple classes.
* Refactored the handling of nested parsing.
* Optimised superfluous input markers.
* Renamed some parsing front-end functions.
* Reduced template instantiation depth of `pegtl_(i)string_t`.
* Increased allowed string length to 512 for `pegtl_(i)string_t`.
* Allowed embedded null bytes for `pegtl_(i)string_t`.
* Added support for GCC 4.7.
* Improved CI tests.

#### 1.3.1

Released 2016-04-06

* Fixed unit test to use `eol` instead of hard-coded line ending.

#### 1.3.0

Released 2016-04-06

* Tentative Android compatibility.
* Fixed build with MinGW on Windows.
* Changed file reader to open files in binary mode.
* Changed `eol` and `eolf` to accept both Unix and MS-DOS line endings.
* Optimised bumping the input forward and removed little used bump function.
* Simplified grammar analysis algorithm (and more `analyze()` tests).

#### 1.2.2

Released 2015-11-12

* Improved the JSON grammar and JSON string escaping.
* Added JSON test suite from http://json.org/JSON_checker/.
* Optimised bumping the input forward and string unescaping.
* Promoted `examples/json_changes.hh` to `pegtl/contrib/changes.hh`.

#### 1.2.1

Released on 2015-09-21

* Added [`file_parser`](Parser-Reference.md#parser-classes) as alias for [`mmap_parser`](Parser-Reference.md#parser-classes) or [`read_parser`](Parser-Reference.md#parser-classes) depending on availability of the former.
* Added Clang 3.7 to the automated tests.
* Added MacOS X with XCode 6 and XCode 7 to the automated tests.
* Added coverage test and improved test coverage to 100%.
* Fixed state changing bug in `json_build_one` example.

#### 1.2.0

Released on 2015-08-23

* Added [`pegtl_string_t`](Rule-Reference.md#pegtl_string_t--) and [`pegtl_istring_t`](Rule-Reference.md#pegtl_istring_t--) to simplify string definitions as follows:
```c++
   pegtl::string< 'h', 'e', 'l', 'l', 'o' >  // Normal
   pegtl_string_t( "hello" )                 // New shortcut
```
* Added [`examples/abnf2pegtl.cc`](Contrib-and-Examples.md#examplesabnf2pegtlcc) application that converts grammars based on [ABNF (RFC 5234)](https://tools.ietf.org/html/rfc5234) into a PEGTL C++ grammar.
* Added [`contrib/alphabet.hh`](Contrib-and-Examples.md#pegtlcontribalphabethh) with integer constants for alphabetic ASCII letters.

#### 1.1.0

Released on 2015-07-31

* Renamed namespace `pegtl::ucs4` to `pegtl::utf32` and generally adopted UTF-32 in all naming.
* Added experimental support for UTF-16 similar to the previously existing UTF-32 parsing rules.
* Added support for merging escaped UTF-16 surrogate pairs to `pegtl/contrib/unescape.hh`.
* Fixed incorrect handling of escaped UTF-16 surrogate pairs in the JSON examples.
* A [state](Rule-Reference.md#state-s-r-)'s `S::success()`-method can now have an extended signature to get access to the current `apply_mode`, *action*- and *control* class (template).
* The `contrib/raw_string` class template now calls `Action<raw_string<...>::content>::apply()` with the user's state(s).

#### 1.0.0

Released on 2015-03-29

* First release.

#### Previous

The changelog of the old 0.x versions was dropped due to the [large refactoring](2014-Refactoring.md) preceding version 1.0.0.

The PEGTL was initially developed in 2008 as an experiment in C++0x based on ideas from the YARD library by Christopher Diggins.
Several years of real-world experience and smaller updates passed until 2014, when some of the more experimental PEGTL features were removed, the remaining parts refactored and refined, and some interesting additions made.

Copyright (c) 2014-2016 Dr. Colin Hirsch and Daniel Frey
