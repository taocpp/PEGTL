// Copyright (c) 2021-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_LAZY_FILE_INPUT_WITH_SOURCE_HPP
#define TAO_PEGTL_INTERNAL_LAZY_FILE_INPUT_WITH_SOURCE_HPP

#include <filesystem>
#include <utility>

#include "../config.hpp"
#include "../position_with_source.hpp"

#include "file_input.hpp"
#include "input_with_lines.hpp"
#include "lazy_input_with_source.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Eol, typename Input = lazy_input_with_source< Eol, std::filesystem::path, file_input > >
   class lazy_file_input_with_source
      : public Input
   {
   public:
      using base_t = Input;
      using data_t = typename Input::data_t;
      using error_position_t = typename Input::error_position_t;
      using rewind_position_t = typename Input::rewind_position_t;

      using eol_rule = Eol;

      template< typename... Ts >
      explicit lazy_file_input_with_source( std::filesystem::path&& s, Ts&&... ts )
         : Input( std::move( s ), static_cast< const std::filesystem::path& >( s ), std::forward< Ts >( ts )... )
      {}

      template< typename... Ts >
      explicit lazy_file_input_with_source( const std::filesystem::path& s, Ts&&... ts )
         : Input( s, s, std::forward< Ts >( ts )... )
      {}
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
