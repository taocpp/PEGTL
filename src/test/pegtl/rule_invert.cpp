// Copyright (c) 2020-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <type_traits>

#include <tao/pegtl/ascii.hpp>
#include <tao/pegtl/rules.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      static_assert( std::is_same_v< invert< any7 >::rule_t, failure::rule_t > );
      static_assert( std::is_same_v< invert< any8 >::rule_t, failure::rule_t > );

      static_assert( std::is_same_v< invert< one< 'a' > >::rule_t, not_one7< 'a' >::rule_t > );
      static_assert( std::is_same_v< invert< one< 'a', 'b', 'c' > >::rule_t, not_one7< 'a', 'b', 'c' >::rule_t > );

      static_assert( std::is_same_v< invert< not_one7< 'a' > >::rule_t, one< 'a' >::rule_t > );
      static_assert( std::is_same_v< invert< not_one7< 'a', 'b', 'c' > >::rule_t, one< 'a', 'b', 'c' >::rule_t > );

      static_assert( std::is_same_v< invert< invert< one< 'a' > > >::rule_t, one< 'a' >::rule_t > );
      static_assert( std::is_same_v< invert< invert< one< 'a', 'b', 'c' > > >::rule_t, one< 'a', 'b', 'c' >::rule_t > );

      static_assert( std::is_same_v< invert< invert< not_one7< 'a' > > >::rule_t, not_one7< 'a' >::rule_t > );
      static_assert( std::is_same_v< invert< invert< not_one7< 'a', 'b', 'c' > > >::rule_t, not_one7< 'a', 'b', 'c' >::rule_t > );

      static_assert( std::is_same_v< invert< one< 'a' > >::rule_t, invert< invert< not_one7< 'a' > > >::rule_t > );
      static_assert( std::is_same_v< invert< one< 'a', 'b', 'c' > >::rule_t, invert< invert< not_one7< 'a', 'b', 'c' > > >::rule_t > );

      static_assert( std::is_same_v< invert< not_one7< 'a' > >::rule_t, invert< invert< one< 'a' > > >::rule_t > );
      static_assert( std::is_same_v< invert< not_one7< 'a', 'b', 'c' > >::rule_t, invert< invert< one< 'a', 'b', 'c' > > >::rule_t > );
   }

}  // namespace TAO_PEGTL_NAMESPACE

int main()
{
   return 0;
}
