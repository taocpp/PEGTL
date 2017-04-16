# Inputs and Parsing

Performing a parsing run consists of two steps:

1. Constructing an *input* class that represents the to-be-parsed data.
2. Calling a PEGTL *parse* function with, among other things, the input.

All classes and functions on this page are in namespace `tao::pegtl`.

## Contents

* [File Inputs](#file-inputs)
* [Memory Input](#memory-input)
* [Stream Inputs](#stream-inputs)
* [Parse Function](#parse-function)
* [Nested Parsing](#nested-parsing)
* [Other Functions](#other-functions)

## File Inputs

The classes `tao::pegtl::file_input`, `tao::pegtl::read_input` and, on supported platforms, `tao::pegtl::mmap_input` can be used to parse the contents of a file.

* `read_input` uses `read(3)`, or `std::read()`.
* `mmap_input` uses `mmap(2)` and is available on POSIX.
* `file_input` is an alias, either for `mmap_input` when available, and `read_input` otherwise.

All file input classes take a single argument, the filename, which can be supplied either as `std::string` or as `const char*`.
The `read_input` reads the entire contents of the file upon construction.

## Memory Input

TODO

## Stream Inputs

The PEGTL includes input classes to parse data incrementally from C-streams (`std::FILE*`) and C++-streams (`std::istream`), respectively.

Please read [Incremental Input](Incremental-Input.md) for details on the `maximum` argument, and **how to prepare a grammar for incremental input support** using the `discard`-rule.

```c++
template< typename Eol = lf_crlf_eol >
struct cstream_input
{
   cstream_input( std::FILE* in_stream, const std::size_t in_maximum, const char* in_source );
};

template< typename Eol = lf_crlf_eol >
struct istream_input
{
   istream_input( std::istream& in_stream, const std::size_t in_maximum, const char* in_source )
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
          template< typename ... > class Action = nothing,
          template< typename ... > class Control = normal,
          apply_mode A = apply_mode::ACTION,
          rewind_mode M = rewind_mode::REQUIRED,
          typename Input,
          typename ... States >
bool parse( Input & in,
            States && ... st );
```

## Nested Parsing

Nested parsing refers to an (inner) parsing run that is performed "in the middle of" another (outer) parsing run, for example when one file "includes" another file.

The difference to the regular `tao::pegtl::parse()` function is that `tao::pegtl::parse_nested()` takes care of adding to the `std::vector` of `tao::pegtl::position` objects in the exception class `tao::pegtl::parse_error`.
This allows generating error messages of the form "error in file F1 line L1 included from file F2 line L2...".

Calling `parse_nested()` requires one additional argument compared to `parse()`, the input from the outer parsing run as first argument.
Everything else remains the same.

```c++
template< typename Rule,
          template< typename ... > class Action = nothing,
          template< typename ... > class Control = normal,
          apply_mode A = apply_mode::ACTION,
          rewind_mode M = rewind_mode::REQUIRED,
          typename Outer,
          typename Input,
          typename ... States >
bool parse_nested( const Outer & oi,
                   Input & in,
                   States && ... st );
```

## Other Functions

The function `tao::pegtl::parse_arg()` declared in the header `<tao/pegtl/parse_arg.hpp>` is a convenience function to parse the command line argument `argv[ argn ]`.
It sets the *source* for the position to `"arg[n]"` where `n` corresponds to `argn`.

```c++
template< typename Rule,
          template< typename... > class Action = nothing,
          template< typename... > class Control = normal,
          typename Eol = lf_crlf_eol,
          tracking_mode P = tracking_mode::IMMEDIATE,
          apply_mode A = apply_mode::ACTION,
          rewind_mode M = rewind_mode::REQUIRED,
          typename... States >
bool parse_arg( const int argn,
                char** argv,
                States&&... st );
```

Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
