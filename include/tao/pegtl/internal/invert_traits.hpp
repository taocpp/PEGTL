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
#include "peek_direct.hpp"
#include "peek_seven.hpp"
#include "range.hpp"
#include "tester.hpp"

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
   struct invert_traits< tester< one< Peek, Cs... > > >
   {
      using rule_t = tester< not_one< Peek, Cs... > >;
   };

   template< typename Peek, typename Peek::data_t... Cs >
   struct invert_traits< tester< not_one< Peek, Cs... > > >
   {
      using rule_t = tester< one< Peek, Cs... > >;
   };

   template< typename Peek, typename Peek::data_t Lo, typename Peek::data_t Hi >
   struct invert_traits< tester< range< Peek, Lo, Hi > > >
   {
      using rule_t = tester< not_range< Peek, Lo, Hi > >;
   };

   template< typename Peek, typename Peek::data_t Lo, typename Peek::data_t Hi >
   struct invert_traits< tester< not_range< Peek, Lo, Hi > > >
   {
      using rule_t = tester< range< Peek, Lo, Hi > >;
   };

   template< char... Cs >
   struct invert_traits< tester< one< peek_char, Cs... > > >
   {
      using rule_t = tester< not_one< peek_seven, Cs... > >;
   };

   template< char... Cs >
   struct invert_traits< tester< not_one< peek_seven, Cs... > > >
   {
      using rule_t = tester< one< peek_char, Cs... > >;
   };

   template< char Lo, char Hi >
   struct invert_traits< tester< range< peek_char, Lo, Hi > > >
   {
      using rule_t = tester< not_range< peek_seven, Lo, Hi > >;
   };

   template< char Lo, char Hi >
   struct invert_traits< tester< not_range< peek_seven, Lo, Hi > > >
   {
      using rule_t = tester< range< peek_char, Lo, Hi > >;
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

// FCOV_EXCL_FILE
#endif
