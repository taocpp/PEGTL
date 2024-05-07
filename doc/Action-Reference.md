# Action Reference

The reference documention for all action classes and class templates.

### Included

Only `tao/pegtl/nothing.hpp` which defines `nothing` and `maybe_nothing` is automatically included with `tao/pegtl.hpp`.
For all other actions the appropriate header in `tao/pegtl/action/` needs to be included manually.

### Functions



### Namespaces

All action classes and class templates reside in namespace `tao::pegtl`.
This default can be changed via the macro `TAO_PEGTL_NAMESPACE` in `tao/pegtl/config.hpp`.


## Contents

* [Nothing](#nothing)
* [Maybe Nothing](#maybe-nothing)
* [Add Guard](#add-guard)
* [Add State](#add-state)
* [Change Action](#change-action)
* [Change Action and State](#change-action-and-state)
* [Change Action and States](#change-action-and-states)
* [Change Control](#change-control)
* [Change State](#change-state)
* [Change States](#change-states)
* [Control Action](#control-action)
* [Disable Action](#disable-action)
* [Enable Action](#enable-action)
* [Require Apply](#require-apply)
* [Require Apply0](#require-apply0)

### Nothing

An action class template that does nothing, simply a *nop* action.

Serves as default `Action` template parameter to `parse()` and `parse_nested()`.

```c++
template< typename Rule >
struct nothing {};
```

Usually also serves as base class for the default case of custom action class templates to indicate that the default is "no action".

```c++
template< typename Rule >
struct my_action
   : tao::pegtl::nothing< Rule > {};

// ...plus some (partial) specialisations of my_action
// that define static apply() or apply0() functions.
```

### Maybe Nothing

An action class alias defined as `nothing< void >`.
An action class for `Rule` that is not derived from `nothing< Rule >` but intentionally has no `apply()` or `apply0()` must/should (TODO) derive from `maybe_nohting` to signal that the absence of these functions is not an error.

### Add Guard
add_guard

### Add State
add_state

### Change Action

An action class template with a `match()` function that parses the rule to which it is attached with its template parameter as action class template.
In other words, an action version of the [`action`](Rule-Reference.md#action) rule.

Publicly derives from [`maybe_nothing`](#maybe-nothing).
Included via `tao/pegtl/action/change_action.hpp`.

### Change Action and State
change_action_and_state

### Change Action and States
change_action_and_states

### Change Control

An action class template with a `match()` function that parses the rule to which it is attached with its template parameter as control class template.
In other words, an action version of the [`control`](Rule-Reference.md#control) rule.

Publicly derives from [`maybe_nothing`](#maybe-nothing).
Included via `tao/pegtl/action/change_control.hpp`.

### Change State
change_state

### Change States
change_states

### Control Action
control_action

### Disable Action

An action class with a `match()` function that parses the rule to which it is attached with actions disabled.
In other words, an action version of the [`disable`](Rule-Reference.md#disable) rule.

Publicly derives from [`maybe_nothing`](#maybe-nothing).
Included via `tao/pegtl/action/disable_action.hpp`.

### Enable Action

An action class with a `match()` function that parses the rule to which it is attached with actions enabled.
In other words, an action version of the [`enable`](Rule-Reference.md#enable) rule.

Publicly derives from [`maybe_nothing`](#maybe-nothing).
Included via `tao/pegtl/action/enable_action.hpp`.

### Require Apply

The class `require_apply` is an empty class used as tag.
An action class that is publicly derived from `require_apply` must define a static `apply()` function that can be called as action function.

Wins against [`maybe_nothing`](#maybe-nothing) whan a class has both as public base classes.
Included via `tao/pegtl/action/require_apply.hpp`.

### Require Apply0

The class `require_apply0` is an empty class used as tag.
An action class that is publicly derived from `require_apply` must define a static `apply0()` function that can be called as action function.

Wins against [`maybe_nothing`](#maybe-nothing) whan a class has both as public base classes.
Included via `tao/pegtl/action/require_apply0.hpp`.

---

This document is part of the [PEGTL](https://github.com/taocpp/PEGTL).

Copyright (c) 2014-2024 Dr. Colin Hirsch and Daniel Frey<br>
Distributed under the Boost Software License, Version 1.0<br>
See accompanying file [LICENSE_1_0.txt](../LICENSE_1_0.txt) or copy at https://www.boost.org/LICENSE_1_0.txt
