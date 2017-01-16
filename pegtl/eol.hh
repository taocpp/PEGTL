// Copyright (c) 2016-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_EOL_HH
#define PEGTL_EOL_HH

#include <utility>
#include <cstdlib>

#include "config.hh"

namespace PEGTL_NAMESPACE
{
   using eol_pair = std::pair< bool, std::size_t >;

} // namespace PEGTL_NAMESPACE

#include "lf_eol.hh"
#include "cr_eol.hh"
#include "crlf_eol.hh"
#include "lf_crlf_eol.hh"
#include "cr_crlf_eol.hh"

#endif
