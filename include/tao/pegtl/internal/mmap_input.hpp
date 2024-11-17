// Copyright (c) 2014-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_MMAP_INPUT_HPP
#define TAO_PEGTL_INTERNAL_MMAP_INPUT_HPP

#include <filesystem>

#include "../config.hpp"
#include "../count_position.hpp"
#if defined( __cpp_exceptions )
#include "../parse_error.hpp"
#endif
#include "../pointer_position.hpp"

#include "mmap_file_base.hpp"
#include "view_input.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Data >
   struct mmap_input
      : private mmap_file_base,
        public view_input< Data >
   {
      using data_t = Data;
      using error_position_t = count_position;
      using offset_position_t = count_position;
      using rewind_position_t = pointer_position< data_t >;
#if defined( __cpp_exceptions )
      using parse_error_t = parse_error< error_position_t >;
#endif

      explicit mmap_input( const std::filesystem::path& path )
         : mmap_file_base( path ),
           view_input< Data >( mmap_file_data< Data >(), mmap_file_size< Data >() )
      {}

      [[nodiscard]] const data_t* start() const noexcept
      {
         return mmap_file_data< Data >();
      }

      [[nodiscard]] const data_t* previous( const error_position_t saved ) const noexcept
      {
         return start() + saved.count;
      }

      [[nodiscard]] const data_t* previous( const rewind_position_t saved ) const noexcept
      {
         return saved.data;
      }

      void restart() noexcept
      {
         this->m_current = start();
      }

      [[nodiscard]] auto current_position() const noexcept
      {
         return previous_position( this->rewind_position() );
      }

      [[nodiscard]] auto previous_position( const error_position_t previous ) const noexcept
      {
         return previous;
      }

      [[nodiscard]] auto previous_position( const rewind_position_t previous ) const noexcept
      {
         return count_position( previous.data - start() );
      }
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
