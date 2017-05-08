# Inputs and Parsing

Assuming that the [grammar rules](Rules-and-Grammars.md) are ready, and the [actions and states](Actions-and-States.md) prepared, performing a parsing run consists of two steps:

1. Constructing an *input* class that represents the to-be-parsed data.
2. Calling a PEGTL *parse* function with the input (and any states).

```c++
using namespace tao::pegtl;

struct my_grammar : ...;

template< typename Rule >
struct my_actions : nothing< Rule > {};

// Specialisations of my_actions as required...

bool my_parse( const std::string& filename, my_state& state )
{
   file_input<> in( filename );
   return parse< my_grammar, my_actions >( in, state );
}
```

In the context of PEGTL input classes and positions, `source` is a string that identifies where the to-be-parsed data comes from.
For example when parsing a file, the filename is the source.

All classes and functions on this page are in namespace `tao::pegtl`.

## Contents

* [Tracking Mode](#tracking-mode)
* [Line Ending](#line-ending)
* [Source](#source)
* [File Input](#file-input)
* [Memory Input](#memory-input)
* [String Input](#string-input)
* [Stream Inputs](#stream-inputs)
* [Argument Input](#argument-input)
* [Parse Function](#parse-function)
* [Nested Parsing](#nested-parsing)

## Tracking Mode

Some input classes allow a choice of tracking mode, or whether the `byte`, `line` and `byte_in_line` counters are continuously updated during a parsing run with `tracking_mode::IMMEDIATE`, or only calculated on-demand in the `position()`-method by scanning the complete input again with `tracking_mode::LAZY`.

Lazy tracking is recommended when the position is used very infrequently, for example only in the case of throwing a `parse_error`.

Immediate tracking is recommended when the position is used frequently and/or in non-exceptional cases, for example when annotating every AST node with the position.

## Line Ending

All input classes allow the choice of which line endings should be recognised by the `eol` and `eolf` rules, and used for line counting.
The supported line endings are `cr`, a single carriage-return/`"\r"`/`0x0d` character as used on classic Mac OS, `lf`, a single line-feed/`"\n"`/`0x0a` as used on Unix, Linux, Mac OS X and macOS, and `crlf`, a sequence of both as used on MS-DOS and Windows.

The default template argument for all input classes is `eol::lf_crlf` which recognises both Unix and MS-DOS line endings.
The supplied alternatives are `eol::cr`, `eol::lf`, `eol::crlf` and `eol::cr_crlf`.

## Source

Some input classes allow a choice of how to store the source parameter. TODO: Explain in more detail.

## File Input

The classes `file_input<>`, `read_input<>` and, on supported platforms, `mmap_input<>`, can be used to parse the contents of a file.

* `read_input<>` uses C "stdio" facilities to read the file.
* `mmap_input<>` uses `mmap(2)` and is available on POSIX compliant systems.
* `file_input<>` is a type alias for `mmap_input<>` when available, and `read_input<>` otherwise.

All file input classes take a single argument, the filename, which can be supplied as `std::string` or `const char*`.
They immediately make available the complete contents of the file; `read_input<>` reads the entire file upon construction.

```c++
template< tracking_mode P = tracking_mode::IMMEDIATE, typename Eol = eol::lf_crlf >
struct read_input
{
   explicit read_input( const char* filename );
   explicit read_input( const std::string& filename );
};

template< tracking_mode P = tracking_mode::IMMEDIATE, typename Eol = eol::lf_crlf >
struct mmap_input  // Only on POSIX compliant systems.
{
   explicit mmap_input( const char* filename );
   explicit mmap_input( const std::string& filename );
};

template< tracking_mode P = tracking_mode::IMMEDIATE, typename Eol = eol::lf_crlf >
using file_input = mmap_input< P, Eol >;  // Or read_input when no mmap_input available.
```

Note that the implementation of the constructors is different than shown.
They should be used "as if" this was the actual signature.

## Memory Input

The class `memory_input<>` can be used to parse existing contiguous blocks of memory like the contents of a `std::string`.
The input neither copies the data nor takes ownership, it only keeps pointers.
The various constructors accept the to-be-parsed data and the source in different formats.

The constructors that only takes a `const char* begin` uses `std::strlen()` to determine the length of the data.
It will therefore *only* work correctly with data that is terminated with a 0-byte (and does not contain embedded 0-bytes).

The constructors that take additional `byte`, `line` and `byte_in_line` arguments initialise the internal counters with the supplied values, rather than the defaults of `0`, `1` and `0`.

```c++
template< tracking_mode P = tracking_mode::IMMEDIATE, typename Eol = eol::lf_crlf, typename Source = const char* >
class memory_input
{
   // Constructors available with any tracking_mode:

   memory_input( const char* begin, const char* end, const char* source ) noexcept;
   memory_input( const char* begin, const char* end, const std::string& source ) noexcept;

   memory_input( const char* begin, const std::size_t size, const char* source ) noexcept;
   memory_input( const char* begin, const std::size_t size, const std::string& source ) noexcept;

   memory_input( const std::string& string, const char* source ) noexcept;
   memory_input( const std::string& string, const std::string& source ) noexcept;

   memory_input( const char* begin, const char* source ) noexcept;
   memory_input( const char* begin, const std::string& source ) noexcept;

   memory_input( const char* begin, const char* end, const char* source,
                 const std::size_t byte, const std::size_t line, const std::size_t byte_in_line ) noexcept;
   memory_input( const char* begin, const char* end, const std::string& source,
                 const std::size_t byte, const std::size_t line, const std::size_t byte_in_line ) noexcept;

   // Constructors available only with tracking_mode::IMMEDIATE:

   memory_input( const internal::iterator& iter, const char* end, const char* source ) noexcept;
   memory_input( const internal::iterator& iter, const char* end, const std::string& source ) noexcept;
};
```

## String Input

The class `string_input<>` can also be used to parse a `std::string`.
Unlike class `memory_input<>`, this class stores a copied (or moved) version of the data for which it takes ownership.

```c++
template< tracking_mode P = tracking_mode::IMMEDIATE, typename Eol = eol::lf_crlf, typename Source = std::string >
class string_input
{
   string_input( const std::string& string, const char* source ) noexcept;
   string_input( const std::string& string, const std::string& source ) noexcept;

   string_input( const char* begin, const char* source ) noexcept;
   string_input( const char* begin, const std::string& source ) noexcept;
};
```

## Stream Inputs

The classes `cstream_input<>` and `istream_input<>` can be used to parse data from C-streams (`std::FILE*`) and C++-streams (`std::istream`), respectively.
Unlike the file inputs above, they internally use `buffer_input<>` and therefore do *not* read the complete stream upon construction.

They all have a single constructor that takes a stream, the maximum buffer size, and the name of the source.
Note that these classes only keep a pointer/reference to the stream and do *not* take ownership; `cstream_input<>` does *not* call `std::close()`.

Please read [Incremental Input](Incremental-Input.md) for details on the `maximum` argument, and how to prepare a grammar for incremental input support using the `discard`-rule.

```c++
template< typename Eol = eol::lf_crlf >
struct cstream_input
{
   cstream_input( std::FILE* stream, const std::size_t maximum, const char* source );
   cstream_input( std::FILE* stream, const std::size_t maximum, const std::string& source );
};

template< typename Eol = eol::lf_crlf >
struct istream_input
{
   istream_input( std::istream& stream, const std::size_t maximum, const char* source );
   istream_input( std::istream& stream, const std::size_t maximum, const std::string& source );
};
```

## Argument Input

The class `argv_input<>` can be used to parse a string passed from the command line.

```c++
template< tracking_mode P = tracking_mode::IMMEDIATE, typename Eol = eol::lf_crlf >
class argv_input
{
   argv_input( char** argv, const std::size_t n, const char* source );
   argv_input( char** argv, const std::size_t n, const std::string& source );
};
```

## Parse Function

The parse functions accept the following template parameters and arguments:

- The [`Rule` class](Rules-and-Grammars.md) represents the top-level parsing rule of the grammar and is mandatory.
- The [`Action<>` class template](Actions-and-States.md) is required to actually do something during a parsing run.
- The [`Control<>` class template](Control-Hooks.md) is only required for grammar debugging or some advanced uses.
- The [`States`](Actions-and-States) are the types of the objects that are passed to all actions and control hooks.

Additionally, two enumeration values can be used to control the behaviour:

- The `apply_mode` which can also be set to `NOTHING` in order to disable action invocations, just like the `disable<>` rule does.
- The `rewind_mode` which can also be set to `DONTCARE` in order to not require rewinding of the input on local failure, a micro optimisation.

The result of a parsing run, i.e. an invocation of `tao::pegtl::parse()`, can be either

- *success*, a return value of `true`,
- *local failure*, a return value of `false`,
- *global failure*, an exception of type `tao::pegtl::parse_error`, or
- any other exception thrown by the input class or an action method.

```c++
template< typename Rule,
          template< typename... > class Action = nothing,
          template< typename... > class Control = normal,
          apply_mode A = apply_mode::ACTION,
          rewind_mode M = rewind_mode::REQUIRED,
          typename Input,
          typename... States >
bool parse( Input& in,
            States&&... st );
```

## Nested Parsing

Nested parsing refers to an (inner) parsing run that is performed "in the middle of" another (outer) parsing run, for example when one file "includes" another file.

The difference to the regular `tao::pegtl::parse()` function is that `tao::pegtl::parse_nested()` takes care of adding to the `std::vector` of `tao::pegtl::position` objects in the exception class `tao::pegtl::parse_error`.
This allows generating error messages of the form "error in file F1 line L1 included from file F2 line L2...".

Calling `parse_nested()` requires one additional argument compared to `parse()`, the input from the outer parsing run as first argument.
Everything else remains the same.

```c++
template< typename Rule,
          template< typename... > class Action = nothing,
          template< typename... > class Control = normal,
          apply_mode A = apply_mode::ACTION,
          rewind_mode M = rewind_mode::REQUIRED,
          typename Outer,
          typename Input,
          typename... States >
bool parse_nested( const Outer& oi,
                   Input& in,
                   States&&... st );
```

Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
