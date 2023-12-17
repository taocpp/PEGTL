// Copyright (c) 2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_BUMP_TRAITS_HPP
#define TAO_PEGTL_INTERNAL_BUMP_TRAITS_HPP

#include <cstddef>
#include <type_traits>

#include "../config.hpp"
#include "../tags.hpp"

#include "any.hpp"
#include "at.hpp"
#include "consume.hpp"
#include "eol.hpp"
#include "eolf.hpp"
#include "everything.hpp"
#include "many.hpp"
#include "until.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Eol, typename Rule, typename = void >
   struct bump_traits;

   template< typename Eol >
   struct bump_traits< Eol, eol_exclude_tag >
   {
      template< typename Data, typename Position >
      static void bump( Position& pos, const Data* /*unused*/, const std::size_t count ) noexcept
      {
         pos.column += count;
         pos.count += count;
      }
   };

   template< typename Eol >
   struct bump_traits< Eol, eol_matched_tag >
   {
      template< typename Data, typename Position >
      static void bump( Position& pos, const Data* /*unused*/, const std::size_t count ) noexcept
      {
         pos.line++;
         pos.column = 1;
         pos.count += count;
      }
   };

   template< typename Eol >
   struct bump_traits< Eol, eol_unknown_tag >
   {
      template< typename Data, typename Position >
      static void bump( Position& pos, const Data* data, const std::size_t count )
      {
         text_eol_scan< Eol >( pos, data, data + count );
      }
   };

   template< typename Eol, typename Rule >
   struct bump_traits< Eol, Rule, std::enable_if_t< std::is_same_v< Eol, Rule > > >
      : bump_traits< Eol, eol_matched_tag >
   {};

   // TODO: Anything else to detect and optimise eol_rule sub-rules?

   template< typename Eol, typename Peek >
   struct bump_traits< Eol, any< Peek > >
      : bump_traits< Eol, eol_unknown_tag >  // TODO: Only when single-char eol possible?
   {};

   template< typename Eol, unsigned Count >
   struct bump_traits< Eol, consume< Count > >
      : bump_traits< Eol, eol_unknown_tag >
   {};

   template< typename Eol, typename Size >
   struct bump_traits< Eol, everything< Size > >
      : bump_traits< Eol, eol_unknown_tag >
   {};

   template< typename Eol, unsigned Count, typename Peek >
   struct bump_traits< Eol, many< Count, Peek > >
      : bump_traits< Eol, eol_unknown_tag >
   {};

   template< typename Eol, typename Cond >
   struct bump_traits< Eol, until< Cond > >
      : bump_traits< Eol, eol_unknown_tag >
   {};

   template< typename Eol >
   struct bump_traits< Eol, until< eol< void > > >
      : bump_traits< Eol, eol_exclude_tag >
   {};

   template< typename Eol >
   struct bump_traits< Eol, until< eolf< void > > >
      : bump_traits< Eol, eol_exclude_tag >
   {};

   template< typename Eol, typename Cond >
   struct bump_traits< Eol, until< Cond >, std::enable_if_t< std::is_same_v< Eol, Cond > > >
      : bump_traits< Eol, eol_exclude_tag >
   {};

   template< typename Eol, typename Cond >
   struct bump_traits< Eol, until< at< Cond > > >
      : bump_traits< Eol, until< typename Cond::rule_t > >
   {};

   // TODO: Anything else that can be made independent of the default case?
   // TODO: Most importantly: one, range(s), (i)string (and the ICU rules?)

   template< typename Eol, typename Rule, typename >
   struct bump_traits
      : bump_traits< Eol, eol_unknown_tag >  // check
   // : bump_traits< Eol, eol_exclude_tag >  // trust
   {};

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
