// Copyright (c) 2018-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_UINT16_HPP
#define TAO_PEGTL_UINT16_HPP

#include "config.hpp"

#include "internal/peek_endian.hpp"
#include "internal/peek_masked.hpp"
#include "internal/rules.hpp"
#include "internal/using.hpp"

#define TAO_PEGTL_INT_NAME uint16_be
#define TAO_PEGTL_INT_TYPE std::uint16_t
#include "internal/int_rules.ipp"

#define TAO_PEGTL_INT_NAME uint16_le
#define TAO_PEGTL_INT_TYPE std::uint16_t
#include "internal/int_rules.ipp"

namespace TAO_PEGTL_NAMESPACE
{
   namespace uint16 = TAO_PEGTL_ENDIAN_SUFFIXED( uint16_ );

}  // namespace TAO_PEGTL_NAMESPACE

// FCOV_EXCL_FILE
#endif
