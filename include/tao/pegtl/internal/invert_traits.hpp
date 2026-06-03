// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_INVERT_TRAITS_HPP
#define TAO_PEGTL_INTERNAL_INVERT_TRAITS_HPP

#include "../config.hpp"

#include "dependent_false.hpp"
#include "ione.hpp"
#include "not_ione.hpp"
#include "not_one.hpp"
#include "not_range.hpp"
#include "not_ranges.hpp"
#include "one.hpp"
#include "range.hpp"
#include "ranges.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Rule, typename = void >
   struct invert_traits
   {
      static_assert( dependent_false< Rule > );
   };

   template< typename Peek, typename Peek::data_t... Cs >
   struct invert_traits< ione< Peek, Cs... > >
   {
      using type = not_ione< Peek, Cs... >;
   };

   template< typename Peek, typename Peek::data_t... Cs >
   struct invert_traits< not_ione< Peek, Cs... > >
   {
      using type = ione< Peek, Cs... >;
   };

   template< typename Peek, typename Peek::data_t... Cs >
   struct invert_traits< not_one< Peek, Cs... > >
   {
      using type = one< Peek, Cs... >;
   };

   template< typename Peek, typename Peek::data_t Lo, typename Peek::data_t Hi >
   struct invert_traits< not_range< Peek, Lo, Hi > >
   {
      using type = range< Peek, Lo, Hi >;
   };

   template< typename Peek, typename Peek::data_t... Cs >
   struct invert_traits< not_ranges< Peek, Cs... > >
   {
      using type = ranges< Peek, Cs... >;
   };

   template< typename Peek, typename Peek::data_t... Cs >
   struct invert_traits< one< Peek, Cs... > >
   {
      using type = not_one< Peek, Cs... >;
   };

   template< typename Peek, typename Peek::data_t Lo, typename Peek::data_t Hi >
   struct invert_traits< range< Peek, Lo, Hi > >
   {
      using type = not_range< Peek, Lo, Hi >;
   };

   template< typename Peek, typename Peek::data_t... Cs >
   struct invert_traits< ranges< Peek, Cs... > >
   {
      using type = not_ranges< Peek, Cs... >;
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
