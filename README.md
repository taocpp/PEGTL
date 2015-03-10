## Welcome to the PEGTL

The Parsing Expression Grammar Template Library (PEGTL) is a C++11 header-only library for creating parsers according to a Parsing Expression Grammar (PEG).
Grammars are written as regular C++ code, created with template programming (not template meta programming), i.e. nested template instantiations that naturally correspond to the inductive definition of PEGs.

A comprehensive set of parser rules that can be combined and extended by the user is included, as are mechanisms for debugging grammars and attaching user-defined actions to grammar rules.
Here is an example of how a PEG grammar rule is implemented as C++ class with the PEGTL.

```c++
  // PEG rule for integers consisting of a non-empty
  // sequence of digits with an optional sign:

  // integer ::= ( '+' / '-' )? digit+

  // The same parsing rule implemented with the PEGTL:

  struct integer
     : pegtl::seq< pegtl::opt< pegtl::one< '+', '-' > >,
                   pegtl::plus< pegtl::digit > > {};
```

PEGs are superficially similar to Context-Free Grammars (CFGs), however the more deterministic and procedural nature of PEGs gives rise to some very important differences.
The included [grammar analysis](https://github.com/ColinH/PEGTL/wiki/Grammar-Analysis) helps to find several typical errors in PEGs including left recursion.
For an in-depth description of PEGs see [Wikipedia page on PEGs](http://en.wikipedia.org/wiki/Parsing_expression_grammar) or [this paper on PEGs](http://pdos.csail.mit.edu/~baford/packrat/popl04/peg-popl04.pdf) by Bryan Ford.

The PEGTL was initially developed in 2008 as an experiment in C++0x based on ideas from the YARD library by Christopher Diggins.
Several years of real-world experience and smaller updates passed until 2014, when some of the more experimental PEGTL features were removed, the remaining parts refactored and refined, and some interesting additions made.

## Status

The master branch of the PEGTL is **always** stable in the sense that all known bugs are fixed and all unit tests run without errors. It is not stable in the sense that incompatible API changes can happen before/between major version releases.

For questions and suggestions please contact the authors at **pegtl (at) colin-hirsch.net**.

## Documentation

* [Getting Started](https://github.com/ColinH/PEGTL/wiki/Getting-Started)
* [Calculator Example](https://github.com/ColinH/PEGTL/wiki/Calculator-Example)
* [Rules and Grammars](https://github.com/ColinH/PEGTL/wiki/Rules-and-Grammars)
* [Actions and States](https://github.com/ColinH/PEGTL/wiki/Actions-and-States)
* [Rule Reference](https://github.com/ColinH/PEGTL/wiki/Rule-Reference)
  * [Combinators](https://github.com/ColinH/PEGTL/wiki/Rule-Reference#combinators)
  * [Basic Rules](https://github.com/ColinH/PEGTL/wiki/Rule-Reference#basic-rules)
  * [ASCII Rules](https://github.com/ColinH/PEGTL/wiki/Rule-Reference#ascii-rules)
  * [UTF-8 Rules](https://github.com/ColinH/PEGTL/wiki/Rule-Reference#utf-8-rules)
  * [UCS-4 Rules](https://github.com/ColinH/PEGTL/wiki/Rule-Reference#ucs-4-rules)
  * [Full Index](https://github.com/ColinH/PEGTL/wiki/Rule-Reference#full-index)
* [Parser Reference](https://github.com/ColinH/PEGTL/wiki/Parser-Reference)
  * [Input and Errors](https://github.com/ColinH/PEGTL/wiki/Parser-Reference#input-and-errors)
  * [Parser Functions](https://github.com/ColinH/PEGTL/wiki/Parser-Reference#parser-functions)
  * [Tracer Functions](https://github.com/ColinH/PEGTL/wiki/Parser-Reference#tracer-functions)
  * [Parser Classes](https://github.com/ColinH/PEGTL/wiki/Parser-Reference#parser-classes)
* [Contrib Reference](https://github.com/ColinH/PEGTL/wiki/Contrib-Reference)
  * [Contrib](https://github.com/ColinH/PEGTL/wiki/Contrib-Reference#contrib)
  * [Examples](https://github.com/ColinH/PEGTL/wiki/Contrib-Reference#examples)
* [Control Hooks](https://github.com/ColinH/PEGTL/wiki/Control-Hooks)
* [Advanced Rules](https://github.com/ColinH/PEGTL/wiki/Advanced-Rules)
* [Grammar Analysis](https://github.com/ColinH/PEGTL/wiki/Grammar-Analysis)
* [2014 Refactoring](https://github.com/ColinH/PEGTL/wiki/2014-Refactoring)

## Changelog

A new changelog will start here with the first major release version 1.0.0.

## License

Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
