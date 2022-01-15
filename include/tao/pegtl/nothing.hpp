// Copyright (c) 2014-2022 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_NOTHING_HPP
#define TAO_PEGTL_NOTHING_HPP

namespace tao::pegtl
{
   template< typename Rule >
   struct nothing
   {};

   using maybe_nothing = nothing< void >;

}  // namespace tao::pegtl

#endif
