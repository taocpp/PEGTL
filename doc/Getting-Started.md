# Getting Started

This page should contain everything to get you started with the PEGTL.

## Contents

* [Hello, world!](#hello-world)
* [Requirements](#requirements)
* [Installation Packages](#installation-packages)
* [CMake Installation](#cmake-installation)
* [Manual Installation](#manual-installation)
* [Embedding the PEGTL](#embedding-the-pegtl)
* [Limitations](#limitations)

## Hello, world!

Since the PEGTL is a parser library, here is an "inverse hello world" example that parses,
rather than prints, the string `Hello, foo!` for any sequence of alphabetic ASCII characters `foo`.

```c++
// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <string>
#include <iostream>

#include <tao/pegtl.hpp>

namespace hello
{
   // Parsing rule that matches a literal "Hello, ".

   struct prefix
      : tao::pegtl::string< 'H', 'e', 'l', 'l', 'o', ',', ' ' >
   {};

   // Parsing rule that matches a non-empty sequence of
   // alphabetic ascii-characters with greedy-matching.

   struct name
      : tao::pegtl::plus< tao::pegtl::alpha >
   {};

   // Parsing rule that matches a sequence of the 'prefix'
   // rule, the 'name' rule, a literal "!", and 'eof'
   // (end-of-file/input), and that throws an exception
   // on failure.

   struct grammar
      : tao::pegtl::must< prefix, name, tao::pegtl::one< '!' >, tao::pegtl::eof >
   {};

   // Class template for user-defined actions that does
   // nothing by default.

   template< typename Rule >
   struct action
      : tao::pegtl::nothing< Rule >
   {};

   // Specialisation of the user-defined action to do
   // something when the 'name' rule succeeds; is called
   // with the portion of the input that matched the rule.

   template<>
   struct action< name >
   {
      template< typename Input >
      static void apply( const Input& in, std::string& name )
      {
         name = in.string();
      }
   };

} // hello

int main( int argc, char* argv[] )
{
   if( argc > 1 ) {
      // Start a parsing run of argv[1] with the string
      // variable 'name' as additional argument to the
      // action; then print what the action put there.

      std::string name;
      tao::pegtl::argv_input<> in( argv, 1 );
      tao::pegtl::parse< hello::grammar, hello::action >( in, name );
      std::cout << "Good bye, " << name << "!" << std::endl;
   }
}
```

Assuming you are in the main directory of the PEGTL, the above source can be
found in the `src/example/pegtl/` directory. Compile the program with something like

```sh
$ g++ --std=c++11 -Iinclude src/example/pegtl/hello_world.cpp -o hello_world
```

and then invoke it as follows:

```sh
$ ./hello_world 'Hello, world!'
Good bye, world!
$ ./hello_world 'Hello, Colin!'
Good bye, Colin!
$ ./hello_world 'Howdy, Paula!'
terminate called after throwing an instance of 'tao::pegtl::parse_error'
  what():  argv[1]:1:0(0): parse error matching hello::prefix
Aborted (core dumped)
```

Frequently an application will include a top-level `try-catch` block to handle
the exception.

## Requirements

The PEGTL requires a C++11-capable compiler, e.g. one of

* GCC 4.8
* Clang 3.4
* Visual Studio 2015

on either

* GNU/Linux
* macOS
* Windows

It requires C++11, e.g. using the `--std=c++11` compiler switch.
Using newer versions of the C++ standard is supported.

It might also work with other compilers with good C++11 support on any sufficiently
compatible platform.

The PEGTL is written with an emphasis on clean code and is compatible with
the `-pedantic`, `-Wall`, `-Wextra` and `-Werror` compiler switches.

## Installation Packages

### Linux Packages

* [Fedora/RHEL/CentOS](https://apps.fedoraproject.org/packages/PEGTL)
* [Debian](https://packages.debian.org/search?keywords=pegtl-dev)
* [Ubuntu](http://packages.ubuntu.com/search?keywords=pegtl-dev)
* [Gentoo](https://packages.gentoo.org/packages/dev-libs/pegtl)

Packages for other distributions might be available, too.

### macOS Packages

* [Homebrew](http://brewformulas.org/Pegtl)

## CMake Installation

The PEGTL can be built and installed using [CMake](https://cmake.org/), e.g.

```sh
$ mkdir build
$ cd build
$ cmake ..
$ make
$ make install
```

The above will install the PEGTL into the standard installation path on a
UNIX system, e.g. `/usr/local/include/`. To change the installation path, use:

```sh
$ cmake .. -DCMAKE_INSTALL_PREFIX=../install
```

in the above. For more options and ways to use CMake, please refer to the [CMake documentation](https://cmake.org/documentation/).

## Manual Installation

Since the PEGTL is a header-only library, _it doesn't itself need to be compiled_.
In terms of installation for use in other projects, the following steps are required.

- The `include/` directory and the `LICENSE` file should be copied somewhere, e.g.

  - to `/usr/local/include/` in order to use it system-wide, or
  - to some appropriate directory within your project,

- A compatible compiler with appropriate compiler switches must be used.
- The compiler search-path for include files must include (no pun intended)
  the directory that contains the `tao/pegtl/` directory and `tao/pegtl.hpp` header.

The `Makefile` and `.cpp`-files included in the PEGTL distribution archive serve
as practical examples on how to develop grammars and applications with the PEGTL.
Invoking `make` in the main PEGTL directory builds all included example programs
and builds and runs all unit tests.

## Embedding the PEGTL

To avoid ODR violations, some precautions are necessary when embedding the PEGTL
source directly in your code or when building libraries, see [Embedding the PEGTL](Embedding.md).

## Limitations

The `Makefile` is as simple as possible, but should manage to build the examples
and unit tests on Linux with GCC and on Mac OS X with Clang (as supplied by Apple).
When running into problems using other combinations, please consult the `Makefile`
for customising the build process.

The following limitations exist when **not** compiling with GCC or Clang.

- The demangling of class names for error messages is not performed (the mangled name is used instead).

The following limitations exist when **not** compiling on Unix or Mac OS X.

- `::mmap(2)`-based file reading is not included (but `std::fread(3)`-based reading is).

Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
