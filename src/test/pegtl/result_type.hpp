// Copyright (c) 2014-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_SRC_TEST_PEGTL_RESULT_TYPE_HPP  // NOLINT
#define TAO_PEGTL_SRC_TEST_PEGTL_RESULT_TYPE_HPP

#include <ostream>

#include <tao/pegtl/config.hpp>

namespace tao
{
   namespace TAO_PEGTL_NAMESPACE
   {
      enum class result_type
      {
         success = 1,
         local_failure = 0,
         global_failure = -1
      };

      inline std::ostream& operator<<( std::ostream& o, const result_type t )
      {
         switch( t ) {
            case result_type::success:
               return o << "success";
            case result_type::local_failure:
               return o << "local failure";
            case result_type::global_failure:
               return o << "global failure";
         }
         return o << int( t );
      }

   }  // namespace TAO_PEGTL_NAMESPACE

}  // namespace tao

#endif
