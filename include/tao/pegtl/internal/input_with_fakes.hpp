// Copyright (c) 2021-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_INPUT_WITH_FAKES_HPP
#define TAO_PEGTL_INTERNAL_INPUT_WITH_FAKES_HPP

#include <cstddef>
#include <utility>

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Input >
   class input_with_fakes
      : public Input
   {
   public:
      using Input::Input;

      [[nodiscard]] decltype( auto ) end( const std::size_t /*unused*/ = 0 ) const noexcept( noexcept( std::declval< Input >().size() ) )
      {
         return Input::end();
      }

      [[nodiscard]] std::size_t size( const std::size_t /*unused*/ = 0 ) const noexcept( noexcept( std::declval< Input >().size() ) )
      {
         return Input::size();
      }

      // [[nodiscard]] std::size_t size( const std::size_t /*unused*/, const std::size_t /*unused*/ ) noexcept( noexcept( std::declval< Input >().size() ) )
      // {
      //    return Input::size();
      // }

      void require( const std::size_t /*unused*/ ) noexcept
      {}

      void discard() noexcept
      {}

      // The buffer_foo() member functions are only for actual buffer inputs.
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
