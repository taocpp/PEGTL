# Input Reference

The reference documention for all inputs.

### Included

All inputs except for the buffer inputs for incremental parsing are automatically included with `tao/pegtl.hpp`.
The buffer inputs are documented together with the other facilities related to incremental parsing.

### Text Inputs

The text inputs us a `text_position` that tracks column and line numbers in addition to the count of parsed objects (usually of type `char`).
The other inputs also have an `Eol` template parameter but only use that for the `eol` and `eolf` rules, not for position tracking.

### Namespaces

All inputs reside in namespace `tao::pegtl`.
This default can be changed via the macro `TAO_PEGTL_NAMESPACE` in `tao/pegtl/config.hpp`.

### Restartable Input

### Input with Source

### Input with Lines

## Contents

* [Argv Input](#argv-input)
* [Base Input](#base-input)
* [View Input](#view-input)
* [Copy Input](#copy-input)
* [File Input](#file-input)
* [Read Input](#read-input)
* [Mmap Input](#mmap-input)
* [Text View Input](#text-view-input)
* [Text Copy Input](#text-copy-input)
* [Text File Input](#text-file-input)
* [Text Read Input](#text-read-input)
* [Text Mmap Input](#text-mmap-input)


## Argv Input

* Used to parse single argument strings `argv[ n ]` for some `n`.
* Sets the source to `"argv[ n ]"` when passed `argv` and `n`.
* Restartable, without lines (by default), with source (by default).

```c++
 template< typename Eol = void, typename Source = std::string >
 struct argv_input
 {
  argv_input( char** argv, const int argn );
 };
```

## Base input

* Does **not** copy the input data.
* Light-weight version of `view_input`.
* Not restartable, with lines (by default), without source.

###### Constructors

```c++
 template< typename Eol = tao_default_eol, typename Data = char >
 struct base_input
 {
  base_input( const Data* begin, const Data* end ) noexcept;
  base_input( const Data* begin, const std::size_t size ) noexcept;

  explicit base_input( std::vector< Data >& data ) noexcept;
  explicit base_input( const std::vector< Data >& data ) noexcept;

  template< std::size_t Size >
  explicit view_input( const std::array< Data, Size >& a ) noexcept;

  // Only available when Data is compatible with char:

  explicit base_input( std::string& data ) noexcept;
  explicit base_input( const std::string& data ) noexcept;
  explicit base_input( const std::string_view data ) noexcept;

  template< std::size_t N >
  explicit view_input( const char ( &data )[ N ] ) noexcept;

  // To prevent accidents with temporaries:

  base_input( std::string&& ) = delete;
  base_input( const std::string&& ) = delete;

  base_input( std::vector< Data >&& ) = delete;
  base_input( const std::vector< Data >&& ) = delete;
 };
```

## View Input

* Does **not** copy the input data.
* Restartable, with lines (by default), without source (by default).

###### C'tors Without Source

```c++
 template< typename Eol = tao_default_eol, typename Data = char, typename InputSource = void, typename ErrorSource = InputSource >
 struct view_input
 {
  view_input( const Data* begin, const Data* end ) noexcept;
  view_input( const Data* begin, const std::size_t size ) noexcept;

  explicit view_input( std::vector< Data >& data ) noexcept;
  explicit view_input( const std::vector< Data >& data ) noexcept;

  view_input( std::string&& ) = delete;
  view_input( const std::string&& ) = delete;

  view_input( std::vector< Data >&& ) = delete;
  view_input( const std::vector< Data >&& ) = delete;

  // Only available when Data is compatible with char:

  explicit view_input( std::string& data ) noexcept;
  explicit view_input( const std::string& data ) noexcept;
  explicit view_input( const std::string_view data ) noexcept;
 };
```

###### C'tors With Source

The `source` parameter is forwarded to construct the data member of type `InputSource` embedded in the input.

```c++
 template< typename Eol = tao_default_eol, typename Data = char, typename InputSource = void, typename ErrorSource = InputSource >
 struct view_input
 {
  template< typename Source >
  view_input( Source&& source, const Data* begin, const Data* end ) noexcept;
  template< typename Source >
  view_input( Source&& source, const Data* begin, const std::size_t size ) noexcept;

  template< typename Source >
  explicit view_input( Source&& source, std::vector< Data >& data ) noexcept;
  template< typename Source >
  explicit view_input( Source&& source, const std::vector< Data >& data ) noexcept;

  template< typename Source >
  view_input( Source&& source, std::string&& ) = delete;
  template< typename Source >
  view_input( Source&& source, const std::string&& ) = delete;

  template< typename Source >
  view_input( Source&& source, std::vector< Data >&& ) = delete;
  template< typename Source >
  view_input( Source&& source, const std::vector< Data >&& ) = delete;

  // Only available when Data is compatible with char:

  template< typename Source >
  explicit view_input( Source&& source, std::string& data ) noexcept;
  template< typename Source >
  explicit view_input( Source&& source, const std::string& data ) noexcept;
  template< typename Source >
  explicit view_input( Source&& source, const std::string_view data ) noexcept;
 };
```

## Copy Input

* Makes and keeps a copy of the input data in an embedded container.
* The container in which to keep the copied data can be changed.
* Restartable, with lines (by default), without source (by default).

###### C'tors Without Source

```c++
 template< typename Eol = tao_default_eol, typename Container = std::string, typename InputSource = void, typename ErrorSource = InputSource >
 struct copy_input
 {
  copy_input( const Data* begin, const Data* end ) noexcept;
  copy_input( const Data* begin, const std::size_t size ) noexcept;

  explicit copy_input( Container&& data ) noexcept;
  explicit copy_input( const Container& data );

  using Data = typename Container::value_type;

  template< std::size_t Size >
  explicit copy_input( const std::array< Data, Size >& data );

  explicit copy_input( const std::initializer_list< Data >& init );
 };
```

###### C'tors With Source

```c++
 template< typename Eol = tao_default_eol, typename Container = std::string, typename InputSource = void, typename ErrorSource = InputSource >
 struct copy_input
 {
  template< typename Source >
  copy_input( Source&& source, const Data* begin, const Data* end ) noexcept;
  template< typename Source >
  copy_input( Source&& source, const Data* begin, const std::size_t size ) noexcept;

  template< typename Source >
  explicit copy_input( Source&& source, Container&& data ) noexcept;
  template< typename Source >
  explicit copy_input( Source&& source, const Container& data );

  using Data = typename Container::value_type;

  template< typename Source >
  explicit copy_input( Source&& source, const std::initializer_list< Data >& data );
 };
```

## File Input

* Shortcut for `mmap_input` when available and `read_input` as fallback.

## Read Input


## Mmap Input


## Text View Input


## Text Copy Input


## Text File Input


## Text Read Input


## Text Mmap Input

---

This document is part of the [PEGTL](https://github.com/taocpp/PEGTL).

Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey<br>
Distributed under the Boost Software License, Version 1.0<br>
See accompanying file [LICENSE_1_0.txt](../LICENSE_1_0.txt) or copy at https://www.boost.org/LICENSE_1_0.txt
