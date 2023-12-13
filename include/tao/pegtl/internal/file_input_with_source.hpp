// Copyright (c) 2021-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_FILE_INPUT_WITH_SOURCE_HPP
#define TAO_PEGTL_INTERNAL_FILE_INPUT_WITH_SOURCE_HPP

#include <filesystem>
#include <utility>

#include "../config.hpp"
#include "../position_with_source.hpp"

#include "file_input.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   class file_input_with_source
      : public file_input
   {
   public:
      using base_t = file_input;
      using data_t = file_input::data_t;
      using error_position_t = position_with_source< std::filesystem::path, file_input::error_position_t >;
      using rewind_position_t = file_input::rewind_position_t;

      template< typename... Ts >
      explicit file_input_with_source( std::filesystem::path&& s, Ts&&... ts )
         : file_input( static_cast< const std::filesystem::path& >( s ), std::forward< Ts >( ts )... ),
           m_source( std::move( s ) )
      {}

      template< typename... Ts >
      explicit file_input_with_source( const std::filesystem::path& s, Ts&&... ts )
         : file_input( s, std::forward< Ts >( ts )... ),
           m_source( s )
      {}

      [[nodiscard]] auto current_position() const
      {
         return error_position_t( m_source, file_input::current_position() );
      }

      [[nodiscard]] auto previous_position( const rewind_position_t& saved ) const
      {
         return error_position_t( m_source, file_input::previous_position( saved ) );
      }

      [[nodiscard]] const std::filesystem::path& direct_source() const noexcept
      {
         return m_source;
      }

   protected:
      std::filesystem::path m_source;
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
