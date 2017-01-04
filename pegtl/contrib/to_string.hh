// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_CONTRIB_TO_STRING_HH
#define PEGTL_CONTRIB_TO_STRING_HH

#include <string>

namespace pegtl
{
   namespace internal
   {
      template< typename >
      struct to_string;

      template< template< char ... > class X, char ... Cs >
      struct to_string< X< Cs ... > >
      {
         static std::string get()
         {
            const char s[] = { Cs ..., 0 };
            return std::string( s, sizeof ... ( Cs ) );
         }
      };

   } // namespace internal

   template< typename T >
   std::string to_string()
   {
      return internal::to_string< T >::get();
   }

} // namespace pegtl

#endif
