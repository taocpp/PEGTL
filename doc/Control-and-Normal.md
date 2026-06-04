# Control and Normal

A **control** is a class that adheres to an informal control interface and is in control of important behind-the-scenes details of a parsing run.


## Contents

* [Introduction](#introduction)
* [Control Interface](#control-interface)
  * [`start`](#start)
  * [`success`](#success)
  * [`failure`](#failure)
  * [`guard`](#guard)
  * [`raise`](#raise)
  * [`raise_nested`](#raise_nested)
  * [`unwind`](#unwind)
  * [`apply`](#apply)
  * [`apply0`](#apply0)
  * [`match`](#match)
* [Normal Control](#normal-control)
  * [`enable`](#enable)
  * [`start`](#start-1)
  * [`success`](#success-1)
  * [`failure`](#failure-1)
  * [`guard`](#guard-1)
  * [`raise`](#raise-1)
  * [`raise_nested`](#raise_nested-1)
  * [`apply`](#apply-1)
  * [`apply0`](#apply0-1)
  * [`match`](#match-1)
* [Changing Control](#changing-control)
  * [Via Rules](#via-rules)
  * [Via Actions](#via-actions)
* [Control Traces](#control-traces)
  * [Rule Success](#rule-success)
  * [Rule Local Failure](#rule-local-failure)
  * [Action Apply](#action-apply)


## Introduction

The control has functions that are called on many occasions during a parsing run.
This control functionality can be customized for multiple reasons, most prominently to

* obtain some debug or trace information from a parsing run, or
* customize and/or extend some behavior of a parsing run.

Examples of customized behavior are how exceptions are thrown and which type they have, and how to obtain a rule-dependent error message for an exception.

The normal PEGTL behavior is implemented in the [normal control](#normal-control).
Additional control adapters are documented in the [control reference](Control-Reference.md).


## Control Interface

A control is a class template that takes the current `Rule` as template parameter and implements some static functions.

```c++
template< typename Rule >
struct foo_control
{
   template< typename ParseInput,
             typename... States >
   static void start( const ParseInput&, States&&... );

   template< typename ParseInput,
             typename... States >
   static void success( const ParseInput&, States&&... );

   template< typename ParseInput,
             typename... States >
   static void failure( const ParseInput&, States&&... );

   template< apply_mode A,
             rewind_mode M,
             template< typename... > class Action,
             template< typename... > class Control,
             typename ParseInput,
             typename... States >
   static auto guard( [[maybe_unused]] ParseInput& in, States&&... /*unused*/ );

   template< typename ParseInput,
             typename... States >
   [[noreturn]] static void raise( const ParseInput&, States&&... );

   template< typename Ambient,
             typename ParseInput,
             typename... States >
   [[noreturn]] static void raise_nested( const Ambient&, const ParseInput&&, States&&... );

   template< typename ParseInput,
             typename... States >
   static void unwind( const ParseInput&, States&&... );

   template< template< typename... > class Action,
             typename Iterator,
             typename ParseInput,
             typename... States >
   static auto apply( const Iterator& begin, const ParseInput& in, States&&... st )
      -> decltype( ... );

   template< template< typename... > class Action,
             typename ParseInput,
             typename... States >
   static auto apply0( const ParseInput&, States&&... st )
      -> decltype( ... );

   template< apply_mode A,
             rewind_mode M,
             template< typename... > class Action,
             template< typename... > class Control,
             typename ParseInput,
             typename... States >
   [[nodisard]] static bool match( ParseInput& in, States&&... st );
};
```

The exact set of control functions is not set in stone, for example the `start()`, `success()` and `failure()` functions are usually only called from `tao::pegtl::match()` and could be removed if that function is replaced.
However we consider it best practice to go with the interface established by [`tao::pegtl::normal`](#normal-control) and `tao::pegtl::match()`.
It is no accident that all [additional controls](Control-Reference.md) included with the PEGTL are adapters that add some functionality to an existing control which, directly or indirectly, always ends up using [`normal`](#normal-control).

###### `start`

The function `C< R >::start()` is called before matching rule `R` with current control `C`.

This function does not *need* to do anything for a normal parsing run and can be empty.

###### `success`

The function `C< R >::success()` is called after matching rule `R` with current control `C` succeeded and after any action attached to `R` was applied and the action has a return type `void` or also succeeded.

This function does not *need* to do anything for a normal parsing run and can be empty.

###### `failure`

The function `C< R >::failure()` is called after matching rule `R` with current control `C` locally failed *or* after `R` succeeded but the action attached to `R` was applied, has a return type `bool` and (locally) failed.

This function does not *need* to do anything for a normal parsing run and can be empty.

###### `guard`

The function `C< R >::guard()` is called when the implementation of `R::match()` requires a [rewind guard](Rules-and-Grammars.md#rewind-guard) with current control `C` to ensure it does not consume input on local failure.

###### `raise`

The function `C< T >::raise()` is called to generate a global error during parsing for type `T` and current control `C`.

Under normal circumstances, for example the global error being due to a [`must`](Rule-Reference.md#must-r-) rule, the type `T` will be the rule that (globally) failed.

This function must not return.

This function will only be called when exception support is enabled.

###### `raise_nested`

The function `C< T >::raise_nested()` is called to propagate a global error during [nested parsing](Inputs-and-Parsing.md#nested-parsing) for type `T` and current control `C`.

Under normal circumstances this will only be called by [`parse_nested`](Inputs-and-Parsing.md#nested-parsing) where `T` will be the top-level rule from the nested parsing run.

This function must not return.

This function will only be called when exception support is enabled.

###### `unwind`

The function `C< R >::unwind()` is called during stack unwinding when matching rule `R` produced a global failure.

The type of exception modeling the global error is not relevant.

Unlike all other control functions the `unwind()` function is **optional**.

* Its presence is auto-detected at compile-time and it is called only when present, or:
* Its presence is ignored when compiling without exception support.

This function does not *need* to do anything for a normal parsing run and can be empty, though, to allow for a micro-optimization, it should be omitted when not needed.

This function will only be called when exception support is enabled.

###### `apply`

The function `C< R >::apply()` is called after matching rule `R` with current control `C` succeeded and `A< R >` for current action `A` has an `apply()` function that can be called with an `action_input` and the current states.

The trailing return type of this function must be set up to match the return type of `A< R >::apply()` when that exists, and to not be defined otherwise.
This disable `C< R >::apply()` via [SFINAE](https://en.cppreference.com/w/cpp/language/sfinae.html) when it is not needed due to no action being attached to `R`.

###### `apply0`

The function `C< R >::apply0()` is called after matching rule `R` with current control `C` succeeded and `A< R >` for current action `A` has an `apply0()` function that can be called with the current states.

The trailing return type of this function must be set up to match the return type of `A< R >::apply0()` when that exists, and to not be defined otherwise.
This disable `C< R >::apply0()` via [SFINAE](https://en.cppreference.com/w/cpp/language/sfinae.html) when it is not needed due to no action being attached to `R`.

###### `match`

The function `C< R >::match()` is called to attempt matching rule `R`.
It is the entry-point to matching `R` and is directly called by [`parse`](Inputs-and-Parsing.md#parse-function) and combinators like [`seq`](Rule-Reference.md#seq-r-) and [`sor`](Rule-Reference.md#sor-r-).

This function is therefore crucial to parsing and has to be implemented by every control.
A minimal implementation would be to forward the call to `tao::pegtl::match()` from `include/tao/pegtl/match.hpp`.


## Normal Control

The `normal` control is the default control of the PEGTL.

It implements all required control functions in a way that provides for "normal" parsing behavior.
It also adds two features, the [`enable`](#enable) variable and the possibility to [delegate matching to an action](Actions-and-States.md#match).
The optional `unwind()` function is **not** implemented.

```c++
template< typename Rule >
struct normal
{
   static constexpr bool enable;  // See also Meta-Data-and-Visit.md

   template< typename ParseInput,
             typename... States >
   static void start( const ParseInput&, States&&... ) noexcept {}

   template< typename ParseInput,
             typename... States >
   static void success( const ParseInput&, States&&... ) noexcept {}

   template< typename ParseInput,
             typename... States >
   static void failure( const ParseInput&, States&&... ) noexcept {}

   template< apply_mode A,
             rewind_mode M,
             template< typename... > class Action,
             template< typename... > class Control,
             typename ParseInput,
             typename... States >
   static auto guard( [[maybe_unused]] ParseInput& in, States&&... /*unused*/ ) noexcept;

   template< typename ParseInput,
             typename... States >
   [[noreturn]] static void raise( const ParseInput&, States&&... );

   template< typename Ambient,
             typename ParseInput,
             typename... States >
   [[noreturn]] static void raise_nested( const Ambient&, const ParseInput&&, States&&... );

   template< template< typename... > class Action,
             typename Iterator,
             typename ParseInput,
             typename... States >
   static auto apply( const Iterator& begin, const ParseInput& in, States&&... st ) noecept( auto )
      -> decltype( ... );

   template< template< typename... > class Action,
             typename ParseInput,
             typename... States >
   static auto apply0( const ParseInput&, States&&... st ) noexcept( auto )
      -> decltype( ... );

   template< apply_mode A,
             rewind_mode M,
             template< typename... > class Action,
             template< typename... > class Control,
             typename ParseInput,
             typename... States >
   [[nodisard]] static bool match( ParseInput& in, States&&... st );
};
```

###### `enable`

When `normal< R >::enable` is `false` for a rule `R` then **no control functions - except [`match`](#match) - will be called** while matching `R`.

> [NOTE!]
> This does **not** disable control functions for sub-rules of `R`, only for `R` itself!

In (nearly?) all cases user-defined controls can use the default from the [`normal`](#normal-control) control (where `enable` is `true` for all user-visible rules) and otherwise ignore the subject (including the following explanation as to why this exists).

By default `enable` is `true` for all user-visible rules, i.e. for all rule classes except those implemented in namespace `tao::pegtl::internal`.
For these internal rule classes `enable` is set to `false`.

To understand which problem this solves consider the fact that some PEGTL combinators are implemented using other rules and combinators.
For example [`minus`](Rule-Reference.md#minus-m-s-) does not have its own implementation, it is built from [`rematch`](Rule-Reference.md#rematch-r-s-), [`not_at`](Rule-Reference.md#not_at-r-) and [`eof`](Rule-Reference.md#eof).

```c++
template< typename M, typename S >
using minus = rematch< M, not_at< S, eof > >;
```

If control functions were to be called for `not_at< S, eof >` and `eof` it would expose the implementation of `minus`.
This is a problem because it makes all changes to `minus` a user-visible breaking change, and, more importantly, it can lead to undesired action application:

If the sub-rules `not_at< S, eof >` and `eof` were to appear elsewhere in the grammar, and have actions attached, these actions would *also* be applied to the sub-rules of `minus`, even though the user never explicitly wrote them down.

The description of the following functions assumes that control functions are enabled since otherwise they would **not** be called.

###### `start`

Empty function.

###### `success`

Empty function.

###### `failure`

Empty function.

###### `guard`

The guard function returns either a [rewind guard](Rules-and-Grammars.md#rewind-guard) or an empty dummy object that does nothing, depending on the current `rewind_mode`.

###### `raise`

The function `normal< R >::raise()` throws an exception of type `parse_error< ParseInput::error_position_t >` where `ParseInput` is the type of the current input.

The message of the exception is either `R::error_message` if it exists, or `"parse error matching"` followed by the demangled name of `R`.
The position in the exception is `in.current_position()` where `in` is the current input of type `ParseInput`.

This function statically asserts that exceptions are enabled.

###### `raise_nested`

This function statically asserts that exceptions are enabled.

###### `apply`

The function `normal< R >::apply()` calls `A< R >::apply()` with an `action_input` and all state objects as arguments assuming that `A` is the current action and actions are enabled.

###### `apply0`

The function `normal< R >::apply0()` calls `A< R >::apply0()` with all state objects assuming that `A` is the current action and actions are enabled.

###### `match`

The normal match function checks whether, for current action `A` and current rule `R`, the class `A< R >` has a static [`match`](Actions-and-States.md#match) function that can be called.
If such a function exists it is called instead of the default which is `tao::pegtl::match()`.


## Changing Control

The control class template is usually supplied to [`parse()`](Inputs-and-Parsing.md#parse-function) or [`parse_nested()`](Inputs-and-Parsing.md#nested-parsing) at the beginning of a parsing run.
The control can also be changed during a parsing run.

### Via Rules

The [`control`](Rule-Reference.md#control-c-r-) combinator parses a sequence of rules just like [`seq`](Rule-Reference.md#seq-r-) but changes the control its first template parameter.

Similarly the following two lines both start parsing `my_grammar` with `my_action` as action and `my_control` as control.
Like any other rule, `control<>` can be used anywhere within a grammar.

```c++
tao::pegtl::parse< my_grammar, my_action, my_control >( ... );
tao::pegtl::parse< tao::pegtl::control< my_control, my_grammar >, my_action >( ... );
```

### Via Actions

The [`change_control`](Action-Reference.md#change_control-c-) action can be used to change the control in a non-intrusive fashion.
For example if the custom action `my_action` is specialized for `my_rule` as follows then parsing `my_grammar` will switch to using `my_control` when starting to match `my_rule`.

```c++
template<>
struct my_action< my_rule >
   : tao::pegtl::change_control< my_control > {};

tao::pegtl::parse< my_grammar, my_action >( ... );
```


## Control Traces

The following traces give insight into when which function is called when during an attempt to match rule `R`.

To keep them readable most template parameters and function arguments have been omitted.
Please consult the appropriate header files if *all* details need to be known.

### Rule Success

Parse `R` success, default Action and Control.

| Function | Event |
| -------- | ----- |
| `tao::pegtl::parse< R >()` | Enter |
| `tao::pegtl::normal< R >::match()` | Enter |
| `tao::pegtl::match< R >()` | Enter |
| `tao::pegtl::normal< R >::guard()` | Full call |
| `tao::pegtl::normal< R >::start()` | Full call |
| `R::match()` | Full call returns `true`  |
| `tao::pegtl::normal< R >::success()` | Full call |
| `tao::pegtl::match< R >()` | Return `true` |
| `tao::pegtl::normal< R >::match()` | Return `true` |
| `tao::pegtl::parse< R >()` | Return `true` |

<details>
<summary>Full Internal Trace</summary>

| Function | Event |
| -------- | ----- |
| `tao::pegtl::parse< R >()` | Enter |
| `tao::pegtl::normal< R >::match()` | Enter |
| `tao::pegtl::match< R >()` | Enter |
| `tao::pegtl::normal< R >::guard()` | Full call returns rewind guard or dummy |
| `tao::petl::internal::rewind_guard::rewind_guard()` | Remember position if not dummy |
| `tao::pegtl::normal< R >::start()` | Full call |
| `tao::pegtl::internal::match_control_unwind< R >()` | Enter and set up unwind guard if needed |
| `tao::pegtl::internal::match_no_control< R >()` | Enter and detect simple or complex match |
| `R::match()` | Full call returns `true`  |
| `tao::pegtl::internal::match_no_control< R >()` | Return `true` |
| `tao::pegtl::internal::match_control_unwind< R >()` | Return `true` |
| `tao::pegtl::normal< R >::success()` | Full call |
| `tao::petl::internal::rewind_guard::~rewind_guard()` | Do nothing or dummy does nothing |
| `tao::pegtl::match< R >()` | Return `true` |
| `tao::pegtl::normal< R >::match()` | Return `true` |
| `tao::pegtl::parse< R >()` | Return `true` |

Whether a rewind guard or a dummy is created depends on the current `rewind_mode`.
</details>

### Rule Local Failure

Parse `R` local failure, default Action and Control.

| Function | Event |
| -------- | ----- |
| `tao::pegtl::parse< R >()` | Enter |
| `tao::pegtl::normal< R >::match()` | Enter |
| `tao::pegtl::match< R >()` | Enter |
| `tao::pegtl::normal< R >::guard()` | Full call |
| `tao::pegtl::normal< R >::start()` | Full call |
| `R::match()` | Full call returns `false`  |
| `tao::pegtl::normal< R >::failure()` | Full call |
| `tao::pegtl::match< R >()` | Return `false` |
| `tao::pegtl::normal< R >::match()` | Return `false` |
| `tao::pegtl::parse< R >()` | Return `false` |

<details>
<summary>Full Internal Trace</summary>

| Function | Event |
| -------- | ----- |
| `tao::pegtl::parse< R >()` | Enter |
| `tao::pegtl::normal< R >::match()` | Enter |
| `tao::pegtl::match< R >()` | Enter |
| `tao::pegtl::normal< R >::guard()` | Full call returns rewind guard or dummy |
| `tao::petl::internal::rewind_guard::rewind_guard()` | Remember position if not dummy |
| `tao::pegtl::normal< R >::start()` | Full call |
| `tao::pegtl::internal::match_control_unwind< R >()` | Enter and set up unwind guard if needed |
| `tao::pegtl::internal::match_no_control< R >()` | Enter and detect simple or complex match |
| `R::match()` | Full call returns `false`  |
| `tao::pegtl::internal::match_no_control< R >()` | Return `false` |
| `tao::pegtl::internal::match_control_unwind< R >()` | Return `false` |
| `tao::pegtl::normal< R >::failure()` | Full call |
| `tao::petl::internal::rewind_guard::~rewind_guard()` | Rewind input if not dummy |
| `tao::pegtl::match< R >()` | Return `false` |
| `tao::pegtl::normal< R >::match()` | Return `false` |
| `tao::pegtl::parse< R >()` | Return `false` |

Whether a rewind guard or a dummy is created depends on the current `rewind_mode`.
</details>

### Action Apply

Parse `R` success, Action `A` has `void apply()` for `R`, default Control.

| Function | Event |
| -------- | ----- |
| `tao::pegtl::parse< R, A >()` | Enter |
| `tao::pegtl::normal< R >::match()` | Enter |
| `tao::pegtl::match< R >()` | Enter |
| `tao::pegtl::normal< R >::guard()` | Full call |
| `tao::pegtl::normal< R >::start()` | Full call |
| `R::match()` | Full call returns `true`  |
| `tao::pegtl::normal< R >::apply()` | Enter |
| `A< R >::apply()` | Full call |
| `tao::pegtl::normal< R >::apply()` | Return |
| `tao::pegtl::normal< R >::success()` | Full call |
| `tao::pegtl::match< R >()` | Return `true` |
| `tao::pegtl::normal< R >::match()` | Return `true` |
| `tao::pegtl::parse< R >()` | Return `true` |

<details>
<summary>Full Internal Trace</summary>

| Function | Event |
| -------- | ----- |
| `tao::pegtl::parse< R, A >()` | Enter |
| `tao::pegtl::normal< R >::match()` | Enter |
| `tao::pegtl::match< R >()` | Enter |
| `tao::pegtl::normal< R >::guard()` | Full call returns rewind guard |
| `tao::petl::internal::rewind_guard::rewind_guard()` | Remember position |
| `tao::pegtl::normal< R >::start()` | Full call |
| `tao::pegtl::internal::match_control_unwind< R >()` | Enter and set up unwind guard if needed |
| `tao::pegtl::internal::match_no_control< R >()` | Enter and detect simple or complex match |
| `R::match()` | Full call returns `true`  |
| `tao::pegtl::internal::match_no_control< R >()` | Return `true` |
| `tao::pegtl::internal::match_control_unwind< R >()` | Return `true` |
| `tao::pegtl::normal< R >::apply()` | Enter |
| `A< R >::apply()` | Full call |
| `tao::pegtl::normal< R >::apply()` | Return |
| `tao::pegtl::normal< R >::success()` | Full call |
| `tao::petl::internal::rewind_guard::~rewind_guard()` | Do nothing or dummy does nothing |
| `tao::pegtl::match< R >()` | Return `true` |
| `tao::pegtl::normal< R >::match()` | Return `true` |
| `tao::pegtl::parse< R >()` | Return `true` |

A rewind guard is created independent of the current `rewind_mode` because the input position at the beginning of the match is needed for the Action invocation.
</details>


---

This page is part of the [PEGTL](https://github.com/taocpp/PEGTL) and its [documentation](README.md).

Copyright (c) 2014-2026 Dr. Colin Hirsch and Daniel Frey<br>
Distributed under the Boost Software License, Version 1.0<br>
See accompanying file [LICENSE_1_0.txt](../LICENSE_1_0.txt) or copy at https://www.boost.org/LICENSE_1_0.txt
