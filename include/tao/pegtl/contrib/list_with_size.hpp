// Copyright (c) 2022 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_CONTRIB_LIST_WITH_SIZE_HPP
#define TAO_PEGTL_CONTRIB_LIST_WITH_SIZE_HPP

#include <cstddef>

#include "../internal/pad.hpp"
#include "../internal/rep.hpp"
#include "../internal/seq.hpp"

namespace tao::pegtl
{
   namespace internal
   {
      template< std::size_t Num, typename Rule, typename Sep >
      using list_with_size = seq< Rule, rep< Num - 1, Sep, Rule > >;

   }  // namespace internal

   // clang-format off
   template< std::size_t Num, typename Rule, typename Sep, typename Pad = void > struct list_with_size : internal::list_with_size< Num, Rule, internal::pad< Sep, Pad > > { static_assert( Num > 1 ); };
   template< std::size_t Num, typename Rule, typename Sep > struct list_with_size< Num, Rule, Sep, void > : internal::list_with_size< Num, Rule, Sep > { static_assert( Num > 1 ); };
   // clang-format on

}  // namespace tao::pegtl

#endif
