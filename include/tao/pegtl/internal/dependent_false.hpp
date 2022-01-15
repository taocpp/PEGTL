// Copyright (c) 2018-2022 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_DEPENDENT_FALSE_HPP
#define TAO_PEGTL_INTERNAL_DEPENDENT_FALSE_HPP

namespace tao::pegtl::internal
{
   template< typename... >
   inline constexpr bool dependent_false = false;

}  // namespace tao::pegtl::internal

#endif
