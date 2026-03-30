// Copyright (c) 2021-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INPUT_WITH_DEPTH_HPP
#define TAO_PEGTL_INPUT_WITH_DEPTH_HPP

#include <cstddef>

#include "config.hpp"

#include "internal/depth_guard.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Input >
   class input_with_depth
      : public Input
   {
   public:
      using Input::Input;

      [[nodiscard]] internal::depth_guard make_depth_guard() noexcept
      {
         return internal::depth_guard( m_depth );  // NOLINT(google-readability-casting)
      }

      [[nodiscard]] std::size_t current_depth() const noexcept
      {
         return m_depth;
      }

   private:
      std::size_t m_depth = 0;
   };

}  // namespace TAO_PEGTL_NAMESPACE

#endif
