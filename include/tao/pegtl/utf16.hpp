// Copyright (c) 2015-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_UTF16_HPP
#define TAO_PEGTL_UTF16_HPP

#include <cstddef>

#include "config.hpp"

#include "internal/peek_utf16.hpp"
#include "internal/rules.hpp"

#define TAO_PEGTL_UTF_NAME utf16_be
#include "internal/utf_rules.ipp"  // NOLINT(readability-duplicate-include)

#define TAO_PEGTL_UTF_NAME utf16_le
#include "internal/utf_rules.ipp"  // NOLINT(readability-duplicate-include)

namespace TAO_PEGTL_NAMESPACE
{
   namespace utf16 = TAO_PEGTL_ENDIAN_SUFFIXED( utf16_ );

}  // namespace TAO_PEGTL_NAMESPACE

// FCOV_EXCL_FILE
#endif
