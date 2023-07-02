// Copyright (c) 2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_PARSE_ERROR_BASE_HPP
#define TAO_PEGTL_PARSE_ERROR_BASE_HPP

#include <cstddef>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

#include "config.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   class parse_error_base
      : public std::runtime_error
   {
   public:
      [[nodiscard]] std::string_view message() const noexcept
      {
         return { what() + m_position_size + 2, m_message_size };
      }

      [[nodiscard]] std::string_view position_string() const noexcept
      {
         return { what(), m_position_size };
      }

   protected:
      const std::size_t m_message_size;
      const std::size_t m_position_size;

      parse_error_base( const std::string& msg, const std::string& pos )
         : std::runtime_error( pos + ": " + msg ),
           m_message_size( msg.size() ),
           m_position_size( pos.size() )
      {}
   };

}  // namespace TAO_PEGTL_NAMESPACE

#endif
