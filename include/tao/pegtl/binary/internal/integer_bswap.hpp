// Copyright (c) 2023-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_BINARY_INTERNAL_INTEGER_BSWAP_HPP
#define TAO_PEGTL_BINARY_INTERNAL_INTEGER_BSWAP_HPP

#if defined( _WIN32 ) && !defined( __MINGW32__ ) && !defined( __CYGWIN__ )
#include "integer_bswap_win32.hpp"
#else
#include "integer_bswap_gcc_clang.hpp"
#endif

// FCOV_EXCL_FILE
#endif
