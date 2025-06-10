// Copyright (c) 2017-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#if !defined( __cpp_exceptions )
#include <iostream>
int main()
{
   std::cerr << "Exception support required, example unavailable." << std::endl;
   return 1;
}
#else

#include <iomanip>

#include <tao/pegtl.hpp>
#include <tao/pegtl/debug/analyze.hpp>

#include "proto3.hpp"

int main( int argc, char** argv )  // NOLINT(bugprone-exception-escape)
{
   using namespace TAO_PEGTL_NAMESPACE;

   if( analyze< proto3::proto >() != 0 ) {
      return 1;
   }
   using input_t = text_file_input< scan::lf_crlf >;
   for( int i = 1; i < argc; ++i ) {
      input_t in( argv[ i ] );
      try {
         parse< proto3::proto >( in );
      }
      catch( const decltype( in )::parse_error_t& e ) {
         const auto& p = e.position_object();
         std::cerr << e.what() << '\n'
                   << in.line_view_at( p ) << '\n'
                   << std::setw( int( p.column ) ) << '^' << '\n';
      }
   }
   return 0;
}

#endif
