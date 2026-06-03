// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_INVERT_HPP
#define TAO_PEGTL_INTERNAL_INVERT_HPP

#include "../config.hpp"

#include "match_mode.hpp"
#include "terminal.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Rule >
   struct invert;

   [[nodiscard]] constexpr match_mode invert_impl( const match_mode m ) noexcept
   {
      return match_mode( !bool( m ) );
   }

   template< match_mode I, template< typename Peek, typename Peek::data_t... Cs > class Impl, typename Peek, typename Peek::data_t... Cs >
   struct invert< terminal< I, Impl< Peek, Cs... > > >
      : terminal< invert_impl( I ), Impl< Peek, Cs... > >
   {};

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
