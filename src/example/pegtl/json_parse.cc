// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include "json_errors.hh"

using grammar = tao::TAOCPP_PEGTL_NAMESPACE::must< tao::TAOCPP_PEGTL_NAMESPACE::json::text, tao::TAOCPP_PEGTL_NAMESPACE::eof >;

int main( int argc, char ** argv )
{
   for ( int i = 1; i < argc; ++i ) {
      tao::TAOCPP_PEGTL_NAMESPACE::parse_arg< grammar, tao::TAOCPP_PEGTL_NAMESPACE::nothing, examples::errors >( i, argv );
   }
   return 0;
}
