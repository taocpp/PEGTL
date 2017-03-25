// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include "test.hh"

namespace tao
{
   namespace pegtl
   {
      void unit_test()
      {
         verify_analyze< identifier >( __LINE__, __FILE__, true, false );

         verify_rule< identifier >( __LINE__, __FILE__,  "_", result_type::SUCCESS, 0 );
         verify_rule< identifier >( __LINE__, __FILE__,  "_a", result_type::SUCCESS, 0 );
         verify_rule< identifier >( __LINE__, __FILE__,  "_1", result_type::SUCCESS, 0 );
         verify_rule< identifier >( __LINE__, __FILE__,  "_123", result_type::SUCCESS, 0 );
         verify_rule< identifier >( __LINE__, __FILE__,  "_1a", result_type::SUCCESS, 0 );
         verify_rule< identifier >( __LINE__, __FILE__,  "_a1", result_type::SUCCESS, 0 );
         verify_rule< identifier >( __LINE__, __FILE__,  "_fro_bble", result_type::SUCCESS, 0 );
         verify_rule< identifier >( __LINE__, __FILE__,  "f_o_o42", result_type::SUCCESS, 0 );
         verify_rule< identifier >( __LINE__, __FILE__,  "", result_type::LOCAL_FAILURE, 0 );
         verify_rule< identifier >( __LINE__, __FILE__,  "1", result_type::LOCAL_FAILURE, 1 );
         verify_rule< identifier >( __LINE__, __FILE__,  " ", result_type::LOCAL_FAILURE, 1 );
         verify_rule< identifier >( __LINE__, __FILE__,  " _", result_type::LOCAL_FAILURE, 2 );
         verify_rule< identifier >( __LINE__, __FILE__,  " a", result_type::LOCAL_FAILURE, 2 );
      }

   } // namespace pegtl

} // namespace tao

#include "main.hh"
