# Welcome to the PEGTL

[![Release](https://img.shields.io/github/release/taocpp/PEGTL.svg)](https://github.com/taocpp/PEGTL/releases/latest)
[![Download](https://api.bintray.com/packages/conan/conan-center/taocpp-pegtl%3A_/images/download.svg)](https://bintray.com/conan/conan-center/taocpp-pegtl%3A_/_latestVersion)
[![TravisCI](https://travis-ci.org/taocpp/PEGTL.svg?branch=master)](https://travis-ci.org/taocpp/PEGTL)
[![AppVeyor](https://ci.appveyor.com/api/projects/status/pa5sbnw68tu650aq/branch/master?svg=true)](https://ci.appveyor.com/project/taocpp/PEGTL)
[![Coverage](https://coveralls.io/repos/github/taocpp/PEGTL/badge.svg?branch=master)](https://coveralls.io/github/taocpp/PEGTL)
[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/taocpp/PEGTL.svg)](https://lgtm.com/projects/g/taocpp/PEGTL/context:cpp)

The Parsing Expression Grammar Template Library (PEGTL) is a zero-dependency C++ header-only parser combinator library for creating parsers according to a [Parsing Expression Grammar](http://en.wikipedia.org/wiki/Parsing_expression_grammar) (PEG).

## Documentation

* [Version 3.x](doc/README.md) (requires C++17)
* [Version 2.x](https://github.com/taocpp/PEGTL/blob/2.x/doc/README.md) (requires C++11)
* [Version 1.x](https://github.com/taocpp/PEGTL/blob/1.x/doc/README.md) (requires C++11)

## Introduction

Grammars are written as regular C++ code, created with template programming (not template meta programming), i.e. nested template instantiations that naturally correspond to the inductive definition of PEGs (and other parser-combinator approaches).

A comprehensive set of [parser rules](doc/Rule-Reference.md) that can be combined and extended by the user is included, as are mechanisms for debugging grammars, and for attaching user-defined [actions](doc/Actions-and-States.md) to grammar rules.
Here is an example of how a PEG grammar rule is implemented as C++ class with the PEGTL.

```c++
// PEG rule for integers consisting of a non-empty
// sequence of digits with an optional sign:

// sign ::= '+' / '-'
// integer ::= sign? digit+

// The same parsing rule implemented with the PEGTL:

using namespace tao::pegtl;

struct sign : one< '+', '-' > {};
struct integer : seq< opt< sign >, plus< digit > > {};
```

PEGs are superficially similar to Context-Free Grammars (CFGs), however the more deterministic nature of PEGs gives rise to some very important differences.
The included [grammar analysis](doc/Grammar-Analysis.md) finds several typical errors in PEGs, including left recursion.

## Design

The PEGTL is designed to be "lean and mean", the core library consists of approximately 6000 lines of code.
Emphasis is on simplicity and efficiency, preferring a well-tuned simple approach over complicated optimisations.

The PEGTL is mostly concerned with parsing combinators and grammar rules, and with giving the user of the library (the possibility of) full control over all other aspects of a parsing run. Whether/which actions are taken, and whether/which data structures are created during a parsing run, is entirely up to the user.

Included are some [examples](doc/Contrib-and-Examples.md#examples) for typical situation like unescaping escape sequences in strings, building a generic [JSON](http://www.json.org/) data structure, and on-the-fly evaluation of arithmetic expressions.

Through the use of template programming and template specialisations it is possible to write a grammar once, and use it in multiple ways with different (semantic) actions in different (or the same) parsing runs.

With the PEG formalism, the separation into lexer and parser stages is usually dropped -- everything is done in a single grammar.
The rules are expressed in C++ as template instantiations, and it is the compiler's task to optimise PEGTL grammars.

## Status

Each commit is automatically tested with multiple architectures, operating systems, compilers, and versions thereof.

* Windows

  * Visual Studio 2017 (x86, x64)
  * Visual Studio 2019 (x86, x64)

* macOS (using libc++)

  * macOS 10.13, Xcode 9.4
  * macOS 10.14, Xcode 10.3
  * macOS 10.14, Xcode 11.4

* Ubuntu 16.04 LTS (using libstdc++)

  * GCC 7.x, 8.x, 9.x
  * Clang 5.x, 6.x, 7.x, 8.x, 9.x, 10.x

Additionally, each commit is checked with Clang's [Static Analyzer](https://clang-analyzer.llvm.org/), GCC's and Clang's [sanitizers](https://github.com/google/sanitizers), [`clang-tidy`](http://clang.llvm.org/extra/clang-tidy/), and [`valgrind`](http://valgrind.org/).
Code coverage is automatically measured and the unit tests cover 100% of the core library code (for releases).

[Releases](https://github.com/taocpp/PEGTL/releases) are done in accordance with [Semantic Versioning](http://semver.org/).
Incompatible API changes are *only* allowed to occur between major versions.
For details see the [changelog](doc/Changelog.md).

## Thank You

In appreciation of all contributions here are the people that have [directly contributed](https://github.com/taocpp/PEGTL/graphs/contributors) to the PEGTL and/or its development.

[![](https://avatars.githubusercontent.com/u/216384?s=117)](https://github.com/andoma)
[![](https://avatars.githubusercontent.com/u/727911?s=117)](https://github.com/bjoe)
[![](https://avatars.githubusercontent.com/u/447049?s=117)](https://github.com/bwagner)
[![](https://avatars.githubusercontent.com/u/1759994?s=117)](https://github.com/cdiggins)
[![](https://avatars.githubusercontent.com/u/35096584?s=117)](https://github.com/delpinux)
[![](https://avatars.githubusercontent.com/u/1353140?s=117)](https://github.com/dkopecek)
[![](https://avatars.githubusercontent.com/u/3957811?s=117)](https://github.com/gene-hightower)
[![](https://avatars.githubusercontent.com/u/231192?s=117)](https://github.com/irrequietus)
[![](https://avatars.githubusercontent.com/u/572755?s=117)](https://github.com/jedelbo)
[![](https://avatars.githubusercontent.com/u/458871?s=117)](https://github.com/joelfrederico)
[![](https://avatars.githubusercontent.com/u/21071787?s=117)](https://github.com/johelegp)
[![](https://avatars.githubusercontent.com/u/6087443?s=117)](https://github.com/jovermann)
[![](https://avatars.githubusercontent.com/u/1225363?s=117)](https://github.com/kneth)
[![](https://avatars.githubusercontent.com/u/1858553?s=117)](https://github.com/kuzmas)
[![](https://avatars.githubusercontent.com/u/1138455?s=117)](https://github.com/lambdafu)
[![](https://avatars.githubusercontent.com/u/433009?s=117)](https://github.com/lichray)
[![](https://avatars.githubusercontent.com/u/8768950?s=117)](https://github.com/michael-brade)
[![](https://avatars.githubusercontent.com/u/13936020?s=117)](https://github.com/mkrupcale)
[![](https://avatars.githubusercontent.com/u/162319?s=117)](https://github.com/newproggie)
[![](https://avatars.githubusercontent.com/u/741109?s=117)](https://github.com/obiwahn)
[![](https://avatars.githubusercontent.com/u/1442822?s=117)](https://github.com/ohanar)
[![](https://avatars.githubusercontent.com/u/70773?s=117)](https://github.com/pauloscustodio)
[![](https://avatars.githubusercontent.com/u/39619854?s=117)](https://github.com/pleroux0)
[![](https://avatars.githubusercontent.com/u/30195320?s=117)](https://github.com/quadfault)
[![](https://avatars.githubusercontent.com/u/4220569?s=117)](https://github.com/robertcampion)
[![](https://avatars.githubusercontent.com/u/245089?s=117)](https://github.com/samhocevar)
[![](https://avatars.githubusercontent.com/u/691989?s=117)](https://github.com/sanssecours)
[![](https://avatars.githubusercontent.com/u/235303?s=117)](https://github.com/sgbeal)
[![](https://avatars.githubusercontent.com/u/23705081?s=117)](https://github.com/skyrich62)
[![](https://avatars.githubusercontent.com/u/799344?s=117)](https://github.com/studoot)
[![](https://avatars.githubusercontent.com/u/1538181?s=117)](https://github.com/svenjo)
[![](https://avatars.githubusercontent.com/u/12001183?s=117)](https://github.com/wickedmic)
[![](https://avatars.githubusercontent.com/u/6502881?s=117)](https://github.com/wravery)
[![](https://avatars.githubusercontent.com/u/43971430?s=117)](https://github.com/zhihaoy)

## The Art of C++

The PEGTL is part of [The Art of C++](https://taocpp.github.io/).

[![Colin Hirsch](https://avatars.githubusercontent.com/u/113184?s=117)](https://github.com/colinh)
[![Daniel Frey](https://avatars.githubusercontent.com/u/3956325?s=117)](https://github.com/d-frey)
[![Uilian Ries](https://avatars.githubusercontent.com/u/4870173?s=117)](https://github.com/uilianries)

## Contact

For questions and suggestions regarding the PEGTL, success or failure stories, and any other kind of feedback, please feel free to open an issue or a PR on GitHub or contact the authors at `taocpp(at)icemx.net`.

## License

The PEGTL is certified [Open Source](http://www.opensource.org/docs/definition.html) software. It may be used for any purpose, including commercial purposes, at absolutely no cost. It is distributed under the terms of the [MIT license](http://www.opensource.org/licenses/mit-license.html) reproduced here.

> Copyright (c) 2007-2020 Dr. Colin Hirsch and Daniel Frey
>
> Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
>
> The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
>
> THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
