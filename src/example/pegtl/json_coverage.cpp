// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <iostream>

#include <tao/pegtl.hpp>

#include <tao/pegtl/contrib/coverage.hpp>
#include <tao/pegtl/contrib/json.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Rule >
   void coverage( const std::string& file )
   {
      file_input in( file );
      coverage_state state;
      visit< coverage_insert, Rule >( state );
      const auto success = parse< Rule, nothing, coverage_control >( in, state );
      std::cout << "PARSE " << file << ( success ? "success" : "failure" ) << std::endl;
      for( const auto& [ k, v ] : state.map ) {
         std::cout << "RULE " << k << " HIT " << v.start << '/' << v.success << '/' << v.failure << std::endl;  // TODO: Format as JSON?
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE

int main( int argc, char** argv )  // NOLINT(bugprone-exception-escape)
{
   for( int i = 1; i < argc; ++i ) {
      tao::pegtl::coverage< tao::pegtl::json::text >( argv[ i ] );
   }
   return 0;
}
