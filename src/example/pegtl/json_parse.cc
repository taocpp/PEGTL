// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include "json_errors.hh"

using grammar = tao::pegtl::must< tao::pegtl::json::text, tao::pegtl::eof >;

int main( int argc, char ** argv )
{
   for ( int i = 1; i < argc; ++i ) {
      tao::pegtl::parse_arg< grammar, tao::pegtl::nothing, examples::errors >( i, argv );
   }
   return 0;
}
