// Copyright (c) 2014-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_UTF8_HPP
#define TAO_PEGTL_UTF8_HPP

#include <cstddef>

#include "config.hpp"

#include "internal/peek_utf8.hpp"
#include "internal/rules.hpp"

#define TAO_PEGTL_UTF_NAME utf8
#include "internal/utf_rules.ipp"

#endif  // FCOV_EXCL_FILE
