# Actions and States

Parsing, i.e. matching an input with a grammar rule, by itself only indicates whether (a portion of) the input is valid according to the grammar.
In order to do something useful with the input, it is usually necessary to attach user-defined *actions* to one or more rules.
An action is *applied* whenever the rule to which it is attached succeeds.
Applying an action means that an `apply()` or `apply0()` static member function is called.
The first argument to `apply()` is always an object that represents the portion of the input consumed by the successful match of the rule.
An action's `apply()` or `apply0()` can both return either `void` or `bool`.

## Contents

* [Actions](#actions)
  * [Apply](#apply)
  * [Apply0](#apply0)
* [Troubleshooting](#troubleshooting)
  * [Boolean Return](#boolean-return)
  * [Signature Mismatch](#signature-mismatch)
* [States](#states)
* [Action Specialisation](#action-specialisation)
* [Changing Actions](#changing-actions)
* [Changing States](#changing-states)
  * [No Switching](#no-switching)
  * [Intrusive Switching](#intrusive-switching)
  * [External Switching](#external-switching)
* [Match](#match)
* [Legacy Actions](#legacy-actions)

## Actions

Actions are implemented as static member functions called `apply()` or `apply0()` of specialisations of custom class templates (which is not quite as difficult as it sounds).
First an action class template has to be defined:

```c++
template< typename Rule >
struct my_actions {};
```

The above class template does not have `apply()` or `apply0()`, hence it does nothing, an empty action.

To attach an action to `Rule`, this class template has to be specialised for `Rule` and an *appropriate* static member function, either called `apply()` or `apply0()`, has to be implemented.

The PEGTL will auto-detect whether an action, i.e. a specialisation of an action class template, contains an appropriate `apply()` or `apply0()`, and whether it returns `void` or `bool`.
It will fail to compile when both `apply()` and `apply0()` are found.

### Apply

When an action's static member function is called `apply()`, it receives a const-reference to an instance of an input class as first argument.

```c++
template<>
struct my_actions< tao::pegtl::plus< tao::pegtl::digit > >
{
   template< typename Input >
   static void apply( const Input& in, /* all the states */ )
   {
      // Called whenever a call to tao::pegtl::plus< tao::pegtl::digit >
      // in the grammar succeeds. The argument named 'in' represents the
      // matched part of the input.
   }

   // OR ALTERNATIVELY

   template< typename Input >
   static bool apply( const Input& in, /* all the states */ )
   {
      // Called whenever a call to tao::pegtl::plus< tao::pegtl::digit >
      // in the grammar succeeds. The argument named 'in' represents the
      // matched part of the input.
      return // see description for apply0() above
   }
}
```

The exact type of the input class passed to `apply()` is not specified.
It is currently best practice to "template over" the type of the input as shown above.

Actions can then assume that the input provides (at least) the following members.
The `Input` template parameter is set to the class of the input used at the point in the parsing run where the action is applied.

For illustrative purposes, we will assume that the input passed to `apply()` is of type `action_input`.
Any resemblance to real classes is not a coincidence.

```c++
template< typename Input >
class action_input
{
public:
   using input_t = Input;
   using iterator_t = typename Input::iterator_t;

   bool empty() const noexcept;
   std::size_t size() const noexcept;

   const char* begin() const noexcept;  // Non-owning pointer!
   const char* end() const noexcept;  // Non-owning pointer!

   std::string string() const { return std::string( begin(), end() ); }
   std::string_view string_view() const noexcept { return std::string_view( begin(), size() ); }

   char peek_char( const std::size_t offset = 0 ) const noexcept;   // { return begin()[ offset ]; }
   std::uint8_t peek_uint8( const std::size_t offset = 0 ) const noexcept;  // As above with cast.

   pegtl::position position() const noexcept;  // Not efficient with lazy inputs.

   const Input& input() const noexcept;  // The input from the parsing run.

   const iterator_t& iterator() const noexcept;
};
```

Note that the `action_input` does **not** own the data it points to, it belongs to the original input used in the parsing run. Therefore **the validity of the pointed-to data might not extend (much) beyond the call to `apply()`**!

When the original input has tracking mode `eager`, the `iterator_t` returned by `action_input::iterator()` will contain the `byte`, `line` and `byte_in_line` counters corresponding to the beginning of the matched input represented by the `action_input`.

When the original input has tracking mode `lazy`, then `action_input::position()` is not efficient because it calculates the line number etc. by scanning the complete original input from the beginning.

Actions often need to store and/or reference portions of the input for after the parsing run, for example when an abstract syntax tree is generated.
Some of the syntax tree nodes will contain portions of the input, for example for a variable name in a script language that needs to be stored in the syntax tree just as it occurs in the input data.

The **default safe choice** is to copy the matched portions of the input data that are passed to an action by storing a deep copy of the data as `std::string`, as obtained by the input class' `string()` member function, in the data structures built while parsing.

### Apply0

In cases where the matched part of the input is not required, an action can implement `apply0()`.
This allows for some internal optimisations compared to when the action's static member function is called `apply()`.

```c++
template<>
struct my_actions< tao::pegtl::plus< tao::pegtl::alpha > >
{
   static void apply0( /* all the states */ )
   {
      // Called whenever a call to tao::pegtl::plus< tao::pegtl::alpha >
      // in the grammar succeeds.
   }

   // OR ALTERNATIVELY

   static bool apply0( /* all the states */ )
   {
      // Called whenever a call to tao::pegtl::plus< tao::pegtl::alpha >
      // in the grammar succeeds.
      return // see below
   }
}
```

When the return type is `bool`, the action can determine whether matching the rule to which it was attached, and which already returned with success, should be retro-actively considered a (local) failure.
For the overall parsing run, there is no difference between a rule or an attached action returning `false` (but of course the action is not called when the rule already returned `false`).
When an action returns `false`, the PEGTL takes care of rewinding the input to where it was when the rule to which the action was attached started its (successful) match (which is unlike the rules' `match()` static member function that has to take care of rewinding themself).

## Troubleshooting

### Boolean Return

Note that actions returning `bool` are an advanced use case that should be used with caution.
They prevent some internal optimisations, in particular when used with `apply0()`.
They can also have weird effects on the semantics of a parsing run, for example `at< rule >` can succeed for the same input for which `rule` fails when there is a `bool`-action attached to `rule` that returns `false` (remembering that actions are disabled within an `at<>` combinator).

### Signature Mismatch

The presence of a *suitable* `apply()` or `apply0()` is auto-detected, however the code will compile without a call to `apply()` or `apply0()` when the signature of the implemented function is not correct!
In these cases the error message from the compiler can be improved by deriving the action specialisation from either `require_apply` or `require_apply0`, respectively.

```c++
template<>
struct my_actions< my_rule >
  : require_apply0
{
   static void apply0( double )
   {
      // ...
   }
}
```

The presence of one of these base classes will force a call to `apply()` or `apply0()`, respectively, and compilation will fail when such a call is not possible.
The error message should make clear why the call was not possible, usually a type mismatch for one of the state parameters, the `double` in the example above.

Note that deriving from `require_apply` or `require_apply0` is optional and usually only used for troubleshooting.

## States

In most applications, actions need some kind of data or user-defined (parser/action) *state* to operate on.
Given that `apply()` and `apply0()` are static member functions, they are called without an instance of the class they belong to (which would usually fill this role).
Therefore a PEGTL parsing run can be performed with arbitrary many *state* parameters that are

* passed by the user as additional arguments to [`tao::pegtl::parse()`](Inputs-and-Parsing.md#parse-function) that starts a parsing run, and then

* passed by the PEGTL as additional arguments to all actions' `apply()` or `apply0()` static member functions.

In other words, the additional arguments to `apply()` and `apply0()` can be chosen freely, however all actions must accept the same argument list since they are all called with the same arguments by default.

For example, in a practical grammar the example from above might use a second argument to store the matched sequence of digits.

```c++
template<> struct my_actions< tao::pegtl::plus< tao::pegtl::digit > >
{
   template< typename Input >
   static void apply( const Input& in,
                      std::vector< std::string >& digit_strings )
   {
      digit_strings.push_back( in.string() );
   }
}
```

Assuming that `my_grammar` contains `tao::pegtl::plus< tao::pegtl::digit >` as rule, it is possible to use

```c++
const std::string parsed_data = ...;
std::vector< std::string > digit_strings;

tao::pegtl::memory_input in( parsed_data, "data-source-name" );
tao::pegtl::parse< my_grammar, my_actions >( in, digit_strings );
```

to collect all `digit_strings` that were detected by the grammar, i.e. the vector will contain one string for every time that the `tao::pegtl::plus< tao::pegtl::digit >` rule was matched against the input.

Since the `parse()` functions are variadic function templates, an arbitrary sequence of state arguments can be used.

## Action Specialisation

The rule class for which the action class template is specialised *must* exactly match how the rule is defined and referenced in the grammar.
For example given the rule

```c++
struct foo : tao::pegtl::plus< tao::pegtl::one< '*' > > {};
```

an action class template can be specialised for `foo`, or for `tao::pegtl::one< '*' >`, but *not* for `tao::pegtl::plus< tao::pegtl::one< '*' > >`.

(The function is called on class `foo`, which happens to inherit `match()` from `tao::pegtl::plus< tao::pegtl::one< '*' > >`, however base classes are not taken into consideration by the C++ language when choosing a specialisation.)

While it is possible to specialize for `tao::pegtl::one< '*' >` in the above rule, any such specialization would also match any other occurrence in the grammar. It is therefore best practice to *always* specialize for explicitly named top-level rules.

To then use these actions in a parsing run, simply pass them as additional template parameter to one of the parser functions defined in `<tao/pegtl/parse.hpp>`.

```c++
tao::pegtl::parse< my_grammar, my_actions >( ... );
```

## Changing Actions

Within a grammar, the action class template can be changed, enabled or disabled using the `action<>`, `enable<>` and `disable<>` rules.

The following two lines effectively do the same thing, namely parse with `my_grammar` as top-level parsing rule without invoking actions (unless actions are enabled again somewhere within the grammar).

```c++
tao::pegtl::parse< my_grammar >( ... );
tao::pegtl::parse< tao::pegtl::disable< my_grammar >, my_actions >( ... );
```

Similarly the following two lines both start parsing `my_grammar` with `my_actions` (again with the caveat that something might change somewhere in the grammar).

```c++
tao::pegtl::parse< my_grammar, my_actions >( ... );
tao::pegtl::parse< tao::pegtl::action< my_actions, my_grammar > >( ... );
```

In other words, `enable<>` and `disable<>` behave just like `seq<>` but enable or disable the calling of actions, while `action<>` changes the active action class template to a new one, which must be supplied as first template parameter to `action<>`.

Note that `action<>` does *not* implicitly enable actions when they were previously explicitly disabled!

User-defined parsing rules can use `action<>`, `enable<>` and `disable<>` just like any other combinator rules, for example to disable actions in LISP-style comments:

```c++
struct comment
   : tao::pegtl::seq< tao::pegtl::one< '#' >, tao::pegtl::disable< cons_list > > {};
```

This also allows using the same rules multiple times with different actions within a grammar.

## Changing States

Implementing a parser with the PEGTL consists of two main parts.

1. The actual grammar that drives the parser.
2. The states and actions that "do something".

For the second part, there are three distinct styles of how to manage the states and actions in non-trivial parsers.

The **main issue** addressed by the switching styles is the **growing complexity** encountered when a single state argument to a parsing run must perform multiple different tasks, including the management of nested data structures.

The way that this issue is addressed is by providing another tool for performing divide-and-conquer: A large state class with multiple tasks can be divided into

- multiple smaller state classes that each take care of a single issue,
- one or more [control classes](Control-and-Debug.md) that switch between the states,
- using the C++ stack for nested structures (rather than manually managing a stack).

The different styles can also be freely mixed within the same parser.

### No Switching

The "no switching style" consists of having one (or more) state-arguments that are passed to a parsing run and that are the arguments to all actions' `apply()` and `apply0()` static member functions.

### Intrusive Switching

The `state<>` and `action<>` [meta combinators](Rule-Reference.md#meta-rules) can be used to hard-code state and actions switches in the grammar.

In some cases a state object is required for the grammar itself, and in these cases embedding the state-switch into the grammar is recommended.

### External Switching

"External switching" is when the states and/or actions are switched from outside of the grammar by using the action class.

For an example of how to build a generic JSON data structure with the "external switching style" see `src/example/pegtl/json_build.cpp`.

The actual switching actions are defined in `<tao/pegtl/contrib/change_*.hpp>` and can be used as template for custom switching.

## Match

Besides `apply()` and `apply0()`, an action class specialization may also have a `match()` static member function.
The standard control class template `normal` will detect the presence of a suitable function and call this function instead of calling `tao::pegtl::match()`.

```c++
template<>
struct my_actions< tao::pegtl::plus< tao::pegtl::digit > >
{
   template< typename Rule,
             apply_mode A,
             rewind_mode M,
             template< typename... > class Action,
             template< typename... > class Control,
             typename Input,
             typename... States >
   static bool match( Input& in, States&&... st )
   {
      // default implementation:
      return tao::pegtl::match< Rule, A, M, Action, Control >( in, st... );
   }
}
```

## Legacy Actions

See the [section on legacy-style action rules](Rule-Reference.md#action-rules).

Copyright (c) 2014-2019 Dr. Colin Hirsch and Daniel Frey
