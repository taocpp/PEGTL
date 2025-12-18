# Input Anatomy

All input classes adhere to an informally defined interface of which some parts are optional.


## Contents

* [Input Interface](#input-interface)
* [Inputs with Start](#inputs-with-start)
* [Inputs with Lines](#inputs-with-lines)
* [Inputs with Source](#inputs-with-source)
* [Input Convenience](#input-convenience)
* [Stream Compatibility](#stream-compatibility)


## Input Interface

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

   [[nodiscard]] bool empty() const noexcept;  // return size() == 0;
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

## Inputs with Start

An *input with start* is an input that remembers the initial return value of `current()` and can be restarted from that position.
Most inputs are inputs with start except for [`base_input`](Input-Reference.md#base-input) -- and of course the [stream inputs](TODO).

```c++
   [[nodiscard]] const data_t* start() const noexcept;

   void restart() noexcept;
```


## Inputs with Lines

An *input with lines* defines an `eol_rule` for the [`eol`](Rule-Reference.md#eol) and [`eolf`](Rule-Reference.md#eolf) rules.

TODO: Link to EOL chapter or insert here?

```c++
   using eol_rule = tao_default_eol or user chosen;
```



```c++
   [[nodiscard]] const data_t* begin_of_line( const error_position_t&, const std::size_t max = 135 ) const noexcept;
   [[nodiscard]] const data_t* end_of_line_or_file( const error_position_t&, const std::size_t max = 135 ) const;
```



## Inputs with Source

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


## Input Convenience

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
It requires an input where `begin_of_line( pos )` and `end_of_line_or_file( pos )` are valid.


## Stream Compatibility

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

Similarly the `require()` and `discard()` functions are implemented for compatibility so that a grammar with [`require`](TODO) and [`discard`](TODO) rules can be used on *all* kinds of input.


---

This document is part of the [PEGTL](https://github.com/taocpp/PEGTL).

Copyright (c) 2024-2025 Dr. Colin Hirsch and Daniel Frey<br>
Distributed under the Boost Software License, Version 1.0<br>
See accompanying file [LICENSE_1_0.txt](../LICENSE_1_0.txt) or copy at https://www.boost.org/LICENSE_1_0.txt
