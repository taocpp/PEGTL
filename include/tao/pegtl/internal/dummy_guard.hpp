// Copyright (c) 2014-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_DUMMY_GUARD_HPP
#define TAO_PEGTL_INTERNAL_DUMMY_GUARD_HPP

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   class dummy_guard
   {
   public:
      constexpr dummy_guard() noexcept = default;

      dummy_guard( dummy_guard&& ) = delete;
      dummy_guard( const dummy_guard& ) = delete;

      ~dummy_guard() = default;

      void operator=( dummy_guard&& ) = delete;
      void operator=( const dummy_guard& ) = delete;

      [[nodiscard]] constexpr bool operator()( const bool result ) const noexcept
      {
         return result;
      }
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
