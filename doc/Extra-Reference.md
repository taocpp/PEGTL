# Extra Reference

The reference documentation for extra and deprecated headers.


## Contents

* [Preamble](#preamble)
* [Extras](#extras)
* [Deprecated](#deprecated)
* [Index](#index)


## Preamble

The extra headers contain classes that are experimental or otherwise not ready to be made part of the "official" public interface.

The deprecated headers contains functionality from previous versions that was not deemed useful or replaced by something better.


## Extras

The extras can be found in [`include/tao/pegtl/extra/`](../include/tao/pegtl/extra).

###### [`charconv.hpp`](../include/tao/pegtl/extra/charconv.hpp)

Provides string-to-integer conversion based on [`std::from_chars()`]](https://en.cppreference.com/w/cpp/utility/from_chars.html) from the standard [`<charconv>`](https://en.cppreference.com/w/cpp/header/charconv.html) header.
The classes defined in this header implement both the `match()` function to serve as rule **and** the `apply()` function to serve as action.

Conversion classes for decimal and hexadecimal are provided, other bases can be selected via the appropriate template parameter.

```c++
template< typename Integral, std::uint8_t Base = 10 > struct from_chars_throws;
template< typename Integral, std::uint8_t Base = 10 > struct from_chars_nothrow;

template< typename Integral > struct from_xchars_throws : from_chars_throws< Integral, 16 > {};
template< typename Integral > struct from_xchars_nothrow : from_chars_nothrow< Integral, 16 > {};
```

The `Base` parameter is passed to `std::from_chars()` as fourth function argument unchanged.
It can be anything between 2 and 36, inclusive.

Remember that `std::from_chars()` does **not**  allow a leading `+` and does **not** recognize the `0x` prefix for hexadecimal numbers.

In both roles any additional state arguments are ignored, and `bool` is returned (as is mandatory for `match()` but optional for `apply()`).

The `_throws` vs. `_nothrow_` suffix chooses between failing locally, i.e. returning `false`, when `std::from_chars()` returns `std::errc::result_out_of_range`, or failing globally, i.e. throwing an exception derived from `tao::pegtl::parse_error`.



 1. Success is when a portion of the parse input, or, as action, the whole action input can be parsed to an integer by `std::from_chars()`. The functions return `true`.
 2. Failure to match is when the parse input does not start with an integer, or the action input is not wholly an integer, respectively. The functions return `false`.
 3. Overflow is when the integer being parsed does not fit into the target type. In this case the `_throws` or `nothrow` suffix comes into play and chooses whether to return `false` or throw a `parse_error`.

When `Integral` is `void` both `apply()` and `match()` use the type of their first state argument
Further, `match()` ignores the `apply_mode` and the first state argument must always be of mutable integral type.

###### [`nested_exceptions.hpp`](../include/tao/pegtl/extra/nested_exceptions.hpp)

The `visit_nested` functions call the visitor once for every exception, including the first/outer one.
The visitor is called first with the inner-most exception, the one that was thrown first from furthest down/inside the call stack, and a nesting count corresponding to the total number of exceptions minus 1.
The visitor is called last with the outer-most exception, the one that was caught in the try-catch that is calling `visit_nested()`, and a nesting count of `0`.

For example, when an exception `E` is caught, and `E` has exception `F` as nested exception, then `visit_nested( E, V )` for some visitor `V` will first call `V( F, 1 )` and then `V( E, 0 )`.

Exceptions in an inheritance hierarchy must be listed from more general to more specific in the template parameter list.
For example `visit_nested< std::exception, std::runtime_error >()` and not vice versa.

```c++
template< typename... Exceptions, typename Visitor >
void visit_nested( Visitor&& );

template< typename... Exceptions, typename Visitor >
void visit_nested( const std::exception_ptr&, Visitor&& );

template< typename... Exceptions, typename Exception, typename Visitor >
void visit_nested( const Exception&, Visitor&& );
```

The following convenience functions call `visit_nested()` produce a flattened vector derived from the nested exceptions.
The `flatten_type()` and `flatten_base()` functions return type sliced copies of the exceptions; `flatten_what()` calls `what()` on the `std::exception`s and returns a vector of strings.

```c++
template< typename Exception >
[[nodiscard]] std::vector< Exception > flatten_type( const std::exception_ptr& = std::current_exception() );
template< typename Exception >
[[nodiscard]] std::vector< Exception > flatten_type( const Exception& );

[[nodiscard]] std::vector< parse_error_base > flatten_base( const std::exception_ptr& = std::current_exception() )
[[nodiscard]] std::vector< parse_error_base > flatten_base( const parse_error_base& );

[[nodiscard]] std::vector< std::string > flatten_what( const std::exception_ptr& = std::current_exception() )
[[nodiscard]] std::vector< std::string > flatten_what( const std::exception& );
```

###### [`parse_tree.hpp`](../include/tao/pegtl/extra/parse_tree.hpp)
###### [`parse_tree_to_dot.hpp`](../include/tao/pegtl/extra/parse_tree_to_dot.hpp)

The [parse tree has its own dedicated page](Parse-Tree.md).

###### [`raw_string.hpp`](../include/tao/pegtl/extra/raw_string.hpp)

###### [`record.hpp`](../include/tao/pegtl/extra/record.hpp)

Builds on [`dispatch.hpp`](#dispatchhpp) to build a linear record of a parsing run.
For each successful match of a selected rule the vector contains an entry with a kind of `view_input` to represent the matched input, and the name of the rule that matched.
Successful sub-rules of a failed rule will not show up in the record.

```c++
template< typename ParseInput >
using record_input = input_with_offset< internal::text_view_alias< typename ParseInput::eol_rule, typename ParseInput::data_t, void, void > >;

template< typename ParseInput >
struct record_value
{
   using data_t = typename ParseInput::data_t;
   using position_t = typename ParseInput::error_position_t;

   record_value( const position_t&, const data_t*, const std::size_t, const std::string_view );

   record_input< ParseInput > input;
   std::string_view rule;
};

template< typename ParseInput >
using record_vector = std::vector< record_value< ParseInput > >;

template< typename... Rules >
struct record
{
   using clause = // clauses to produce the record for Rules...

   template< typename Rule >
   using action = // clauses converted to an action

   template< typename Rule,
             template< typename... > class Control = normal,
             typename ParseInput >
   [[nodiscard]] static auto parse( ParseInput&& )
   {
      // parse with action and return a record_vector< ParseInput >
   }
};
```

###### [`type_to_string.hpp`](../include/tao/pegtl/extra/type_to_string.hpp)

Functions to convert compile-time sequences of `char` into a `std::string` or `std::string_view`.

```c++
template< typename T >
[[nodiscard]] std::string type_to_string();

template< char... Cs >
[[nodiscard]] std::string type_to_string();

template< typename T >
[[nodiscard]] constexpr std::string_view type_to_string_view() noexcept;

template< char... Cs >
[[nodiscard]] constexpr std::string_view type_to_string_view() noexcept;
```

The overloads that take a single type can be used with `tao::pegtl::string<>` or any other type that takes a sequence of `char` as template parameters.

###### [`unescape.hpp`](../include/tao/pegtl/extra/unescape.hpp)

A slightly refactored version of the previous unescape header which is now moved to `deprecated/unescape.hpp`.

It defines individual action classes for common unescape operations as well as an action class template that attaches these actions to corresponding grammar rules.

All actions output to their first state (second argument) which must be a mutable `std::string`.

```c++
struct all_of_these_actions
{
   template< typename ActionInput >
   static void apply( const ActionInput& in, std::string& out );
};
```

`unescape_append` is for non-escaped characters that need to just be copied from the input unchanged by calling `out.append( in.string_view() );`

`unescape_char<>` is for single character escape sequences like `\n`.
It takes any number of template parameters of type `char` and has to be attached to a rule of type `(ascii::)one<>` with the same number of template parameters.

During unescape the n-th template parameter of the `one` is mapped to the n-th template parameter of `unescape_char`.

`unescape_byte` is for bytes represented by two hex digits and must be attached to a rule that matches exactly that.

`unescape_unicode<>` is for hexadecimal Unicode code points, excluding surrogates.
The template parameter of type `std::size_t` must match the fixed (!) number of hex digits the rule it is attached to matches, usually 4 or 8.

`json_unescape_unicode` is for sequences of hexadecimal UTF-16 code points with 4 hex digits each.
It unescapes each individual code point while checking for and merging consecutive surrogate pairs as required by RFC 8259.

> [!WARNING]
> The actions do **not** double-check that the matched portion of the input they are called with has the expected size and data!

The action template `unescape<>` is set up with the usual default

```c++
template< typename Rule > unescape< Rule > : nothing< Rule > {};
```

and some useful specializations for some generic rules and some specific rules defined in [`include/tao/pegtl/example/escaped.hpp`](Example-Reference.md#escapedhpp).
Please check out `include/tao/pegtl/extra/unescape.hpp` to see which individual unescape action is attached to which rule by `unescape<>`.


## Deprecated

The deprecated headers can be found in [`include/tao/pegtl/deprecated/`](../include/tao/pegtl/deprecated).
For now.

###### [`alphabet.hpp`](../include/tao/pegtl/deprecated/alphabet.hpp)

###### [`if_then.hpp`](../include/tao/pegtl/deprecated/if_then.hpp)

###### [`integer.hpp`](../include/tao/pegtl/deprecated/integer.hpp)

###### [`rep_one_min_max.hpp`](../include/tao/pegtl/deprecated/rep_one_min_max.hpp)

###### [`rep_string.hpp`](../include/tao/pegtl/deprecated/rep_string.hpp)

###### [`unescape.hpp`](../include/tao/pegtl/deprecated/unescape.hpp)


## Index

* [`alphabet.hpp`](#alphabethpp) <sup>[(deprecated)](#deprecated)</sup>
* [`charconv.hpp`](#charconvhpp) <sup>[(extra)](#extras)</sup>
* [`dispatch.hpp`](#dispatchhpp) <sup>[(extra)](#extras)</sup>
* [`if_then.hpp`](#if_thenhpp) <sup>[(deprecated)](#deprecated)</sup>
* [`integer.hpp`](#integerhpp) <sup>[(deprecated)](#deprecated)</sup>
* [`nested_exceptions.hpp`](#nested_exceptionshpp) <sup>[(extra)](#extras)</sup>
* [`parse_tree.hpp`](#parse_treehpp) <sup>[(extra)](#extras)</sup>
* [`parse_tree_to_dot.hpp`](#parse_tree_to_dothpp) <sup>[(extra)](#extras)</sup>
* [`raw_string.hpp`](#raw_stringhpp) <sup>[(extra)](#extras)</sup>
* [`record.hpp`](#recordhpp) <sup>[(extra)](#extras)</sup>
* [`rep_one_min_max.hpp`](#rep_one_min_maxhpp) <sup>[(deprecated)](#deprecated)</sup>
* [`rep_string.hpp`](#rep_stringhpp) <sup>[(deprecated)](#deprecated)</sup>
* [`type_to_string.hpp`](#type_to_stringhpp) <sup>[(extra)](#extras)</sup>
* [`unescape.hpp`](#unscapehpp) <sup>[(extra)](#extras)</sup>
* [`unescape.hpp`](#unscapehpp-1) <sup>[(deprecated)](#deprecated)</sup>


---

This page is part of the [PEGTL](https://github.com/taocpp/PEGTL) and its [documentation](README.md).

Copyright (c) 2014-2026 Dr. Colin Hirsch and Daniel Frey<br>
Distributed under the Boost Software License, Version 1.0<br>
See accompanying file [LICENSE_1_0.txt](../LICENSE_1_0.txt) or copy at https://www.boost.org/LICENSE_1_0.txt
