// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include <pegtl.hh>

// pegtl.hh makes sure you can test for _POSIX_MAPPED_FILES
#if defined(_POSIX_MAPPED_FILES)

int main( int argc, char ** argv )
{
   for ( int i = 1; i < argc; ++i ) {
      std::cout << pegtl::mmap_parser( argv[ 1 ] ).input().string() << std::endl;
   }
   return 0;
}

#else

#include <iostream>

int main()
{
   std::cout << "mmap() unsupported on this platform" << std::endl;
   return 0;
}

#endif
