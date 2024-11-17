// Copyright (c) 2015-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#if !defined( __cpp_exceptions ) || !defined( _POSIX_MAPPED_FILES )
#include <iostream>
int main()
{
   std::cout << "Exception support disabled, skipping test..." << std::endl;
}
#else

#include <tao/pegtl/file_input.hpp>

#include "test.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      try {
         internal::file_mapper dummy( "include" );
         // LCOV_EXCL_START
         std::cerr << "pegtl: unit test failed for [ internal::file_mapper ]" << std::endl;
         ++failed;
         // LCOV_EXCL_STOP
      }
      catch( const std::filesystem::filesystem_error& ) {
      }
      // LCOV_EXCL_START
      catch( ... ) {
         std::cerr << "pegtl: unit test failed for [ internal::file_mapper ] with unexpected exception" << std::endl;
         ++failed;
      }
      // LCOV_EXCL_STOP

      const std::string s = "dummy content\n";
      const std::string dummy_content = s + s + s + s + s + s + s + s + s + s + s;

      internal::file_mapper mapper( "src/test/pegtl/data/test_data.txt" );
      TAO_PEGTL_TEST_ASSERT( !mapper.empty() );
      TAO_PEGTL_TEST_ASSERT( mapper.size() == 154 );
      TAO_PEGTL_TEST_ASSERT( std::string_view( mapper.data(), mapper.size() ) == dummy_content );
      TAO_PEGTL_TEST_ASSERT( std::string_view( mapper.begin(), mapper.end() - mapper.begin() ) == dummy_content );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"

#endif
