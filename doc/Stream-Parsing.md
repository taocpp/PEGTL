# Stream Parsing

The PEGTL is primarily designed and optimised for parsing contiguous blocks of memory like a memory-mapped file or the contents of a `std::string`.
It also supports *stream parsing* where only a small portion of a large input is kept in a memory buffer at any time.

The stream parsing facilities can be included via `<tao/pegtl/stream.hpp>` -- or via the individual include files in `tao/pegtl/stream`.


## Contents

* [Overview](#overview)
* [Buffers](#buffers)
  * [Alloc Buffer](#alloc-buffer)
  * [Array Buffer](#array-buffer)
  * [Other Buffer](#other-buffer)
* [Readers](#readers)
  * [CStream Reader](#cstream-reader)
  * [CString Reader](#cstring-reader)
  * [IStream Reader](#istream-reader)
  * [Iterator Reader](#iterator-reader)
* [Inputs](#inputs)
  * [Plain Inputs](#plain-inputs)
  * [Plain Auto Inputs](#plain-auto-inputs)
  * [Text Inputs](#text-inputs)
  * [Text Auto Inputs](#text-auto-inputs)
* [Rules](#rules)
  * [`discard`](#discard)
  * [`is_stream`](#is_stream)
  * [`require< Num >`](#require-num-)
* [Actions](#actions)
  * [`discard_input`](#discard_input)
  * [`discard_input_on< Bool >`](#discard_input_on-bool-)
  * [`discard_input_on_failure`](#discard_input_on_failure)
  * [`discard_input_on_success`](#discard_input_on_success)


## Overview

Stream parsing is performed by using a *stream input* for a parsing run.
A stream input has the same interface as other inputs as well as some stream input specific functions.
It also has two stream input specific components, the *buffer* and the *reader*.

The buffer wraps the memory region in which a portion of the input data is kept.
Different [buffers](#Buffers) allocate memory in different ways, e.g. an embedded `std::array` or via `operator new[]`.

The buffer size, i.e. how many bytes can be kept in memory at any one time, and the chunk size, i.e. the minimum number of bytes requested from the reader when it needs more data, can be freely chosen.

The reader object provides a simple interface to read more input data.
Different [readers](#Readers) read data from different sources, e.g. a `std::istream` or a range of input iterators.

During a parsing run, all rules that directly attempt to match some input bytes, e.g. `any` as opposed to `seq`, tell the input how many bytes they want to match against.
If the buffer does not contain enough data the stream input will call the reader to supply more.

The [`require`](#require-num-) rule can be used to manually prefetch a chunk of input data into the buffer.

Removing parsed bytes from the buffer is called a *discard*.
This making space for more to-be-parsed data needs to happen regularly while parsing and can be done either manually or automatically.

The [`discard`](#discard) rule and/or the [`discard_input`](#discard-input) and [`discard_input_on`](#discard-input-on) actions are used for manual discards.
The stream inputs whose name contains `_auto_` perform aggressive automatic discarding.

> [!NOTE]
> A buffer discard can move data in the buffer, i.e. pointers to non-discarded input data can become invalid!

For choosing the buffer size in general, and when choosing where to perform a manual discard, both the attached actions and the backtracking behaviour of the grammar need to be taken into consideration.

When an action `A` is attached to a rule `R` and `A< R >::apply()` (not `apply0()`) exists it will be called with an action input representing the matched portion of the input, wherefore no discard is allowed to happen while matching `R` and anything -- directly or indirectly -- called from `R`. Similarly, when local failure of a rule `R` can lead to backtracking then no discard is allowed to happen while matching `R` and anything called from `R`.

In other words, a discard is only possible when there are no active rewind guards, which is exactly what the auto-discard inputs keep track of.

All stream parsing related classes and class templates reside in namespace `tao::pegtl`.
This default can be changed via the macro `TAO_PEGTL_NAMESPACE` in `tao/pegtl/config.hpp`.


## Buffers

There is no direct interaction with buffer objects, however the arguments to a buffer constructor need to be supplied to any input using that buffer (same for template parameters).

Note that the expositions of the buffer classes are not accurate, they only serve to document their specific constructor arguments and template parameters.
To see how the buffer classes are actually implemented we refer the reader to their implementation in the corresponding header files.

###### Alloc Buffer

The alloc buffer takes the size of the buffer and the read chunk size as run-time arguments to its constructor.
It uses `operator new[]` to dynamically allocate the buffer memory.

```c++
class alloc_buffer
{
public:
   alloc_buffer( const std::size_t buffer_size, const std::size_t chunk_size );
};
```

###### Array Buffer

The array buffer takes the size of the buffer and the read chunk size at compile time, as template parameters.
It contains a `std::array` of that size as non-static data member.

```c++
template< size_t BufferSize, size_t ChunkSize >
class array_buffer
{
public:
   array_buffer();
};
```

###### Other Buffer

The other buffer adapts a memory region that is managed elsewhere.
It takes the address and the size of the buffer and the read chunk size as run-time arguments to its constructor.

```c++
class other_buffer
{
public:
   other_buffer( const char* buffer, const std::size_t buffer_size, const std::size_t chunk_size );
   // Does NOT take ownership of the pointer.
};
```


## Readers

Readers need to implement only two functionalities, construction with reader-specific arguments and a read function.
The read function takes a pointer to a buffer and the length of the buffer.
It attempts to read `length` bytes returning how many bytes were actually read (or zero at end-of-file).
Errors are reported as exception, or by terminating the program when compiling with exceptions disabled.

```c++
class reader
{
public:
   [[nodiscard]] std::size_t read( char* buffer, const std::size_t length );
};
```

This is an informal interface that all readers implement -- possibly declared `noexcept`.

There is no direct interaction with reader objects, however the arguments to a reader constructor need to be supplied to any input using that reader (same for template parameters).

###### CString Reader

The cstring reader takes a pointer to a C string.
It does not need to be told how long the string is, nor does it use `std::strlen()` or similar to compute the length.
The read function produces more data until it reaches the terminating `'\0'`-byte.

```c++
class cstring_reader
{
public:
   explicit cstring_reader( const char* cstring ) noexcept;
   // Asserts cstring != nullptr.
   // Does NOT take ownership of the pointer.
};
```

###### CStream Reader

The cstream reader takes a `std::FILE*` to an open C file stream.

```c++
class cstream_reader
{
public:
   explicit cstream_reader( std::FILE* cstream ) noexcept;
   // Asserts cstream != nullptr.
   // Does NOT take ownership of the pointer.
};
```

###### IStream Reader

The istream reader takes a reference to a C++ stream to which it keeps a reference.

```c++
class istream_reader
{
public:
   explicit istream_reader( std::istream& stream ) noexcept;
};
```

###### Iterator Reader

The iterator reader takes a range of input iterators.
It copies the iterators, but not (initially) the range of bytes they represent.

```c++
template< typename InputIterator >
class iterator_reader
{
public:
   iterator_reader( const InputIterator& begin, const InputIterator& end );
};
```


## Inputs

The stream input constructors take the arguments of their buffer and their reader, in that order.
For example the `alloc_istream_input` has the arguments `const std::size_t buffer_size, const std::size_t chunk_size, std::istream& stream`, the first two for the alloc buffer, the last for the istream reader.

The stream inputs only support streams of `char`, are only implemented as type aliases, and do not come with deduction guides.

###### Plain Inputs

The plain inputs use `count_position` or `position_with_source< Source, count_position >` for position tracking.

```c++
template< typename Eol = tao_stream_eol,
          typename InputSource = void,
          typename ErrorSource = InputSource >
using alloc_cstream_input = /* unspecified */

template< typename Eol = tao_stream_eol,
          typename InputSource = void,
          typename ErrorSource = InputSource >
using alloc_cstring_input = /* unspecified */

template< typename Eol = tao_stream_eol,
          typename InputSource = void,
          typename ErrorSource = InputSource >
using alloc_istream_input = /* unspecified */

template< typename InputIterator,
          typename Eol = tao_stream_eol,
          typename InputSource = void,
          typename ErrorSource = InputSource >
using alloc_iterator_input = /* unspecified */

template< typename Eol = tao_stream_eol,
          typename InputSource = void,
          typename ErrorSource = InputSource,
          std::size_t BufferSize = default_buffer_size,
          std::size_t ChunkSize = default_chunk_size >
using array_cstream_input = /* unspecified */

template< typename Eol = tao_stream_eol,
          typename InputSource = void,
          typename ErrorSource = InputSource,
          std::size_t BufferSize = default_buffer_size,
          std::size_t ChunkSize = default_chunk_size >
using array_cstring_input = /* unspecified */

template< typename Eol = tao_stream_eol,
          typename InputSource = void,
          typename ErrorSource = InputSource,
          std::size_t BufferSize = default_buffer_size,
          std::size_t ChunkSize = default_chunk_size >
using array_istream_input = /* unspecified */

template< typename InputIterator,
          typename Eol = tao_stream_eol,
          typename InputSource = void,
          typename ErrorSource = InputSource,
          std::size_t BufferSize = default_buffer_size,
          std::size_t ChunkSize = default_chunk_size >
using array_iterator_input = /* unspecified */

template< typename Eol = tao_stream_eol,
          typename InputSource = void,
          typename ErrorSource = InputSource >
using other_cstream_input = /* unspecified */

template< typename Eol = tao_stream_eol,
          typename InputSource = void,
          typename ErrorSource = InputSource >
using other_cstring_input = /* unspecified */

template< typename Eol = tao_stream_eol,
          typename InputSource = void,
          typename ErrorSource = InputSource >
using other_istream_input = /* unspecified */

template< typename InputIterator,
          typename Eol = tao_stream_eol,
          typename InputSource = void,
          typename ErrorSource = InputSource >
using other_iterator_input = /* unspecified */
```

###### Plain Auto Inputs

The inputs with `auto` in their name perform auto discard.

```c++
template< typename Eol = tao_stream_eol,
          typename InputSource = void,
          typename ErrorSource = InputSource >
using alloc_cstream_auto_input = /* unspecified */

template< typename Eol = tao_stream_eol,
          typename InputSource = void,
          typename ErrorSource = InputSource >
using alloc_cstring_auto_input = /* unspecified */

template< typename Eol = tao_stream_eol,
          typename InputSource = void,
          typename ErrorSource = InputSource >
using alloc_istream_auto_input = /* unspecified */

template< typename InputIterator,
          typename Eol = tao_stream_eol,
          typename InputSource = void,
          typename ErrorSource = InputSource >
using alloc_iterator_auto_input = /* unspecified */

template< typename Eol = tao_stream_eol,
          typename InputSource = void,
          typename ErrorSource = InputSource,
          std::size_t BufferSize = default_buffer_size,
          std::size_t ChunkSize = default_chunk_size >
using array_cstream_auto_input = /* unspecified */

template< typename Eol = tao_stream_eol,
          typename InputSource = void,
          typename ErrorSource = InputSource,
          std::size_t BufferSize = default_buffer_size,
          std::size_t ChunkSize = default_chunk_size >
using array_cstring_auto_input = /* unspecified */

template< typename Eol = tao_stream_eol,
          typename InputSource = void,
          typename ErrorSource = InputSource,
          std::size_t BufferSize = default_buffer_size,
          std::size_t ChunkSize = default_chunk_size >
using array_istream_auto_input = /* unspecified */

template< typename InputIterator,
          typename Eol = tao_stream_eol,
          typename InputSource = void,
          typename ErrorSource = InputSource,
          std::size_t BufferSize = default_buffer_size,
          std::size_t ChunkSize = default_chunk_size >
using array_iterator_auto_input = /* unspecified */

template< typename Eol = tao_stream_eol,
          typename InputSource = void,
          typename ErrorSource = InputSource >
using other_cstream_auto_input = /* unspecified */

template< typename Eol = tao_stream_eol,
          typename InputSource = void,
          typename ErrorSource = InputSource >
using other_cstring_auto_input = /* unspecified */

template< typename Eol = tao_stream_eol,
          typename InputSource = void,
          typename ErrorSource = InputSource >
using other_istream_auto_input = /* unspecified */

template< typename InputIterator,
          typename Eol = tao_stream_eol,
          typename InputSource = void,
          typename ErrorSource = InputSource >
using other_iterator_auto_input = /* unspecified */
```

###### Text Inputs

The text inputs use `text_position` or `position_with_source< Source, text_position >` with the [same limitations](TODO!) as the regular inputs regarding the line and column numbers.

```c++
template< typename Eol = tao_stream_eol,
          typename InputSource = void,
          typename ErrorSource = InputSource >
using alloc_text_cstream_input = /* unspecified */

template< typename Eol = tao_stream_eol,
          typename InputSource = void,
          typename ErrorSource = InputSource >
using alloc_text_cstring_input = /* unspecified */

template< typename Eol = tao_stream_eol,
          typename InputSource = void,
          typename ErrorSource = InputSource >
using alloc_text_istream_input = /* unspecified */

template< typename InputIterator,
          typename Eol = tao_stream_eol,
          typename InputSource = void,
          typename ErrorSource = InputSource >
using alloc_text_iterator_input = /* unspecified */

template< typename Eol = tao_stream_eol,
          typename InputSource = void,
          typename ErrorSource = InputSource,
          std::size_t BufferSize = default_buffer_size,
          std::size_t ChunkSize = default_chunk_size >
using array_text_cstream_input = /* unspecified */

template< typename Eol = tao_stream_eol,
          typename InputSource = void,
          typename ErrorSource = InputSource,
          std::size_t BufferSize = default_buffer_size,
          std::size_t ChunkSize = default_chunk_size >
using array_text_cstring_input = /* unspecified */

template< typename Eol = tao_stream_eol,
          typename InputSource = void,
          typename ErrorSource = InputSource,
          std::size_t BufferSize = default_buffer_size,
          std::size_t ChunkSize = default_chunk_size >
using array_text_istream_input = /* unspecified */

template< typename InputIterator,
          typename Eol = tao_stream_eol,
          typename InputSource = void,
          typename ErrorSource = InputSource,
          std::size_t BufferSize = default_buffer_size,
          std::size_t ChunkSize = default_chunk_size >
using array_text_iterator_input = /* unspecified */

template< typename Eol = tao_stream_eol,
          typename InputSource = void,
          typename ErrorSource = InputSource >
using other_text_cstream_input = /* unspecified */

template< typename Eol = tao_stream_eol,
          typename InputSource = void,
          typename ErrorSource = InputSource >
using other_text_cstring_input = /* unspecified */

template< typename Eol = tao_stream_eol,
          typename InputSource = void,
          typename ErrorSource = InputSource >
using other_text_istream_input = /* unspecified */

template< typename InputIterator,
          typename Eol = tao_stream_eol,
          typename InputSource = void,
          typename ErrorSource = InputSource >
using other_text_iterator_input = /* unspecified */
```

###### Text Auto Inputs

The inputs with `text` and `auto` in their name combine the name-giving features from the [text inputs](#text-inputs) and the [auto inputs](#auto-inputs).

```c++
template< typename Eol = tao_stream_eol,
          typename InputSource = void,
          typename ErrorSource = InputSource >
using alloc_text_cstream_auto_input = /* unspecified */

template< typename Eol = tao_stream_eol,
          typename InputSource = void,
          typename ErrorSource = InputSource >
using alloc_text_cstring_auto_input = /* unspecified */

template< typename Eol = tao_stream_eol,
          typename InputSource = void,
          typename ErrorSource = InputSource >
using alloc_text_istream_auto_input = /* unspecified */

template< typename InputIterator,
          typename Eol = tao_stream_eol,
          typename InputSource = void,
          typename ErrorSource = InputSource >
using alloc_text_iterator_auto_input = /* unspecified */

template< typename Eol = tao_stream_eol,
          typename InputSource = void,
          typename ErrorSource = InputSource,
          std::size_t BufferSize = default_buffer_size,
          std::size_t ChunkSize = default_chunk_size >
using array_text_cstream_auto_input = /* unspecified */

template< typename Eol = tao_stream_eol,
          typename InputSource = void,
          typename ErrorSource = InputSource,
          std::size_t BufferSize = default_buffer_size,
          std::size_t ChunkSize = default_chunk_size >
using array_text_cstring_auto_input = /* unspecified */

template< typename Eol = tao_stream_eol,
          typename InputSource = void,
          typename ErrorSource = InputSource,
          std::size_t BufferSize = default_buffer_size,
          std::size_t ChunkSize = default_chunk_size >
using array_text_istream_auto_input = /* unspecified */

template< typename InputIterator,
          typename Eol = tao_stream_eol,
          typename InputSource = void,
          typename ErrorSource = InputSource,
          std::size_t BufferSize = default_buffer_size,
          std::size_t ChunkSize = default_chunk_size >
using array_text_iterator_auto_input = /* unspecified */

template< typename Eol = tao_stream_eol,
          typename InputSource = void,
          typename ErrorSource = InputSource >
using other_text_cstream_auto_input = /* unspecified */

template< typename Eol = tao_stream_eol,
          typename InputSource = void,
          typename ErrorSource = InputSource >
using other_text_cstring_auto_input = /* unspecified */

template< typename Eol = tao_stream_eol,
          typename InputSource = void,
          typename ErrorSource = InputSource >
using other_text_istream_auto_input = /* unspecified */

template< typename InputIterator,
          typename Eol = tao_stream_eol,
          typename InputSource = void,
          typename ErrorSource = InputSource >
using other_text_iterator_auto_input = /* unspecified */
```


## Rules

The `discard` and `require` rules do nothing on non-stream inputs.

All rules are included with `<tao/pegtl/buffer.hpp>` or can be included individually.

Unlike [most other rules](Rule-Reference.md) they have no separate implementation in namespace `tao::pegtl::internal`.

The analyze traits for these rules are in `<tao/pegtl/buffer/analyze_traits.hpp>` which is *not* included automatically.

###### `discard`

* [Equivalent] to `success`, but also
* calls the input's `discard()` member function.
* Make sure that all consumed data can in fact be discarded!
* [Meta data] and [implementation] mapping:
  - `discard::rule_t` is `discard`

###### `is_stream`

* Succeeds when parsing with a stream input.
* Fails (local failure) for all other inputs.
* The result is determined at compile time.
* [Meta data] and [implementation] mapping:
  - `is_stream::rule_t` is `is_stream`

###### `require< Num >`

* Succeeds if at least `Num` further input bytes are, or can be made, available.
* With stream inputs the reader is called to provide more input if necessary.
* [Meta data] and [implementation] mapping:
  - `require< 0 >::rule_t` is `internal::success`
  - `require< N >::rule_t` is `require< N >`


## Actions

###### `discard_input`

* Action with (only) a `match()` function.
* Calls `discard()` on the input if the rule it is attached to returns.
* Publicly derives from [`maybe_nothing`](Action-Reference.md#maybe-nothing).

###### `discard_input_on< Bool >`

* Action with (only) a `match()` function.
* Takes a `bool B` as template parameter.
* Calls `discard()` on the input if the rule it is attached to returns `B`.
* Publicly derives from [`maybe_nothing`](Action-Reference.md#maybe-nothing).

###### `discard_input_on_failure`

* Type alias for `discard_input_on< false >`.

###### `discard_input_on_success`

* Type alias for `discard_input_on< true >`.


---

This document is part of the [PEGTL](https://github.com/taocpp/PEGTL).

Copyright (c) 2025 Dr. Colin Hirsch and Daniel Frey<br>
Distributed under the Boost Software License, Version 1.0<br>
See accompanying file [LICENSE_1_0.txt](../LICENSE_1_0.txt) or copy at https://www.boost.org/LICENSE_1_0.txt
