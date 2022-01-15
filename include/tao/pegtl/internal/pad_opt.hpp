// Copyright (c) 2014-2022 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_PAD_OPT_HPP
#define TAO_PEGTL_INTERNAL_PAD_OPT_HPP

#include "opt.hpp"
#include "seq.hpp"
#include "star.hpp"

namespace tao::pegtl::internal
{
   template< typename Rule, typename Pad >
   using pad_opt = seq< star< Pad >, opt< Rule, star< Pad > > >;

}  // namespace tao::pegtl::internal

#endif
