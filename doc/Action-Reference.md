# Action Reference

The reference documention for all included actions
The actions related to [stream parsing](Stream-Parsing.nd) are [documented here](Stream-Parsing.md#actions).
The actions not considered part of the core library are [documented here](TODO).


## Contents

* [Preamble](#preamble)
* [Actions](#actions)
* [Index](#index)


## Preamble

#### Included

Only `tao/pegtl/nothing.hpp` which defines `nothing` and `maybe_nothing` is automatically included with `tao/pegtl.hpp`.
For all other actions the appropriate header in `tao/pegtl/action/` needs to be included manually.

#### Functions



#### Namespaces

All action classes and class templates reside in namespace `tao::pegtl`.
This default can be changed via the macro `TAO_PEGTL_NAMESPACE` in `tao/pegtl/config.hpp`.


## Actions

###### `add_guard`

Publicly derives from [`maybe_nothing`](#maybe-nothing).
Included via `tao/pegtl/action/add_guard.hpp`.

###### `add_state`

Publicly derives from [`maybe_nothing`](#maybe-nothing).
Included via `tao/pegtl/action/add_state.hpp`.

###### `change_action`

An action class template with a `match()` function that parses the rule to which it is attached with its template parameter as action class template.
In other words, an action version of the [`action`](Rule-Reference.md#action) rule.

Publicly derives from [`maybe_nothing`](#maybe-nothing).
Included via `tao/pegtl/action/change_action.hpp`.

###### `change_action_and_state`

###### `change_action_and_states`

###### `change_control`

An action class template with a `match()` function that parses the rule to which it is attached with its template parameter as control class template.
In other words, an action version of the [`control`](Rule-Reference.md#control) rule.

Publicly derives from [`maybe_nothing`](#maybe-nothing).
Included via `tao/pegtl/action/change_control.hpp`.

###### `change_state`

###### `change_states`

###### `control_action`

###### `disable_action`

An action class with a `match()` function that parses the rule to which it is attached with actions disabled.
In other words, an action version of the [`disable`](Rule-Reference.md#disable) rule.

Publicly derives from [`maybe_nothing`](#maybe-nothing).
Included via `tao/pegtl/action/disable_action.hpp`.

###### `enable_action`

An action class with a `match()` function that parses the rule to which it is attached with actions enabled.
In other words, an action version of the [`enable`](Rule-Reference.md#enable) rule.

Publicly derives from [`maybe_nothing`](#maybe-nothing).
Included via `tao/pegtl/action/enable_action.hpp`.

####### `maybe_nothing`

An action class alias defined as `nothing< void >`.
An action class for `Rule` that is not derived from `nothing< Rule >` but intentionally has no `apply()` or `apply0()` must derive from `maybe_nothing` to signal that the absence of these functions is not an error.

This is usually the case for actions that define a `match()` function, and which might, but often don't, have an `apply()` or `apply0()` added later down the inheritance chain.

###### `nothing< R >`

An class template that implements nothing.

Serves as default `Action` template parameter to `parse()` and `parse_nested()`.

```c++
template< typename Rule >
struct nothing {};
```

Also serves as base class for the default case of custom action class templates.

```c++
template< typename Rule >
struct my_action
   : tao::pegtl::nothing< Rule > {};

// ...plus some (partial) specialisations of my_action
// that define static apply() or apply0() functions.
```

When `my_action< Rule >` is publicly derived from `tao::pegtl::nothing< Rule >` it lets the PEGTL know that no `apply()` or `apply0()` should be expected in `my_action< Rule >`.

###### `require_apply`

The class `require_apply` is an empty class used as tag.
An action class that is publicly derived from `require_apply` must define a static `apply()` function that can be called as action function.

Wins against [`maybe_nothing`](#maybe-nothing) whan a class has both as public base classes.
Included via `tao/pegtl/action/require_apply.hpp`.

###### `require_apply0`

The class `require_apply0` is an empty class used as tag.
An action class that is publicly derived from `require_apply0` must define a static `apply0()` function that can be called as action function.

Wins against [`maybe_nothing`](#maybe-nothing) whan a class has both as public base classes.
Included via `tao/pegtl/action/require_apply0.hpp`.


## Index

* [`add_guard`](#add_guard) <sup>[(actions)](#actions)</sup>
* [`add_state`](#add_state) <sup>[(actions)](#actions)</sup>
* [`change_action`](#change_action) <sup>[(actions)](#actions)</sup>
* [`change_action_and_state`](#change_action_and_state) <sup>[(actions)](#actions)</sup>
* [`change_action_and_states`](#change_action_and_states) <sup>[(actions)](#actions)</sup>
* [`change_control`](#change_control) <sup>[(actions)](#actions)</sup>
* [`change_state`](#change_state) <sup>[(actions)](#actions)</sup>
* [`change_states`](#change_states) <sup>[(actions)](#actions)</sup>
* [`control_action`](#control_action) <sup>[(actions)](#actions)</sup>
* [`disable_action`](#disable_action) <sup>[(actions)](#actions)</sup>
* [`enable_action`](#enable_action) <sup>[(actions)](#actions)</sup>
* [`nothing`](#nothing) <sup>[(actions)](#actions)</sup>
* [`maybe_nothing`](#maybe-nothing) <sup>[(actions)](#actions)</sup>
* [`require_apply`](#require-apply) <sup>[(actions)](#actions)</sup>
* [`require_apply0`](#require-apply0) <sup>[(actions)](#actions)</sup>


---

This page is part of the [PEGTL](https://github.com/taocpp/PEGTL) and its [documentation](README.md).

Copyright (c) 2014-2025 Dr. Colin Hirsch and Daniel Frey<br>
Distributed under the Boost Software License, Version 1.0<br>
See accompanying file [LICENSE_1_0.txt](../LICENSE_1_0.txt) or copy at https://www.boost.org/LICENSE_1_0.txt
