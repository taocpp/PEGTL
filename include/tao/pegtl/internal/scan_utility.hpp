// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_SCAN_UTILITY_HPP
#define TAO_PEGTL_INTERNAL_SCAN_UTILITY_HPP

#include <cstddef>

#include "../config.hpp"

#include "char_eol_scan.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   struct scan_nop_base
   {
      template< typename Data, typename Position >
      static void scan( Position& /*unused*/, const Data* /*unused*/, const std::size_t /*unused*/ ) noexcept
      {}
   };

   template< char Eol >
   struct scan_eol_base
   {
      template< typename Data, typename Position >
      static void scan( Position& pos, const Data* data, const std::size_t count )
      {
         char_eol_scan< Eol >( pos, data, count );
      }
   };

   struct scan_line_base
   {
      template< typename Data, typename Position >
      static void scan( Position& pos, const Data* /*unused*/, const std::size_t count ) noexcept
      {
         ++pos.line;
         pos.column = 1;
         pos.count += count;
      }
   };

   struct scan_skip_base
   {
      template< typename Data, typename Position >
      static void scan( Position& pos, const Data* /*unused*/, const std::size_t count ) noexcept
      {
         pos.column += count;
         pos.count += count;
      }
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
