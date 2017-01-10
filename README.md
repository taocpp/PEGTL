## Welcome to the PEGTL

[![Release](https://img.shields.io/github/release/ColinH/PEGTL.svg)](https://github.com/ColinH/PEGTL/releases/latest)
[![License](https://img.shields.io/github/license/ColinH/PEGTL.svg)](#license)
[![TravisCI](https://travis-ci.org/ColinH/PEGTL.svg)](https://travis-ci.org/ColinH/PEGTL)
[![AppVeyor](https://ci.appveyor.com/api/projects/status/github/ColinH/PEGTL?svg=true)](https://ci.appveyor.com/project/ColinH/PEGTL)
[![Doozer.io](https://doozer.io/badge/ColinH/PEGTL/buildstatus/master)](https://doozer.io/user/ColinH/PEGTL)
[![Coverage](https://img.shields.io/coveralls/ColinH/PEGTL.svg)](https://coveralls.io/github/ColinH/PEGTL)
[![Issues](https://img.shields.io/github/issues/ColinH/PEGTL.svg)](https://github.com/ColinH/PEGTL/issues)

The Parsing Expression Grammar Template Library (PEGTL) is a zero-dependency C++11 header-only library for creating parsers according to a [Parsing Expression Grammar](http://en.wikipedia.org/wiki/Parsing_expression_grammar) (PEG).

### Documentation

* [Master Branch Documentation](https://github.com/ColinH/PEGTL/blob/master/doc/README.md)
* [Version 1.3.x Documentation](https://github.com/ColinH/PEGTL/blob/1.3.x/doc/README.md)

### Introduction

Grammars are written as regular C++ code, created with template programming (not template meta programming), i.e. nested template instantiations that naturally correspond to the inductive definition of PEGs or other parser combinator approaches.

A comprehensive set of parser rules that can be combined and extended by the user is included, as are mechanisms for debugging grammars and attaching user-defined actions to grammar rules.
Here is an example of how a PEG grammar rule is implemented as C++ class with the PEGTL.

```c++
// PEG rule for integers consisting of a non-empty
// sequence of digits with an optional sign:

// integer ::= ( '+' / '-' )? digit+

// The same parsing rule implemented with the PEGTL:

using namespace pegtl;

struct integer
   : seq< opt< one< '+', '-' > >,
          plus< digit > > {};
```

PEGs are superficially similar to Context-Free Grammars (CFGs), however the more deterministic nature of PEGs gives rise to some very important differences.
The included [grammar analysis](https://github.com/ColinH/PEGTL/blob/master/doc/Grammar-Analysis.md) finds several typical errors in PEGs, including left recursion.

### Design

The PEGTL is mostly concerned with parsing combinators and grammar rules, and with giving the user control over what else happens during a parsing run.

The PEGTL is designed to be "lean and mean", the actual core library has about 3000 lines of code.
Emphasis is on simplicity and efficiency but without adding any large constructions for optimising parsers.

Whether and which actions are taken, and which data structures are created during a parsing run, is entirely up to the user of the library, however we provide some [examples](https://github.com/ColinH/PEGTL/blob/master/doc/Contrib-and-Examples.md#examples) for typical situation like unescaping escape sequences in strings, building a generic [JSON](http://www.json.org/) data structure, and on-the-fly evaluation of arithmetic expressions.

Through the use of template programming and template specialisations it is possible to write a grammar once, and use it in multiple ways with different (semantic) actions in different (or the same) parsing runs.

Unlike [Antlr](http://www.antlr.org/) and Yacc/[Bison](http://www.gnu.org/software/bison/), the grammar is expressed in C++ and is part of the C++ source code.
Also, with the PEG formalism the separation into lexer and parser stages is usually dropped -- everything is done in a single grammar.

Unlike [Spirit](http://boost-spirit.com/), the grammar is implemented with compile-time template instantiations rather than run-time operator calls.
This leads to slightly increased compile times as the C++ compiler is given the task of optimising PEGTL grammars.

### Status

The master branch of the PEGTL is stable in the sense that all known bugs are fixed and all unit tests run without errors.

Each commit is [automatically](https://travis-ci.org/ColinH/PEGTL) [tested](https://ci.appveyor.com/project/ColinH/PEGTL) with [multiple](https://doozer.io/user/ColinH/PEGTL) architectures, operating systems, compilers and versions, currently:

* Debian 8 (i386), GCC (4.9) with libstdc++
* Ubuntu 12.04 LTS (amd64), GCC (4.7, 4.8, 4.9, 5, 6) with libstdc++
* Ubuntu 12.04 LTS (amd64), Clang (3.4, 3.5, 3.6, 3.7, 3.8) with libstdc++
* Ubuntu 14.04 LTS (i386, amd64), GCC (4.8) with libstdc++
* Ubuntu 16.04 LTS (i386, amd64, armhf, arm64), GCC (5) with libstdc++
* Fedora 24 (x86_64), GCC (6) with libstdc++
* Fedora 24 (x86_64), Clang (3.8) with libstdc++
* Mac OS X, Xcode (6.4, 7.3, 8.2) with libc++
* Windows, Visual Studio 2015

Systems supported but not automatically tested, *yet*:

* Android

Additionally, each commit is checked with GCC's and Clang's sanitizers as well as [`valgrind`](http://valgrind.org/).
The coverage is also automatically measured, our unit tests cover 100% of the core library code.

[Releases](https://github.com/ColinH/PEGTL/releases) are done in accordance with [Semantic Versioning](http://semver.org/).
This means that incompatible API changes can happen, but only between major versions.
For details see the [changelog](https://github.com/ColinH/PEGTL/blob/master/doc/Changelog.md).

### Thank You

* Christopher Diggins and the YARD parser for the general idea.
* Stephan Beal for the bug reports, suggestions and discussions.
* Johannes Overmann for his invaluable [`streplace`](https://code.google.com/p/streplace/) command-line tool.
* Sam Hocevar for contributing Visual Studio 2015 compatibility.
* George Makrydakis for the [inspiration](https://github.com/irrequietus/typestring) to `pegtl_string_t`.
* Kenneth Geisshirt for Android compatibility.
* Paulo Custodio for Windows-related fixes.
* Kuzma Shapran for EOL testing and fixes.
* Michael Becker and Sven Johannsen for help with CMake.

### Contact

For questions and suggestions about the PEGTL please contact the authors at `pegtl(at)colin-hirsch.net`.

### License

The PEGTL is certified [Open Source](http://www.opensource.org/docs/definition.html) software. It may be used for any purpose, including commercial purposes, at absolutely no cost. It is distributed under the terms of the [MIT license](http://www.opensource.org/licenses/mit-license.html) reproduced here.

> Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
>
> Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
>
> The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
>
> THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
