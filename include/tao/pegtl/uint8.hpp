// Copyright (c) 2018-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_UINT8_HPP
#define TAO_PEGTL_UINT8_HPP

#include "config.hpp"

#include "internal/peek_direct.hpp"
#include "internal/peek_masked.hpp"
#include "internal/rules.hpp"
#include "internal/using.hpp"

#define TAO_PEGTL_INT_NAME uint8
#define TAO_PEGTL_INT_TYPE std::uint8_t
#include "internal/int_rules.ipp"

#endif  // FCOV_EXCL_FILE
