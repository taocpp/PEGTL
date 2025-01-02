# Rules and Grammars

Writing a PEGTL grammar means implementing custom parsing rules.

Implementing custom parsing rules can be done either by

* combining existing rules into new rules, or

* implementing a new rule from scratch.

## Contents

* [Combining Existing Rules](#combining-existing-rules)
  * [Recursive Rules](#recursive-rules)
  * [S-Expression Grammar](#s-expression-grammar)
* [Implementing New Rules](#implementing-new-rules)
  * [Simple Rules](#simple-rules)
  * [Complex Rules](#complex-rules)

## Combining Existing Rules

Combining existing rules is, by far, the more common way of creating new rules.

Here is an example that shows how existing rules are combined into a new rule.

```c++
using namespace tao::pegtl;

struct integer
   : seq<
        opt< one< '+', '-' > >,  // ('+'/'-')?
        plus< digit >            // digit+
     > {};
```

It defines a new rule named `integer` that is a sequence of two parts, an optional character that can be one of `+` or `-`, followed by a non-empty repetition of a digit.

While it is possible to use a type alias, new rules are usually defined by creating a new type, a `struct` that *inherits*` from a combination of existing rules.
Using a new type and inheritance rather than a type alias has several advantages.

* Some PEGTL debug facilities will ouput the (shorter) type name rather than the (longer) expansion.
* Using new types makes it possible to attach different actions to different "names" of the same rule.

And, fortunately, using inheritance for rule definitions incurs no run-time penalty.

See the [Rule Reference](Rule-Reference.md) for a complete list of all rules and combinators included with the PEGTL.
Some included "unofficial" rules can be found on the [Contrib and Examples](Contrib-and-Examples.md) page.

A *grammar* is nothing else than a collection of related rules.
In theory, as long as a grammar does not contain cycles, complete grammars could be implemented as a single, large rule.
In practice, this is not advisable as it greatly reduces the readability and testability of the grammar, in addition to being quite unmaintainable.

When defining a large set of grammar rules it can be advisable to include a `using namespace tao::pegtl;`-definition at the beginning in order to prevent the frequent repetition of the `tao::pegtl::` namespace qualifier.
This `using`-definition is often combined with the practice of confining a PEGTL grammar to a single translation unit, in which case there is no `namespace`-pollution, and the compile time is kept low by including the PEGTL only in the translation unit with the grammar.

### Recursive Rules

Recursion, or cycles in the grammar, can be implemented after a forward-declaration of one or more rules.

```c++
struct number
   : tao::pegtl::plus< tao::pegtl::digit > {};

struct addition;  // Forward declaration to break the cyclic dependency.

struct bracket
   : tao::pegtl::if_must< tao::pegtl::one< '(' >, addition, tao::pegtl::one< ')' > > {};

struct atomic
   : tao::pegtl::sor< number, bracket > {};

struct addition
   : tao::pegtl::list< atomic, tao::pegtl::one< '+' > > {};
```

### S-Expression Grammar

To give another example of what a small real-world grammar might look like, below is the grammar for a toy-version of S-expressions.
It only supports proper lists, symbols, comments and numbers.
Numbers are non-empty sequences of ASCII digits.

The rule named `file` is the intended top-level rule of the grammar, i.e. the rule that is supplied as template argument to [the `parse()` function](Inputs-and-Parsing.md#parse-function) in order to start a parsing run with this grammar.

```c++
struct hash_comment
   : tao::pegtl::until< tao::pegtl::eolf > {};

struct list;

struct list_comment
   : tao::pegtl::if_must< tao::pegtl::at< tao::pegtl::one< '(' > >, tao::pegtl::disable< list > > {};

struct read_include
   : tao::pegtl::seq< tao::pegtl::one< ' ' >, tao::pegtl::one< '"' >, tao::pegtl::plus< tao::pegtl::not_one< '"' > >, tao::pegtl::one< '"' > > {};

struct hash_include
   : tao::pegtl::if_must< tao::pegtl::string< 'i', 'n', 'c', 'l', 'u', 'd', 'e' >, read_include > {};

struct hashed
   : tao::pegtl::if_must< tao::pegtl::one< '#' >, tao::pegtl::sor< hash_include, list_comment, hash_comment > > {};

struct number
   : tao::pegtl::plus< tao::pegtl::digit > {};

struct symbol
   : tao::pegtl::identifier {};

struct atom
   : tao::pegtl::sor< number, symbol > {};

struct anything;

struct list
   : tao::pegtl::if_must< tao::pegtl::one< '(' >, tao::pegtl::until< tao::pegtl::one< ')' >, anything > > {};

struct normal
   : tao::pegtl::sor< atom, list > {};

struct anything
   : tao::pegtl::sor< tao::pegtl::space, hashed, normal > {};

struct main
   : tao::pegtl::until< tao::pegtl::eof, tao::pegtl::must< anything > > {};
```

In order to let a parsing run do more than verify whether an input conforms to the grammar, it is necessary to attach user-defined *actions* to some grammar rules, as explained in [Actions and States](Actions-and-States.md).

## Implementing New Rules

Sometimes a grammar requires a parsing rule that can not be readily created as combination of the existing rules.
In these cases a custom grammar rule, i.e. a class with a static member function called `match()` that has to adhere to one of two possible interfaces or prototypes, can be implemented from scratch.

When implementing a custom rule class, it is important to remember that the input passed to `match()` represents the *remainder* of the complete input.
At the beginning of a parsing run, the input represents the complete data-to-be-parsed.
During the parsing run, some rules will *consume* the data they matched from the input.
Consuming data from an input advances the pointer to the data that the input's `begin()` member function returns, and decrements the size by the same amount.

The PEGTL makes one **important** assumption about all parsing rules.
If a call to `match()` returns with `false`, then the rule **must not** have consumed input (for [complex rules](#complex-rules): only when the `rewind_mode` is `required`).
For performance reasons this assumption is neither ensured nor verified by the PEGTL.

### Simple Rules

In the simplified rule, `match()` is called with a single argument, the input.
It returns a `bool` to indicate success or (local) failure.
Rules with the simplified interface are called without the states as arguments.

```c++
struct simple_rule
{
   template< typename ParseInput >
   static bool match( ParseInput& in ) { ... }
};
```

Here is an excerpt from the included example program `src/example/pegtl/modulus_match.cpp` that shows a simple custom rule.
The - slightly artificial - rule `my_rule` uses three important `input` functions,

1. first `empty()` to check whether the input is not empty,

2. then `current()` to access the data and check whether the remainder of the first remaining input character `C` happens to satisfy `C % M == R`,

3. and finally `bump()` to consume one `char` from the input if the two above conditions are satisfied.

Note how the return value reflects the result of the checks, and how input is only consumed when the return value is `true`.
The remainder of the program checks that all characters of `argv[ 1 ]` have a value equal to 0 when divided by 3.

```c++
using namespace tao::pegtl;

namespace modulus
{
   template< unsigned M, unsigned R = 0 >
   struct my_rule
   {
      static_assert( M > 1, "Modulus must be greater than 1" );
      static_assert( R < M, "Remainder must be less than modulus" );

      template< typename ParseInput >
      static bool match( ParseInput& in )
      {
         if( !in.empty() ) {
            if( ( ( *in.current() ) % M ) == R ) {
               in.bump( 1 );
               return true;
            }
         }
         return false;
      }
   };

   struct grammar
      : until< eolf, must< my_rule< 3 > > >
   {};

}  // namespace modulus

int main( int argc, char** argv )
{
   if( argc > 1 ) {
      argv_input in( argv, 1 );
      parse< modulus::grammar >( in );
   }
   return 0;
}
```

### Complex Rules

The complex calling convention gives a rule's `match()` member function access to "everything", i.e. some modes, the action- and control class, and all state arguments.
All of these parameters are required for custom rules that need to themselves call other rules for matching.

The signature of `match()` in a complex rule takes the following form.

```c++
struct complex_rule
{
   using rule_t = complex_rule;
   using subs_t = tao::pegtl::empty_list;  // Or tao::pegtl::rule_list< sub_rules_of_complex_rule... >.

   template< tao::pegtl::apply_mode A,
             tao::pegtl::rewind_mode M,
             template< typename... > class Action,
             template< typename... > class Control,
             typename ParseInput,
             typename... States >
   static bool match( ParseInput& in, States&&... )
   { ... }
};
```

#### Modes

The `apply_mode` can take the value `apply_mode::action` or `apply_mode::nothing`, depending on whether actions are currently enabled or disabled.
Most custom parsing rules will either ignore, or pass on the `apply_mode` unchanged; usually only the control interprets the `apply_mode`.

The `rewind_mode` can take the value `rewind_mode::required` or `rewind_mode::optional`.
When `M` is `rewind_mode::required`, the custom rule's `match()`-implementation **must**, on local failure, rewind the input to where it (the input) was when `match()` was called.

When `M` is `rewind_mode::optional` it is not necessary to perform rewinding; either some other rule further up the call stack is already taking care of it or rewinding is not necessary for other reasons.
For example within a `must<>`-rule (which converts local failure, a return value of `false` from `match()`, to global failure, an exception) the `rewind_mode` is `optional`.

The following partial implementation of the `seq`-rule's `match()` function shows how to correctly handle the `rewind_mode`.
The control's `guard()` function uses the `rewind_mode` to choose whether to return an object that performs rewinding or just an empty dummy object.
In either case the calls to the sub-rules' `match()` functions in the return-statement can be performend with the `rewind_mode` being `optional`.
The return value of `match()` is passed through the guard object `m` so that it can rewind the input when necessary, i.e. when `m` is not a dummy and the return value is `false`.

```c++
template< typename... Rules >
struct seq
{
    template< apply_mode A,
              rewind_mode M,
              template< typename... > class Action,
              template< typename... > class Control,
              typename ParseInput,
              typename... States >
    static bool match( ParseInput& in, States&&... st )
    {
       auto m = Control< seq >::template guard< A, M, Action, Control >( in, st... );
       return m( ( Control< Rules >::template match< A, rewind_mode::optional, Action, Control >( in, st... ) && ... ) );
    }
};
```

#### Example

The following excerpt from the included example program `src/example/pegtl/dynamic_match.cpp` shows a complex custom rule that itself makes use of a state argument.
This is necessary to cleanly implement dynamic matching, i.e. where a (set of) string(s) that a rule is intended to match depends on some run-time data structure rather than some compile-time type (the latter of which includes all template arguments).

The aim is to parse a kind of *long string literal*, an arbitrary string literal that does not require escaping of any special characters, as is common in many scripting languages.
In order to allow for arbitrary content without escaping it has to be possible to choose a string sequence that is not part of the string literal as delimiter.

For this example we adopt the convention that a long string literal begins with `"[foo["` and ends with `"]foo]"` where `"foo"` is any non-empty string that does not contain a `"["` (quotation marks always excluded).

Please note that the following code snippets are not in actual source code order.

First we define a rule for the opening of a long string literal as explained above.

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

Then we implement an action class with a specialisation for what is the `"foo"`-part of the long string literal's opening sequence.
The action stores the matched string that corresponds to `"foo"` in a string variable that is passed as state argument.

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

The rule for the closing sequence is similar to the opening, with closing instead of opening brackets, and with a custom rule to check for the `"foo"`-part.

```c++
   struct long_literal_close
      : tao::pegtl::seq< tao::pegtl::one< ']' >,
                         long_literal_mark,
                         tao::pegtl::one< ']' > > {};
```

The custom rule itself

1. first checks whether the input contains enough bytes to match the string stored by the action,

2. then checks whether the input bytes match the stored string, and

3. finally calls `consume()` on the input when both checks succeed.

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

The grammar is completed with another two rules for putting everything together, and an action that stores the body of the long string literal in a second state argument.
In this case the rule `long_literal_body` is redundant, however real-world examples frequently contain a rule like `tao::pegtl::any` multiple times, and so it is necessary to give it another name in order to attach different actions to different uses of the same rule.

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
         body += in.string();
      }
   };

}  // namespace dynamic
```

Given the main function...

```c++
int main( int argc, char* argv[] )
{
   if( argc > 1 ) {
      std::string id;
      std::string body;

      tao::pegtl::argv_input in( argv, 1 );
      tao::pegtl::parse< dynamic::grammar, dynamic::action >( in, id, body );

      std::cout << "long literal id was: " << id << std::endl;
      std::cout << "long literal body was: " << body << std::endl;
   }
   return 0;
}
```

...we can see the grammar in action in the shell:

```sh
$ build/src/example/pegtl/dynamic_match '[foo["[bla]"]foo]'
long literal id was: foo
long literal body was: "[bla]"

$ build/src/example/pegtl/dynamic_match '["fraggle"["[foo["]"fraggle"]'
long literal id was: "fraggle"
long literal body was: "[foo["
```

---

This document is part of the [PEGTL](https://github.com/taocpp/PEGTL).

Copyright (c) 2014-2024 Dr. Colin Hirsch and Daniel Frey<br>
Distributed under the Boost Software License, Version 1.0<br>
See accompanying file [LICENSE_1_0.txt](../LICENSE_1_0.txt) or copy at https://www.boost.org/LICENSE_1_0.txt
