// Copyright (c) 2014-2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_INTERNAL_ANY_HPP
#define TAO_PEGTL_INTERNAL_ANY_HPP

#include "../config.hpp"

#include "enable_control.hpp"
#include "peek_char.hpp"

#include "../rule_list.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Peek >
   struct any;

   template<>
   struct any< peek_char >
   {
      using rule_t = any;
      using subs_t = empty_list;

      template< typename Input >
      [[nodiscard]] static bool match( Input& in ) noexcept( noexcept( in.empty() ) )
      {
         if( !in.empty() ) {
            in.bump();
            return true;
         }
         return false;
      }
   };

   template< typename Peek >
   struct any
   {
      using rule_t = any;
      using subs_t = empty_list;

      template< typename Input >
      [[nodiscard]] static bool match( Input& in ) noexcept( noexcept( in.size( Peek::max_input_size ) ) )
      {
         if( const std::size_t s = in.size( Peek::max_input_size ); s >= Peek::min_input_size ) {
            if( const auto t = Peek::peek( in, s ) ) {
               in.bump( t.size );
               return true;
            }
         }
         return false;
      }
   };

   template< typename Peek >
   inline constexpr bool enable_control< any< Peek > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
