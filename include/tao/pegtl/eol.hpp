// Copyright (c) 2016-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_EOL_HPP
#define TAOCPP_PEGTL_INCLUDE_EOL_HPP

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

#include "cr_crlf_eol.hpp"
#include "cr_eol.hpp"
#include "crlf_eol.hpp"
#include "lf_crlf_eol.hpp"
#include "lf_eol.hpp"

#endif
