# Installing and Using

## Contents

* [Requirements](#requirements)
* [Installation Packages](#installation-packages)
* [Using CMake](#using-cmake)
  * [CMake Installation](#cmake-installation)
  * [`find_package`](#find_package)
  * [`add_subdirectory`](#add_subdirectory)
  * [Mixing `find_package` and `add_subdirectory`](#mixing-find_package-and-add_subdirectory)
* [Conan](#conan)
  * [Using a development version](#using-a-development-version)
  * [Developing with conan editable](#developing-with-conan-editable)
* [Manual Installation](#manual-installation)
* [Embedding the PEGTL](#embedding-the-pegtl)
  * [Embedding in Binaries](#embedding-in-binaries)
  * [Embedding in Libraries](#embedding-in-libraries)
  * [Embedding in Library Interfaces](#embedding-in-library-interfaces)
* [Single Header Version](#single-header-version)

## Requirements

The PEGTL requires a C++17-capable compiler, e.g. one of

* GCC 7
* Clang 5
* Visual Studio 2017

on either

* Linux
* macOS
* Windows

It requires C++17, e.g. using the `--std=c++17` compiler switch.
Using newer versions of the C++ standard is supported.

Larger projects will frequently require the `/bigobj` option when compiling with Visual Studio on Windows.

It should also work with other C++17 compilers on other Unix systems (or any sufficiently compatible platform).

The PEGTL is written with an emphasis on clean code and is compatible with
the `-pedantic`, `-Wall`, `-Wextra` and `-Werror` compiler switches.

## Installation Packages

Installation packages are available from several package managers. Note that some of the listed packages are not updated regularly.

[![Packaging status](https://repology.org/badge/vertical-allrepos/pegtl.svg)](https://repology.org/metapackage/pegtl)

### Other

* [Conan]
* [Spack]

## Using CMake

### CMake Installation

The PEGTL can be built and installed using [CMake], e.g.

```sh
$ mkdir build
$ cd build
$ cmake ..
$ make
$ make install
```

The above will install the PEGTL into the standard installation path on a UNIX
system, e.g. `/usr/local/include/`. To change the installation path, use:

```sh
$ cmake .. -DCMAKE_INSTALL_PREFIX=../install
```

in the above.

### `find_package`

Installation creates a `pegtl-config.cmake` which allows CMake
projects to find the PEGTL using `find_package`:

```cmake
find_package(pegtl)
```

This exports the `taocpp::pegtl` target which can be linked against any other
target. Linking against `taocpp:pegtl` automatically sets the include
directories and required flags for C++17 or later. For example:

```cmake
add_executable(myexe mysources...)
target_link_libraries(myexe PRIVATE taocpp::pegtl)
```

### `add_subdirectory`

The PEGTL can also be added as a dependency with `add_subdirectory`:

```cmake
add_subdirectory(path/to/PEGTL)
```

This also exports the `taocpp::pegtl` target which can be linked against any
other target just as with the installation case.

Due to the global nature of CMake targets the target `pegtl` is also defined,
but only `taocpp::pegtl` should be used for consistency.

If `PEGTL_BUILD_TESTS` is true then the test targets, `pegtl-test-*`, are also
defined and their corresponding tests registered with `add_test`.
If `PEGTL_BUILD_EXAMPLES` is true then the example targets, `pegtl-example-*`,
are also defined.

### Mixing `find_package` and `add_subdirectory`

With the advent of improved methods of managing dependencies (such as [Conan],
[Spack], [CMake FetchContent]), multiple package inclusion methods needs to be
able to co-exist.

If PEGTL was first included with `find_package` then subsequent calls to
`add_subdirectory(path/to/PEGTL)` will skip over the body of the
`CMakeLists.txt` and use the installed package if the version matches.
If the version does not match a fatal error will be signalled.

If PEGTL was first included with `add_subdirectory` then a dummy
`pegtl-config.cmake` is created and `pegtl_DIR` is set. Subsequent calls to
`find_package(pegtl)` will then use the already added package if the version
matches. If the version does not match a fatal error will be signalled.

Since CMake targets are global, there exists no way for a CMake project to use
two different versions of PEGTL simultaneously and signalling a fatal error
becomes the only practical way of handling the inclusion of multiple different
PEGTL versions.

For more options and ways to use CMake, please refer to the
[CMake documentation].

Conan
---

The [PEGTL conan package](https://bintray.com/taocpp/public-conan/pegtl%3Ataocpp) is automatically updated when a release is made, and it should always be up to date with the latest official release.

Simply add

```ini
pegtl/<version>@taocpp/stable
```

as a dependency to your conan project where `<version>` is the version of PEGTL you want to use.

### Using a development version

If a not yet released PEGTL version is required, then PEGTL can be exported in its current state.

```bash
mkdir build
conan install -if build
conan export-pkg -if build -bf build . pegtl/<version>@taocpp/devel
```

Then proceed by adding

```ini
pegtl/<version>@taocpp/devel
```

as a dependency to your conan project.

### Developing with conan editable

If it is required to develop PEGTL alongside another library/application then the package can be put into editable mode with

```bash
conan editable add . pegtl/<version>@taocpp/devel
```

If the editable layout has `[builddirs]` set correctly and one is using the `cmake_paths` or `cmake`generator

```cmake
find_package(pegtl)
```

will work as expected. It will find the editable package and add it to the current CMake project. An editable package implies that it is under development so tests and examples will be automatically built unless `PEGTL_BUILD_TESTS` and `PEGTL_BUILD_EXAMPLES` are turned off.

Caveats with the editable package:

- Currently, if the package is included with `CONAN_PKG::pegtl` or used in a build system other than CMake then the tests and examples won't be built as the CMake config script is bypassed.
- CMake will compulsively rebuild tests and examples if the build directory is reconfigured from another directory.

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
default, is set to `tao::pegtl`. To change the namespace, simply define
`TAO_PEGTL_NAMESPACE` to a unique name before including the PEGTL, for example:

```c++
#define TAO_PEGTL_NAMESPACE mylib::pegtl

#include <tao/pegtl.hpp>
#include <tao/contrib/json.hpp>

int main( int argc, char* argv[] )
{
   if( argc > 1 ) {
     mylib::pegtl::argv_input in( argv, 1 );
     mylib::pegtl::parse< mylib::pegtl::json::text >( in );
   }
}

```

### Embedding in Library Interfaces

When PEGTL headers are included in headers of a library, setting the namespace
to a unique name via `TAO_PEGTL_NAMESPACE` is not sufficient since both the
application's and the library's copy of the PEGTL use the same macro names.

In this case it is necessary to change the prefix of all macros of the embedded
PEGTL from `TAO_PEGTL_` to another unique string in order to prevent macros
from clashing. In a Unix-shell, the following command will achieve this:

```sh
$ sed -i 's/TAO_PEGTL_/MYLIB_PEGTL_/g' $(find -name '[^.]*.[hc]pp')
```

The above command needs to run from the top-level directory of the embedded
PEGTL. Additionally, `MYLIB_PEGTL_NAMESPACE` needs to be set as explained
above; alternatively `include/tao/pegtl/config.hpp` can be directly modified.

A practical example of how the result looks like can be found in our own
header-only JSON library [taoJSON](https://github.com/taocpp/json/).

## Single Header Version

You can generate a single-header-version of the PEGTL with the included `Makefile`.
In a Unix-shell, the following command will achieve this:

```sh
$ make amalgamate
```

The above will generate a `build/amalgamated/pegtl.hpp` which will consist of the headers `tao/pegtl.hpp`, `tao/pegtl/analyze.hpp`, their dependencies, and all headers in `tao/pegtl/contrib/` except for the headers in `tao/pegtl/contrib/icu/`.

Copyright (c) 2014-2019 Dr. Colin Hirsch and Daniel Frey

[CMake]: https://cmake.org/
[CMake documentation]: https://cmake.org/documentation/
[CMake FetchContent]: https://cmake.org/cmake/help/latest/module/FetchContent.html
[Conan]: https://bintray.com/taocpp/public-conan/pegtl%3Ataocpp
[Spack]: http://spack.readthedocs.io/en/latest/package_list.html#pegtl
