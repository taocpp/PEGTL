// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <type_traits>

#include <tao/pegtl/ascii.hpp>
#include <tao/pegtl/utf8.hpp>
#include <tao/pegtl/utf32.hpp>

#include <tao/pegtl/internal/get_eol_rule_char.hpp>

using namespace TAO_PEGTL_NAMESPACE;

int main()
{
   static_assert( internal::get_eol_rule_char_v< void > == false );
   static_assert( std::is_same_v< decltype( internal::get_eol_rule_char_v< void > ), const bool > );
   static_assert( internal::get_eol_rule_char_v< char > == false );
   static_assert( std::is_same_v< decltype( internal::get_eol_rule_char_v< char > ), const bool > );
   static_assert( internal::get_eol_rule_char_v< std::string > == false );
   static_assert( std::is_same_v< decltype( internal::get_eol_rule_char_v< std::string > ), const bool > );

   static_assert( internal::get_eol_rule_char_v< ascii::lf > == '\n' );
   static_assert( std::is_same_v< decltype( internal::get_eol_rule_char_v< ascii::lf > ), const char > );
   static_assert( internal::get_eol_rule_char_v< ascii::cr_lf > == false );
   static_assert( std::is_same_v< decltype( internal::get_eol_rule_char_v< ascii::cr_lf > ), const bool > );

   static_assert( internal::get_eol_rule_char_v< utf8::lf > == '\n' );
   static_assert( std::is_same_v< decltype( internal::get_eol_rule_char_v< utf8::lf > ), const char > );
   static_assert( internal::get_eol_rule_char_v< utf8::cr_lf > == false );
   static_assert( std::is_same_v< decltype( internal::get_eol_rule_char_v< utf8::cr_lf > ), const bool > );

   static_assert( internal::get_eol_rule_char_v< utf32::lf > == '\n' );
   static_assert( std::is_same_v< decltype( internal::get_eol_rule_char_v< utf32::lf > ), const char32_t > );
   static_assert( internal::get_eol_rule_char_v< utf32::cr_lf > == false );
   static_assert( std::is_same_v< decltype( internal::get_eol_rule_char_v< utf32::cr_lf > ), const bool > );

   static_assert( internal::get_eol_rule_char_v< ascii::digit > == false );
   static_assert( std::is_same_v< decltype( internal::get_eol_rule_char_v< ascii::digit > ), const bool > );

   return 0;
}
