# Rules and Grammars

A (parsing) rule is a class that matches a portion of the input against some condition and either succeeds, possibly consuming a portion of the input, or fails.
A grammar is a set of one or more related parsing rules -- and an informally distinguished top-level rule that serves as entry point.
The top-level rule is also called a grammar.

Some grammars have more than one top-level rule, and any rule *can* be used as top-level rule -- the rule that drives a parsing run -- which can be useful to partially test a grammar by testing some of its rules individually.

## Contents

 * [Rule Basics](#rule-basics)
 * [Creating Rules](#creating-rules)
 * [Recursive Rules](#recursive-rules)
 * [S-Expression Grammar](#s-expression-grammar)
 * [Implementing Rules](#implementing-rules)

## Rule Basics

The rules provided with the PEGTL are either classes with fixed parsing behaviour or paremeterizable class templates.
Most rules that are parameterizable class templates either take values or parsing rules as parameters.
Rules that take other rules as parameters are also called (parsing) combinator rules, or combinators.

The rule [`tao::pegtl::eof`](Rule-Reference.md#eof) is declared as `struct eof`, a rule class without parameters.

The rule [`tao::pegtl::consume`](Rule-Reference.md#consume-num-) is declared as `template< std::size_t Num > struct consume`, a rule class template with a value parameter.

The rule [`tao::pegtl::seq`](Rule-Reference.md#seq-r-) is declared as `template< typename... Rule > struct seq`, a rule class template with other rule classes as type parameters, which makes it a combinator.

Creating custom rules and grammars is done by parameterizing and combining the rules provided with the PEGTL.

When a grammar goes beyond what is (easily) achievable with this approach a fully custom rule can be [implemented from scratch](#implementing-rules).

Defining new types instead of type aliases is preferred for two main reasons.

1. Some debug facilities will ouput the (shorter) type name rather than the (longer) expansion.
2. Using new types makes it possible to attach different [actions](Actions-and-States.md) to "different names of the same rule".

## Creating Rules

The usual way to create a new rule is to define a struct that inherits from the desired rule implementation.

```c++
struct sign : tao::pegtl::one< '+', '-' > {};
```

Such a rule can again be a class templates that need to be instantiated to yield an actual parsing rule.

```c++
template< char N >
struct n_and_n_plus_one : tao::pegtl::one< N, N + 1 > {};
```

Now consider the rules from the [main PEGTL readme](../README.md).

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

This defines two rules.

1. A rule for the sign of a number, **one** character which is either a plus or a minus.
2. A rule for an integer, a **seq**uence of an **opt**ional sign followed by one or more digits.

This is it, this is how new rules are created from existing rules, using C++ template syntax!

See the [Rule Reference](Rule-Reference.md) for a complete list of all rules and combinators included with the PEGTL.

Some included "unofficial" rules can be found on the [Contrib and Examples](Contrib-and-Examples.md) page.

## Recursive Rules

Recursion, or cycles in the grammar, can be implemented with a forward-declaration of one or more rules.

```c++
struct number
   : tao::pegtl::plus< tao::pegtl::digit > {};

struct addition;  // Forward declaration to break the cyclic dependency.

struct bracket
   : tao::pegtl::if_must< tao::pegtl::one< '(' >, addition, tao::pegtl::one< ')' > > {};

struct atomic
   : tao::pegtl::sor< number, bracket > {};

struct addition
   : tao::pegtl::list< atomic, tao::pegtl::one< '+' > > {};
```

In many cases the same grammar can be implemented either recursively *or* with some iterating combinators like `star<>` or `list<>`.

## S-Expression Grammar

To give another example of what a small real-world grammar might look like, here is the grammar for a toy-version of S-expressions.
It only supports proper lists, symbols, comments, and numbers, which are non-empty sequences of ASCII digits.

The rule named `file` is the intended top-level rule of the grammar, i.e. the rule that is supplied as template argument to [the `parse()` function](Inputs-and-Parsing.md#parse-function) in order to start a parsing run with this grammar.

```c++
struct hash_comment
   : tao::pegtl::until< tao::pegtl::eolf > {};

struct list;

struct list_comment
   : tao::pegtl::if_must< tao::pegtl::at< tao::pegtl::one< '(' > >, tao::pegtl::disable< list > > {};

struct read_include
   : tao::pegtl::seq< tao::pegtl::one< ' ' >, tao::pegtl::one< '"' >, tao::pegtl::plus< tao::pegtl::not_one< '"' > >, tao::pegtl::one< '"' > > {};

struct hash_include
   : tao::pegtl::if_must< tao::pegtl::string< 'i', 'n', 'c', 'l', 'u', 'd', 'e' >, read_include > {};

struct hashed
   : tao::pegtl::if_must< tao::pegtl::one< '#' >, tao::pegtl::sor< hash_include, list_comment, hash_comment > > {};

struct number
   : tao::pegtl::plus< tao::pegtl::digit > {};

struct symbol
   : tao::pegtl::identifier {};

struct atom
   : tao::pegtl::sor< number, symbol > {};

struct anything;

struct list
   : tao::pegtl::if_must< tao::pegtl::one< '(' >, tao::pegtl::until< tao::pegtl::one< ')' >, anything > > {};

struct normal
   : tao::pegtl::sor< atom, list > {};

struct anything
   : tao::pegtl::sor< tao::pegtl::space, hashed, normal > {};

struct main
   : tao::pegtl::until< tao::pegtl::eof, tao::pegtl::must< anything > > {};
```

In order to let a parsing run do more than verify whether an input conforms to the grammar, it is necessary to attach user-defined *actions* to some grammar rules, as explained in [Actions and States](Actions-and-States.md).

## Implementing Rules

When a grammar goes beyond what is (easily) achievable with this approach a fully custom rule can be implemented from scratch.
The page [Rule Anatomy](Rule-Anatomy.md) explains what it takes to implement a parsing rule and its match function.

---

This document is part of the [PEGTL](https://github.com/taocpp/PEGTL).

Copyright (c) 2014-2025 Dr. Colin Hirsch and Daniel Frey<br>
Distributed under the Boost Software License, Version 1.0<br>
See accompanying file [LICENSE_1_0.txt](../LICENSE_1_0.txt) or copy at https://www.boost.org/LICENSE_1_0.txt
