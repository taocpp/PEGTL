// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_SCAN_BASE_CLASSES_HPP
#define TAO_PEGTL_INTERNAL_SCAN_BASE_CLASSES_HPP

#include <cstddef>
#include <type_traits>

#include "../config.hpp"

#include "type_traits.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   struct nop_scan
   {
      template< typename Data, typename Position >
      static void scan( Position& /*unused*/, const Data* /*unused*/, const std::size_t /*unused*/ ) noexcept
      {}
   };

   struct inc_line_scan
   {
      template< typename Data, typename Position >
      static void scan( Position& pos, const Data* /*unused*/, const std::size_t count ) noexcept
      {
         ++pos.line;
         pos.column = 1;
         pos.count += count;
      }
   };

   struct add_column_scan
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
