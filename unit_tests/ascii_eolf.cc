// Copyright (c) 2014-2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include "test.hh"

namespace pegtl
{
   void unit_test()
   {
      verify_analyze< eolf >( __LINE__, __FILE__, false, false );

      verify_rule< eolf >( __LINE__, __FILE__,  "", result_type::SUCCESS, 0 );

      for ( char i = 1; i < 127; ++i ) {
         verify_char< eolf >( __LINE__, __FILE__, i, (( i == '\n' ) || ( i == '\r' )) ? result_type::SUCCESS : result_type::LOCAL_FAILURE );
      }
      verify_rule< eolf >( __LINE__, __FILE__,  "\r\n", result_type::SUCCESS, 0 );
      verify_rule< eolf >( __LINE__, __FILE__,  "\n\r", result_type::SUCCESS, 1 );
      verify_rule< eolf >( __LINE__, __FILE__,  "\na", result_type::SUCCESS, 1 );
      verify_rule< eolf >( __LINE__, __FILE__,  "\ra", result_type::SUCCESS, 1 );
      verify_rule< eolf >( __LINE__, __FILE__,  "\r\na", result_type::SUCCESS, 1 );
      verify_rule< eolf >( __LINE__, __FILE__,  "\n\ra", result_type::SUCCESS, 2 );
      verify_rule< eolf >( __LINE__, __FILE__,  "\r\r\r", result_type::SUCCESS, 2 );
      verify_rule< eolf >( __LINE__, __FILE__,  "\r\r\n", result_type::SUCCESS, 2 );
      verify_rule< eolf >( __LINE__, __FILE__,  "\r\n\r", result_type::SUCCESS, 1 );
      verify_rule< eolf >( __LINE__, __FILE__,  "\r\n\n", result_type::SUCCESS, 1 );
      verify_rule< eolf >( __LINE__, __FILE__,  "\n\r\r", result_type::SUCCESS, 2 );
      verify_rule< eolf >( __LINE__, __FILE__,  "\n\r\n", result_type::SUCCESS, 2 );
      verify_rule< eolf >( __LINE__, __FILE__,  "\n\n\r", result_type::SUCCESS, 2 );
      verify_rule< eolf >( __LINE__, __FILE__,  "\n\n\n", result_type::SUCCESS, 2 );
   }

} // pegtl

#include "main.hh"
