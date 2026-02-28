// Copyright (c) 2023-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_SRC_TEST_PEGTL_TEST_UTILITY_HPP
#define TAO_PEGTL_SRC_TEST_PEGTL_TEST_UTILITY_HPP

#include <cassert>
#include <cstddef>
#include <iostream>
#include <string>
#include <type_traits>

#include <tao/pegtl/inputs.hpp>

namespace TAO_PEGTL_NAMESPACE::test
{
   template< typename Eol, typename Peek >
   struct eol_to_lazy_eol
      : Eol::rule_t
   {
      using eol_lazy_peek = Peek;
   };

   template< typename Data >
   [[nodiscard]] const Data* rewind_adapt( const Data* start, const count_position c ) noexcept
   {
      return start + c.count;
   }

   template< typename Data >
   [[nodiscard]] const Data* rewind_adapt( const Data* /*unused*/, const pointer_position< Data > p ) noexcept
   {
      return p.data;
   }

   template< typename T >
   [[nodiscard]] bool equal( const T& position, const std::size_t line, const std::size_t column, const std::size_t count ) noexcept
   {
      if( ( position.count == count ) && ( position.line == line ) && ( position.column == column ) ) {
         return true;
      }
      const text_position reference( line, column, count );
      std::cerr << "Not equal position " << position << " reference " << reference << std::endl;
      return false;
   }

   [[nodiscard]] inline std::string endless( const std::string& data, const std::size_t offset, const std::size_t count )
   {
      std::string t;

      while( t.size() < ( offset + count ) ) {
         t += data;
      }
      return t.substr( offset, count );
   }

   // Compiler warnings are a good thing, except when you are writing low-level test code and it makes some very simple things more complicated.

   template< typename I >
   [[nodiscard]] constexpr I choose_int( const long s, const long u ) noexcept
   {
      assert( s < 0 );
      assert( u > 0 );

      if constexpr( std::is_signed_v< I > ) {
         return I( s );
      }
      else {
         return I( u );
      }
   }

   [[nodiscard]] inline constexpr char choose_char( const long s, const long u ) noexcept
   {
      return choose_int< char >( s, u );
   }

}  // namespace TAO_PEGTL_NAMESPACE::test

#endif
