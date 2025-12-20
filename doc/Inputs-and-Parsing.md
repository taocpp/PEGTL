# Inputs and Parsing

**Input data** is a (usually contiguous) sequence of bytes (or other objects) that are intended to be parsed.
An **input** is a class (template) that adheres to an informal interface, the instances of which encapsulate some input data.


## Contents

* [Introduction](#introduction)
* [Input Classes](#input-classes)
  * [Classification](#classification)
  * [Parameters](#parameters)
* [Ends of Lines](#ends-of-lines)
  * [Rule Tracking](#rule-tracking)
  * [Scan Tracking](#scan-tracking)
  * [Lazy Tracking](#lazy-tracking)
* [Parse Function](#parse-function)
* [Nested Parsing](#nested-parsing)
* [Position Classes](#position-classes)
* [Error Reporting](#error-reporting)
* [Input Interface](#input-interface)
  * [Basic Interface](#basic-interface)
  * [Inputs with Start](#inputs-with-start)
  * [Inputs with Lines](#inputs-with-lines)
  * [Inputs with Source](#inputs-with-source)
  * [Input Convenience](#input-convenience)
  * [Stream Compatibility](#stream-compatibility)


## Introduction

Performing a parsing run requires (at least) the following steps.

1. The [rules of the grammar](Rules-and-Grammars.md) have to be implemented.
2. An input that represents the to-be-parsed data needs to be constructed.
3. The [parse function](#parse-function) has to be called with the grammar and the input.

The following steps are also frequently included to do something useful while parsing.

4. An [action class template](Actions-and-States.md) has to be implemented and also passed to the parse function.
5. One or more [state objects](Actions-and-States.md) are instantiated and also passed to the parse function.

More advanced use cases might also pass a control class to the parsing run, or use other functions to start it (the parsing run).

```c++
using namespace tao::pegtl;

class my_state;

// Implementation of required grammar rules:

struct some_rule : ... {};

struct my_grammar : ... {};

template< typename Rule >
struct my_actions
   : tao::pegtl::nothing< Rule >{};

// Specializations of my_actions as required, including:

template<>
struct my_actions< some_rule >
{
   template< class ActionInput >
   static void apply( const ActionInput& in, my_state& state )
   { ... }
};

bool my_parse( const std::filesystem::path& file, my_state& state )
{
   file_input in( file );
   return parse< my_grammar, my_actions >( in, state );
}
```


## Input Classes

The PEGTL includes several [input classes](Input-Reference.md) for parsing memory, standard library containers, and files.
Additionally there are dedicated [stream inputs](Stream-Parsing.md#inputs) for [stream parsing](Stream-Parsing.md).

On closer inspection one will not be suprised to find the inputs to actually be class templates that can be further customized in various ways.
We will first classify the inputs according to the components of their names, explaining what e.g. a `text_mmap_input` is, and then explain their template parameters.

### Classification

The inputs for data in memory are either

- `view` inputs that keep a reference (pointer and size) without taking ownership, or
- `copy` inputs that make a copy of the data in a private container like `std::string`.

The `argv` and `base` inputs are also `view` inputs according to this classification.

The `copy` inputs do not copy anything when they can move from the source during construction.

For all inputs the data **must** reside in a contiguous piece of memory, e.g. a `std::vector` works, a `std::list` does **not**.

The inputs for data in the filesystem are either

- `read` inputs that use `std::fread()` to read the file into memory upon construction, or
- `mmap` inputs that use `::mmap(2)` (or similar) to map the file into the address space, or
- `file` inputs that inherits an `mmap` input when available with a `read` input as fallback.

All of the above inputs are also either

- plain inputs whose position information is "simple", most often a count from the start of the input data, or
- `text` inputs whose position information includes a line and column number based on the `Eol` parameter explained below.

### Parameters

The `typename Eol` template parameter determines which (sequence of) object(s) constitute an end-of-line.

When this parameter is set to `void` the [`eol`](Rule-Reference.md#eol) and [`eolf`](Rule-Reference.md#eolf) rules can not be used, and neither can the `begin_of_line()`, `end_of_file_or_line()` and `line_view_at()` input member functions.

All input classes except for `argv_input` use the default end-of-line rule as default for the `Eol` template parameter.

On Windows the default end-of-line rule is [`tao::pegtl::(ascii::)scan::lf_crlf`](Rule-Reference.md#lf_crlf) which matches both Unix and MS-DOS line endings.

On Unix and Linux, including macOS and Android, the default end-of-line rule is [`tao::pegtl::(ascii::)scan::lf`](Rule-Reference.md#lf) which matches Unix line endings.

For an explanation of the three tracking modes and tables of the included end-of-line rules see the [ends of lines](#ends-of-lines) section below.

The `typename Data` template parameter determines the type of objects in the [input data](Introduction.md#essential-terminology).
For inputs that have this template parameter it defaults to `char` and can be changed, for all others it is hard-wired to `char`.

The `typename Source` template parameter determines the data type of an optional fixed part of the position information.
The name "source" comes from the most frequent use case, the source filename.
For inputs that have this template parameter it defaults to `void`, no source information is stored.
For all filesystem inputs this is hard-wired to `std::filesystem::path` because error messages like "parse error in line 10" *without* the filename have made me want to throw my computer out the window on too many occasions.

Many inputs have two source types, `InputSource` and `ErrorSource`.
In this case the source is stored as type `InputSource` in the input, but as type `ErrorSource` in thrown `parse_error_base`-derived exceptions.
A common use case for the separation is to use `std::string_view` as `InputSource` and `std::string` as `ErrorSource`; cheap view in the input, more expensive copy in the exception to make it self-contained.

The `typename Container` template parameter determines the container in which `copy` inputs keep the input data.
It defaults to `std::string` but can be changed to match an already existing object to enable moving.


## Ends of Lines

There are three ways `text` inputs can use the `Eol` rule to track or calculate the line and column numbers during a parsing run.
Rule and scan tracking are both eager while lazy tracking is lazy.

### Rule Tracking

Rule tracking continuously updates the line and column numbers during a parsing run.

The column number is updated with every successful rule match.
The line number is **only** updated when the [`eol`](Rule-Reference.md#eol) or [`eolf`](Rule-Reference.md#eolf) rules match.

- Low overhead while parsing, but
- care has to be taken to not accidentally match the character(s) (or object(s)) constituting an end-of-line with a rule that is **not** `eol` or `eolf`.
- There are no limitations or requirements for the `Eol` rule.

Rules that enable rule tracking are just any normal rules in the ASCII and Unicode namespaces (`tao::pegtl::ascii`, `tao::pegtl::utf8` etc.)

| Class | ASCII | Unicode |
| ----- | ----- | ------- |
| `...::cr` | [rule](Rule-Reference.md#cr) | [rule](Rule-Reference.md#cr-1) |
| `...:lf` | [rule](Rule-Reference.md#lf) | [rule](Rule-Reference.md#lf-1) |
| `...::crlf` | [rule](Rule-Reference.md#crlf) | [rule](Rule-Reference.md#crlf-1) |
| `...::cr_lf` | [rule](Rule-Reference.md#cr_lf) | [rule](Rule-Reference.md#cr_lf-1) |
| `...::cr_crlf` | [rule](Rule-Reference.md#cr_crlf) | [rule](Rule-Reference.md#cr_crlf-1) |
| `...::lf_crlf` | [rule](Rule-Reference.md#lf_crlf) | [rule](Rule-Reference.md#lf_crlf-1) |
| `...::cr_lf_crlf` | [rule](Rule-Reference.md#cr_lf_crlf) | [rule](Rule-Reference.md#cr_lf_crlf-1) |
| `...::ls` | - | [Unicode](Rule-Reference.md#ls) |
| `...::nel` | - | [Unicode](Rule-Reference.md#nel) |
| `...::ps` | - | [Unicode](Rule-Reference.md#ps) |
| `...::eol1` | - | [Unicode](Rule-Reference.md#eol1) |
| `...::eolu` | - | [Unicode](Rule-Reference.md#eolu) |

The table shows the most commonly used end-of-line rules, however anything (outside of the `scan` and `lazy` sub-namespaces) can be used.

This tracking mode was introduced in PEGTL 4.0.

### Scan Tracking

Scan tracking also continuously updates the line and column numbers during a parsing run.

After every successful rule match the matched portion of the input is scanned for any occurrences of an end-of-line.

- Some overhead while parsing, and
- only works for end-of-line rules with a designated code point that signifies end-of-line, but
- not a problem if a rule distinct from [`eol`](Rule-Reference.md#eol) and [`eolf`](Rule-Reference.md#eolf) matches an end-of-line.

Rules that enable scan tracking can be found in the `scan` sub-namespace of the ASCII and Unicode namespaces (`tao::pegtl::ascii`, `tao::pegtl::utf8` etc.)

| Class | ASCII | Unicode |
| ----- | ----- | ------- |
| `...::scan::cr` | [rule](Rule-Reference.md#cr) | [rule](Rule-Reference.md#cr-1) |
| `...::scan::lf` | [rule](Rule-Reference.md#lf) | [rule](Rule-Reference.md#lf-1) |
| `...::scan::lf_crlf` | [rule](Rule-Reference.md#lf_crlf) | [rule](Rule-Reference.md#lf_crlf-1) |
| `...::scan::ls` | - | [Unicode](Rule-Reference.md#ls) |
| `...::scan::nel` | - | [Unicode](Rule-Reference.md#nel) |
| `...::scan::ps` | - | [Unicode](Rule-Reference.md#ps) |

This tracking mode corresponds to the eager tracking in PEGTL versions prior to 4.0.

Note that the scan is skipped if it can be statically proven that the matched input does not contain an end-of-line, for example in the case of a `tao::pegtl::(ascii::)string` where all characters in the string are **not** the designated end-of-line character.

### Lazy Tracking

Lazy tracking does not continuously update the line and column numbers during a parsing run.

The position information is calculated on demand, i.e. when `current_position()` or `previous_position()` are called on the input.
In that case an eol scan is performed on the input data from the start to the point for which position information was requested.

- Zero overhead while parsing, but
- linear complexity in size of input data to calculate line and column number on demand.
- Not a problem if a rule distinct from [`eol`](Rule-Reference.md#eol) and [`eolf`](Rule-Reference.md#eolf) matches an end-of-line.
- There are no limitations or requirements for the `Eol` rule, except:
- it needs to define an appropriate`eol_lazy_peek` type alias.

This type alias is used by the eol scan to skip to the next place in the input data at which to attempt an `Eol` match.

Rules that enable lazy tracking can be found in the `lazy` sub-namespace of the ASCII and Unicode namespaces (`tao::pegtl::ascii`, `tao::pegtl::utf8` etc.)

| Class | ASCII | Unicode |
| ----- | ----- | ------- |
| `...::lazy::cr` | [rule](Rule-Reference.md#cr) | [rule](Rule-Reference.md#cr-1) |
| `...::lazy::lf` | [rule](Rule-Reference.md#lf) | [rule](Rule-Reference.md#lf-1) |
| `...::lazy::crlf` | [rule](Rule-Reference.md#crlf) | [rule](Rule-Reference.md#crlf-1) |
| `...::lazy::cr_lf` | [rule](Rule-Reference.md#cr_lf) | [rule](Rule-Reference.md#cr_lf-1) |
| `...::lazy::cr_crlf` | [rule](Rule-Reference.md#cr_crlf) | [rule](Rule-Reference.md#cr_crlf-1) |
| `...::lazy::lf_crlf` | [rule](Rule-Reference.md#lf_crlf) | [rule](Rule-Reference.md#lf_crlf-1) |
| `...::lazy::cr_lf_crlf` | [rule](Rule-Reference.md#cr_lf_crlf) | [rule](Rule-Reference.md#cr_lf_crlf-1) |
| `...::lazy::ls` | - | [Unicode](Rule-Reference.md#ls) |
| `...::lazy::nel` | - | [Unicode](Rule-Reference.md#nel) |
| `...::lazy::ps` | - | [Unicode](Rule-Reference.md#ps) |
| `...::lazy::eolu` | - | [Unicode](Rule-Reference.md#eolu) |

This tracking mode was extended in PEGTL 4.0 -- previously lazy tracking had the same restrictions as scan tracking.

Note that the case of simple end-of-line rules, i.e. `tao::pegtl::(ascii::)one< N >` for a single `N` uses a slightly more optimized scan.


## Parse Function

The `parse()` function is the single most important user-facing function, it starts a [parsing run](Introduction.md#essential-terminology).

```c++
template< typename Rule,
          template< typename... > class Action = nothing,
          template< typename... > class Control = normal,
          apply_mode A = apply_mode::enabled,
          rewind_mode M = rewind_mode::dontcare,
          typename ParseInput,
          typename... States >
bool parse( ParseInput& in,
            States&&... st );
```

- The [`Rule` class](Rules-and-Grammars.md) represents the top-level parsing rule of the grammar and is mandatory.
- The [`Action<>` class template](Actions-and-States.md) defaults to an action that does nothing. It is required to pass a user-defined action for a parsing run to do more, e.g. build some data structure, than validate an input against the grammar.
- The [`Control<>` class template](Control-and-Debug.md) defaults to the normal control class that implements the expected and documented behaviour. It can be changed for debugging, e.g. printing all rule match attempts and their outcomes, and for some other advanced use cases, e.g. gathering rule invocation statistics.
- The [`States`](Actions-and-States.md#changing-states) are the types of the additional state objects `st` that are passed to all rules' `match()` functions, all actions' `apply()` and `apply0()` functions, and all control functions. What is needed here depends on what the actions (and control functions) expect.
- The `apply_mode` defaults to `apply_mode::enabled` which enables actions. Can be changed to `rewind_mode::disabled` or in the grammar with the [`enable`](Rule-Reference.md#enable-r-) and [`disable`](Rule-Reference.md#disable-r-) rules.
- The `rewind_mode` defaults to `rewind_mode::dontcare` in which case the input might not be rewound to its start when `parse()` returns `false`. Rewinding can be enabled by passing `rewind_mode::required`.

A parsing run can have the [same three outcomes](Rules-and-Grammars.md#match-function) as the match function of a rule.
Note that the terminology "local" and "global" does not make too much sense at top-level, however for consistency sake we keep these terms here, too.

- *success*, a return value of `true`,
- *local failure*, a return value of `false`,
- *global failure*, an exception of type `tao::pegtl::parse_error`, or also
- any other exception thrown during a parsing run.


## Nested Parsing

Nested parsing refers to an (inner) parsing run that is performed during another (outer) parsing run, for example when a file being parsed includes another file.

When an exception is thrown within a nested parsing run it will be caught by `tao::pegtl::parse_nested()` and a new exception thrown via `Control< Rule >::raise_nested()`.
The new exception contains a position from the argument of type `OuterInput` and the previous exception as nested exception.

The functions in the header `tao/pegtl/contrib/nested_exceptions.hpp` can be used to work with these nested exceptions.
The inner-most exception that was thrown first will be the "most nested" exception, i.e. the final one in the linked list of nested exceptions.

The position information contained in the nested exceptions allows for error messages like "error in file F1 line L1 included from file F2 line L2 etc."

Calling `parse_nested()` requires the input from the outer parsing run, or the position whithin the outer parsing run, as additional first argument ("additional" as compared to `parse()`).

```c++
template< typename Rule,
          template< typename... > class Action = nothing,
          template< typename... > class Control = normal,
          apply_mode A = apply_mode::enabled,
          rewind_mode M = rewind_mode::dontcare,
          typename OuterInput,
          typename ParseInput,
          typename... States >
bool parse_nested( const OuterInput& oi,
                   ParseInput& in,
                   States&&... st );
```

The `OuterInput` will usually be the input from the outer parsing run; it can also be the position obtained from that input.
More precisely, if `oi.current_position()` is **not** callable then `oi` is assumed to be a position itself, otherwise it is called to obtain a position.


## Position Classes

Positions occur as return type of the input functions `current_position()` and `previous_position()`, and as template parameter, and therefore position object, in their parse errors which are instances of `tao::pegtl::parse_error<>`.

All `text` inputs use `tao::pegtl::text_position` for their position reporting; when the source parameters are not `void` the type is `tao::pegtl::position_with_source< SourceType, tao::pegtl::text_position >`.
For all filesystem inputs the `SourceType` is `std::filesystem::path`, for all other inputs it defaults to `void`.

Most non-`text` inputs use `tao::pegtl::count_position` for their position reporting; when the source parameters are not `void` the type is `tao::pegtl::position_with_source< SourceType, tao::pegtl::count_position >`.
The exception are the `base` inputs which are so basic that they neither keep track nor can compute the number of objects from the start of the input data, they use `tao::pegtl::pointer_position` instead.
For all filesystem inputs the `SourceType` is `std::filesystem::path`, for all other inputs it defaults to `void` except for the `argv` input which defaults to `std::string`.


## Error Reporting

When reporting an error, one often wants to print the complete line from the input where the error occurred and a marker at the position where the error is found within that line.
To support this, all [inputs with lines](#inputs-with-lines) make available a [convenience](#input-convenience) function `line_view_at()` that returns a `std::string_view` for the line containing the position passed to it.

```c++
some_input in( ... );
try {
   tao::pegtl::parse< ... >( in, ... );
}
catch( const decltype( in )::parse_error_t& e ) {
   const auto& p = e.position_object();
   std::cerr << e.what() << '\n'
             << in.line_view_at( in, p ) << '\n'
             << std::setw( p.column ) << '^' << std::endl;
}
catch( const parse_error_base& e ) {
   std::cerr << e.what() << std::endl;
}
```

Please note that the character indicated by the caret will only be correct if the input data is restricted to graphical ASCII characters plus the end-of-line character(s).


## Input Interface

All input classes adhere to an informally defined interface of which some parts are optional.
Some rules or other facilities will not function when the optional interface parts they rely on are not present.

### Basic Interface

The basic interface implemented by all inputs.

```c++
   using namespace tao::pegtl;

   // Type aliases:

   using data_t = char ... or something else;
   using error_position_t = ...one of the position classes;
   using offset_position_t = void;
   using rewind_position_t = ...one of the position classes;
#if defined( __cpp_exceptions )
   using parse_error_t = parse_error< error_position_t >;
#endif

   [[nodiscard]] bool empty() const noexcept;
   [[nodiscard]] std::size_t size() const noexcept;  // Number of unconsumed input objects.

   [[nodiscard]] const data_t* current( const std::size_t offset = 0 ) const noexcept
   [[nodiscard]] const data_t* end() const noexcept;

   [[nodiscard]] const data_t* previous( const rewind_position_t saved ) const noexcept;
   [[nodiscard]] const data_t* previous( const error_position_t saved ) const noexcept;

   template< typename Rule >
   void consume( const std::size_t count ) noexcept;

   [[nodiscard]] rewind_position_t rewind_position() const noexcept;
   void rewind_to_position( const rewind_position_t saved ) noexcept;

   [[nodiscard]] error_position_t current_position() const noexcept;
   [[nodiscard]] error_position_t previous_position( const rewind_position_t saved ) const noexcept;
```

### Inputs with Start

An *input with start* is an input that remembers the initial return value of `current()` and can be restarted from that position.
Most inputs are inputs with start except for [`base_input`](Input-Reference.md#base-input) -- and [all stream inputs](Stream-Parsing.md#inputs).

```c++
   [[nodiscard]] const data_t* start() const noexcept;

   void restart() noexcept;
```

### Inputs with Lines

An *input with lines* defines an `eol_rule` type alias which enables use of the [`eol`](Rule-Reference.md#eol) and [`eolf`](Rule-Reference.md#eolf) rules.

Note that an input with lines does not necessarily include line and column numbers in its position tracking, that is only provided by [`text` inputs](#classification).

Note that when one of the user-facing input classes is given `void` as `Eol` template parameter it disables the `eol_rule` type alias and is **not** considered an *input with lines*.

For an in-depth explanation of the choices regarding the `Eol` template parameter please see the [ends of lines](#ends-of-lines) section above.

```c++
   using eol_rule = Eol;  // template parameter, usually defaults to tao_default_eol
```

Inputs with lines also implement the following functions that rely on the presence of `eol_rule`.

```c++
   [[nodiscard]] const data_t* begin_of_line( const error_position_t&, const std::size_t max = 135 ) const noexcept;
   [[nodiscard]] const data_t* end_of_line_or_file( const error_position_t&, const std::size_t max = 135 ) const;
```

### Inputs with Source

An *input with source* keeps an object that is part of the [position](TODO) but does not change over the parsing run.
For inputs that read from a file the source is the filename in a `std::filesystem::path`.

There are two source type aliases, `input_source_t` is the type of the source object embedded in the input, and `error_source_t` is the type of the source object embedded in the `error_position_t` which will be some `position_with_source<>` that is also used in the `parse_error<>` exceptions.

> [!Note]
> If either of `input_source_t` or `error_source_t` is `void` then both must be `void`.

> [!Note]
> It must be possible to construct an `error_source_t` from a `const input_source_t&`.

```c++
   using input_source_t = ...std::string or user chosen;
   using error_source_t = ...std::string or user chosen;

   [[nodiscard]] const input_source_t& direct_source() const noexcept;
```

### Input Convenience

All inputs, including `action_input`, implement a set of convenience functions.

```c++
   [[nodiscard]] const data_t& peek( const std::size_t offset = 0 ) const noexcept
   {
      return *current( offset );
   }

   template< typename T >
   [[nodiscard]] T peek_as( const std::size_t offset = 0 ) const noexcept
   {
      static_assert( sizeof( T ) == sizeof( data_t ) );
      return static_cast< T >( peek( offset ) );
   }

   [[nodiscard]] char peek_char( const std::size_t offset = 0 ) const noexcept
   {
      return peek_as< char >( offset );
   }

   [[nodiscard]] std::byte peek_byte( const std::size_t offset = 0 ) const noexcept
   {
      return peek_as< std::byte >( offset );
   }

   [[nodiscard]] std::int8_t peek_int8( const std::size_t offset = 0 ) const noexcept
   {
      return peek_as< std::int8_t >( offset );
   }

   [[nodiscard]] std::uint8_t peek_uint8( const std::size_t offset = 0 ) const noexcept
   {
      return peek_as< std::uint8_t >( offset );
   }

   [[nodiscard]] std::string string() const
   {
      static_assert( sizeof( data_t ) == 1 );
      return std::string( static_cast< const char* >( this->current() ), this->size() );
   }

   [[nodiscard]] std::string_view string_view() const noexcept
   {
      static_assert( sizeof( data_t ) == 1 );
      return std::string_view( static_cast< const char* >( this->current() ), this->size() );
   }

   [[nodiscard]] std::vector< data_t > vector() const
   {
      return std::vector< data_t >( this->current(), this->current() + this->size() );
   }

   template< typename Position >
   [[nodiscard]] std::string_view line_view_at( const Position& pos )
   {
      static_assert( sizeof( data_t ) == 1 );
      const char* const b = static_cast< const char* >( this->begin_of_line( pos ) );
      const char* const e = static_cast< const char* >( this->end_of_line_or_file( pos ) );
      return { b, std::size_t( e - b ) };
   }
```

The `line_view_at` function returns a `std::string_view` of the line of the input containing `pos`.
It requires an input `in` where `in.begin_of_line( pos )` and `in.end_of_line_or_file( pos )` are valid function calls.

### Stream Compatibility

The PEGTL is designed to minimize the impact of the existence of the [stream inputs](TODO) on the core library.
This goal was mostly achieved with the exception of some input functions and how the rules use them.
All non-stream input classes implement the following functions for compatibility with the stream inputs.

```c++
   [[nodiscard]] decltype( auto ) end( const std::size_t /*unused*/ ) const noexcept( auto )
   {
      return end();
   }

   [[nodiscard]] std::size_t size( const std::size_t /*unused*/ ) const noexcept( auto )
   {
      return size();
   }

   void require( const std::size_t /*unused*/ ) const noexcept
   {}

   void discard() const noexcept
   {}
```

All rules that need to be compatible with [stream inputs](TODO) need to use the `end()` and `size()` variants *with* argument.
The argument tells the stream input how much data it needs to -- at least -- prefetch and stream for the rule to make a match.

That is why, for example, the implementation of [`consume< Num >`](Rule-Reference.md#consume-num-) uses `if( in.size( Num ) >= Num )` instead of `if( in.size() >= Num )` to test whether the Input `in` contains at least `Num` further objects.

Similarly the `require()` and `discard()` functions are implemented for compatibility so that a grammar with [`require`](Stream-Parsing.md#require) and [`discard`](Stream-Parsing.md#discard) can be used on *all* inputs.


---

This document is part of the [PEGTL](https://github.com/taocpp/PEGTL).

Copyright (c) 2014-2025 Dr. Colin Hirsch and Daniel Frey<br>
Distributed under the Boost Software License, Version 1.0<br>
See accompanying file [LICENSE_1_0.txt](../LICENSE_1_0.txt) or copy at https://www.boost.org/LICENSE_1_0.txt
