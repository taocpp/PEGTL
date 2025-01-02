# Control Reference

The reference documention for all control class templates.

### Included

Only `tao/pegtl/normal.hpp` which defines `normal` is automatically included with `tao/pegtl.hpp`.
For all other controls the appropriate header in `tao/pegtl/control/` needs to be included manually.

### Templates

Template parameters to control adapters that are documented as `Base` need to be control classes.

Template parameters to control adapters that are documented as `Control` need to be control class templates.

TODO: Examples? Explanations?

### Forwarding

The control class adapters that modify the states before forwarding the control functions to a different control will modify the states for all control functions **except** `match()` -- otherwise the modification would be applied multiple times for nested rule invocations.

### Namespaces

All control class templates reside in namespace `tao::pegtl`.
This default can be changed via the macro `TAO_PEGTL_NAMESPACE` in `tao/pegtl/config.hpp`.


## Contents

* [Normal](#normal)
* [Input Control](#input-control)
* [Must-If](#must-if)
* [Remove First State](#remove-first-state)
* [Remove Last States](#remove-last-states)
* [Reverse States](#reverse-states)
* [Rewind Control](#rewind-control)
* [Rewind State Control](#rewind-state-control)
* [Rotate States Left](#rotate-states-left)
* [Rotate States Right](#rotate-states-right)
* [Shuffle States](#shuffle-states)
* [State Control](#state-control)

### Normal

The control class template `normal< Rule >` contains the default control functions.

Serves as default `Control` template parameter to `parse()` and `parse_nested()`.

### Input Control

TODO

### Must-If

TODO

### Remove First State

The control class adapter `remove_first_state< Base >` calls the control functions in `Base` with the first state removed.

### Remove Last States

The control class adapter `remove_last_states< Base, N >` calls the control functions in `Base` with the last `N` states removed.

### Reverse States

The control class adapter `reverse_states< Base >` calls the control functions in `Base` with the states in reverse order.

This control adapter class template is implemented via `shuffle_states` and defined in `tao/pegtl/control/shuffle_states.hpp`.

### Rewind Control

The control class adapter `rewind_control< Control >::type< Rule >` adds rewind control functions.

More precisely, it implements a `guard()` control function that will call additional control functions on `Control< Rule >`, namely

* `prep_rewind()` when a rewind guard is instantiated,
* `will_rewind()` just before a rewind guard will rewind,
* `wont_rewind()` when a rewind guard will not rewind.

These rewind control functions are only called when the `rewind_mode` is `required`.

These rewind control functions are called with the input and all states as arguments.

These rewind control functions are not part of the normal control functions because calling them incurs a run-time overhead.

When using `rewind_control< Control >::type` instead of `Control` then `Control< Rule >::guard()` is never called.

### Rewind State Control

TODO

### Rotate States Left

The control class adapter `rotate_states_left< Base, N = 1 >` calls the control functions in `Base` with the states rotated left by `N`.

This control adapter class template is implemented via `shuffle_states` and defined in `tao/pegtl/control/shuffle_states.hpp`.

### Rotate States Right

The control class adapter `rotate_states_right< Base, N = 1 >` calls the control functions in `Base` with the states rotated right by `N`.

This control adapter class template is implemented via `shuffle_states` and defined in `tao/pegtl/control/shuffle_states.hpp`.

### Shuffle States

The control class adapter `shuffle_states< Base, Shuffle >` calls the control functions in `Base` with the states shuffled according to `Shuffle`.

The `Shuffle` template parameter must be a type that defines a variable template `value`.
The `I`-th shuffled state with `S` total states will be `shuffle< I, S >::value`.

```c++
struct shuffle
{
   template< std::size_t I, std::size_t S >
   static constexpr std::size_t value = ...
};
```

### State Control

TODO

---

This document is part of the [PEGTL](https://github.com/taocpp/PEGTL).

Copyright (c) 2014-2025 Dr. Colin Hirsch and Daniel Frey<br>
Distributed under the Boost Software License, Version 1.0<br>
See accompanying file [LICENSE_1_0.txt](../LICENSE_1_0.txt) or copy at https://www.boost.org/LICENSE_1_0.txt
