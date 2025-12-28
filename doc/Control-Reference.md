# Control Reference

The reference documention for all control class templates.
The actions not considered part of the core library are [documented here](TODO).


## Contents

* [Preamble](#preamble)
* [Controls](#controls)
* [Index](#index)


## Preamble

Only `include/tao/pegtl/normal.hpp`, which defines `normal`, is automatically included with `<tao/pegtl.hpp>`.
For all other controls the appropriate headers from `include/tao/pegtl/control/` need to be included manually.

By [default](Introduction.md#namespace-structure) all controls reside in namespace `tao::pegtl`.

Template parameters to control adapters that are documented as `Base` need to be control classes.

Template parameters to control adapters that are documented as `Control` need to be control class templates.

TODO: Examples? Explanations?

> [!IMPORTANT]
> The control class adapters that modify the states before forwarding the control functions to a different control will modify the states for all control functions **except** `match()`.
> This is to prevent the modification from being applied multiple times, once for every nested rule invocation.


## Controls

###### `input_control`

Input control is a control adapter that forwards *most* control function calls both to another control and the *input*.

The limitations leading to only "most function calls" being forwarded to both destinations are

1. `raise()` and `raise_nested()` throw an exception which prevents the second call from being made, and
2. `match()` is only forwarded to the other control because there is no other way this would make sense?

The `input_control` will check a static Boolean variable template called `enable` on the input's class to check whether to forward control functions to the input.
The control functions in the input are similar to their normal control functions counterparts, however there are differences in invocation.

1. Normal control functions are `static`, input control functions are non-static member functions.
2. Normal control functions receive the rule they are being called for indirectly via the template parameter to the control class, input control functions receive the rule directly as template parameter themselves.
3. In some cases not all arguments and template parameters are forwarded to the input control functions.

Put differently, the first and second change amount to normal control functions being called as `Control< Rule >::function()` while input control functions are called as `in.function< Rule >()` assuming that `in` is the current `ParseInput` object.

```c++
class my_input
   : actual_input
{
   template< typename Rule >
   static constexpr bool enable = true;  // Or something else...

   template< typename Rule, typename ParseInput, typename... States >
   void start( const ParseInput&, States&&... );

   template< typename Rule, typename ParseInput, typename... States >
   void success( const ParseInput&, States&&... );

   template< typename Rule, typename ParseInput, typename... States >
   void failure( const ParseInput&, States&&... );

   template< typename Rule, typename ParseInput, typename... States >
   void raise( const ParseInput&, States&&... );

   template< typename Rule, typename Ambient, typename ParseInput, typename... States >
   void raise_nested( const Ambient&, const ParseInput&, States&&... );

   template< typename Rule, typename ParseInput, typename... States >
   void unwind( const ParseInput&, States&&... );

   template< typename Rule, typename ParseInput, typename... States >
   void apply( const ParseInput&, States&&... );

   template< typename Rule, typename ParseInput, typename... States >
   void apply0( const ParseInput&, States&&... );
};

Note that, unlike for normal control classes, if `ParseInput< Rule >::enable == true` then `unwind()` is **not** optional.

```

Regarding the order of calls forwarded by `input_control` the following rules apply.

* For `start()` the other control's function is called before that of the input.
* For `success()`, `failure()`, `raise()`, `raise_nested()` and `unwind()` the input's function is called before that of the other control. The input is allowed to not throw an exception in `raise()` and `raise_nested()` in which case the second call to the other control's function will take place and take care of throwing.
* For `apply()` and `apply0()` the input's function is also called before that of the other control. The input's function is neither expected to nor capable of calling the action's corresponding function, that is taken care of by the other control.

TODO: How to actually use it!


###### `must_if`

TODO

###### `normal`

The control class template `normal< Rule >` contains the default control functions.

Serves as default `Control` template parameter to `parse()` and `parse_nested()`.

###### `remove_first_state`

The control class adapter `remove_first_state< Base >` calls the control functions in `Base` with the first state removed.

###### `remove_last_states`

The control class adapter `remove_last_states< Base, N >` calls the control functions in `Base` with the last `N` states removed.

###### `reverse_states`

The control class adapter `reverse_states< Base >` calls the control functions in `Base` with the states in reverse order.

This control adapter class template is implemented via `shuffle_states` and defined in `tao/pegtl/control/shuffle_states.hpp`.

###### `rewind_control`

The control class adapter `rewind_control< Control >::type< Rule >` adds rewind control functions.

More precisely, it implements a `guard()` control function that will call additional control functions on `Control< Rule >`, namely

* `prep_rewind()` when a rewind guard is instantiated,
* `will_rewind()` just before a rewind guard will rewind,
* `wont_rewind()` when a rewind guard will not rewind.

These rewind control functions are only called when the `rewind_mode` is `required`.

These rewind control functions are called with the input and all states as arguments.

These rewind control functions are not part of the normal control functions because calling them incurs a run-time overhead.

When using `rewind_control< Control >::type` instead of `Control` then `Control< Rule >::guard()` is never called.

###### `rewind_state_control`

TODO

###### `rotate_states_left`

The control class adapter `rotate_states_left< Base, N = 1 >` calls the control functions in `Base` with the states rotated left by `N`.

This control adapter class template is implemented via `shuffle_states` and defined in `tao/pegtl/control/shuffle_states.hpp`.

###### `rotate_states_right`

The control class adapter `rotate_states_right< Base, N = 1 >` calls the control functions in `Base` with the states rotated right by `N`.

This control adapter class template is implemented via `shuffle_states` and defined in `tao/pegtl/control/shuffle_states.hpp`.

###### `shuffle_states`

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

###### `state_control`

TODO


## Index

* [`input_control`](#input_control) <sup>[(controls)](#controls)</sup>
* [`must_if`](#must_if) <sup>[(controls)](#controls)</sup>
* [`normal]`(#normal) <sup>[(controls)](#controls)</sup>
* [`remove_first_state`](#remove_first_state) <sup>[(controls)](#controls)</sup>
* [`remove_last_states`](#remove_last_states) <sup>[(controls)](#controls)</sup>
* [`reverse_states`](#reverse_states) <sup>[(controls)](#controls)</sup>
* [`rewind_control`](#rewind_control) <sup>[(controls)](#controls)</sup>
* [`rewind_state_control`](#rewind_state_control) <sup>[(controls)](#controls)</sup>
* [`rotate_states_left`](#rotate_states_left) <sup>[(controls)](#controls)</sup>
* [`rotate_states_right`](#rotate_states_right) <sup>[(controls)](#controls)</sup>
* [`shuffle_states`](#shuffle_states) <sup>[(controls)](#controls)</sup>
* [`state_control`](#state_control) <sup>[(controls)](#controls)</sup>


---

This page is part of the [PEGTL](https://github.com/taocpp/PEGTL) and its [documentation](README.md).

Copyright (c) 2014-2025 Dr. Colin Hirsch and Daniel Frey<br>
Distributed under the Boost Software License, Version 1.0<br>
See accompanying file [LICENSE_1_0.txt](../LICENSE_1_0.txt) or copy at https://www.boost.org/LICENSE_1_0.txt
