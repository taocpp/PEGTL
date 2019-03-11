// Copyright (c) 2017-2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_INTERNAL_ALNUM_HPP
#define TAO_PEGTL_INTERNAL_ALNUM_HPP

#include "../config.hpp"

#include "peek_char.hpp"
#include "ranges.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   using alnum = ranges< peek_char, 'a', 'z', 'A', 'Z', '0', '9' >;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
