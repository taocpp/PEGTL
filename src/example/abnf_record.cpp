// Copyright (c) 2024-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#if !defined( __cpp_exceptions )
#include <iostream>
int main()
{
   std::cout << "Exception support disabled, example is dummy..." << std::endl;
}
#else

#include <iomanip>
#include <iostream>

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/record.hpp>
#include <tao/pegtl/example/abnf_abnf.hpp>

#include "abnf_errors.hpp"

namespace pegtl = TAO_PEGTL_NAMESPACE;

int main( int argc, char** argv )  // NOLINT(bugprone-exception-escape)
{
   if( argc != 2 ) {
      std::cerr << "Usage: " << argv[ 0 ] << " ABNF\n"
                << "Parse an ABNF file.\n\n"
                << "Example: " << argv[ 0 ] << " filename.abnf" << std::endl;
      return 1;
   }
   using input_t = pegtl::text_file_input<>;
   input_t in( argv[ 1 ] );
   try {
      const auto v = pegtl::record< pegtl::abnf::num_val,
                                    pegtl::abnf::char_val,
                                    pegtl::abnf::prose_val,
                                    pegtl::abnf::group,
                                    pegtl::abnf::option,
                                    pegtl::abnf::rulename,
                                    pegtl::abnf::repetition,
                                    pegtl::ascii::digit >::parse< pegtl::abnf::rulelist,
                                                                  pegtl::abnf::control >( in );
      std::cout << v;
   }
   catch( const decltype( in )::parse_error_t& e ) {
      const auto& p = e.position_object();
      std::cerr << e.what() << '\n'
                << in.line_view_at( p ) << '\n'
                << std::setw( int( p.count ) ) << '^' << std::endl;
      return 1;
   }
   return 0;
}

#endif
