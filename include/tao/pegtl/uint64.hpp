// Copyright (c) 2018-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_UINT64_HPP
#define TAO_PEGTL_UINT64_HPP

#include "config.hpp"

#include "internal/peek_endian.hpp"
#include "internal/peek_masked.hpp"
#include "internal/rules.hpp"
#include "internal/using.hpp"

#define TAO_PEGTL_INT_NAME uint64_be
#define TAO_PEGTL_INT_TYPE std::uint64_t
#include "internal/int_rules.ipp"

#define TAO_PEGTL_INT_NAME uint64_le
#define TAO_PEGTL_INT_TYPE std::uint64_t
#include "internal/int_rules.ipp"

namespace TAO_PEGTL_NAMESPACE
{
   namespace uint64 = TAO_PEGTL_ENDIAN_SUFFIXED( uint64_ );

}  // namespace TAO_PEGTL_NAMESPACE

#endif  // FCOV_EXCL_FILE
