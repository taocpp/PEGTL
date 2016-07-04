# Rules and Grammars

Writing a PEGTL grammar means implementing custom parsing rules.

Implementing custom parsing rules can be done either by

* combining existing rules and combinators into new rules through inheritance, or

* implementing a rule from scratch, i.e. writing a class with certain properties.

## Combining Existing Rules

Combining existing rules is by far the more frequent way of creating new rules.

Here is an example that shows how existing rules are combined into a new rule through inheritance:

```c++
struct integer
   : pegtl::seq< pegtl::opt< pegtl::one< '+', '-' > >,
                 pegtl::plus< pegtl::digit > > {};
```

It defines a new rule named `integer` that is a sequence of two parts, an optional character that can be one of `+` or `-`, followed by a non-empty repetition of a digit.
Using inheritance in this way incurs no run-time penalty.

See the [Rule Reference](Rule-Reference.md) for a complete list of all rules and combinators included with the PEGTL.

Recursion, or cycles in the grammar, can be implemented after a forward-declaration of one or more rules.

```c++
struct number
   : pegtl::plus< pegtl::digit > {};

struct addition;  // Forward declaration to break the cyclic dependency.

struct bracket
   : pegtl::if_must< pegtl::one< '(' >, addition, one< ')' > > {};

struct atomic
   : pegtl::sor< number, bracket > {};

struct addition
   : pegtl::list< atomic, pegtl::one< '+' > > {};
```

When defining a large set of grammar rules in this way it can be advisable to include a `using namespace pegtl;`-definition at the beginning in order to prevent the frequent repetition of the `pegtl::` namespace qualifier.
This `using`-definition is often combined with the practice of confining a PEGTL grammar to a single translation unit, in which case there is no `namespace`-pollution and the compile time is kept low by only including the PEGTL in this one translation unit.

A grammar is nothing else than a collection of rules.
In theory, as long as a grammar does not contain cycles, complete grammars could be implemented as a single, large rule, however in practice this is not advisable since it greatly reduces the readability and testability of the grammar, in addition to being quite unmaintainable.

## Implementing New Rules

Writing a new rule from scratch is covered in the [Advanced Rules](Advanced-Rules.md) section.

## Toy S-Expression Grammar

To give another example of what a small real-world grammar might look like, below is the grammar for a toy-version of S-expressions.
It only supports proper lists, symbols, comments and numbers, and the numbers are only non-empty sequences of ASCII digits.

The rule named `file` is the intended top-level rule of the grammar, i.e. the rule that is supplied as template argument to one of the [`parse()`-functions](Parser-Reference.md) in order to start a parsing run with this grammar.

```c++
using namespace pegtl;

struct line_comment
   : until< eolf > {};

struct list;

struct list_comment
   : if_must< at< one< '(' >, disable< list > > > {};

struct comment
   : if_must< one< '#' >, sor< list_comment, line_comment > > {};

struct nothing
   : sor< space, comment > {};

struct number
   : plus< digit > {};

struct symbol
   : identifier {};

struct atom
   : sor< number, symbol > {};

struct anything;

struct list
   : if_must< one< '(' >, until< one< ')' >, anything > > {};

struct something
   : sor< atom, list > {};

struct anything
   : sor< nothing, something > {};

struct file
   : until< eof, anything > {};
```

In order to let a parsing run do more than verify whether an input conforms to the grammar, it is necessary to attach some user-defined actions to some grammar rules, as explained in the section on [Actions and States](Actions-and-States.md).

Copyright (c) 2014-2016 Dr. Colin Hirsch and Daniel Frey
