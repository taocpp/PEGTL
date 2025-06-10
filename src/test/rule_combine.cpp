// Copyright (c) 2024-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <type_traits>

#include <tao/pegtl/ascii.hpp>
#include <tao/pegtl/rules.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      static_assert( std::is_same_v< combine< ascii::one< 'a' >, ascii::one< 'b' > >::rule_t, ascii::one< 'a', 'b' >::rule_t > );
      static_assert( std::is_same_v< combine< ascii::one< 'a', 'c' >, ascii::one< 'b' > >::rule_t, ascii::one< 'a', 'c', 'b' >::rule_t > );
      static_assert( std::is_same_v< combine< ascii::one< 'a', 'c' >, ascii::one< 'b', 'd' > >::rule_t, ascii::one< 'a', 'c', 'b', 'd' >::rule_t > );
      static_assert( std::is_same_v< combine< ascii::one< 'c' >, ascii::one< 'b', 'd' > >::rule_t, ascii::one< 'c', 'b', 'd' >::rule_t > );

      static_assert( std::is_same_v< combine< ascii::cr, ascii::lf >::rule_t, ascii::one< '\r', '\n' >::rule_t > );

      static_assert( std::is_same_v< combine< ascii::digit, ascii::lower >::rule_t, ascii::ranges< '0', '9', 'a', 'z' >::rule_t > );

      static_assert( std::is_same_v< combine< ascii::range< '0', '5' >, ascii::range< '5', '9' > >::rule_t, ascii::digit::rule_t > );
      static_assert( std::is_same_v< combine< ascii::range< '4', '9' >, ascii::range< '0', '4' > >::rule_t, ascii::digit::rule_t > );

      static_assert( std::is_same_v< combine< ascii::range< '0', '7' >, ascii::range< '0', '9' > >::rule_t, ascii::digit::rule_t > );
      static_assert( std::is_same_v< combine< ascii::range< '4', '7' >, ascii::range< '0', '9' > >::rule_t, ascii::digit::rule_t > );
      static_assert( std::is_same_v< combine< ascii::range< '4', '9' >, ascii::range< '0', '9' > >::rule_t, ascii::digit::rule_t > );
      static_assert( std::is_same_v< combine< ascii::range< '4', '9' >, ascii::range< '0', '6' > >::rule_t, ascii::digit::rule_t > );
   }

}  // namespace TAO_PEGTL_NAMESPACE

int main()
{
   return 0;
}
