// Copyright (c) 2021-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_INPUT_DOUBLE_PATH_HPP
#define TAO_PEGTL_INTERNAL_INPUT_DOUBLE_PATH_HPP

#include <filesystem>
#include <utility>

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Input >
   class input_double_path
      : public Input
   {
   public:
      template< typename... Ts >
      explicit input_double_path( std::filesystem::path&& s, Ts&&... ts )
         : Input( std::move( s ), static_cast< const std::filesystem::path& >( s ), std::forward< Ts >( ts )... )
      {
         // We can use std::move( s ) as first argument to input_with_source because we know that input_with_source will initialize its m_source member from the first argument last.
      }

      template< typename... Ts >
      explicit input_double_path( const std::filesystem::path& s, Ts&&... ts )
         : Input( s, s, std::forward< Ts >( ts )... )
      {}
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
