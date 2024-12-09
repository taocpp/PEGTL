// Copyright (c) 2023-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_BYTESWAP_HPP
#define TAO_PEGTL_INTERNAL_BYTESWAP_HPP

#if defined( _WIN32 ) && !defined( __MINGW32__ ) && !defined( __CYGWIN__ )
#include "byteswap_win32.hpp"
#else
#include "byteswap_gcc_clang.hpp"
#endif

// FCOV_EXCL_FILE
#endif
