// Copyright (c) 2014-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_EOLS_HPP
#define TAO_PEGTL_EOLS_HPP

#include "ascii.hpp"
#include "config.hpp"

namespace TAO_PEGTL_NAMESPACE::eols
{
   namespace scan
   {
      // clang-format off
      struct cr : ascii::cr { using eol_char_rule = ascii::cr; };
      struct lf : ascii::lf { using eol_char_rule = ascii::lf; };

      struct cr_crlf : ascii::cr_crlf { using eol_char_rule = ascii::cr; };
      struct lf_crlf : ascii::lf_crlf { using eol_char_rule = ascii::lf; };
      // clang-format on
   }  // namespace scan

}  // namespace TAO_PEGTL_NAMESPACE::eols

#endif
