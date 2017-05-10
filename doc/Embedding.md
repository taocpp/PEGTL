# Embedding the PEGTL

When embedding the PEGTL into your projects, several problem may come up
due to the nature of C++ header-only libraries. Depending on the szenario,
the PEGTL offers three major solutions.

## Embedding the PEGTL in binaries

When you are writing binaries, i.e. executable files, you can copy the
source tree to some subdirectory and simply add it to the compilers or
projects include paths. No further changes are needed.

## Embedding the PEGTL in library code

When you are writing libraries and you are using the PEGTL, you need to
take care of ODR violations if a user of your library also uses the PEGTL,
but with a slightly different version. Since the PEGTL does *not* guarantee
ABI compatibility between versions (even between minor- or patch-releases),
one needs to make sure that all symbols of the two different PEGTL header
sets are distinct. To support this use case, the PEGTL does support setting
`TAOCPP_PEGTL_NAMESPACE`. By default, it will be set to `pegtl`, result in
all symbols residing in the namespace `tao::pegtl`.

If you want to change the namespace name, simply define `TAOCPP_PEGTL_NAMESPACE`
to a unique name before including any header from the embedded PEGTL set.
A simple example might look like this:

```c++
#define TAOCPP_PEGTL_NAMESPACE mylib_pegtl

#include <tao/pegtl.hpp>
#include <tao/contrib/json.hpp>

int main( int argc, char* argv[] )
{
   if( argc > 1 ) {
     // pretty useless, but everything is in tao::mylib_pegtl now
     tao::mylib_pegtl::argv_input<> in( argv, 1 );
     tao::mylib_pegtl::parse< tao::mylib_pegtl::json::text >( in );
   }
}

```

## Embedding the PEGTL in library interfaces

In case the PEGTL headers are included in the headers of your library,
setting `TAOCPP_PEGTL_NAMESPACE` is not sufficient. Both copies of the PEGTL
would use `TAOCPP_PEGTL_NAMESPACE` for the symbols as well as the identical
names for internal macros and include guards. Our solution to this problem is
as simple as it is efficient: The embedded copy needs to replace all occurrences
of the string `TAOCPP_PEGTL_` with a different (and unique) prefix. On a unix-shell,
the following command will achieve this:

```sh
$ sed -i 's/TAOCPP_PEGTL_/MYLIB_PEGTL_/g' $(find -name '[^.]*.[hc]pp')
```

Run the above command from the directory, where the embedded PEGTL is stored.
Afterwards, don't forget to set `MYLIB_PEGTL_NAMESPACE` or, alternatively,
modify `include/tao/pegtl/config.hpp`.

A practical example of how the result looks like can be found in our
header-only [JSON library](https://github.com/taocpp/json/).

Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
