// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <cstddef>
#include <cstdint>

#include <tao/pegtl/internal/integer_size.hpp>
#include <tao/pegtl/internal/view_input.hpp>

using namespace TAO_PEGTL_NAMESPACE;

int main()
{
   static_assert( internal::integer_size< char, char >() == 1 );
   static_assert( internal::integer_size< char, std::uint8_t >() == 1 );
   static_assert( internal::integer_size< short, std::uint16_t >() == 1 );  // Yes, I know...
   static_assert( internal::integer_size< int, unsigned >() == 1 );
   static_assert( internal::integer_size< std::int32_t, char32_t >() == 1 );

   static_assert( internal::integer_size< char32_t, char >() == sizeof( char32_t ) );
   static_assert( internal::integer_size< std::int64_t, std::byte >() == sizeof( std::int64_t ) );

   static_assert( internal::integer_input_size< char, internal::view_input< char > >() == 1 );
   static_assert( internal::integer_input_size< char, internal::view_input< std::uint8_t > >() == 1 );
   static_assert( internal::integer_input_size< short, internal::view_input< std::uint16_t > >() == 1 );  // Yes, I know...
   static_assert( internal::integer_input_size< int, internal::view_input< unsigned > >() == 1 );
   static_assert( internal::integer_input_size< std::int32_t, internal::view_input< char32_t > >() == 1 );

   static_assert( internal::integer_input_size< char32_t, internal::view_input< char > >() == sizeof( char32_t ) );
   static_assert( internal::integer_input_size< std::int64_t, internal::view_input< std::byte > >() == sizeof( std::int64_t ) );

   return 0;
}
