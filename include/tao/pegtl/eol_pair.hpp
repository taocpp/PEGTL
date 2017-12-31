// Copyright (c) 2017-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_EOL_PAIR_HPP
#define TAOCPP_PEGTL_INCLUDE_EOL_PAIR_HPP

#include <cstddef>
#include <utility>

#include "config.hpp"

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      using eol_pair = std::pair< bool, std::size_t >;

   }  // namespace TAOCPP_PEGTL_NAMESPACE

}  // namespace tao

#endif
