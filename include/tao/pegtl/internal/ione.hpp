// Copyright (c) 2014-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_IONE_HPP
#define TAO_PEGTL_INTERNAL_IONE_HPP

#include "../config.hpp"

#include "ascii_utility.hpp"
#include "enable_control.hpp"
#include "dependent_false.hpp"
#include "terminal.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Peek, typename Peek::data_t... Cs >
   struct ione
      : terminal< ione< Peek, Cs... > >
   {
      template< typename Data >
      [[nodiscard]] static constexpr bool test( const Data c ) noexcept
      {
         return ( ascii_ichar_equal< Cs >( c ) || ... );
      }
   };

   template< typename Peek >
   struct ione< Peek >
   {
      static_assert( dependent_false< Peek > );
   };

   template< typename Peek, typename Peek::data_t... Cs >
   inline constexpr bool enable_control< ione< Peek, Cs... > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
