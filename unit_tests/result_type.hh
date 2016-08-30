// Copyright (c) 2014-2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_UNIT_TESTS_RESULT_TYPE_HH
#define PEGTL_UNIT_TESTS_RESULT_TYPE_HH

#include <ostream>

namespace pegtl
{
   enum class result_type
   {
      SUCCESS = 1,
      LOCAL_FAILURE = 0,
      GLOBAL_FAILURE = -1
   };

   inline std::ostream & operator<< ( std::ostream & o, const result_type t )
   {
      switch ( t ) {
         case result_type::SUCCESS:
            return o << "success";
         case result_type::LOCAL_FAILURE:
            return o << "local failure";
         case result_type::GLOBAL_FAILURE:
            return o << "global failure";
      }
      return o << int( t );
   }

} // pegtl

#endif
