// Copyright (c) 2024-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <cstdint>

#include "test.hpp"

#if defined( __cpp_exceptions )

#include <tao/pegtl/contrib/integer.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      for( char c = 2; c < 122; ++c ) {
         TAO_PEGTL_TEST_ASSERT( internal::is_digit( c ) == ( ( '0' <= c ) && ( c <= '9' ) ) );
      }
      {
         std::uint16_t t = 0;
         TAO_PEGTL_TEST_ASSERT( internal::accumulate_digit( t, '0' ) );
         TAO_PEGTL_TEST_ASSERT( t == 0 );
         TAO_PEGTL_TEST_ASSERT( internal::accumulate_digit( t, '6' ) );
         TAO_PEGTL_TEST_ASSERT( t == 6 );
         TAO_PEGTL_TEST_ASSERT( internal::accumulate_digit( t, '5' ) );
         TAO_PEGTL_TEST_ASSERT( t == 65 );
         TAO_PEGTL_TEST_ASSERT( internal::accumulate_digit( t, '5' ) );
         TAO_PEGTL_TEST_ASSERT( t == 655 );
         TAO_PEGTL_TEST_ASSERT( internal::accumulate_digit( t, '3' ) );
         TAO_PEGTL_TEST_ASSERT( t == 6553 );
         TAO_PEGTL_TEST_ASSERT( internal::accumulate_digit( t, '5' ) );
         TAO_PEGTL_TEST_ASSERT( t == 65535 );
         TAO_PEGTL_TEST_ASSERT( !internal::accumulate_digit( t, '0' ) );
         TAO_PEGTL_TEST_ASSERT( t == 65535 );
      }
      {
         std::uint16_t t = 0;
         TAO_PEGTL_TEST_ASSERT( internal::accumulate_digit( t, '0' ) );
         TAO_PEGTL_TEST_ASSERT( t == 0 );
         TAO_PEGTL_TEST_ASSERT( internal::accumulate_digit( t, '6' ) );
         TAO_PEGTL_TEST_ASSERT( t == 6 );
         TAO_PEGTL_TEST_ASSERT( internal::accumulate_digit( t, '5' ) );
         TAO_PEGTL_TEST_ASSERT( t == 65 );
         TAO_PEGTL_TEST_ASSERT( internal::accumulate_digit( t, '5' ) );
         TAO_PEGTL_TEST_ASSERT( t == 655 );
         TAO_PEGTL_TEST_ASSERT( internal::accumulate_digit( t, '3' ) );
         TAO_PEGTL_TEST_ASSERT( t == 6553 );
         TAO_PEGTL_TEST_ASSERT( !internal::accumulate_digit( t, '6' ) );
         TAO_PEGTL_TEST_ASSERT( t == 6553 );
      }
      {
         std::uint16_t t = 0;
         TAO_PEGTL_TEST_ASSERT( internal::accumulate_digits( t, "000" ) );
         TAO_PEGTL_TEST_ASSERT( t == 0 );
         TAO_PEGTL_TEST_ASSERT( internal::accumulate_digits( t, "00065535" ) );
         TAO_PEGTL_TEST_ASSERT( t == 65535 );
         t = 0;
         TAO_PEGTL_TEST_ASSERT( !internal::accumulate_digits( t, "000655300" ) );
         TAO_PEGTL_TEST_ASSERT( t == 65530 );
         t = 0;
         TAO_PEGTL_TEST_ASSERT( !internal::accumulate_digits( t, "00065536" ) );
         TAO_PEGTL_TEST_ASSERT( t == 6553 );
      }
      {
         std::uint16_t t = 0;
         TAO_PEGTL_TEST_ASSERT( internal::convert_positive( t, "000" ) );
         TAO_PEGTL_TEST_ASSERT( t == 0 );
         TAO_PEGTL_TEST_ASSERT( internal::convert_positive( t, "00065535" ) );
         TAO_PEGTL_TEST_ASSERT( t == 65535 );
         t = 0;
         TAO_PEGTL_TEST_ASSERT( !internal::convert_positive( t, "000655300" ) );
         TAO_PEGTL_TEST_ASSERT( t == 65530 );
         t = 0;
         TAO_PEGTL_TEST_ASSERT( !internal::convert_positive( t, "00065536" ) );
         TAO_PEGTL_TEST_ASSERT( t == 6553 );
      }
      {
         std::int16_t t = 0;
         TAO_PEGTL_TEST_ASSERT( internal::convert_positive( t, "000" ) );
         TAO_PEGTL_TEST_ASSERT( t == 0 );
         TAO_PEGTL_TEST_ASSERT( internal::convert_positive( t, "00032767" ) );
         TAO_PEGTL_TEST_ASSERT( t == 32767 );
         t = 0;
         TAO_PEGTL_TEST_ASSERT( !internal::convert_positive( t, "000327600" ) );
         TAO_PEGTL_TEST_ASSERT( t == 32760 );
         t = 0;
         TAO_PEGTL_TEST_ASSERT( !internal::convert_positive( t, "00032768" ) );
         TAO_PEGTL_TEST_ASSERT( t == 3276 );
      }
      {
         std::int16_t t = 0;
         TAO_PEGTL_TEST_ASSERT( internal::convert_negative( t, "000" ) );
         TAO_PEGTL_TEST_ASSERT( t == 0 );
         TAO_PEGTL_TEST_ASSERT( internal::convert_negative( t, "00032768" ) );
         TAO_PEGTL_TEST_ASSERT( t == -32768 );
         t = 0;
         TAO_PEGTL_TEST_ASSERT( !internal::convert_negative( t, "000327600" ) );
         TAO_PEGTL_TEST_ASSERT( t == 0 );
         t = 0;
         TAO_PEGTL_TEST_ASSERT( !internal::convert_negative( t, "00032769" ) );
         TAO_PEGTL_TEST_ASSERT( t == 0 );
      }
      {
         std::uint16_t t = 0;
         TAO_PEGTL_TEST_ASSERT( internal::convert_unsigned( t, "000" ) );
         TAO_PEGTL_TEST_ASSERT( t == 0 );
         TAO_PEGTL_TEST_ASSERT( internal::convert_unsigned( t, "00065535" ) );
         TAO_PEGTL_TEST_ASSERT( t == 65535 );
         t = 0;
         TAO_PEGTL_TEST_ASSERT( !internal::convert_unsigned( t, "000655300" ) );
         TAO_PEGTL_TEST_ASSERT( t == 65530 );
         t = 0;
         TAO_PEGTL_TEST_ASSERT( !internal::convert_unsigned( t, "00065536" ) );
         TAO_PEGTL_TEST_ASSERT( t == 6553 );
      }
      {
         std::int16_t t = 0;
         TAO_PEGTL_TEST_ASSERT( internal::convert_signed( t, "000" ) );
         TAO_PEGTL_TEST_ASSERT( t == 0 );
         TAO_PEGTL_TEST_ASSERT( internal::convert_signed( t, "00032767" ) );
         TAO_PEGTL_TEST_ASSERT( t == 32767 );
         t = 0;
         TAO_PEGTL_TEST_ASSERT( !internal::convert_signed( t, "000327600" ) );
         TAO_PEGTL_TEST_ASSERT( t == 32760 );
         t = 0;
         TAO_PEGTL_TEST_ASSERT( !internal::convert_signed( t, "00032768" ) );
         TAO_PEGTL_TEST_ASSERT( t == 3276 );
      }
      {
         std::int16_t t = 0;
         TAO_PEGTL_TEST_ASSERT( internal::convert_signed( t, "+000" ) );
         TAO_PEGTL_TEST_ASSERT( t == 0 );
         TAO_PEGTL_TEST_ASSERT( internal::convert_signed( t, "+00032767" ) );
         TAO_PEGTL_TEST_ASSERT( t == 32767 );
         t = 0;
         TAO_PEGTL_TEST_ASSERT( !internal::convert_signed( t, "+000327600" ) );
         TAO_PEGTL_TEST_ASSERT( t == 32760 );
         t = 0;
         TAO_PEGTL_TEST_ASSERT( !internal::convert_signed( t, "+00032768" ) );
         TAO_PEGTL_TEST_ASSERT( t == 3276 );
      }
      {
         std::int16_t t = 0;
         TAO_PEGTL_TEST_ASSERT( internal::convert_signed( t, "-000" ) );
         TAO_PEGTL_TEST_ASSERT( t == 0 );
         TAO_PEGTL_TEST_ASSERT( internal::convert_signed( t, "-00032768" ) );
         TAO_PEGTL_TEST_ASSERT( t == -32768 );
         t = 0;
         TAO_PEGTL_TEST_ASSERT( !internal::convert_signed( t, "-000327600" ) );
         TAO_PEGTL_TEST_ASSERT( t == 0 );
         t = 0;
         TAO_PEGTL_TEST_ASSERT( !internal::convert_signed( t, "-00032769" ) );
         TAO_PEGTL_TEST_ASSERT( t == 0 );
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE

#else

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      std::cerr << "Skipping convert test..." << std::endl;
   }

}  // namespace TAO_PEGTL_NAMESPACE

#endif

#include "main.hpp"
