// Copyright (c) 2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_BUMP_TRAITS_HPP
#define TAO_PEGTL_INTERNAL_BUMP_TRAITS_HPP

#include <cstddef>

#include "../config.hpp"

#include "any.hpp"
#include "many.hpp"
#include "text_eol_tags.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Eol, typename Rule, typename = void >
   struct bump_traits
   {
      // template< typename Data, typename Position >
      // static void bump( Position& pos, const Data* /*unused*/, const std::size_t count ) noexcept
      // {
      //    pos.column += count;
      //    pos.count += count;
      // }

      template< typename Data, typename Position >
      static void bump( Position& pos, const Data* data, const std::size_t count ) noexcept
      {
         text_eol_scan< Eol >( pos, data, data + count );
      }
   };

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
         pos.column = 1;  // TODO: Incrementing column while matching Eol is redundant!
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

   template< typename Eol, typename Peek >
   struct bump_traits< Eol, any< Peek > >
   {
      template< typename Data, typename Position >
      static void bump( Position& pos, const Data* data, const std::size_t count )
      {
         text_eol_scan< Eol >( pos, data, data + count );
      }
   };

   template< typename Eol, unsigned Count, typename Peek >
   struct bump_traits< Eol, many< Count, Peek > >
   {
      template< typename Data, typename Position >
      static void bump( Position& pos, const Data* data, const std::size_t count )
      {
         text_eol_scan< Eol >( pos, data, data + count );
      }
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
