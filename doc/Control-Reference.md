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

> [!IMPORTANT]
> The control class adapters that modify the states before forwarding the control functions to a different control will modify the states for all control functions **except** `match()`.
> This is to prevent the modification from being applied multiple times, once for every nested rule invocation.


## Controls

By [default](Introduction.md#namespace-structure) all controls reside in namespace `tao::pegtl`.

###### `input_control`

Input control is a control adapter that forwards *most* control function calls both to another control and the *input*.

It is *very* similar to [`state_control`](#state_control) which performs the second control function calls on the first state.

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
struct my_input
   : actual_input
{
   using actual_input::actual_input;

   // ...or actually implement an actual input.

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
```

Note that, unlike for normal control classes, if `ParseInput< Rule >::enable == true` then `unwind()` is **not** optional.

Regarding the order of calls forwarded by `input_control` the following rules apply.

* For `start()` the other control's function is called before that of the input.
* For `success()`, `failure()`, `raise()`, `raise_nested()` and `unwind()` the input's function is called before that of the other control. The input is allowed to not throw an exception in `raise()` and `raise_nested()` in which case the second call to the other control's function will take place and take care of throwing.
* For `apply()` and `apply0()` the input's function is also called before that of the other control. The input's function is neither expected to nor capable of calling the action's corresponding function, that is taken care of by the other control.

To use `input_control` one must choose the other control, typically `tao::pegtl::normal`, set up an input with the required control functions, and use `tao::pegtl::input_control< other_control >::type` as control template parameter to [the parse function](Inputs-and-Parsing.md#parse-function).

###### `must_if`

The `must_if` control adapter provides a non-intrusive way to selectively make rules of a grammar behave "as if" they were inside of a [`must`](Rule-Reference.md#must-r-) rule.

Whether to transform local errors into global errors for a rule depends on the presence of a custom error message for that rule.
The presence of such an error message can also be enforced for all types `T` for which `raise()` is called.

The declaration of `must_if` is as follows.

```c++
template< typename Errors, template< typename... > class Base = normal, bool RequireMessage = true >
struct must_if;
```

The first template parameter `Errors` contains the template variable with the custom error messages.

```c++
struct errors
{
   template< typename Rule >
   static constexpr const char* message;
};
```

When `message< R >` is equal to `nullptr` for a rule `R` then matching `R` behaves as it would without use of the `must_if`.

When `message< R >` is not the `nullptr` then the `failure()` control function that is called after a local failure of `R` will call `raise()` to produce a global failure, and it will use `message< R >` as message in the `parse_error` exception.

One way to set up the messages is to define a global variable template, specialize it for all rules as required, and reference it from the `message` member variable of `Errors`.

```c++
template< typename > inline constexpr const char* error_message = nullptr;

template<> inline constexpr const char* error_message< some_rule > = "parse error because fibble";
template<> inline constexpr const char* error_message< other_rule > = "parse error because fobble";

struct errors
{
   template< typename Rule >
   static constexpr auto message = error_message< Rule >;
};
```

The second template parameter `Base` is the control to use as fallback when `must_if` has no specific behaviour.
It defaults to `tao::pegtl::normal`, the usual default control.

When the third template parameter `RequireMessage` is set to `true` then `must_if< E, B >::type< T >::raise()` will statically assert `E::message< T > != nullptr` and use that message in the exception.
In the default case of `false` it will fall back to the default message generated by `B< T >::raise()` which by default is `"parse error matching"` followed by the (demangled) rule class name as per `tao::pegtl::normal`.

To use `must_if` one must set up a suitable type for the `Errors` parameters, optionally choose a different base control and/or change `RequireMessage`, and use `tao::pegtl::must_if< ... >::type as control template parameter to [the parse function](Inputs-and-Parsing.md#parse-function).

###### `normal`

The control class template `normal< Rule >` is the control for normal PEGTL behaviour.

It is the default [control](Control-and-Debug.md) for the [`parse()`](Inputs-and-Parsing.md#parse-funciton) and [`parse_nested()`](Inputs-and-Parsing.md#nested-parsing) functions.

###### `remove_first_state`

The control class adapter `remove_first_state< Base >` calls the control functions in `Base` with the first state removed.

###### `remove_last_states`

The control class adapter `remove_last_states< Base, N >` calls the control functions in `Base` with the last `N` states removed.

###### `reverse_states`

The control class adapter `reverse_states< Base >` calls the control functions in `Base` with the states in reverse order.

This control adapter class template is a type alias for `shuffle_states` with a pre-defined shuffle for the reverse.
It is defined in `include/tao/pegtl/control/shuffle_states.hpp`.

###### `rewind_control`

Rewind control is a control adapater that adds rewind control function calls.

More precisely, it implements a `guard()` control function that will call the following additional functions on `Control< Rule >`.

* `prep_rewind()` when a rewind guard is instantiated
* `will_rewind()` just before a rewind guard will rewind
* `wont_rewind()` when a rewind guard will not rewind

These rewind control functions are only called when the `rewind_mode` is `required`.

These rewind control functions are called with the input and all states as arguments.

These rewind control functions are not part of the normal control functions because calling them might incur a run-time overhead even when the functions are inlined and do nothing.

When using `rewind_control` it will take over the `guard()` control function resuling in this control function not being called on the control that is being adapted.

To use `rewind_control` one must choose the other control, typically something derived from `tao::pegtl::normal` that adds the three rewind control functions listed above, and use `tao::pegtl::rewind_control< other_control >::type` as control template parameter to [the parse function](Inputs-and-Parsing.md#parse-function).

###### `rewind_input_control`

This control adapter will add [rewind control functions](#rewind_control) to `input_control` but it won't be implemented until somebody actually needs it.

###### `rewind_state_control`

This control adapter adds [rewind control functions](#rewind_control) to `state_control` but it won't be documented until somebody actually needs it.

###### `rotate_states_left`

The control class adapter `rotate_states_left< Base, N = 1 >` calls the control functions in `Base` with the states rotated left by `N`.

This control adapter class template is a type alias for `shuffle_states` with a pre-defined shuffle for the rotate.
It is defined in `include/tao/pegtl/control/shuffle_states.hpp`.

###### `rotate_states_right`

The control class adapter `rotate_states_right< Base, N = 1 >` calls the control functions in `Base` with the states rotated right by `N`.

This control adapter class template is a type alias for `shuffle_states` with a pre-defined shuffle for the rotate.
It is defined in `include/tao/pegtl/control/shuffle_states.hpp`.

###### `shuffle_states`

The control class adapter `shuffle_states< Base, Shuffle >` calls the control functions in `Base` with the states shuffled according to `Shuffle`.

The `Shuffle` template parameter must be a type that defines a variable template `value`.
The `I`-th shuffled state with `S` total states will be `shuffle< I, S >::value`.

```c++
struct shuffle
{
   template< std::size_t I, std::size_t S >
   static constexpr std::size_t value = /* 0 ... S-1 */
};
```

###### `state_control`

State control is a control adapter that forwards *most* control function calls both to another control and the *first statet*.

It is *very* similar to [`input_control`](#input_control) which performs the second control function calls on the input.

The limitations leading to only "most function calls" being forwarded to both destinations are

1. `raise()` and `raise_nested()` throw an exception which prevents the second call from being made, and
2. `match()` is only forwarded to the other control because there is no other way this would make sense?

The `state_control` will check a static Boolean variable template called `enable` on the state's class to check whether to forward control functions to the state.
The control functions in the state are similar to their normal control functions counterparts, however there are differences in invocation.

1. Normal control functions are `static`, state control functions are non-static member functions.
2. Normal control functions receive the rule they are being called for indirectly via the template parameter to the control class, state control functions receive the rule directly as template parameter themselves.
3. In some cases not all arguments and template parameters are forwarded to the state control functions.

Put differently, the first and second change amount to normal control functions being called as `Control< Rule >::function()` while state control functions are called as `st.function< Rule >()` assuming that `st` is the first state of type `State`.

```c++
struct my_state
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
```

Note that, unlike for normal control classes, if `State< Rule >::enable == true` then `unwind()` is **not** optional.

Regarding the order of calls forwarded by `state_control` the following rules apply.

* For `start()` the other control's function is called before that of the state.
* For `success()`, `failure()`, `raise()`, `raise_nested()` and `unwind()` the state's function is called before that of the other control. The state is allowed to not throw an exception in `raise()` and `raise_nested()` in which case the second call to the other control's function will take place and take care of throwing.
* For `apply()` and `apply0()` the state's function is also called before that of the other control. The state's function is neither expected to nor capable of calling the action's corresponding function, that is taken care of by the other control.

For all control functions, the calls to the first state do not include the first state as explicit function argument (it remains as implicit `this` pointer).
The calls to the adapted control's functions are subject to [`rotate_states_right`](#rotate_states_right) which rotates the state on which the control functions are called from first to last position for all functions except `match()`, as usual.

To use `state_control` one must choose the other control, typically `tao::pegtl::normal`, set up a state with the required control functions, and use `tao::pegtl::state_control< other_control >::type` as control template parameter to [the parse function](Inputs-and-Parsing.md#parse-function).


## Index

* [`input_control`](#input_control) <sup>[(controls)](#controls)</sup>
* [`must_if`](#must_if) <sup>[(controls)](#controls)</sup>
* [`normal`](#normal) <sup>[(controls)](#controls)</sup>
* [`remove_first_state`](#remove_first_state) <sup>[(controls)](#controls)</sup>
* [`remove_last_states`](#remove_last_states) <sup>[(controls)](#controls)</sup>
* [`reverse_states`](#reverse_states) <sup>[(controls)](#controls)</sup>
* [`rewind_control`](#rewind_control) <sup>[(controls)](#controls)</sup>
* [`rewind_input_control`](#rewind_input_control) <sup>[(controls)](#controls)</sup>
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
