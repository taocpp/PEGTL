// Copyright (c) 2024-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_SCAN_BASE_CLASSES_HPP
#define TAO_PEGTL_INTERNAL_SCAN_BASE_CLASSES_HPP

#include "../config.hpp"

#include "scan_input.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   struct nop_scan
   {
      template< typename... As >
      static void scan( As&&... /*unused*/ )
      {}
   };

   struct inc_line_scan
   {
      template< typename Position, typename Data >
      static void scan( Position& pos, const scan_input< Data >& in ) noexcept
      {
         ++pos.line;
         pos.column = 1;
         pos.count += in.size();
      }
   };

   struct add_column_scan
   {
      template< typename Position, typename Data >
      static void scan( Position& pos, const scan_input< Data >& in ) noexcept
      {
         pos.column += in.size();
         pos.count += in.size();
      }
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
