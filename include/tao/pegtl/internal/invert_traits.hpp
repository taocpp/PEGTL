// Copyright (c) 2023-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_INVERT_TRAITS_HPP
#define TAO_PEGTL_INTERNAL_INVERT_TRAITS_HPP

#include "../config.hpp"

#include "any.hpp"
#include "failure.hpp"
#include "not_one.hpp"
#include "not_range.hpp"
#include "one.hpp"
#include "peek_ascii7.hpp"
#include "peek_ascii8.hpp"
#include "range.hpp"
#include "single.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename, typename = void >
   struct invert_traits;

   template< typename Peek >
   struct invert_traits< any< Peek > >
   {
      using rule_t = failure;
   };

   template<>
   struct invert_traits< failure >
   {
      // Unfortunately at this point we don't know what Peek was.
   };

   template< typename Peek, typename Peek::data_t... Cs >
   struct invert_traits< single< one< Peek, Cs... > > >
   {
      using rule_t = single< not_one< Peek, Cs... > >;
   };

   template< typename Peek, typename Peek::data_t... Cs >
   struct invert_traits< single< not_one< Peek, Cs... > > >
   {
      using rule_t = single< one< Peek, Cs... > >;
   };

   template< typename Peek, typename Peek::data_t Lo, typename Peek::data_t Hi >
   struct invert_traits< single< range< Peek, Lo, Hi > > >
   {
      using rule_t = single< not_range< Peek, Lo, Hi > >;
   };

   template< typename Peek, typename Peek::data_t Lo, typename Peek::data_t Hi >
   struct invert_traits< single< not_range< Peek, Lo, Hi > > >
   {
      using rule_t = single< range< Peek, Lo, Hi > >;
   };

   template< char... Cs >
   struct invert_traits< single< one< peek_ascii8, Cs... > > >
   {
      using rule_t = single< not_one< peek_ascii7, Cs... > >;
   };

   template< char... Cs >
   struct invert_traits< single< not_one< peek_ascii7, Cs... > > >
   {
      using rule_t = single< one< peek_ascii8, Cs... > >;
   };

   template< char Lo, char Hi >
   struct invert_traits< single< range< peek_ascii8, Lo, Hi > > >
   {
      using rule_t = single< not_range< peek_ascii7, Lo, Hi > >;
   };

   template< char Lo, char Hi >
   struct invert_traits< single< not_range< peek_ascii7, Lo, Hi > > >
   {
      using rule_t = single< range< peek_ascii8, Lo, Hi > >;
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
