// Copyright (c) 2024-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_SYSTEM_HPP
#define TAO_PEGTL_SYSTEM_HPP

#if defined( _WIN32 )
#  define TAO_PEGTL_SYSTEM_WINDOWS
#  define TAO_PEGTL_DEFAULT_EOL ascii::scan::lf_crlf
#  define TAO_PEGTL_MMAP_AVAILABLE
#  define TAO_PEGTL_MMAP_INCLUDE "mmap_file_win32.hpp"
#elif defined( __unix__ ) || ( defined( __APPLE__ ) && defined( __MACH__ ) )
#  define TAO_PEGTL_SYSTEM_POSIX
#  define TAO_PEGTL_DEFAULT_EOL ascii::scan::lf
#  include <unistd.h>
#  if defined( _POSIX_MAPPED_FILES )
#    define TAO_PEGTL_MMAP_AVAILABLE
#    define TAO_PEGTL_MMAP_INCLUDE "mmap_file_posix.hpp"
#  endif
#else
#  error "Unknown operating system!"
#endif

// FCOV_EXCL_FILE
#endif
