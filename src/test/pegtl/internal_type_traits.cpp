// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <tao/pegtl/ascii.hpp>
#include <tao/pegtl/inputs.hpp>
#include <tao/pegtl/utf8.hpp>

#include <tao/pegtl/internal/type_traits.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   enum class test_enum
   {
   };

   static_assert( internal::is_simple_type_v< int > );
   static_assert( internal::is_simple_type_v< long > );
   static_assert( internal::is_simple_type_v< char32_t > );
   static_assert( internal::is_simple_type_v< std::byte > );
   static_assert( internal::is_simple_type_v< test_enum > );

   static_assert( !internal::is_simple_type_v< int& > );
   static_assert( !internal::is_simple_type_v< long& > );
   static_assert( !internal::is_simple_type_v< char32_t& > );
   static_assert( !internal::is_simple_type_v< std::byte& > );
   static_assert( !internal::is_simple_type_v< test_enum& > );

   static_assert( !internal::is_simple_type_v< int* > );
   static_assert( !internal::is_simple_type_v< long* > );
   static_assert( !internal::is_simple_type_v< char32_t* > );
   static_assert( !internal::is_simple_type_v< std::byte* > );
   static_assert( !internal::is_simple_type_v< test_enum* > );

   static_assert( !internal::is_simple_type_v< void > );
   static_assert( !internal::is_simple_type_v< float > );
   static_assert( !internal::is_simple_type_v< std::string > );

   static_assert( internal::is_simple_type_v< bool > );  // TODO?

   static_assert( internal::has_eol_char_rule< ascii::scan::lf > );
   static_assert( internal::has_eol_char_rule< ascii::scan::cr_crlf > );
   static_assert( !internal::has_eol_char_rule< ascii::lf > );
   static_assert( !internal::has_eol_char_rule< utf8::crlf > );

   static_assert( internal::has_eol_rule< internal::input_with_lines< ascii::any7, internal::view_input< char > > > );
   static_assert( !internal::has_eol_rule< internal::input_with_lines< void, internal::view_input< char > > > );
   static_assert( !internal::has_eol_rule< internal::view_input< char > > );

   static_assert( internal::has_start< argv_input<> > );
   static_assert( internal::has_start< view_input<> > );
   static_assert( internal::has_start< file_input<> > );
   static_assert( internal::has_start< read_input<> > );
   static_assert( internal::has_start< mmap_input<> > );

   static_assert( !internal::has_start< base_input<> > );

}  // namespace TAO_PEGTL_NAMESPACE

int main()
{
   return 0;
}
