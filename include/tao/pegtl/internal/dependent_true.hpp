// Copyright (c) 2022 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_INTERNAL_DEPENDENT_TRUE_HPP
#define TAO_PEGTL_INTERNAL_DEPENDENT_TRUE_HPP

namespace tao::pegtl::internal
{
   template< typename... >
   inline constexpr bool dependent_true = true;

}  // namespace tao::pegtl::internal

#endif
