// Copyright (c) 2014-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#if defined( _POSIX_MAPPED_FILES ) || defined( _WIN32 )

#include <type_traits>

#include "test.hpp"
#include "verify_file.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      using lazy_input = lazy_mmap_input< ascii::lf_crlf >;
      using text_input = text_mmap_input< ascii::lf_crlf >;

      static_assert( std::is_base_of_v< internal::mmap_input< char >, lazy_input > );
      static_assert( std::is_base_of_v< internal::mmap_input< char >, text_input > );

      verify_file< lazy_input >();
      verify_file< text_input >();
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"

#else

int main()
{
   return 0;
}

#endif
