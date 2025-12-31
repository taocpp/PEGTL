# Action Reference

The reference documention for all included actions
The actions related to [stream parsing](Stream-Parsing.nd) are [documented here](Stream-Parsing.md#actions).
The actions not considered part of the core library are [documented here](TODO).


## Contents

* [Preamble](#preamble)
* [Actions](#actions)
* [Index](#index)


## Preamble

Only `include/tao/pegtl/nothing.hpp`, which defines `nothing` and `maybe_nothing`, is automatically included with `<tao/pegtl.hpp>`.
For all other actions the appropriate headers from `include/tao/pegtl/action/` need to be included manually.

The action classes shown below are selectively attached in the usual way.
For example we can invoke [`change_action`](#change_action-a-) to switch the parsing run from `my_action` to `other_action` while parsing `some_rule` as follows.

```c++
template< typename Rule >
struct other_action;

template< typename Rule >
struct my_action
   : tao::pegtl::nothing< Rule >
{};

template<>
struct my_action< some_rule >
   : tao::pegtl::change_action< other_action >
{};
```

> [!IMPORTANT]
> Please remember that the current `apply_mode` enables or disables only the `apply()` and `apply0()` action functions.
> When an action has a `match()` function it will be called regardless of the `apply_mode`.

Some actions have a `success()` function that is called when the rule the action was attached to succeeds.
In some cases there is a default implementation that can be replaced with a user-defined function, in other cases there is no default and a user-defined function has to be supplied.

In either case, the procedure to supply a user-defined `success()` is the same.

```c++
template< typename Rule >
struct my_action
   : tao::pegtl::nothing< Rule >
{ /* base definition */ }

struct action_that_calls_success;

template<>
struct my_action< some_rule >
   : action_that_calls_success
{
   template< typename ParseInput,
             typename... States >
   static void success( const ParseInput&, States&&... )
   {
      ...
   }
};
```

Alternatively an intermediate class can be created that inherits from `action_that_calls_success` and implements `success()`, but is not itself attached to a rule.
Then specializations of `my_action` would inherit from that intermediate class.
This approach requires more code but is more modular.

Note that the correct `success()` function will be called by `action_that_calls_success::match()` even though the function is not `virtual` because `action_that_calls_success::match()` does not call `action_that_calls_success::success()`.
Instead, assuming the currently matched rule is `Rule` and the current action class template is `Action`, it calls `Action< Rule >::success()`.

This pattern is used throughout the PEGTL and is the reason for all action and control functions being passed the current action and control classes as template parameters, to make functions in derived classes visible and callable.


## Actions

By [default](Introduction.md#namespace-structure) all action classes and class templates reside in namespace `tao::pegtl`.

###### `add_guard< G >`

* Creates an object of type `G` before parsing the rule `R` it is attached to.
* If `G` can be default-constructed it will be, or
* it will be constructed with the input and all previous states as arguments.
* If actions are enabled and `G` has a `success()` method it will be called with the input and all states if `R` succeeds.
* Included via `include/tao/pegtl/action/add_guard.hpp`.
* This action implements only a `match()` function and therefore:
* Publicly derives from [`maybe_nothing`](#maybe-nothing).

The difference between `add_guard` and `add_state` is that `add_guard` does not add the newly constructed object to the states while parsing the rule it is attached to.

###### `add_state< S >`

* Creates an object of type `S` before parsing the rule `R` it is attached to.
* Adds this new object of type `S` as first state object while parsing `R`.
* If `S` can be default-constructed it will be, or
* it will be constructed with the input and all previous states as arguments.
* If actions are enabled and `S` has a `success()` method it will be called with the input and all states if `R` succeeds.
* Included via `include/tao/pegtl/action/add_state.hpp`.
* This action implements only a `match()` function and therefore:
* Publicly derives from [`maybe_nothing`](#maybe-nothing).

The difference between `add_guard` and `add_state` is that `add_state` adds the newly constructed object as first state while parsing the rule it is attached to.

###### `change_action< A >`

* Parses the rule it is attached to substituting `A` as current action.
* Does **not** change the current `apply_mode`.
* Non-intrusive action equivalent of the [`action`](Rule-Reference.md#action-a-r-) rule.
* Included via `include/tao/pegtl/action/change_action.hpp`.
* This action implements only a `match()` function and therefore:
* Publicly derives from [`maybe_nothing`](#maybe-nothing).

###### `change_action_and_state< A, S >`

* Parses the rule it is attached to substituting `A` as current action.
* Does **not** change the current `apply_mode`.
* Creates an object of type `S` before parsing the rule `R` it is attached to.
* Uses the new object of type `S` as **only** state object while parsing `R`.
* If `S` can be default-constructed it will be, or
* it will be constructed with the input and all previous states as arguments.
* If actions are enabled and `S` has a `success()` method it will be called with the input and all states if `R` succeeds.
* Combines [`change_action`](#change_action-a-) and [`change_state`](#change_state-s-) into a single action.
* Included via `include/tao/pegtl/action/change_action_and_state.hpp`.
* This action implements only a `match()` function and therefore:
* Publicly derives from [`maybe_nothing`](#maybe-nothing).

###### `change_action_and_states< A, S... >`

* Combines [`change_action`](#change_action-a-) and [`change_states`](#change_states-s-) into a single action.
* Parses the rule it is attached to substituting `A` as current action.
* Does **not** change the current `apply_mode`.
* Creates objects of types `S...` before parsing the rule `R` it is attached to.
* Uses the new objects of types `S...` as state objects while parsing `R`.
* The new states are all default-constructed.
* If actions are enabled it calls `A< R >::success()` with the new and old states as arguments in that order.
* There is no default implementation for `success()`, it **must** be user defined.
* Included via `include/tao/pegtl/action/change_action_and_states.hpp`.
* This action implements only a `match()` function and therefore:
* Publicly derives from [`maybe_nothing`](#maybe-nothing).

###### `change_control< C >`

* Parses the rule it is attached to substituting `C` as current control.
* Non-intrusive action equivalent of the [`control`](Rule-Reference.md#control-c-r-) rule.
* Included via `include/tao/pegtl/action/change_control.hpp`.
* This action implements only a `match()` function and therefore:
* Publicly derives from [`maybe_nothing`](#maybe-nothing).

###### `change_state< S >`

* Creates an object of type `S` before parsing the rule `R` it is attached to.
* Uses the new object of type `S` as **only** state object while parsing `R`.
* If `S` can be default-constructed it will be, or
* it will be constructed with the input and all previous states as arguments.
* If actions are enabled and `S` has a `success()` method it will be called with the input and all states if `R` succeeds.
* Included via `include/tao/pegtl/action/change_state.hpp`.
* This action implements only a `match()` function and therefore:
* Publicly derives from [`maybe_nothing`](#maybe-nothing).

###### `change_states< S... >`

* Creates objects of types `S...` before parsing the rule `R` it is attached to.
* Uses the new objects of types `S...` as state objects while parsing `R`.
* The new states are all default-constructed.
* If actions are enabled it calls `A< R >::success()` with the new and old states as arguments in that order.
* There is no default implementation for `success()`, it **must** be user defined.
* Included via `include/tao/pegtl/action/change_states.hpp`.
* This action implements only a `match()` function and therefore:
* Publicly derives from [`maybe_nothing`](#maybe-nothing).

###### `control_action`

* Adds the `start()`, `success()` and `failure()` control functions to the action.
* Implements do-nothing default versionf of these functions.
* Optionally adds the `unwind()` control function to the action.
* Included via `include/tao/pegtl/action/control_action.hpp`.
* This action implements only a `match()` function and therefore:
* Publicly derives from [`maybe_nothing`](#maybe-nothing).

###### `disable_action`

* Parses the rule it is attached to with actions disabled:
* Changes the current `apply_mode` to `apply_mode::disabled`.
* Non-intrusive action equivalent of the [`disable`](Rule-Reference.md#disable) rule.
* Included via `include/tao/pegtl/action/disable_action.hpp`.
* This action implements only a `match()` function and therefore:
* Publicly derives from [`maybe_nothing`](#maybe-nothing).

###### `enable_action`

* Parses the rule it is attached to with actions enabled:
* Changes the current `apply_mode` to `apply_mode::enabled`.
* Non-intrusive acion equivalent of the [`enable`](Rule-Reference.md#enable) rule.
* Included via `include/tao/pegtl/action/enable_action.hpp`.
* This action implements only a `match()` function and therefore:
* Publicly derives from [`maybe_nothing`](#maybe-nothing).

###### `match_typed_state< T >`

* Parses the rule it is attached to with only the state of type `T`.
* Included via `include/tao/pegtl/action/match_typed_state.hpp`.
* This action implements only a `match()` function and therefore:
* Publicly derives from [`maybe_nothing`](#maybe-nothing).
* Related to the [control](Control-and-Debug.md) class [`apply_typed_state`](Control-Reference.md#apply_typed_state).

###### `maybe_nothing`

* A type alias for `tao::pegtl::nothing< void >`.
* Included via `include/tao/pegtl/nothing.hpp` or `include/tao/pegtl.hpp`.

This type (alias) serves as a tag type in two strongly related roles.
When `my_action< void >` has `tao::pegtl::nothing< void >` aka. `maybe_nothing` as public base class then the PEGTL will `static_assert` the presence of a callable `apply()` or `apply0()` function for all action class template specializations that do not also have `maybe_nothing` as public base class.

###### `nothing< R >`

* A "do nothing" action, defined as an empty class template.
* Included via `include/tao/pegtl/nothing.hpp` or `include/tao/pegtl.hpp`.
* The default action for [`parse()`](Inputs-and-Parsing.md#parse-function) and [`parse_nested()`](Inputs-and-Parsing.md#nested-parsing).

```c++
template< typename Rule >
struct nothing
{};
```

It is recommended, though not required, to use `nothing` as base class for the default case of custom action templates.
This enable the static assert explained with [`maybe_nothing`](#maybe_nothing).

```c++
template< typename Rule >
struct my_action
   : tao::pegtl::nothing< Rule >
{};

// ... (partial) specialisations of my_action ...
```

###### `require_apply`

* An empty tag class related to actions, not an action itself.
* Included via `include/tao/pegtl/action/require_apply.hpp`.

When an action (specialization for a rule) has `require_apply` as public base class the the PEGTL statically asserts the presence of a callable `apply()` function in the action.
This check is performed independent of any presence of absence of `maybe_nothing`.

###### `require_apply0`

* An empty tag class related to actions, not an action itself.
* Included via `include/tao/pegtl/action/require_apply0.hpp`.

When an action (specialization for a rule) has `require_apply0` as public base class the the PEGTL statically asserts the presence of a callable `apply0()` function in the action.
This check is performed independent of any presence or absence of `maybe_nothing`.


## Index

* [`add_guard`](#add_guard-g-) <sup>[(actions)](#actions)</sup>
* [`add_state`](#add_state-s-) <sup>[(actions)](#actions)</sup>
* [`change_action`](#change_action-a-) <sup>[(actions)](#actions)</sup>
* [`change_action_and_state`](#change_action_and_state-a-s-) <sup>[(actions)](#actions)</sup>
* [`change_action_and_states`](#change_action_and_states-a-s-) <sup>[(actions)](#actions)</sup>
* [`change_control`](#change_control-c-) <sup>[(actions)](#actions)</sup>
* [`change_state`](#change_state-s-) <sup>[(actions)](#actions)</sup>
* [`change_states`](#change_states-s-) <sup>[(actions)](#actions)</sup>
* [`control_action`](#control_action) <sup>[(actions)](#actions)</sup>
* [`disable_action`](#disable_action) <sup>[(actions)](#actions)</sup>
* [`enable_action`](#enable_action) <sup>[(actions)](#actions)</sup>
* [`match_typed_state`](#match_typed_state-t-) <sup>[(actions)](#actions)</sup>
* [`maybe_nothing`](#maybe_nothing) <sup>[(actions)](#actions)</sup>
* [`nothing`](#nothing-r-) <sup>[(actions)](#actions)</sup>
* [`require_apply`](#require_apply) <sup>[(actions)](#actions)</sup>
* [`require_apply0`](#require_apply0) <sup>[(actions)](#actions)</sup>


---

This page is part of the [PEGTL](https://github.com/taocpp/PEGTL) and its [documentation](README.md).

Copyright (c) 2014-2026 Dr. Colin Hirsch and Daniel Frey<br>
Distributed under the Boost Software License, Version 1.0<br>
See accompanying file [LICENSE_1_0.txt](../LICENSE_1_0.txt) or copy at https://www.boost.org/LICENSE_1_0.txt
