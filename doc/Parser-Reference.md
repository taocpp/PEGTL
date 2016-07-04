# Parser Reference

All parser functions and classes make use of the following template parameters and arguments:

- `Rule` is the class that represents [the top-level parsing rule of the grammar](Rules-and-Grammars.md) to be parsed.
- `Action<>` is the class template [specialised for user-defined actions during the parsing run](Actions-and-States.md).
- `Control<>` is the class template that [controls debugging and throwing of exceptions](Control-Hooks.md).
- `States` are [the additional arguments that are forwarded to all actions](Actions-and-States) and [control hooks](Control-Hooks.md).
- `Outer` is the type of the outer input for a nested parsing run inside an outer parsing run.

The `Rule` class *has* to be provided in order to have something to match against while parsing, some `Action<>` *usually* has to be provided (to actually *do* something while parsing, rather than only checking adherence of the input data to the grammar), whereas for `Control<>`, at least initially, the provided default will often be sufficient.

Nested parsing runs are typically performed when a file "includes" another file in the middle of being parsed.
In order to allow for error messages of the form "error in file F1 line L1 included from file F2 line L2..." the exception class `pegtl::parse_error` contains a whole vector of `pegtl::position_info` objects, and using the nested parsing functions gives the PEGTL the information it needs to correctly fill that vector.

