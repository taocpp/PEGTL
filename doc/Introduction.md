# Introduction

The Parsing Expression Grammar Template Library (PEGTL) is a zero-dependency C++ header-only parser combinator library for creating parsers according to a [Parsing Expression Grammar](http://en.wikipedia.org/wiki/Parsing_expression_grammar) (PEG).


## Contents

* [Hello, World!](#hello-world-)
* [Default Namespace](#default-namespace)
* [Parsing Expression Grammars](#parsing-expression-grammars)
* [Essential Terminology](#essential-terminology)


## Hello, World!

Since the PEGTL is a parser library our "Hello, world!" example parses, rather than prints, the string `Hello, foo!`, allowing for any sequence of alphabetic ASCII characters in place of `foo`.

```c++
#include <string>
#include <iostream>

// Include the commonly used parts of the core
// library with a single include directive:

#include <tao/pegtl.hpp>

namespace pegtl = TAO_PEGTL_NAMESPACE;

namespace hello
{
   // Parsing rule that matches a literal "Hello, ".

   struct prefix
      : pegtl::string< 'H', 'e', 'l', 'l', 'o', ',', ' ' >
   {};

   // Parsing rule that matches a non-empty sequence of
   // alphabetic ascii-characters (with greedy-matching).
   // As PEG or Posix regex this would be '[a-zA-Z]+'.

   struct name
      : pegtl::plus< pegtl::alpha >
   {};

   // Parsing rule that matches a sequence of the 'prefix'
   // rule, the 'name' rule, a literal "!", and 'eof'
   // (end-of-file/input). Due to 'must` instead of 'seq'
   // the rule will throw an exception when it does not
   // match the input.

   struct grammar
      : pegtl::must< prefix, name, pegtl::one< '!' >, pegtl::eof >
   {};

   // Class template for user-defined actions that does
   // nothing by default.

   template< typename Rule >
   struct action
          : pegtl::nothing< Rule >
   {};

   // Specialisation of the user-defined action to do
   // something when the 'name' rule succeeds; is called
   // with the portion of the input that matched the rule,
   // packaged in an instance of a dedicated input class.

   template<>
   struct action< name >
   {
      template< typename ActionInput >
      static void apply( const ActionInput& in, std::string& v )
      {
         v = in.string();
      }
   };

}  // namespace hello

int main( int argc, char** argv )
{
   if( argc < 2 ) {
      return 1;
   }
   // Start a parsing run of argv[1] with the string
   // variable 'name' as additional argument that will
   // be passed to all called actions, including the
   // one we attached to the 'name' rule above.

   std::string name;
   pegtl::argv_input in( argv, 1 );

   if( !pegtl::parse< hello::grammar, hello::action >( in, name ) ) {
      std::cout << "I can't parse you!" << std::endl;
      return 1;
   }
   std::cout << "Good bye, " << name << "!" << std::endl;
   return 0;
}
```

Assuming the current directory is the main directory of the PEGTL this source can be found in `src/example/hello_world.cpp`.
On Linux and Unix, including macOS, this can be compiled with a command like

```sh
$ g++ --std=c++17 -Iinclude src/example/hello_world.cpp -o hello_world
```

or with a call to `make(1)` to build *all* examples and tests via the included `Makefile`.
Once the example is built it can be invoked as follows (taking care to use single quotes as shown, and noting that the executable will reside in `build/bin/example/hello_world` when using `make`):

```sh
$ ./hello_world 'Hello, world!'
Good bye, world!
$ ./hello_world 'Hello, Colin!'
Good bye, Colin!
$ ./hello_world 'Howdy, Paula!'
I can't parse you!
```


## Default Namespace

By default, the PEGTL resides in namespace `tao::pegtl`, however this can be changed as explained in [Embedding in Libraries](Installing-and-Using.md#embedding-in-libraries).
**The entire PEGTL documentation assumes the default namespace.**

Some parts of the library use sub-namespaces, for example the parsing rules specific to UTF-8 encoded text are in namespace `tao::pegtl::utf8`.
Similarly the parsing rules for ASCII text are in namespace `tao::pegtl::ascii`, which is an [inline namespace](https://en.cppreference.com/w/cpp/language/namespace.html), making them (also) accessible "as if" they were in namespace `tao::pegtl`.

For this reason we frequently put the `ascii` in brackets and write `tao::pegtl::(ascii::)` to designate the ASCII namespace throughout this documentation.


## Parsing Expression Grammars

The PEGTL creates parsers according to a [Parsing Expression Grammar](http://en.wikipedia.org/wiki/Parsing_expression_grammar) (PEG).
The table below shows how the classical PEG combinators, or composite parsing expressions, map to PEGTL [rule class templates](Rules-and-Grammars.md).

| PEG                               | `tao::pegtl::`                                                                                            |
| ---                               | ---                                                                                                       |
| &*e*                              | [`at< R... >`](Rule-Reference.md#at-r-) <sup>[(combinators)](Rule-Reference.md#combinators)</sup>         |
| !*e*                              | [`not_at< R... >`](Rule-Reference.md#not_at-r-) <sup>[(combinators)](Rule-Reference.md#combinators)</sup> |
| *e*?                              | [`opt< R... >`](Rule-Reference.md#opt-r-) <sup>[(combinators)](Rule-Reference.md#combinators)</sup>       |
| *e*+                              | [`plus< R... >`](Rule-Reference.md#plus-r-) <sup>[(combinators)](Rule-Reference.md#combinators)</sup>     |
| *e*<sub>1</sub>*e*<sub>2</sub>    | [`seq< R... >`](Rule-Reference.md#seq-r-) <sup>[(combinators)](Rule-Reference.md#combinators)</sup>       |
| *e*<sub>1</sub> / *e*<sub>2</sub> | [`sor< R... >`](Rule-Reference.md#sor-r-) <sup>[(combinators)](Rule-Reference.md#combinators)</sup>       |
| *e**                              | [`star< R... >`](Rule-Reference.md#star-r-) <sup>[(combinators)](Rule-Reference.md#combinators)</sup>     |

And the next table shows how some of the more common atomic PEG expressions are expressed with PEGTL [rule classes](Rules-and-Grammars.md).

| PEG                               | `tao::pegtl::`                                                                         |
| ---                               | ---                                                                                    |
| *E*                               | [`eof`](Rule-Reference.md#eof) <sup>[(atomic)](Rule-Reference.md#atomic)</sup>         |
| ε                                 | [`success`](Rule-Reference.md#success) <sup>[(atomic)](Rule-Reference.md#atomic)</sup> |
| ⊥                                 | [`failure`](Rule-Reference.md#failure) <sup>[(atomic)](Rule-Reference.md#atomic)</sup> |
| .                                 | [`any`](Rule-Reference.md#any) <sup>[(ascii)](#ascii)</sup>                            |
| "a"                               | [`one< 'a' >`](Rule-Reference.md#one-c-) <sup>[(ascii)](#ascii)</sup>                  |
| "[a-h]"                           | [`range< 'a', 'h' >`](Rule-Reference.md#range-c-d-) <sup>[(ascii)](#ascii)</sup>       |
| "[a-zA-Z]"                        | [`alpha`](Rule-Reference.md#alpha) <sup>[(ascii)](#ascii)</sup>                        |

The PEGTL comes with [very many rules](Rule-Reference.md#index) that go far beyond the basics, and the possibility to [implement custom rules](Rules-and-Grammars.md#implementing-rules).


## Essential Terminology

A (parsing) **rule** is a class (or a class template) with a (suitable) static `match()` function (and some type aliases).

A **grammar** is a set of one or more related parsing rules, with one (or more) designated top-level rules as entry-point(s).

**Input data** is a (usually contiguous) sequence of bytes (or other objects) that are intended to be parsed.

An **input** is a class (template) that adheres to an informal interface, the instances of which represent input data.

A (semantic) **action** is a class template with a (suitable) static `apply()` or `apply0()` function -- and/or, for advanced use cases, a (suitable) static `match()` function.

A **control** is a class template with a set of (suitable) static functions that can change and observe most details of a parsing run.

The **states** are (optional) user-defined objects that are passed to all rules, actions and control functions.

A **parsing run** is everything that happens during a call to `tao::pegtl::parse()` with a grammar and, optionally, an action, a control and states.

A **nested parsing** run similarly refers to a call to `tao::pegtl::parse_nested()` during a parsing run (usually from an action).

A **position** is an instance of a class that indicates an object in the input data, possibly with auxiliary information like filename and line number.

Input is **consumed** when the reference to what is considered the current object in the input data is advanced by a count of one or more.

**Stream parsing** refers to parsing with an input that only provides a small contiguous buffer as window into some larger or unbounded input data.

When calling an action **attached** to a rule then the action in question is a specialization of an action class template for said rule.

An action is **applied** when its `apply()` or `apply0()` function is called after the rule it is attached to succeeded (returned `true`).

(Strictly speaking the function is called on the specialization of the action class template for the rule in question; we'll often use the shorter form.)

**Success** is when a rule's (or action's) `match()` function returns `true` after being called on some input.

**Local failure** is when a rule's (or action's) `match()` function returns `false` which can lead to backtracking.

**Global failure** is when a rule's (or action's) match()` function throws an exception which (usually) aborts the parsing run.

The **matched input** is the portion of the input data consumed by a parsing rule during a successful call to its `match()` function.

(On local failure a rule **must not** consume any input; for global failures there are no such requirements.)


---

This document is part of the [PEGTL](https://github.com/taocpp/PEGTL).

Copyright (c) 2014-2025 Dr. Colin Hirsch and Daniel Frey<br>
Distributed under the Boost Software License, Version 1.0<br>
See accompanying file [LICENSE_1_0.txt](../LICENSE_1_0.txt) or copy at https://www.boost.org/LICENSE_1_0.txt
