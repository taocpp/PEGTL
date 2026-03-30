// Copyright (c) 2021-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_DEPTH_GUARD_HPP
#define TAO_PEGTL_INTERNAL_DEPTH_GUARD_HPP

#include <cstddef>

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   class [[nodiscard]] depth_guard
   {
   public:
      explicit depth_guard( std::size_t& depth ) noexcept
         : m_depth( depth )
      {
         ++m_depth;
      }

      depth_guard( depth_guard&& ) = delete;
      depth_guard( const depth_guard& ) = delete;

      ~depth_guard()
      {
         --m_depth;
      }

      void operator=( depth_guard&& ) = delete;
      void operator=( const depth_guard& ) = delete;

      [[nodiscard]] std::size_t current_depth() const noexcept
      {
         return m_depth;
      }

   private:
      std::size_t& m_depth;
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
