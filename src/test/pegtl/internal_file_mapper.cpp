// Copyright (c) 2015-2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <tao/pegtl/file_input.hpp>

#if defined( _POSIX_MAPPED_FILES )

#include "test.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      try {
         internal::file_mapper dummy( "include" );
         std::cerr << "pegtl: unit test failed for [ internal::file_mapper ]" << std::endl;
         ++failed;
      }
      catch( const internal::filesystem::filesystem_error& ) {
      }
      catch( ... ) {
         std::cerr << "pegtl: unit test failed for [ internal::file_mapper ] with unexpected exception" << std::endl;
         ++failed;
      }

      const std::string s = "dummy content\n";
      const std::string dummy_content = s + s + s + s + s + s + s + s + s + s + s;

      internal::file_mapper mapper( "src/test/pegtl/file_data.txt" );
      TAO_PEGTL_TEST_ASSERT( !mapper.empty() );
      TAO_PEGTL_TEST_ASSERT( mapper.size() == 154 );
      TAO_PEGTL_TEST_ASSERT( std::string_view( mapper.data(), mapper.size() ) == dummy_content );
      TAO_PEGTL_TEST_ASSERT( std::string_view( mapper.begin(), mapper.end() - mapper.begin() ) == dummy_content );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"

#else

int main()
{
   return 0;
}

#endif
