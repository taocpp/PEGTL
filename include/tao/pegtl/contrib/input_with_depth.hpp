// Copyright (c) 2021-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_CONTRIB_INPUT_WITH_DEPTH_HPP
#define TAO_PEGTL_CONTRIB_INPUT_WITH_DEPTH_HPP

#include <cstddef>
#include <type_traits>

#include "../config.hpp"

#include "../internal/type_traits.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   namespace internal
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

   }  // namespace internal

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
