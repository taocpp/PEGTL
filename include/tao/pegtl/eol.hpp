// Copyright (c) 2014-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_EOL_HPP
#define TAO_PEGTL_EOL_HPP

#include "config.hpp"

#include "internal/one.hpp"
#include "internal/peek_ascii.hpp"
#include "internal/result_on_found.hpp"
#include "internal/sor.hpp"
#include "internal/string.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   inline namespace ascii
   {
      // clang-format off
      struct cr : internal::one< internal::result_on_found::success, internal::peek_ascii8, '\r' > {};
      struct crlf : internal::string< '\r', '\n' > {};
      struct lf : internal::one< internal::result_on_found::success, internal::peek_ascii8, '\n' > {};
      struct lfcr : internal::string< '\n', '\r' > {};
      struct cr_lf : internal::one< internal::result_on_found::success, internal::peek_ascii8, '\r', '\n' > {};
      struct cr_crlf : internal::sor< internal::string< '\r', '\n' >, internal::one< internal::result_on_found::success, internal::peek_ascii8, '\r' > > {};
      struct lf_crlf : internal::sor< internal::one< internal::result_on_found::success, internal::peek_ascii8, '\n' >, internal::string< '\r', '\n' > > {};
      struct cr_lf_crlf : internal::sor< internal::string< '\r', '\n' >, internal::one< internal::result_on_found::success, internal::peek_ascii8, '\r', '\n' > > {};
      // clang-format on

   }  // namespace ascii

}  // namespace TAO_PEGTL_NAMESPACE

#endif