## Contents

  * [Input Data](#input-data)
  * [Parser Functions](#parser-functions)
  * [Tracer Functions](#tracer-functions)
  * [Parser Classes](#parser-classes)

## Input Data

During a parsing run, the PEGTL uses the an *input* class, e.g. `pegtl::memory_input`, to reference, and pass around references to, (portions of) the input data.

When dealing with, or implementing, *any* function that is called with an instance of an input class, or internally uses one like the `pegtl::parse()`-functions, it has to be taken into consideration that

1. the input classes do *not* copy the data they reference, they only keep pointers, therefore
2. the parser functions do *not* copy the data they parse, therefore
3. it depends on the caller of the top-most function whether, and for how long, the data used during a parsing run is valid afterwards.

This holds true for `parse()`-functions, rules' (`match()`-functions), actions' (`apply()`-functions) and all [control-hooks](Control-Hooks.md).

In some cases it might be necessary to use highly optimised data structures that do not copy the input data, which makes it necessary to ensure that the lifetime of the input data, i.e. the validity of all pointers and references to the input data, exceeds the lifetime of the generated data structure:

The parsing of files is performed with [parser classes](#parser-classes), rather than stand-alone functions, that allow extending the lifetime of the input data beyond the call to the `parse()`-function.

```c++
pegtl::file_parser p( "path/to/file" );
something_with_pointers s;
p.parse< my_grammar >( s );
// p still exists and s is safe to contain
// pointers to the input data stored in p.
```

## Parser Functions

All of the following parser functions reside in namespace `pegtl`.

#### Input Parser Functions

The first functions take an already initialised instance of an input class and parses it with the given `Rule`, `Action` and `Control`.

```c++
template< typename Rule,
          template< typename ... > class Action = nothing,
          template< typename ... > class Control = normal,
          typename Input,
          typename ... States >
bool parse_input( Input & in,
                  States && ... st );

template< typename Rule,
          template< typename ... > class Action = nothing,
          template< typename ... > class Control = normal,
          typename Outer,
          typename Input,
          typename ... States >
bool parse_input_nested( const Outer & oi,
                         Input & in,
                         States && ... st );
```

#### Memory Parser Functions

The following parse functions only keep a reference to a memory block and **do not copy** the input data!

```c++
template< typename Rule,
          template< typename ... > class Action = nothing,
          template< typename ... > class Control = normal,
          typename ... States >
bool parse_memory( const char * data,
                   const char * dend,
                   const char * source,
                   States && ... st );

template< typename Rule,
          template< typename ... > class Action = nothing,
          template< typename ... > class Control = normal,
          typename ... States >
bool parse_memory( const char * data,
                   const std::size_t size,
                   const char * source,
                   States && ... st );

template< typename Rule,
          template< typename ... > class Action = nothing,
          template< typename ... > class Control = normal,
          typename ... States >
bool parse_string( const std::string & data,
                   const std::string & source,
                   States && ... st );

template< typename Rule,
          template< typename ... > class Action = nothing,
          template< typename ... > class Control = normal,
          typename ... States >
bool parse_arg( const int argn,
            char ** argv,
            States && ... st );
```

#### Nested Memory Parser Functions

```c++
template< typename Rule,
          template< typename ... > class Action = nothing,
          template< typename ... > class Control = normal,
          typename Outer,
          typename ... States >
bool parse_memory_nested( const Outer & io,
                          const char * data,
                          const char * dend,
                          const char * source,
                          States && ... st );

template< typename Rule,
          template< typename ... > class Action = nothing,
          template< typename ... > class Control = normal,
          typename Outer,
          typename ... States >
bool parse_memory_nested( const Outer & io,
                          const char * data,
                          const std::size_t size,
                          const char * source,
                          States && ... st );

template< typename Rule,
          template< typename ... > class Action = nothing,
          template< typename ... > class Control = normal,
          typename Outer,
          typename ... States >
bool parse_string_nested( const Outer & io,
                          const std::string & data,
                          const std::string & source,
                          States && ... st );
```

#### Stream Parser Functions

Please read [Incremental Input](Incremental-Input.md) for details on the `maximum` argument, and **how to prepare a grammar for incremental input support** using the `discard`-rule.

* The `parse_stdin()` function calls `parse_cstream()` with the appropriate arguments.
* The `parse_cstring()` function performs incremental parsing until a terminating `\0`-byte is found, it does not call `strlen()` or similar to determine the length of the string (as would be required for one of the memory parser functions above).

```c++
template< typename Rule,
          template< typename ... > class Action = nothing,
          template< typename ... > class Control = normal,
          typename ... States >
bool parse_stdin( const std::size_t maximum,
                  States && ... st );

template< typename Rule,
          template< typename ... > class Action = nothing,
          template< typename ... > class Control = normal,
          typename ... States >
bool parse_cstream( std::FILE * stream,
                    const char * source,
                    const std::size_t maximum,
                    States && ... st );

template< typename Rule,
          template< typename ... > class Action = nothing,
          template< typename ... > class Control = normal,
          typename ... States >
bool parse_cstring( const char * string,
                    const char * source,
                    const std::size_t maximum,
                    States && ... st );

template< typename Rule,
          template< typename ... > class Action = nothing,
          template< typename ... > class Control = normal,
          typename ... States >
bool parse_istream( std::istream & stream,
                    const std::string & source,
                    const std::size_t maximum,
                    States && ... st );
```

#### Nested Stream Parser Functions

```c++
template< typename Rule,
          template< typename ... > class Action = nothing,
          template< typename ... > class Control = normal,
          typename Outer,
          typename ... States >
bool parse_cstream_nested( const Outer & oi,
                           std::FILE * stream,
                           const char * source,
                           const std::size_t maximum,
                           States && ... st );

template< typename Rule,
          template< typename ... > class Action = nothing,
          template< typename ... > class Control = normal,
          typename Outer,
          typename ... States >
bool parse_cstring_nested( const Outer & oi,
                           const char * string,
                           const char * source,
                           const std::size_t maximum,
                           States && ... st );

template< typename Rule,
          template< typename ... > class Action = nothing,
          template< typename ... > class Control = normal,
          typename Outer,
          typename ... States >
bool parse_istream_nested( const Outer & oi,
                           std::istream & stream,
                           const std::string & source,
                           const std::size_t maximum,
                           States && ... st );
```

## Tracer Functions

The header file `pegtl/trace.hh` contains a [control class](Control-Hooks.md) called `tracer` that shows how to debug a grammar by obtaining a trace on which grammar rule is matched where and with which result during a parsing run.

For convenience, a group of functions called `trace_foo()` is included, too, that behaves exactly like the corresponding `parse_foo()`-functions explained above, except that the template argument `Control` is removed and the `parse()`-functions are called with `tracer` as control class.

To see the tracing in action, the supplied example `examples/uri_trace.cc` can be used; the compiled binary can be found in `build/examples/uri_trace` after building the PEGTL examples and unit tests by invoking `make`.

## Parser Classes

All of the following parser classes reside in namespace `pegtl`.

The parser class `file_parser` allows parsing the contents of a file.

It is implemented as a class, rather than a function, in order to retain the to-be-parsed data across the call to the `parse()`-function so that, in case of an exception while parsing, it is possible to access the data and use it for error messages.

The (template) arguments to the `parse()` member function are the same as for the stand-alone `parse()` functions above.

The `input` returned by the `input()` method represents the *remaining* data from the file, more precisely:

- After the constructor was called it represents the complete file data;
- each time `parse()` is called, and data is consumed by that parsing run, the input is advanced.

Advancing the input consists of advancing the pointer to the beginning of the data, and decrementing the size accordingly.

```c++
class file_parser
{
public:
   explicit
   file_parser( const std::string & filename );

   // Returns the source of the data, i.e. the filename.
   const std::string & source() const;

   // Returns an input for the remaining content of the file.
   const pegtl::input & input() const;

   template< typename Rule,
             template< typename ... > class Action = nothing,
             template< typename ... > class Control = normal,
             typename ... States >
   bool parse( States && ... st );

   template< typename Rule,
             template< typename ... > class Action = nothing,
             template< typename ... > class Control = normal,
             typename Outer,
             typename ... States >
   bool parse_nested( const Outer & io, States && ... st );
};
```

###### Implementation

The interface shown above is actually of the two classes `read_parser` and `mmap_parser`.
Their interfaces are identical and the only difference is with how the file is accessed:

- Class `read_parser` reads with `read(2)` and caches the entire content of the file when it is constructed.
- Class `mmap_parser` uses `mmap(2)` to map the contents of the file and only caches the file size in the constructor.

The `mmap_parser` is only available when compiling on Unix or Mac OS X, in which case `file_parser` is an alias for `mmap_parser`. Otherwise `file_parser` is an alias for `read_parser`.

It is also possible to directly use the classes `read_parser` and `mmap_parser` as such, taking into account that the latter will not always be available.

Copyright (c) 2014-2016 Dr. Colin Hirsch and Daniel Frey
