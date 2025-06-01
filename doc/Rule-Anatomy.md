# Rule Anatomy

A (parsing) rule is a class (or a class template) with a static match function (and some type aliases).

## Contents

 * [Match Function](#match-function)
 * [Simple Match](#simple-match)
 * [Simple Example](#simple-example)
 * [Complex Match)(#complex-match)
 * [Rewind Guard](#rewind-guard)
 * [Complex Example)(#complex-example)
 * [Type Aliases](#type-aliases)
 * [Exceptions](#exceptions)
 * [Complete Example](#complete-example)

## Match Function

The match function must adhere to either the simple or the complex interface.

The simple interface can often be used for leaf rules, i.e. rules that do not call other rules.

The complex interface is required when calling other rules and/or some static (the apply mode, rewind mode, Action and Control) or dynamic (the State arguments) state of the current parsing run is needed.

All match functions return `bool`, making for *three* possible outcomes:

1. *Success*, when the rule returns `true`.
2. *Local failure*, when the rule returns `false`.
3. *Global failure*, when the rule throws an exception.

Global failures are "global" because, unlike local failures, they do not allow for backtracking -- exceptions (usually) abort the parsing run.

## Simple Match

The simple match function only takes the input as argument and has no further template parameters.

```c++
struct rule_with_simple_match
{
   // Type aliases omitted.

   template< typename ParseInput >
   static bool match( ParseInput& );
};
```

In case of local failure the simple match function **must not** consume input.
The PEGTL requires, but neither verifies nor enforces, that all rules follow this informal protocol.

In case of global failure the PEGTL makes no assumption on whether input was consumed.

We consider it advisable to always template over the type of the input.

## Simple Example

The `even` parsing rule below shows a typical simple `match` function for leaf rules using the simple interface.

```c++
struct even
{
   // Type aliases omitted.

   template< typename ParseInput >
   static bool match( ParseInput& in )
   {
      if( !in.empty() ) {
         if( ( ( *in.current() ) & 1 ) == 0 ) {
            in.template consume< even >( 1 );
            return true;
         }
      }
      return false;
   }
};
```

The first `if` checks whether the Input is not empty, i.e. whether we are not at `eof` and there is a next input object to examine.
To check for at least `n` further objects in the input we would write `if( in.size( n ) >= n )`.
See the section on buffer inputs for why `n` is passed to `in.size()`.

If there is a next input object, the second `if` checks whether it is even.
The function `in.current( const std::size_t n = 0 )` returns a pointer to the n-plus-first next input object.
If the rule only needed to work with characters we could use `if( ( in.peek_char() & 1 ) == 0 )` instead.

If both `if` conditions are satisfied we (a) have a next input object and (b) it is an even number.
The next step is to consume this input object from the input, which advances the pointer returned by `in.current()` and decreases the remaining size (by one).
Returning `true` concludes the successful match.

If either of the `if` conditions fails the match fails and we return `false` to signal local (match) failure.
Since no code path leading to the `return false` consumed any input it is not necessary to rewind to the starting point of the rule match.

In production code something like `static_assert( std::is_integral_v< typename ParseInput::data_t > )` should be added to the beginning of the function to make explicit the types of input objects on which this rule can operate.

## Complex Match

The complex match function has additional template parameters and is called with all State arguments (the additional parameters passed to the top-level parse function).

```c++
struct rule_with_complex_match
{
   // Type aliases omitted.

   template< tao::pegtl::apply_mode A,
             tao::pegtl::rewind_mode M,
             template< typename... > class Action,
             template< typename... > class Control,
             typename ParseInput,
             typename... States >
   static bool match( ParseInput&, States&&... );
};
```

The apply mode can be `action` or `nothing`.

The value shows whether actions are currently enabled or disabled; usually only the Control needs to change its behaviour accordingly.

The rewind mode can be `required` or `optional`.

When it is `required`, a complex match function **must not** consume input on local failure, just like all simple match functions.
When it is `optional`, this requirement is not in effect and any rewinding provisions can be optimized away.

The rewind guard explained below automatically adapts to the rewind mode and makes it easy to write efficient rules that adhere to the required protocol.

The Action and Control are required for calling sub-rules as shown in the [example below](#complex-example).

The States are also used for calling sub-rules, though it is not unheard of for parsing rules themselves make use of them.
The two major reasons for rules to use states are (a) when a rule also does the job of an action, and (b) when a rule requires some dynamic information to determine when to match.

For an example of (a) consider the rule `maximum_rule_with_action` in `include/tao/pegtl/contrib/integer.hpp`.
For an example of (b) consider the grammar in `src/example/pegtl/dynamic_match.cpp`.

## Rewind Guard

The rewind guard is an object created by the Control that takes the rewind mode into account.

If the rewind mode is `rewind_mode::optional` the rewind guard returned by the default Control is a dummy guard that does nothing.

```c++
class dummy_guard
{
public:
   constexpr dummy_guard() noexcept = default;

   dummy_guard( dummy_guard&& ) = delete;
   dummy_guard( const dummy_guard& ) = delete;

   ~dummy_guard() = default;

   void operator=( dummy_guard&& ) = delete;
   void operator=( const dummy_guard& ) = delete;

   [[nodiscard]] constexpr bool operator()( const bool result ) const noexcept
   {
      return result;
   }
};
```

If the rewind mode is `rewind_mode::required` the rewind guard returned by the default Control will perform rewinding when required.
More precisely, it will remember the current state of the input in its constructor, and rewind the input to that state when the destructor is called and `operator()( const bool )` was **not** called with a value of `true`.

To use the rewind guard call the `guard()` function on the Control to create a local object, here `m`, and use `return m( bar )` instead of `return bar` throughout the `match()` function.

```c++
struct foo
{
   // Type aliases omitted.

   template< tao::pegtl::apply_mode A,
             tao::pegtl::rewind_mode M,
             template< typename... > class Action,
             template< typename... > class Control,
             typename ParseInput,
             typename... States >
   static bool match( ParseInput& in, States&&... st )
   {
      auto m = Control< foo >::template guard< A, M, Action, Control >( in, st... );
      // Code with multiple points that can consume
      // and/or where the decision on whether to fail
      // or succeed is made after consuming input.
      return m( result );  // Instead of return result;
   }
};
```

## Complex Example

Consider the implementation of `tao::pegtl::internal::seq` for more than one sub-rule, i.e. when `sizeof...( Rules ) > 1`.

```c++
template< typename... Rules >
struct seq
{
   // Type aliases omitted.

   template< apply_mode A,
             rewind_mode M,
             template< typename... > class Action,
             template< typename... > class Control,
             typename ParseInput,
             typename... States >
   [[nodiscard]] static bool match( ParseInput& in, States&&... st )
   {
      auto m = Control< seq >::template guard< A, M, Action, Control >( in, st... );
      return m( ( Control< Rules >::template match< A, rewind_mode::optional, Action, Control >( in, st... ) && ... ) );
   }
};
```

All template parameters and function arguments provided by the complex interface are needed for the Control functions.

The rewind guard is instantiated just as in the exposition above.
The interesting part is the rewind mode passed to the sub-rules inside of the logical-and fold-expression.

If `M == rewind_mode::optional` then `seq` is allowed to consume on local failure, and therefore the sub-rules are allowed to consume on local failure.

If `M == rewind_mode::required` then the sub-rules are called "under the umbrella" of what will be a non-dummy rewind guard instantiated in the first line of the match function.

In either case the sub-rules themsleves do not need to rewind on local failure, wherefore we can always invoke them with `rewind_mode::optional`.
This allows the sub-rules to optimize away their rewind guards to dummy guards.

## Type Aliases

The grammar analysis (and some other parts) of the PEGTL require rules to define two type aliases.

The aliases `rule_t` and `subs_t` are (most commonly) defined in the rule classes that define the static match function.

The alias `rule_t` (most commonly) is defined as the containing class.

The alias `subs_t` (most commonly) is defined as the meta type list `tao::pegtl::type_list<>` with all sub-rules.

The type alias `tao::pegtl::empty_list` can be used insteaed of the empty type list `tao::pegtl::type_list<>`.

The type aliases for a rule *without* sub-rules are as follows.

```c++
struct rule_without_sub_rules
{
   using rule_t = rule_without_sub_rules;
   using subs_t = tao::pegtl::empty_list;

   // Match function omitted.
};
```

The type aliases for a rule *with* sub-rules are as follows.

```c++
template< typename... Rules >
struct rule_with_sub_rules
{
   using rule_t = rule_with_sub_rules;
   using subs_t = tao::pegtl::type_list< Rules... >;

   // Match function omitted.
};
```

These type aliases are required for the meta-programming-based features of the PEGTL to recover the type and sub-rules of a rule.
The meta-data mapping part of every rule's description in the [Rule Reference](Rule-Reference.md) shows the definition of `rule_t` for all included rules.

## Exceptions

Global failures are exceptions thrown by a rule's match function.

Exceptions thrown by rules can be broadly categorized as either

1. failure to satisfy the condition for a successful match, or
2. anything else, most frequently an I/O error of a buffer input.

In general, neither case requires a rule to directly throw an exception itself.

Any I/O error encountered by the reader of a buffer input will already throw an exception.

Failure to match *should* result in a local error as it *should* be the author of a grammar to decide if and where to switch to global errors with the [must-family of rules](Rule-Reference.md#exceptional).
If, however, a parsing rule must, for some reason, throw an exception itself then it *should* use the `raise` Control function which throws an exception of the appropriate type, namely `tao::pegtl::parse_error< ParseInput::error_t >` assuming `ParseInput` is the type of the input.

## Complete Example

The following excerpt from `src/example/pegtl/dynamic_match.cpp` shows a complex custom rule that uses a state argument.

The grammar parses a *long string literal*, a string literal that can contain any character sequence with neither the necessity nor the possibility of escape sequences, as is common in many scripting languages.
The implementation here adopts the convention that a long string literal begins with `[foo[` and ends with `]foo]` where `foo` is any non-empty string that does not contain a `[`.

We could implement a single custom rule `long_string_literal` to match these strings, however then an action attached to that rule would receive the string literal *including* the delimiters as matched input.
Since the action will probably only be interested in the string literal *excluding* the delimiters we will spread things out to allow an action to attach to the content.

Please note that the following code snippets are not in actual source code order.
First we define rules for the opening sequence of a long string literal.

```c++
namespace dynamic
{
   struct long_literal_id
      : tao::pegtl::plus< tao::pegtl::not_one< '[' > > {};

   struct long_literal_open
      : tao::pegtl::seq< tao::pegtl::one< '[' >,
                         long_literal_id,
                         tao::pegtl::one< '[' > > {};
```

Then we implement an action class with a specialisation for the variable part of the long string literal opening.
The action stores that variable matched string in the State `id`.

```c++
   template< typename Rule >
   struct action {};

   template<>
   struct action< long_literal_id >
   {
      template< typename ActionInput >
      static void apply( const ActionInput& in,
                         std::string& id,
                         const std::string& )
      {
         id = in.string();
      }
   };
```

The rule for the closing sequence is similar to the opening, with closing instead of opening brackets.
This is where we require a custom rule to match the variable part to the content of the state variable `id`.

```c++
   struct long_literal_close
      : tao::pegtl::seq< tao::pegtl::one< ']' >,
                         long_literal_mark,
                         tao::pegtl::one< ']' > > {};
```

The custom rule `long_literal_mark` needs to

1. first check whether the input contains enough bytes to match `id`,
2. then check whether the input bytes match the stored string, and
3. finally call `consume()` on the input when both checks succeed.

```c++
   struct long_literal_mark
   {
      template< tao::pegtl::apply_mode A,
                tao::pegtl::rewind_mode M,
                template< typename... > class Action,
                template< typename... > class Control,
                typename ParseInput >
      static bool match( ParseInput& in,
                         const std::string& id,
                         const std::string& )
      {
         if( in.size( id.size() ) >= id.size() ) {
            if( std::memcmp( in.begin(), id.data(), id.size() ) == 0 ) {
               in.consume< long_literal_mark >( id.size() );
               return true;
            }
         }
         return false;
      }
   };
```

The grammar is completed with another two rules for putting everything together, and an action that stores the body of the long string literal in the state argument `body`.

```c++
   struct long_literal_body
      : tao::pegtl::any {};

   struct grammar
      : tao::pegtl::if_must< long_literal_open,
                             tao::pegtl::until< long_literal_close,
                                                long_literal_body >,
                             tao::pegtl::eof > {};

   template<> struct action< long_literal_body >
   {
      template< typename ActionInput >
      static void apply( const ActionInput& in,
                         const std::string&,
                         std::string& body )
      {
         body += in.string_view();
      }
   };

}  // namespace dynamic
```

Given the main function...

```c++
int main( int argc, char** argv )
{
   for( int i = 1; i < argc; ++i ) {
      std::string id;
      std::string body;

      tao::pegtl::argv_input< tao::pegtl::scan::lf_crlf > in( argv, i );
      if( tao::pegtl::parse< dynamic::grammar, dynamic::action >( in, id, body ) ) {
         std::cout << "long literal id was: " << id << std::endl;
         std::cout << "long literal body was: " << body << std::endl;
      }
      else {
         std::cerr << "parse error for: " << argv[ i ] << std::endl;
         return 1;
      }
   }
   return 0;
}
```

...we can see the grammar in action in the shell:

```sh
$ build/bin/example/pegtl/dynamic_match '[foo["[bla]"]foo]'
long literal id was: foo
long literal body was: "[bla]"

$ build/bin/example/pegtl/dynamic_match '["fraggle"["[foo["]"fraggle"]'
long literal id was: "fraggle"
long literal body was: "[foo["

$ build/bin/example/pegtl/dynamic_match "[===[first line
second line]===]"
long literal id was: ===
long literal body was: first line
second line
```

---

This document is part of the [PEGTL](https://github.com/taocpp/PEGTL).

Copyright (c) 2014-2025 Dr. Colin Hirsch and Daniel Frey<br>
Distributed under the Boost Software License, Version 1.0<br>
See accompanying file [LICENSE_1_0.txt](../LICENSE_1_0.txt) or copy at https://www.boost.org/LICENSE_1_0.txt
