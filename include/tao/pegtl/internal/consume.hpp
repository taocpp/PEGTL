// Copyright (c) 2017-2022 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_CONSUME_HPP
#define TAO_PEGTL_INTERNAL_CONSUME_HPP

#include <cstddef>

#include "../config.hpp"
#include "../type_list.hpp"

#include "enable_control.hpp"
#include "success.hpp"
#include "utility.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< std::size_t Count, typename Reference = void >
   struct consume
   {
      using rule_t = consume;
      using subs_t = empty_list;

      template< typename ParseInput >
      [[nodiscard]] static bool match( ParseInput& in ) noexcept
      {
         static_assert( is_integer_fraction( sizeof( Reference ), sizeof( *in.current() ) ) );

         if( in.size( Count ) >= Count ) {
            in.template consume< consume >( Count );
            return true;
         }
         return false;
      }
   };

   template< typename Reference >
   struct consume< 0, Reference >
      : success
   {};

   template< std::size_t Count >
   struct consume< Count, void >
   {
      using rule_t = consume;
      using subs_t = empty_list;

      template< typename ParseInput >
      [[nodiscard]] static bool match( ParseInput& in ) noexcept
      {
         if( in.size( Count ) >= Count ) {
            in.template consume< consume >( Count );
            return true;
         }
         return false;
      }
   };

   template< unsigned Count, typename Reference >
   inline constexpr bool enable_control< consume< Count, Reference > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
