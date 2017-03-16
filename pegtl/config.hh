// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_CONFIG_HH
#define PEGTL_CONFIG_HH

#include "version.hh"

#ifndef PEGTL_NAMESPACE
#define PEGTL_NAMESPACE pegtl
#endif

// MSVC before VS2015 do not support some of C++11 features:
#if defined(_MSC_VER) && (_MSC_VER < 1900) // msvc <2015
#  define PEGTL_CONSTEXPR const
#  define PEGTL_NOEXCEPT throw()
#else
#  define PEGTL_CONSTEXPR constexpr
#  define PEGTL_NOEXCEPT noexcept
#endif

#endif
