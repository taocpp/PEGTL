# Incremental Input

The PEGTL is designed and optimised for parsing single contiguous blocks of memory, e.g. the contents of a file made available via `mmap(2)`, or the contents of a `std::string`.

In cases where the input does not fit into memory, or there are other reasons to not create a single memory block containing all input data, it is possible, with a little help from the grammar, to perform incremental parsing, where the data is incrementally made available, e.g. when reading from a stream.

## Grammars and Buffering

A buffer is used to keep a portion of the input data in a contiguous memory block.
The buffer is allocated at the begin of the parsing run with a user-supplied maximum size.

The maximum buffer size usually depends on the grammar, the actions, and the input data.
It must be chosen large enough to keep the data required for (a) any backtracking, and (b) all action invocations.

The buffer is automatically filled by the parsing rules that require input data, however **discarding data from the buffer is** (currently) **not automatic**:
The grammar has to call [`discard`](Rule-Reference.md#discard) in appropriate places to free the buffer again.

More precisely, each rule that uses one of the following methods on the input will implicitly make a call to `tao::pegtl::buffer_input::require( amount )`.
(The `empty()`-method uses a hard-coded `amount` of 1.)

```c++
namespace pegtl
{
   template< class Reader > class buffer_input
   {
      empty();
      size( const std::size_t amount );
      end( const std::size_t amount );
      ...
   };
}
```

This tells the input that a rule wants to inspect and/or consume a certain `amount` of input bytes, and it will attempt to fill the buffer accordingly.
The returned `size()`, and the distance from `begin()` to `end()`, can also be larger than the requested amount.

For example, the rule `tao::pegtl::ascii::eol`, which (usually) checks for both `"\r\n"` and "`\n`", calls `size(2)` because it needs to inspect up to two bytes.
Depending on whether the result of `size(2)` is `0`, `1` or `2`, it will choose which of these two sequences it can attempt to match.
The number of actually consumed bytes can again be `0`, `1` or `2`, depending on whether they match a valid `eol`-sequence.

To prevent the buffer from overflowing, the `discard()`-method of class `tao::pegtl::buffer_input` must be called, usually by using the `discard` parsing rule.
It discards all data in the buffer that precedes the current `begin()`-point, and any remaining data is moved to the beginning of the buffer.
**A `discard` invalidates all pointers to the input's data, including those contained in other input objects.**

```
Buffer Memory Layout

B                   begin of buffer space
:
B + X               begin of unconsumed buffered data as per begin()
:
B + X + size( 0 )   end of unconsumed buffered data as per end( 0 )
:
B + maximum         end of buffer space
```

A discard moves the data in the buffer such that `X` is zero, and updates `begin()` to point at the beginning of the buffer.

## Custom Data Sources

The PEGTL contains a set of stream parser functions that take care of everything (except discarding data from the buffer, see above) for certain data sources.
In order to support other data sources, it is necessary to create a custom input class, usually by creating a suitable *reader* class that can be supplied as template argument to class `tao::pegtl::buffer_input`.

The reader class can be anything that can be called like the following standard function wrapper:

```c++
std::function< std::size_t( char * buffer, const std::size_t length ) >
```

The arguments and return value are similar to other `read()`-style functions:
Attempt to read up to `length` bytes into the memory pointed to by `buffer` and return the number of bytes actually read.
Reaching the end of the input should be the only reason for the reader to return zero.

The steps required to use a custom reader for a parsing run are:

1. Create a suitable reader class `Reader` (or function).
2. Create an instance of class `tao::pegtl::buffer_input< Reader >`, using the fact that the `buffer_input`'s constructor can pass arbitrary arguments to the embedded reader instance.
3. Call `tao::pegtl::parse_input()` (or `tao::pegtl::parse_input_nested()`) with the previously created `buffer_input` instance as first argument.

The included examples for C- and C++-style streams can also be used as reference on how to create and use suitable readers, simply `grep(1)` for `cstring_reader`, `cstream_reader` and `istream_reader` in the PEGTL source code.

Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
