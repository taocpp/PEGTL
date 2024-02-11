// Copyright (c) 2014-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_UTF32_HPP
#define TAO_PEGTL_UTF32_HPP

#include "config.hpp"

#include "internal/peek_utf32.hpp"
#include "internal/rules.hpp"

#define TAO_PEGTL_UTF_NAME utf32_be
#include "internal/utf_rules.ipp"

#define TAO_PEGTL_UTF_NAME utf32_le
#include "internal/utf_rules.ipp"

namespace TAO_PEGTL_NAMESPACE
{
   namespace utf32 = TAO_PEGTL_ENDIAN_SUFFIXED( utf32_ );

}  // namespace TAO_PEGTL_NAMESPACE

#endif
