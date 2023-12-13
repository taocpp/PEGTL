// Copyright (c) 2014-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_ENUM_INVERT_BOOL_HPP
#define TAO_PEGTL_INTERNAL_ENUM_INVERT_BOOL_HPP

#include <type_traits>

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Enum >
   [[nodiscard]] constexpr Enum enum_invert_bool( const Enum e ) noexcept
   {
      static_assert( std::is_enum_v< Enum > );
      static_assert( std::is_same_v< std::underlying_type_t< Enum >, bool > );

      return Enum( !bool( e ) );
   }

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
