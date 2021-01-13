// Copyright (c) 2015-2021 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <tao/pegtl/file_input.hpp>

#if defined( _POSIX_MAPPED_FILES ) || defined( _WIN32 )

#include "test.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
#if defined( __cpp_exceptions )
      const internal::file_opener fo( "Makefile" );
      ::close( fo.m_fd );  // Provoke exception, nobody would normally do this.
      try {
         (void)fo.size();  // expected to throw

         // LCOV_EXCL_START
         std::cerr << "pegtl: unit test failed for [ internal::file_opener ] " << std::endl;
         ++failed;
         // LCOV_EXCL_END
      }
      catch( const std::exception& ) {
      }
#endif
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"

#else

int main()
{
   return 0;
}

#endif
