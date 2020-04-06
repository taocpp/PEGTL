// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <iostream>
#include <map>
#include <typeindex>

#include <tao/pegtl.hpp>

#include <tao/pegtl/contrib/json.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   namespace internal
   {
      struct coverage_info
      {
         explicit coverage_info( const std::string_view in_name )
            : name( in_name.data(), in_name.size() )
         {}

         std::string name;
         std::size_t start = 0;
         std::size_t success = 0;
         std::size_t failure = 0;
         // TODO: Raise?
         // TODO: Actions?
      };

      struct coverage_state
      {
         std::map< std::type_index, coverage_info > map;
      };

      template< typename Rule >
      struct coverage_prepare
      {
         static void call( coverage_state& state )
         {
            state.map.try_emplace( std::type_index( typeid( Rule ) ), demangle< Rule >() );
         }
      };

      template< typename Rule >
      struct coverage_control
         : normal< Rule >
      {
         template< typename Input >
         static void start( const Input& /*unused*/, coverage_state& state )
         {
            ++state.map.at( std::type_index( typeid( Rule ) ) ).start;
         }

         template< typename Input >
         static void success( const Input& /*unused*/, coverage_state& state )
         {
            ++state.map.at( std::type_index( typeid( Rule ) ) ).success;
         }

         template< typename Input >
         static void failure( const Input& /*unused*/, coverage_state& state )
         {
            ++state.map.at( std::type_index( typeid( Rule ) ) ).failure;
         }
      };

   }  // namespace internal

   template< typename Rule >
   void coverage( const std::string& file )
   {
      file_input in( file );
      internal::coverage_state state;
      visit< internal::coverage_prepare, Rule >( state );
      const auto success = parse< Rule, nothing, internal::coverage_control >( in, state );
      std::cout << "PARSE " << file << ( success ? "success" : "failure" ) << std::endl;
      for( const auto& p : state.map ) {
         std::cout << "RULE " << p.second.name << " HIT " << p.second.start << '/' << p.second.success << '/' << p.second.failure << std::endl;
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE

int main( int argc, char** argv)  // NOLINT(bugprone-exception-escape)
{
   for( int i = 1; i < argc; ++i ) {
      tao::pegtl::coverage< tao::pegtl::json::text >( argv[ i ] );
   }
   return 0;
}
