// Copyright (c) 2014-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <type_traits>

#include "test.hpp"

#if defined( _POSIX_MAPPED_FILES ) || defined( _WIN32 )

#include "verify_file.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      using lazy_input = text_mmap_input< lazy::lf_crlf >;
      using text_input = text_mmap_input< scan::lf_crlf >;

      static_assert( std::is_base_of_v< internal::mmap_input< char >, lazy_input > );
      static_assert( std::is_base_of_v< internal::mmap_input< char >, text_input > );

      verify_file< lazy_input >();
      verify_file< text_input >();
   }

}  // namespace TAO_PEGTL_NAMESPACE

#else

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      std::cerr << "Skipping mmap test..." << std::endl;
   }

}  // namespace TAO_PEGTL_NAMESPACE

#endif

#include "main.hpp"
