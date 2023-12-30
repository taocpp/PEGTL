// Copyright (c) 2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INVERT_TRAITS_HPP
#define TAO_PEGTL_INVERT_TRAITS_HPP

#include "config.hpp"
#include "forward.hpp"

#include "internal/any.hpp"
#include "internal/failure.hpp"
#include "internal/not_one.hpp"
#include "internal/not_range.hpp"
#include "internal/one.hpp"
#include "internal/peek_ascii7.hpp"
#include "internal/peek_ascii8.hpp"
#include "internal/range.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Peek >
   struct invert_traits< internal::any< Peek > >
   {
      using rule_t = internal::failure;
   };

   template<>
   struct invert_traits< internal::failure >
   {
      // Unfortunately at this point we don't know what Peek was.
   };

   template< typename Peek, typename Peek::data_t... Cs >
   struct invert_traits< internal::single< internal::one< Peek, Cs... > > >
   {
      using rule_t = internal::single< internal::not_one< Peek, Cs... > >;
   };

   template< typename Peek, typename Peek::data_t... Cs >
   struct invert_traits< internal::single< internal::not_one< Peek, Cs... > > >
   {
      using rule_t = internal::single< internal::one< Peek, Cs... > >;
   };

   template< typename Peek, typename Peek::data_t Lo, typename Peek::data_t Hi >
   struct invert_traits< internal::single< internal::range< Peek, Lo, Hi > > >
   {
      using rule_t = internal::single< internal::not_range< Peek, Lo, Hi > >;
   };

   template< typename Peek, typename Peek::data_t Lo, typename Peek::data_t Hi >
   struct invert_traits< internal::single< internal::not_range< Peek, Lo, Hi > > >
   {
      using rule_t = internal::single< internal::range< Peek, Lo, Hi > >;
   };

   template< char... Cs >
   struct invert_traits< internal::single< internal::one< internal::peek_ascii8, Cs... > > >
   {
      using rule_t = internal::single< internal::not_one< internal::peek_ascii7, Cs... > >;
   };

   template< char... Cs >
   struct invert_traits< internal::single< internal::not_one< internal::peek_ascii7, Cs... > > >
   {
      using rule_t = internal::single< internal::one< internal::peek_ascii8, Cs... > >;
   };

   template< char Lo, char Hi >
   struct invert_traits< internal::single< internal::range< internal::peek_ascii8, Lo, Hi > > >
   {
      using rule_t = internal::single< internal::not_range< internal::peek_ascii7, Lo, Hi > >;
   };

   template< char Lo, char Hi >
   struct invert_traits< internal::single< internal::not_range< internal::peek_ascii7, Lo, Hi > > >
   {
      using rule_t = internal::single< internal::range< internal::peek_ascii8, Lo, Hi > >;
   };

}  // namespace TAO_PEGTL_NAMESPACE

#endif
