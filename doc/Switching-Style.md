# Switching Style

Implementing a parser with the PEGTL consists of two main parts.

1. The actual grammar that drives the parser.
2. The states and actions that "do something".

For the second part, there are three distinct styles of how to manage the states and actions in non-trivial parsers.

The **main issue** addressed by the switching styles is the **growing complexity** encountered when a single state argument to a parsing run must perform multiple different tasks, including the management of nested data structures.

The way that this issue is addressed is by providing another tool for performing divide-and-conquer: A large state class with multiple tasks can be divided into

- multiple smaller state classes that each take care of a single issue,
- one or more [control classes](Control-Hooks.md) that switch between the states,
- using the C++ stack for nested structures (rather than manually managing a stack).

The different styles can also be freely mixed within the same parser.

## No Switching

The "no switching style" consists of having one (or more) state-arguments that are passed to a parsing run and that are the arguments to all action's `apply()`-methods.

For an example of how to build a generic JSON data structure with the "no switching style" see `examples/json_build_two.cc`.

## Intrusive Switching

The `state<>` and `action<>` [meta combinators](Rule-Reference.md#meta-rules) can be used to hard-code state and actions switches in the grammar.

In some cases a state object is required for the grammar itself, and in these cases embedding the state-switch into the grammar is recommended.

For an example see `pegtl/contrib/raw_string.hh` where a state-switch is necessary for the parsing rules to function correctly.

## External Switching

"External switching" is when the states and/or actions are switched from outside of the grammar by providing a specialised control class.

For an example of how to build a generic JSON data structure with the "external switching style" see `examples/json_build_one.cc`.

The actual switching control classes are defined in `pegtl/contrib/changes.hh` and can be used as template for custom switching.

Copyright (c) 2014-2016 Dr. Colin Hirsch and Daniel Frey
