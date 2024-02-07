// Copyright (c) 2023-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <cstddef>
#include <string>

#include "test.hpp"
#include "test_utility.hpp"

#include <tao/pegtl/text_position.hpp>
#include <tao/pegtl/internal/data_and_size.hpp>
#include <tao/pegtl/internal/dependent_false.hpp>
#include <tao/pegtl/internal/lazy_scan_traits.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   struct dummy_peek
   {
      using data_t = char;
      using pair_t = internal::data_and_size< char, std::uint8_t >;

      template< typename ParseInput >
      [[nodiscard]] static constexpr bool bulk() noexcept
      {
         static_assert( internal::dependent_false< ParseInput > );
         return false;
      }

      template< typename ParseInput >
      [[nodiscard]] static constexpr std::size_t size() noexcept
      {
         static_assert( internal::dependent_false< ParseInput > );
         return 0;
      }

      template< typename ParseInput >
      [[nodiscard]] static pair_t peek( ParseInput& /*unused*/, const std::size_t /*unused*/ = 0 ) noexcept
      {
         static_assert( internal::dependent_false< ParseInput > );
         return pair_t( 0, 0 );
      }
   };

   template< typename Eol >
   void lazy_test( const std::string& str, const std::size_t line, const std::size_t column, const std::size_t count )
   {
      text_position pos;
      internal::lazy_scan_traits< typename Eol::rule_t >::scan( pos, str.data(), str.size() );
      TAO_PEGTL_TEST_ASSERT( test::equal( pos, line, column, count ) );
   }

   void unit_test()
   {
      struct dummy_cr
         : internal::tester< internal::one< dummy_peek, '\r' > >
      {};

      lazy_test< dummy_cr >( "", 1, 1, 0 );
      lazy_test< dummy_cr >( " ", 1, 2, 1 );
      lazy_test< dummy_cr >( "\n", 1, 2, 1 );
      lazy_test< dummy_cr >( "     ", 1, 6, 5 );
      lazy_test< dummy_cr >( "\r", 2, 1, 1 );
      lazy_test< dummy_cr >( " \r", 2, 1, 2 );
      lazy_test< dummy_cr >( "   \r", 2, 1, 4 );
      lazy_test< dummy_cr >( "\r ", 2, 2, 2 );
      lazy_test< dummy_cr >( "   \r ", 2, 2, 5 );
      lazy_test< dummy_cr >( "   \r   ", 2, 4, 7 );
      lazy_test< dummy_cr >( "\r\r", 3, 1, 2 );
      lazy_test< dummy_cr >( "\r \r", 3, 1, 3 );
      lazy_test< dummy_cr >( " \r \r", 3, 1, 4 );
      lazy_test< dummy_cr >( " \r \r ", 3, 2, 5 );
      lazy_test< dummy_cr >( "   \r\r\r   \r\r\r", 7, 1, 12 );
      lazy_test< dummy_cr >( "   \r\r\r   \r\r\r   ", 7, 4, 15 );

      lazy_test< ascii::lf >( "", 1, 1, 0 );
      lazy_test< ascii::lf >( " ", 1, 2, 1 );
      lazy_test< ascii::lf >( "\r", 1, 2, 1 );
      lazy_test< ascii::lf >( "     ", 1, 6, 5 );
      lazy_test< ascii::lf >( "\n", 2, 1, 1 );
      lazy_test< ascii::lf >( " \n", 2, 1, 2 );
      lazy_test< ascii::lf >( "   \n", 2, 1, 4 );
      lazy_test< ascii::lf >( "\n ", 2, 2, 2 );
      lazy_test< ascii::lf >( "   \n ", 2, 2, 5 );
      lazy_test< ascii::lf >( "   \n   ", 2, 4, 7 );
      lazy_test< ascii::lf >( "\n\n", 3, 1, 2 );
      lazy_test< ascii::lf >( "\n \n", 3, 1, 3 );
      lazy_test< ascii::lf >( " \n \n", 3, 1, 4 );
      lazy_test< ascii::lf >( " \n \n ", 3, 2, 5 );
      lazy_test< ascii::lf >( "   \n\n\n   \n\n\n", 7, 1, 12 );
      lazy_test< ascii::lf >( "   \n\n\n   \n\n\n   ", 7, 4, 15 );

      lazy_test< ascii::crlf >( "", 1, 1, 0 );
      lazy_test< ascii::crlf >( " ", 1, 2, 1 );
      lazy_test< ascii::crlf >( "  ", 1, 3, 2 );
      lazy_test< ascii::crlf >( "\r", 1, 2, 1 );
      lazy_test< ascii::crlf >( "\n", 1, 2, 1 );
      lazy_test< ascii::crlf >( "\n\r", 1, 3, 2 );
      lazy_test< ascii::crlf >( "\r\n", 2, 1, 2 );
      lazy_test< ascii::crlf >( "  \r\n  ", 2, 3, 6 );

      lazy_test< ascii::lf_crlf >( "", 1, 1, 0 );
      lazy_test< ascii::lf_crlf >( " ", 1, 2, 1 );
      lazy_test< ascii::lf_crlf >( "     ", 1, 6, 5 );
      lazy_test< ascii::lf_crlf >( "\n", 2, 1, 1 );
      lazy_test< ascii::lf_crlf >( " \n", 2, 1, 2 );
      lazy_test< ascii::lf_crlf >( "   \n", 2, 1, 4 );
      lazy_test< ascii::lf_crlf >( "\n ", 2, 2, 2 );
      lazy_test< ascii::lf_crlf >( "   \n ", 2, 2, 5 );
      lazy_test< ascii::lf_crlf >( "   \n   ", 2, 4, 7 );
      lazy_test< ascii::lf_crlf >( "\n\n", 3, 1, 2 );
      lazy_test< ascii::lf_crlf >( "\n \n", 3, 1, 3 );
      lazy_test< ascii::lf_crlf >( " \n \n", 3, 1, 4 );
      lazy_test< ascii::lf_crlf >( " \n \n ", 3, 2, 5 );
      lazy_test< ascii::lf_crlf >( "   \n\n\n   \n\n\n", 7, 1, 12 );
      lazy_test< ascii::lf_crlf >( "   \n\n\n   \n\n\n   ", 7, 4, 15 );

      lazy_test< ascii::lf_crlf >( "", 1, 1, 0 );
      lazy_test< ascii::lf_crlf >( " ", 1, 2, 1 );
      lazy_test< ascii::lf_crlf >( "  ", 1, 3, 2 );
      lazy_test< ascii::lf_crlf >( "\r", 1, 2, 1 );
      lazy_test< ascii::lf_crlf >( "\n", 2, 1, 1 );
      lazy_test< ascii::lf_crlf >( "\n\r", 2, 2, 2 );
      lazy_test< ascii::lf_crlf >( "\r\n", 2, 1, 2 );
      lazy_test< ascii::lf_crlf >( "  \r\n  ", 2, 3, 6 );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
