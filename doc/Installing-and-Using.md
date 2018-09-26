# Installing and Using

## Contents

* [Requirements](#requirements)
* [Installation Packages](#installation-packages)
* [CMake Installation](#cmake-installation)
* [Manual Installation](#manual-installation)
* [Embedding the PEGTL](#embedding-the-pegtl)
  * [Embedding in Binaries](#embedding-in-binaries)
  * [Embedding in Libraries](#embedding-in-libraries)
  * [Embedding in Library Interfaces](#embedding-in-library-interfaces)

## Requirements

The PEGTL requires a C++11-capable compiler, e.g. one of

* GCC 4.8
* Clang 3.4
* Visual Studio 2015

on either

* Linux
* macOS
* Windows

It requires C++11, e.g. using the `--std=c++11` compiler switch.
Using newer versions of the C++ standard is supported.

It should also work with other C++11 compilers on other Unix systems (or any sufficiently compatible platform).

The PEGTL is written with an emphasis on clean code and is compatible with
the `-pedantic`, `-Wall`, `-Wextra` and `-Werror` compiler switches.

## Installation Packages

Installation packages are available from several package managers. Note that some of the listed packages are not updated regularly.

[![Packaging status](https://repology.org/badge/vertical-allrepos/pegtl.svg)](https://repology.org/metapackage/pegtl)

### Other

* [Conan]
* [Spack]

## CMake Installation

The PEGTL can be built and installed using [CMake], e.g.

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

in the above. For more options and ways to use CMake, please refer to the [CMake documentation].

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

The `Makefile` is as simple as possible, but should manage to build the examples
and unit tests on Linux with GCC and on macOS with Clang (as supplied by Apple).
When running into problems using other combinations, please consult the `Makefile`
for customising the build process.

## Embedding the PEGTL

When embedding the PEGTL into other projects, several problems may come up
due to the nature of C++ header-only libraries. Depending on the scenario,
there are various ways of working around these problems.

### Embedding in Binaries

When creating application binaries, i.e. executable files, the PEGTL source
tree can be copied to some subdirectory in the application source, and added
to the compiler's or project's include paths. No further changes are needed.

### Embedding in Libraries

When writing libraries with the PEGTL, it has to be ensured that applications
that are built with these libraries, and that themselves use the PEGTL, do not
violate the One Definition Rule (ODR) as would be the case when application
and libraries contain different versions of the PEGTL.

Since the PEGTL does *not* guarantee ABI compatibility, not even across minor
or patch releases, libraries *have* to ensure that the symbols for the PEGTL
they include differ from those of the applications that use them.

This can be achieved by changing the macro `TAO_PEGTL_NAMESPACE` which, by
default, is set to `pegtl`, which leads to all symbols residing in namespace
`tao::pegtl`. To change the namespace, simply define `TAO_PEGTL_NAMESPACE`
to a unique name before including the PEGTL, for example:

```c++
#define TAO_PEGTL_NAMESPACE mylib_pegtl

#include <tao/pegtl.hpp>
#include <tao/contrib/json.hpp>

int main( int argc, char* argv[] )
{
   if( argc > 1 ) {
     tao::mylib_pegtl::argv_input<> in( argv, 1 );
     tao::mylib_pegtl::parse< tao::mylib_pegtl::json::text >( in );
   }
}

```

### Embedding in Library Interfaces

When PEGTL headers are included in headers of a library, setting the namespace
to a unique name via `TAO_PEGTL_NAMESPACE` is not sufficient since both the
application's and the library's copy of the PEGTL use the same macro names.

In this case it is necessary to change the prefix of all macros of the embedded
PEGTL from `TAO_PEGTL_` to another unique string in order to prevent macros
from clashing. In a Unix-shell, the following commands will achieve this:

```sh
$ sed -i 's/TAO_PEGTL_/MYLIB_TAO_PEGTL_/g' $(find -name '[^.]*.[hc]pp')
$ sed -i 's/TAOCPP_PEGTL_/MYLIB_TAOCPP_PEGTL_/g' $(find -name '[^.]*.[hc]pp')
```

The above commands needs to run from the top-level directory of the embedded
PEGTL. Additionally, `MYLIB_TAO_PEGTL_NAMESPACE` needs to be set as explained
above; alternatively `include/tao/pegtl/config.hpp` can be directly modified.

Note that the second command is only needed because the PEGTL contains
some compatibility macros for older versions that start with `TAOCPP_PEGTL_`
instead of `TAO_PEGTL_`. Those older compatibility macros will be removed
in version 3.0. Starting with version 2.4.0 you should only use macros
that start with `TAO_PEGTL_`.

A practical example of how the result looks like can be found in our own
header-only [JSON library](https://github.com/taocpp/json/).

Copyright (c) 2014-2018 Dr. Colin Hirsch and Daniel Frey

[CMake]: https://cmake.org/
[CMake documentation]: https://cmake.org/documentation/
[Conan]: https://bintray.com/taocpp/public-conan/pegtl%3Ataocpp
[Spack]: http://spack.readthedocs.io/en/latest/package_list.html#pegtl
