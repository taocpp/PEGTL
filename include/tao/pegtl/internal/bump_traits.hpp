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

   template< typename Eol, typename Peek >
   struct bump_traits< Eol, any< Peek > >
      : bump_traits< Eol, eol_unknown_tag >
   {
      // TODO: Only when single-char eol possible?
      // TODO: Document that multi-char eols are a problem.
   };

   template< typename Eol, unsigned Count >
   struct bump_traits< Eol, consume< Count > >
      : bump_traits< Eol, eol_unknown_tag >
   {};

   template< typename Eol >
   struct bump_traits< Eol, everything >
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

   template< typename Eol, typename Rule, typename >
   struct bump_traits
      : bump_traits< Eol, eol_unknown_tag >  // check
   // : bump_traits< Eol, eol_exclude_tag >  // trust
   {
      // TODO: Make default case configurable?
      // TODO: Apply default case to specialisations?
   };

   // eol_exclude_tag eol_matched_tag eol_unknown_tag -- are obvious.
   // any consume everything many -- can match anything.
   // binary_property istring one property_value range ranges rep_one_min_max string -- it depends.
   // until -- slightly tricky.
   // nested void -- can be ignored here.

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
