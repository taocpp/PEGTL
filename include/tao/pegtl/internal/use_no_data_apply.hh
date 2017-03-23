// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_INTERNAL_USE_NO_DATA_APPLY_HH
#define TAOCPP_PEGTL_INCLUDE_INTERNAL_USE_NO_DATA_APPLY_HH

#include <type_traits>

#include "../config.hh"

namespace TAOCPP_PEGTL_NAMESPACE
{
   namespace internal
   {
      template< typename Action, typename ... States >
      struct use_no_data_apply
      {
      private:
         typedef std::true_type yes;
         typedef std::false_type no;

         template< typename U > static auto test( int ) -> decltype( U::apply( std::declval< States && >() ... ), yes() );
         template< typename > static no test( ... );

      public:
         static constexpr bool value = std::is_same< decltype( test< Action >( 0 ) ), yes >::value;
      };

   } // namespace internal

} // namespace TAOCPP_PEGTL_NAMESPACE

#endif
