# Parser Reference

All parser functions and classes make use of the following template parameters and arguments:

- `Rule` is the class that represents [the top-level parsing rule of the grammar](Rules-and-Grammars) to be parsed.
- `Action<>` is the class template [specialised for user-defined actions during the parsing run](Actions-and-States).

- `Control<>` is the class template that [controls debugging and throwing of exceptions](Control-Hooks).
- `States` are [the additional arguments that are forwarded to all actions](Actions-and-States) and [control hooks](Control-Hooks).

The `Rule` class *has* to be provided in order to have something to match against while parsing, some `Action<>` *usually* has to be provided (to actually *do* something while parsing, rather than only checking adherence of the input data to the grammar), whereas for `Control<>`, at least initially, the provided default will often be sufficient.

## Contents

  * [Input and Errors](#input-and-errors)
  * [Parser Functions](#parser-functions)
  * [Tracer Functions](#tracer-functions)
  * [Parser Classes](#parser-classes)

## Input and Errors

During a parsing run, the PEGTL uses the `pegtl::input` class to reference, and pass around references to, (portions of) the input data.

When dealing with or implementing *any* function that is called with an instance of class `input`, or internally uses one like the `pegtl::parse()`-functions, it has to be taken into consideration that

1. the `input` class does *not* copy the data it references, it only keeps pointers, therefore
2. the parser functions do *not* copy the data they parse, therefore
3. it depends on the caller of the top-most function whether and for how long the data used during a parsing run is valid afterwards.

This holds true for `parse()`-functions, rules (`match()`-functions), actions (`apply()`-functions) and all [control-hooks](Control-Hooks).

The two places that most frequently need to store and/or reference portions of the input beyond the parsing run are actions and errors.

**For actions**, this happens for example when an abstract syntax tree is generated.
Some of the syntax tree nodes will contain portions of the input, for example for a variable name in a script language that needs to be stored in the syntax tree just as it occurs in the input data.

The **default safe choice** is to copy the matched portions of the input data that are passed to an action as instance of class `input` by storing it as `std::string` in the data structures built while parsing.
This ensures that a deep copy of the data is performed, rather than storing a reference to it.

In some cases it might be necessary to use highly optimised data structures that do not copy the input data, which makes it necessary to ensure that the lifetime of the input data, i.e. the validity of all pointers and references to the input data, exceeds the lifetime of the generated data structure, similar to the handling of errors:

**For errors**, this happens because the class `pegtl::parse_error` contains a vector of `position_info` objects whose `begin` data member is a pointer to the input data.
It indicates at which position in the input the parse error occurred, i.e. the first byte of the input that was not previously consumed (by a parsing/grammar rule).

When the code surrounding a `parse()`-function catches such an exception and proceeds to use the `begin` pointer to output the offending portion of the input in a diagnostic message, it **has to make sure that the data is still valid**!

The parsing of files is performed with parser classes, rather than stand-alone functions, that allow extending the lifetime of the input data beyond the call to the `parse()`-function:

```c++
pegtl::read_parser p( "path/to/file" );
try {
   p.parse< my_grammar >( ... );
}
catch ( const pegtl::parse_error & e ) {
   // p still exists, so all references to the data are valid.
}
```

## Parser Functions

All of the following parser functions reside in namespace `pegtl`.

The first parser function takes an already initialised `input` instance and parses it with the given `Rule`, `Action` and `Control`.

```c++
template< typename Rule,
          template< typename ... > class Action = nothing,
          template< typename ... > class Control = normal,
          typename Input,
          typename ... States >
bool parse_input( Input & in,
                  States && ... st );
```

**All** of the following functions that parse a memory block only keep a pointer and do **not** copy the data!

The following parser functions parse a given memory block (from which the required `input` instance is created internally).

The first two functions should be self-explanatory, the third function uses `data.data()` and `data.size()`, and the final function uses `argv[ argn ]` as pointer to the block and calls `::strlen()` to determine its length.

```c++
template< typename Rule,
          template< typename ... > class Action = nothing,
          template< typename ... > class Control = normal,
          typename ... States >
bool parse( const char * data,
            const char * dend,
            const char * source,
            States && ... st );

template< typename Rule,
          template< typename ... > class Action = nothing,
          template< typename ... > class Control = normal,
          typename ... States >
bool parse( const char * data,
            const std::size_t size,
            const char * source,
            States && ... st );

template< typename Rule,
          template< typename ... > class Action = nothing,
          template< typename ... > class Control = normal,
          typename ... States >
bool parse( const std::string & data,
            const std::string & source,
            States && ... st );

template< typename Rule,
          template< typename ... > class Action = nothing,
          template< typename ... > class Control = normal,
          typename ... States >
bool parse( const int argn,
            char ** argv,
            States && ... st );
```

The final parser functions are for nested parsing, i.e. when starting a parsing run within an already active one.
The `nest` argument is only required to generate error messages that take into account the nested parsing.

For example when parsing a file A that includes a file B, any error messages and error positions in the `parse_error` class should indicate both the position in B where the actual error occurred as well as the position in A where file B was included and parsed.

```c++
template< typename Rule,
          template< typename ... > class Action = nothing,
          template< typename ... > class Control = normal,
          typename Input,
          typename ... States >
bool parse_nested( const Input & nest,
                   const char * data,
                   const char * dend,
                   const char * source,
                   States && ... st );

template< typename Rule,
          template< typename ... > class Action = nothing,
          template< typename ... > class Control = normal,
          typename Input,
          typename ... States >
bool parse_nested( const Input & nest,
                   const char * data,
                   const std::size_t size,
                   const char * source,
                   States && ... st );

template< typename Rule,
          template< typename ... > class Action = nothing,
          template< typename ... > class Control = normal,
          typename Input,
          typename ... States >
bool parse_nested( const Input & nest,
                   const std::string & data,
                   const std::string & source,
                   States && ... st );
```

## Tracer Functions

The header file `pegtl/trace.hh` contains a [control class](Control-Hooks) called `tracer` that shows how to debug a grammar by obtaining a trace on which grammar rule is matched where and with which result during a parsing run.

For convenience, a function called `trace()` is included, too, that behaves exactly like the `parse()`-functions explained above, except that the template argument `Control` is removed and the `parse()`-functions are called with `tracer` as control class.

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
   file_parser( const std::string & filename,
                const input & from )

   // Returns the source of the data, i.e. the filename.
   const std::string & source() const;

   // Returns an input for the remaining content of the file.
   const pegtl::input & input() const;

   template< typename Rule,
             template< typename ... > class Action = nothing,
             template< typename ... > class Control = normal,
             typename ... States >
   bool parse( States && ... st );
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
