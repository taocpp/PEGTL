// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <iostream>

#include <tao/pegtl.hh>
#include <tao/pegtl/trace.hh>
#include <tao/pegtl/contrib/uri.hh>

using grammar = tao::TAOCPP_PEGTL_NAMESPACE::must< tao::TAOCPP_PEGTL_NAMESPACE::uri::URI >;

int main( int argc, char ** argv )
{
   for ( int i = 1; i < argc; ++i ) {
      std::cout << "Parsing " << argv[ i ] << std::endl;
      tao::TAOCPP_PEGTL_NAMESPACE::trace_arg< grammar >( i, argv );
   }
   return 0;
}
