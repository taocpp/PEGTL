// Copyright (c) 2023-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_TEXT_SCAN_TRAITS_HPP
#define TAO_PEGTL_INTERNAL_TEXT_SCAN_TRAITS_HPP

#include <cstddef>
#include <type_traits>

#include "../config.hpp"
#include "../tags.hpp"

#include "at.hpp"
#include "dependent_false.hpp"
#include "eol.hpp"
#include "eolf.hpp"
#include "single.hpp"
#include "until.hpp"

#include "char_eol_scan.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Eol, typename Rule, typename = void >
   struct text_scan_traits;

   template< typename Eol >
   struct text_scan_traits< Eol, void >
   {
      static_assert( dependent_false< Eol > );
   };

   template< typename Eol >
   struct text_scan_traits< Eol, eol_exclude_tag >
   {
      template< typename Data, typename Position >
      static void scan( Position& pos, const Data* /*unused*/, const std::size_t count ) noexcept
      {
         pos.column += count;
         pos.count += count;
      }
   };

   // template< typename Eol >
   // struct text_scan_traits< Eol, eol_matched_tag >
   // {
   //    template< typename Data, typename Position >
   //    static void scan( Position& pos, const Data* /*unused*/, const std::size_t count ) noexcept
   //    {
   //       pos.line++;
   //       pos.column = 1;
   //       pos.count += count;
   //    }
   // };

   template< typename Eol >
   struct text_scan_traits< Eol, eol_matched_tag >
      : text_scan_traits< Eol, eol_unknown_tag >
   {};

   template< typename Eol >
   struct text_scan_traits< Eol, eol_unknown_tag >
   {
      static_assert( Eol::eol_char > 0 );

      template< typename Data, typename Position >
      static void scan( Position& pos, const Data* data, const std::size_t count )
      {
         char_eol_scan< Eol::eol_char >( pos, data, data + count );
      }
   };

   template< typename Eol, typename Rule >
   struct text_scan_traits< Eol, single< Rule >, std::enable_if_t< Rule::test( typename Rule::data_t( Eol::eol_char ) ) > >
      : text_scan_traits< Eol, eol_unknown_tag >
   {
      static_assert( Eol::eol_char > 0 );
   };

   template< typename Eol, typename Rule >
   struct text_scan_traits< Eol, single< Rule >, std::enable_if_t< !Rule::test( typename Rule::data_t( Eol::eol_char ) ) > >
      : text_scan_traits< Eol, eol_exclude_tag >
   {
      static_assert( Eol::eol_char > 0 );
   };

   template< typename Eol, typename Cond >
   struct text_scan_traits< Eol, until< Cond > >
      : text_scan_traits< Eol, eol_unknown_tag >
   {};

   template< typename Eol >
   struct text_scan_traits< Eol, until< eol< void > > >
      : text_scan_traits< Eol, eol_exclude_tag >
   {};

   template< typename Eol >
   struct text_scan_traits< Eol, until< eolf< void > > >
      : text_scan_traits< Eol, eol_exclude_tag >
   {};

   template< typename Eol, typename Cond >
   struct text_scan_traits< Eol, until< Cond >, std::enable_if_t< std::is_same_v< typename Eol::rule_t, typename Cond::rule_t > > >
      : text_scan_traits< Eol, eol_exclude_tag >
   {};

   template< typename Eol, typename Rule >
   struct text_scan_traits< Eol, until< single< Rule > >, std::enable_if_t< !Rule::test( typename Rule::data_t( Eol::eol_char ) ) > >
      : text_scan_traits< Eol, eol_exclude_tag >
   {
      static_assert( Eol::eol_char > 0 );
   };

   template< typename Eol, typename Cond >
   struct text_scan_traits< Eol, until< at< Cond > > >
      : text_scan_traits< Eol, until< typename Cond::rule_t > >
   {};

   template< typename Eol, typename Rule >
   struct text_scan_traits< Eol, Rule, std::enable_if< !std::is_same_v< Rule, typename Rule::rule_t > > >
      : text_scan_traits< Eol, typename Rule::rule_t >
   {};

   template< typename Eol, typename Rule, typename >
   struct text_scan_traits
      : text_scan_traits< Eol, eol_unknown_tag >
   {};

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
