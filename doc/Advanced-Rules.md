# Advanced Rules

Sometimes a grammar requires a parsing rule that can not be readily created as combination of the existing rules.
In these cases a custom grammar rule, i.e. a class with a static `match()`-method that has to adhere to one of two possible interfaces or prototypes, can be implemented from scratch.

When implementing a custom rule class, it is important to remember that the input passed to a rules' `match()`-method represents the *remainder* of the complete input.
At the beginning of a parsing run, the input represents the complete data-to-be-parsed.
During the parsing run, many rules *consume* the data that matched from the input.
Consuming data from an input advances the pointer to the data that the input's `begin()`-method returns, and decrements the size by the same amount.

The PEGTL makes one important assumption about all parsing rules; if a call to a `match()`-method does not return with `true` then the rule **must not** have consumed input.
For performance reasons this assumption is neither ensured nor verified by the PEGTL.

## Contents

* [Simple Rules](#simple-rules)
* [Complex Rules](#complex-rules)

## Simple Rules

In the simplified rule, the `match()`-function is called with a single argument, the input.
All rules' `match()`-method return a `bool` to indicate success or (local) failure.
Rules with the simplified interface are called without the states as arguments.

```c++
struct simple_rule
{
   template< typename Input >
   static bool match( Input & in ) { ... }
};
```

Here is an excerpt from the included example program `examples/modulus_match.cc` that shows a simple custom rule.
The - slightly artificial - rule `my_rule` uses three important `input` methods,

1. first `size()` to check whether the input is not empty,

2. then `begin()` to access the data and check whether the remainder of the first remaining input character `C` happens to satisfy `C % M == R`,

3. and finally `bump()` to consume one `char` from the input if the two above conditions are satisfied.

```c++
namespace modulus
{
   template< unsigned M, unsigned R = 0 >
   struct my_rule
   {
      static_assert( M > 1, "Modulus must be greater than 1" );
      static_assert( R < M, "Remainder must be less than modulus" );

      template< typename Input >
      static bool match( Input & in )
      {
         if ( ! in.empty() ) {
            if ( ( ( * in.begin() ) % M ) == R ) {
               in.bump( 1 );
               return true;
            }
         }
         return false;
      }
   };
```

Note how the return value reflects the result of the checks, and how input is only consumed when the return value is `true`.
The remainder of the program checks that all characters of `argv[ 1 ]` are equal to 0 when divided by 3.

```c++
   struct grammar
      : pegtl::until< pegtl::eof, my_rule< 3 > > {};

}  // modulus

int main( int argc, char * argv[] )
{
   if ( argc > 1 ) {
      pegtl::parse_arg< modulus::grammar >( 1, argv );
   }
   return 0;
}
```

This shows how custom rule classes using the simple calling convention are restricted to using template arguments (and global or function-static variables).

## Complex Rules

The complex calling convention gives a rule's `match()`-method access to "everything", i.e. the error mode, the action and control classes, and all state arguments.
All of these parameters are required for custom rules that need to themselves call other rules for matching.

The `match()`-method in a complex rule takes the following form.

```c++
struct complex_rule
{
   // Optional; explained in the section on Grammar Analysis:
   using analyze_t = ...;

   template< pegtl::apply_mode A,
             pegtl::marker_mode M,
             template< typename ... > class Action,
             template< typename ... > class Control,
             typename Input,
             typename ... States >
   static bool match( Input & in, States & ... )
   { ... }
};
```

The following excerpt from the included example program `examples/dynamic_match.cc` shows a complex custom rule that itself makes use of a state argument.
This is necessary to cleanly implement dynamic matching, i.e. where (set of) string(s) that a rule is intended to match depends on some run-time data structure rather than some compile-time type (the latter of which includes all template arguments).

The aim is to parse a kind of *long string literal*, an arbitrary string literal that does not require escaping of any special characters, as is common in many scripting languages.
In order to allow for arbitrary content without escaping it has to be possible to choose a string sequence that is not part of the string literal as delimiter.

For this example we adopt the convention that a long string literal begins with `"[foo["` and ends with `"]foo]"` where `"foo"` is any non-empty string that does not contain a `"["` (quotation marks always excluded).

Please note that the following code snippets are not in actual source code order.

First we define a rule for the opening of a long string literal as explained above.

```c++
namespace dynamic
{
   struct long_literal_open
      : pegtl::seq< pegtl::one< '[' >,
                    pegtl::plus< pegtl::not_one< '[' > >,
                    pegtl::one< '[' > > {};
```

Then we implement an action class with a specialisation for what is the `"foo"`-part of the long string literal's opening sequence.
The action stores the matched string that corresponds to `"foo"` in a string variable that is passed as state argument.

```c++
   template< typename Rule >
   struct action
      : pegtl::nothing< Rule > {};

   template<> struct action< pegtl::plus< pegtl::not_one< '[' > > >
   {
      template< typename Input >
      static void apply( const Input & in,
                         std::string & long_literal_mark,
                         const std::string & )
      {
         long_literal_mark = in.string();
      }
   };
```

The rule for the closing sequence is similar to the opening, with closing instead of opening brackets, and with a custom rule to check for the `"foo"`-part.

```c++
   struct long_literal_close
      : pegtl::seq< pegtl::one< ']' >,
                    long_literal_mark,
                    pegtl::one< ']' > > {};
```

The custom rule itself

1. first checks whether the input contains enough bytes to match the string stored by the action,

2. then checks whether the input bytes match the stored string, and

3. finally calls `bump()` to consume the correct number of bytes from the input when both checks succeed.

```c++
   struct long_literal_mark
   {
      template< pegtl::apply_mode A,
                pegtl::marker_mode M,
                template< typename ... > class Action,
                template< typename ... > class Control
                typename Input >
      static bool match( Input & in,
                         const std::string & long_literal_mark,
                         const std::string & )
      {
         if ( in.size( long_literal_mark.size() ) >= long_literal_mark.size() ) {
            if ( ::memcmp( in.begin(),
                           long_literal_mark.data(),
                           long_literal_mark.size() ) == 0 ) {
               in.bump( long_literal_mark.size() );
               return true;
            }
         }
         return false;
      }
   };
```

The grammar is completed with another two rules for putting everything together, and an action that stores the body of the long string literal in a second state argument.
In this case the rule `long_literal_body` is redundant, however real-world examples frequently contain a rule like `pegtl::any` multiple times, and so it is necessary to give it another name in order to attach different actions to different uses of the same rule.

```c++
   struct long_literal_body
      : pegtl::any {};

   struct grammar
      : pegtl::if_must< long_literal_open,
                        pegtl::until< long_literal_close,
                                      long_literal_body >,
                        pegtl::eof > {};

   template<> struct action< long_literal_body >
   {
      template< typename Input >
      static void apply( const Input & in,
                         const std::string &,
                         std::string & long_literal_body )
      {
         long_literal_body += in.string();
      }
   };

} // dynamic
```

Given the main function...

```c++
int main( int argc, char * argv[] )
{
   if ( argc > 1 ) {
      std::string long_literal_mark;
      std::string long_literal_body;
      pegtl::parse_arg< dynamic::grammar,
                        dynamic::action >( 1, argv
                                           long_literal_mark,
                                           long_literal_body );
      std::cout << "long literal mark was: "
                << long_literal_mark
                << std::endl;
      std::cout << "long literal body was: "
                << long_literal_body
                << std::endl;
   }
   return 0;
}
```

...we can see the grammar in action in the shell:

```sh
$ build/examples/dynamic_match '[foo["[bla]"]foo]'
long literal mark was: foo
long literal body was: "[bla]"

$ build/examples/dynamic_match '["fraggle"["[foo["]"fraggle"]'
long literal mark was: "fraggle"
long literal body was: "[foo["
```

Copyright (c) 2014-2016 Dr. Colin Hirsch and Daniel Frey
