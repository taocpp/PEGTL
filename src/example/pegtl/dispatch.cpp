// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <iostream>

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/dispatch.hpp>
#include <tao/pegtl/contrib/json.hpp>

namespace example
{
   namespace pegtl = TAO_PEGTL_NAMESPACE;

   struct action1
   {
      template< typename ActionInput >
      static void apply( const ActionInput& in )
      {
         std::cerr << "ONE " << in.string() << std::endl;
      }
   };

   template< typename Rule >
   struct action2
   {
      static void apply0()
      {
         std::cerr << "TWO " << pegtl::demangle< Rule >() << std::endl;
      }
   };

   void run( const std::filesystem::path& path )
   {
      pegtl::file_input<> in( path );
      using clause1 = pegtl::clause1< action1, pegtl::json::string_content, pegtl::json::key_content >;
      using clause2 = pegtl::clause2< action2, pegtl::json::begin_array, pegtl::json::end_array, pegtl::json::begin_object, pegtl::json::end_object >;
      const auto b = pegtl::dispatch< clause1, clause2 >::parse< pegtl::seq< pegtl::json::text, pegtl::eof > >( in );
      std::cerr << "SUCCESS " << b << std::endl;
   }

}  // namespace example

int main( int argc, char** argv )
{
   for( int i = 1; i < argc; ++i ) {
      example::run( argv[ i ] );
   }
   return 0;
}
