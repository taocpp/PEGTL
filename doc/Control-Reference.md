# Control Reference

The reference documention for all control class templates.


## Contents

* [Preamble](#preamble)
* [Controls](#controls)
* [Index](#index)


## Preamble

Only `include/tao/pegtl/normal.hpp`, which defines `normal`, is automatically included with `<tao/pegtl.hpp>`.
For all other controls the appropriate headers from `include/tao/pegtl/control/` need to be included manually.

The [`normal`](#normal) control is the only one that implements the control functionality -- all other controls are adapters that modify the behavior of another control.

The control adapters are made available through two or three type aliases.
The alias `control_adapter_n< Control >::template type` is itself a control and can be used as `Control` template parameter to [`parse()`](Inputs-and-Parsing.md#parse-function).
The alias `control_adapter_r< Control, Rule >` is easier to use as base class for a (partial) specialization of a control.

```c++
template< typename Base >
using control_adapter_b = internal::...;

template< template< typename... > class Control, typename Rule >
using control_adapter_r = control_adapter_b< Control< Rule > >;

template< template< typename... > class Control >
struct control_adapter_n
{
   template< typename Rule >
   using type = control_adapter_r< Control, Rule >;
};
```

Any control adapter-specific template parameters will be placed before the always-required template parameters shown above.
In cases where it is expected to be useful the `Control` parameter defaults to `normal`.


## Controls

By [default](Introduction.md#namespaces) all controls reside in namespace `tao::pegtl`.

###### [`apply_typed_state`](../include/tao/pegtl/control/apply_typed_state.hpp)

A control adapter that forwards only one single state argument, selected by its type, to the adapted control's `apply()` and `apply0()` functions.

* The parameter `State` is the type selected for `apply()` and `apply0()`.
* All other control functions are taken from the adapted control.
* Included via `include/tao/pegtl/control/apply_typed_state.hpp`.
* Related to the [action](Actions-and-States.md) class [`match_typed_state`](Action-Reference.md#match_typed_state-t-).

```c++
template< typename State, typename Base >
using apply_typed_state_b = internal::...;

template< typename State, template< typename... > class Control, typename Rule >
using apply_typed_state_r;

template< typename State, template< typename...> class Control = normal >
struct apply_typed_state_n
{
   template< typename Rule >
   using type;
};
```

###### [`input_control`](../include/tao/pegtl/control/input_control.hpp)

A control adapter that forwards (most) control functions to both another control *and* [the input](Inputs-and-Parsing.md#input-classes).

* The `match()` function is **only** forwarded to the other control, not the input.
* The `start()` function is first called on the other control and then on the input.
* All other functions are first called on the input and then on the other control.
* If `raise()` or `raise_nested()` on the input throw an exception the corresponding call to the other control will not be made.
* For `apply()` and `apply0()` the input does not receive all information required to perform the action invocation.
* Included via `include/tao/pegtl/control/input_control.hpp`.
* Similar to [`state_control`](#state_control) that forwards to another control and the first state.

```c++
template< template< typename... > class Control, typename Rule >
using input_control_r = internal::...;

template< template< typename... > class Control = normal >
struct input_control_n
{
   template< typename Rule >
   using type = state_control_r< Control, Rule >;
};
```

The `input_control` will make calls to the input's control functions only when the static member variable `template< typename Rule > static constexpr bool enable` is `true` for the current `Rule`.
This is independent of the usual way `other_control< Rule >::enable` is used to decide whether to call regular control functions.

The control functions in the input are similar to their normal control functions counterparts, however there are differences in invocation.

1. Normal control functions are `static`, input control functions are non-static member functions.
2. Normal control functions are invoked as `Control< Rule >::function()` while input control functions as `in.function< Rule >()` assuming that `in` is the current `ParseInput` object.
3. In some cases not all arguments and template parameters are forwarded to the input control functions.

```c++
struct my_input
   : actual_input
{
   using actual_input::actual_input;

   // ...or implement the actual input, too.

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

Note that, unlike for normal control classes, if `ParseInput< Rule >::enable == true` then `unwind()` is **not** optional for the input.

###### [`must_if`](../include/tao/pegtl/control/must_if.hpp)

A control adapter that provides a non-intrusive way to selectively make rules behave "as if" they were inside of a [`must`](Rule-Reference.md#must-r-) rule.

* Uses custom error messages for `raise()` and `raise_nested()`.
* Uses custom error messages to select rules for `must`-like behavior.
* Can enforce custom error message to all calls to `raise()` and `raise_nested()`.
* Modifies `failure()`, `raise()` and `raise_nested()`.
* All other control functions are taken from the adapted control.
* Included via `include/tao/pegtl/control/must_if.hpp`.

```c++
 template< typename Errors, bool RequireMessage, template< typename... > class Control, typename Rule >
using must_if_r = internal::...;

template< typename Errors, bool RequireMessage = true, template< typename... > class Control = normal >
struct must_if_n
{
   template< typename Rule >
   using type = must_if_r< Errors, RequireMessage, Control, Rule >;
};
```

The template parameter `Errors` has to contain the variable with the custom error messages.

```c++
struct errors
{
   template< typename Rule >
   static constexpr const char* message;
};
```

When `Errors::message< R >` is equal to `nullptr` for a rule `R` then matching `R` behaves like it would with `Control` as control.

When `Errors::message< R >` is **not** `nullptr` then a call to `failure()` will make a call to `raise()`, converting the local failure to a global failure using that message in the `parse_error` exception (instead of the default from `normal` which is `"parse error matching "` followed by `demangle< R >()`).

One way to set up the messages is to define a global variable template, specialize it for all rules as required, and reference it from the `message` member variable of the dedicated `Errors` type.

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

When the template parameter `RequireMessage` is set to `true` then `must_if< E, B >::type< T >::raise()` will statically assert `E::message< T > != nullptr` and use that message in the `parse_error` exception.
This case is triggered when `T` is not a parsing rule in the grammar or when `T` occurs inside of `must` or some related combinator.

###### [`normal`](../include/tao/pegtl/normal.hpp)

The [control](Control-and-Normal.md) class (template) `normal` is the control for normal PEGTL behavior.

It is the only control that implements the control functionality -- all other controls are adapters that modify the behavior of another control.

It is also the default [control](Control-and-Normal.md) for the [`parse()`](Inputs-and-Parsing.md#parse-function) and [`parse_nested()`](Inputs-and-Parsing.md#nested-parsing) functions.

###### [`remove_first_state`](../include/tao/pegtl/control/remove_first_state.hpp)

A control adapter that calls most control functions with the first state removed.

* The adapted `match()` function receives all states unchanged.
* All other adapted control functions are called with the first state removed.
* Included via `include/tao/pegtl/control/remove_first_state.hpp`.

```c++
template< typename Base >
using remove_first_state_b = internal::...;

template< template< typename... > class Control, typename Rule >
using remove_first_state_r = remove_first_state_b< Control< Rule > >;

template< template< typename... > class Control >
struct remove_first_state_n
{
   template< typename Rule >
   using type = remove_first_state_r< Control, Rule >;
};
```

###### [`remove_first_states`](../include/tao/pegtl/control/remove_first_states.hpp)

A control adapter that calls most control functions with the first `N` states removed.

* The adapted `match()` function receives all states unchanged.
* All other adapted control functions are called with the first `N` states removed.
* Included via `include/tao/pegtl/control/remove_first_states.hpp`.

```c++
template< std::size_t N, typename Base >
using remove_first_states_b = internal::...;

template< std::size_t N, template< typename... > class Control, typename Rule >
using remove_first_states_r = remove_first_states_b< N, Control< Rule > >;

template< std::size_t N, template< typename... > class Control >
struct remove_first_states_n
{
   template< typename Rule >
   using type = remove_first_states_r< N, Control, Rule >;
};
```

###### [`remove_last_state`](../include/tao/pegtl/control/remove_last_states.hpp)

A control adapter that calls most control functions with the last state removed.

* The adapted `match()` function receives all states unchanged.
* All other adapted control functions are called with the last state removed.
* Included via `include/tao/pegtl/control/remove_last_states.hpp` (not a typo).

```c++
template< typename Base >
using remove_last_state_b = internal::...;

template< template< typename... > class Control, typename Rule >
using remove_last_state_r = remove_last_state_b< Control< Rule > >;

template< template< typename... > class Control >
struct remove_last_state_n
{
   template< typename Rule >
   using type = remove_last_state_r< Control, Rule >;
};
```

###### [`remove_last_states`](../include/tao/pegtl/control/remove_last_states.hpp)

A control adapter that calls most control functions with the last `N` states removed.

* The adapted `match()` function receives all states unchanged.
* All other adapted control functions are called with the last `N` states removed.
* Included via `include/tao/pegtl/control/remove_last_states.hpp`.

```c++
template< std::size_t N, typename Base >
using remove_last_states_b = internal::...;

template< std::size_t N, template< typename... > class Control, typename Rule >
using remove_last_states_r = remove_last_states_b< N, Control< Rule > >;

template< std::size_t N, template< typename... > class Control >
struct remove_last_states_n
{
   template< typename Rule >
   using type = remove_last_states_r< N, Control, Rule >;
};
```

###### [`reverse_states`](../include/tao/pegtl/control/reverse_states.hpp)

A control adapter that calls most control functions with the states in reverse order.

* The adapted `match()` function receives all states unchanged.
* All other adapted control functions are called with the states in reverse order.
* Included via `include/tao/pegtl/control/reverse_states.hpp`.

```c++
template< typename Base >
using reverse_states_b = internal::...;

template< template< typename... > class Control, typename Rule >
using reverse_states_r = reverse_states_b< Control< Rule > >;

template< template< typename... > class Control >
struct reverse_states_n
{
   template< typename Rule >
   using type = reverse_states_r< Control, Rule >;
};
```

###### [`rewind_control`](../include/tao/pegtl/control/rewind_control.hpp)

A control adapater that adds control function calls to monitor input rewinding.

* Wraps the adapted `guard()` control function.
* Calls new rewind control functions on the adapted control,
* but only when `rewind_mode == rewind_mode::required`:
  * `prep_rewind()` when a rewind guard is instantiated,
  * `will_rewind()` just before a rewind guard will rewind,
  * `wont_rewind()` when a rewind guard will not rewind.
* All other control functions are taken from the adapted control.
* Included via `include/tao/pegtl/control/rewind_control.hpp`.

```c++
template< template< typename... > class Control, typename Rule >
using rewind_control_r = internal::...;

template< template< typename... > class Control >
struct rewind_control_n
{
   template< typename Rule >
   using type = rewind_control_r< Control, Rule >;
};
```

The new rewind control functions are not part of the normal control functions because calling them might incur a run-time overhead even when the functions do nothing and are inlined.
These functions are called with the input and all states as arguments.

```c++
template< typename Rule >
struct my_control
{
   template< typename ParseInput, typename... States >
   static void prep_rewind( const ParseInput&, States&&... );

   template< typename ParseInput, typename... States >
   static void will_rewind( const ParseInput&, States&&... );

   template< typename ParseInput, typename... States >
   static void wont_rewind( const ParseInput&, States&&... );
};
```

###### `rewind_input_control`

This control adapter will add [rewind control functions](#rewind_control) to `input_control` but it won't be implemented until somebody actually needs it.

###### `rewind_state_control`

This control adapter adds [rewind control functions](#rewind_control) to `state_control` but it won't be documented until somebody actually needs it.

###### [`rotate_states_left`](../include/tao/pegtl/control/rotate_states_left.hpp)

A control adapter that forwards most control functions with the states rotated left by `N`.

* The adapted `match()` function receives all states unchanged.
* All other adapted control functions are called with the states rotated left by `N`.
* Included via `include/tao/pegtl/control/rotate_states_left.hpp`.

```c++
template< std::size_t N, typename Base >
using rotate_states_left_b = internal::...;

template< std::size_t N, template< typename... > class Control, typename Rule >
using rotate_states_left_r = rotate_states_left_b< N, Control< Rule > >;

template< std::size_t N, template< typename... > class Control >
struct rotate_states_left_n
{
   template< typename Rule >
   using type = rotate_states_left_r< N, Control, Rule >;
};
```

###### [`rotate_states_right`](../include/tao/pegtl/control/rotate_states_right.hpp)

A control adapter that forwards most control functions with the states rotated right by `N`.

* The adapted `match()` function receives all states unchanged.
* All other adapted control functions are called with the states rotated right by `N`.
* Included via `include/tao/pegtl/control/rotate_states_right.hpp`.

```c++
template< std::size_t N, typename Base >
using rotate_states_right_b = internal::...;

template< std::size_t N, template< typename... > class Control, typename Rule >
using rotate_states_right_r = rotate_states_right_b< N, Control< Rule > >;

template< std::size_t N, template< typename... > class Control >
struct rotate_states_right_n
{
   template< typename Rule >
   using type = rotate_states_right_r< N, Control, Rule >;
};
```

###### [`shuffle_states`](../include/tao/pegtl/control/shuffle_states.hpp)

A control adapter that calls most control function with the states in arbitrary order.

* The adapted `match()` function receives all states unchanged.
* All other adapted control functions are called with the states shuffled.
* Included via `include/tao/pegtl/control/rotate_states_right.hpp`.

```c++
template< typename Shuffle, typename Base >
using shuffle_states_b = internal::...;

template< typename Shuffle, template< typename... > class Control, typename Rule >
using shuffle_states_r = shuffle_states_b< Shuffle, Control< Rule > >;

template< typename Shuffle, template< typename...> class Control >
struct shuffle_states_n
{
   template< typename Rule >
   using type = shuffle_states_r< Shuffle, Control, Rule >;
};
```

The template parameter `Shuffle` has to define two variables that determine the number and order of states for the forwarded calls.

```c++
struct shuffle
{
   template< std::size_t S >
   static constexpr std::size_t count = S;  // Or smaller value.

   template< std::size_t I, std::size_t S >
   static constexpr std::size_t value = /* 0 ... S-1 */
};
```

The value of `Shuffle::count< S >` determines how many states are forwarded given an input of `S` states.

The value of `Shuffle::value< I, S >` determines the index of the input state to forward as `I`-th state given an input of `S` states.

The control adapters [`remove_first_state`](#remove_first_state), [`remove_last_state`](#remove_last_state), [`remove_last_states`](#remove_last_states), [`reverse_states`](#reverse_states), [`rotate_states_left`](#rotate_states_left) and [`rotate_states_right`](#rotate_states_right) internally use `shuffle_states`.
Their respective `Shuffle` implementations can be used as examples.

###### [`state_control`](../include/tao/pegtl/control/state_control.hpp)

A control adapter that forwards (most) control functions to both another control *and* [the first state](Actions-and-States.md#states).

* The `match()` function is **only** forwarded to the other control, not the first state.
* The `start()` function is first called on the other control and then on the first state.
* All other functions are first called on the first state and then on the other control.
* If `raise()` or `raise_nested()` on the first state throw an exception the corresponding call to the other control will not be made.
* For `apply()` and `apply0()` the first state does not receive all information required to perform the action invocation.
* Included via `include/tao/pegtl/control/state_control.hpp`.
* Similar to [`input_control`](#input_control) that forwards to another control and the input.

```c++
template< template< typename... > class Control, typename Rule >
using state_control_r = internal::...;

template< template< typename... > class Control = normal >
struct state_control_n
{
   template< typename Rule >
   using type = state_control_r< Control, Rule >;
};
```

The `state_control` will make calls to the first state's control functions only when the static member variable `template< typename Rule > static constexpr bool enable` is `true` for the current `Rule`.
This is independent of the usual way `other_control< Rule >::enable` is used to decide whether to call regular control functions.

The control functions in the input are similar to their normal control functions counterparts, however there are differences in invocation.

1. Normal control functions are `static`, state control functions are non-static member functions.
2. Normal control functions are invoked as `Control< Rule >::function()` while state control functions as `st.function< Rule >()` assuming that `st` is the current first state.
3. In some cases not all arguments and template parameters are forwarded to the state control functions.

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

Note that, unlike for normal control classes, if `ParseInput< Rule >::enable == true` then `unwind()` is **not** optional for the state.


## Index

* [`apply_typed_state`](#apply_typed_state) <sup>[(controls)](#controls)</sup>
* [`input_control`](#input_control) <sup>[(controls)](#controls)</sup>
* [`must_if`](#must_if) <sup>[(controls)](#controls)</sup>
* [`normal`](#normal) <sup>[(controls)](#controls)</sup>
* [`remove_first_state`](#remove_first_state) <sup>[(controls)](#controls)</sup>
* [`remove_last_states`](#remove_last_states) <sup>[(controls)](#controls)</sup>
* [`reverse_states`](#reverse_states) <sup>[(controls)](#controls)</sup>
* [`rewind_control`](#rewind_control) <sup>[(controls)](#controls)</sup>
* [`rewind_input_control`](#rewind_input_control-todo) <sup>[(controls)](#controls)</sup>
* [`rewind_state_control`](#rewind_state_control) <sup>[(controls)](#controls)</sup>
* [`rotate_states_left`](#rotate_states_left) <sup>[(controls)](#controls)</sup>
* [`rotate_states_right`](#rotate_states_right) <sup>[(controls)](#controls)</sup>
* [`shuffle_states`](#shuffle_states) <sup>[(controls)](#controls)</sup>
* [`state_control`](#state_control) <sup>[(controls)](#controls)</sup>


---

This page is part of the [PEGTL](https://github.com/taocpp/PEGTL) and its [documentation](README.md).

Copyright (c) 2014-2026 Dr. Colin Hirsch and Daniel Frey<br>
Distributed under the Boost Software License, Version 1.0<br>
See accompanying file [LICENSE_1_0.txt](../LICENSE_1_0.txt) or copy at https://www.boost.org/LICENSE_1_0.txt
