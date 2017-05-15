# Embedding the PEGTL

When embedding the PEGTL into other projects, several problem may come up
due to the nature of C++ header-only libraries. Depending on the scenario,
there are various ways of working around these problems.

## Embedding in Binaries

When creating application binaries, i.e. executable files, the PEGTL source
tree can be copied to some subdirectory in the application source, and added
to the compiler's or project's include paths. No further changes are needed.

## Embedding in Libraries

When writing libraries with the PEGTL, it has to be ensured that applications
that are built with these libraries, and that themselves use the PEGTL, do not
violate the One Definition Rule (ODR) as would be the case when application
and libraries contain different versions of the PEGTL.

Since the PEGTL does *not* guarantee ABI compatibility, not even across minor
or patch releases, libraries *have* to ensure that the symbols for the PEGTL
they include differ from those of the applications that use them.

This can be achieved by changing the macro `TAOCPP_PEGTL_NAMESPACE` which, by
default, is set to `pegtl`, which leads to all symbols residing in namespace
`tao::pegtl`. To change the namespace, simply define `TAOCPP_PEGTL_NAMESPACE`
to a unique name before including the PEGTL, for example:

```c++
#define TAOCPP_PEGTL_NAMESPACE mylib_pegtl

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

## Embedding in Library Interfaces

When PEGTL headers are included in headers of a library, setting the namespace
to a unique name via `TAOCPP_PEGTL_NAMESPACE` is not sufficient since both the
application's and the library's copy of the PEGTL use the same macro names.

In this case it is necessary to change the prefix of all macros of the embedded
PEGTL from `TAOCPP_PEGTL_` to another unique string in order to prevent macros
from clashing. In a Unix-shell, the following command will achieve this:

```sh
$ sed -i 's/TAOCPP_PEGTL_/MYLIB_PEGTL_/g' $(find -name '[^.]*.[hc]pp')
```

The above command needs to run from the top-level directory of the embedded PEGTL.
Additionally, `MYLIB_PEGTL_NAMESPACE` needs to be set as explained above;
alternatively `include/tao/pegtl/config.hpp` can be directly modified.

A practical example of how the result looks like can be found in our own
header-only [JSON library](https://github.com/taocpp/json/).

Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
