// Copyright (c) 2017-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#if !defined( __cpp_exceptions )
#include <iostream>
int main()
{
   std::cerr << "Exception support required, example unavailable." << std::endl;
   return 1;
}
#else

#include <iostream>
#include <stdexcept>
#include <string>

#include <tao/pegtl.hpp>
#include <tao/pegtl/action/builders.hpp>
#include <tao/pegtl/example/uri.hpp>

namespace pegtl = TAO_PEGTL_NAMESPACE;

namespace example
{
   struct uri
   {
      std::string scheme;
      std::string authority;
      std::string userinfo;
      std::string host;
      std::string port;
      std::string path;
      std::string query;
      std::string fragment;

      template< typename ParseInput >
      explicit uri( ParseInput& in );
   };

   // clang-format off
   template< typename Rule > struct action {};

   template<> struct action< pegtl::uri::scheme > : pegtl::value_to< &uri::scheme > {};
   template<> struct action< pegtl::uri::authority > : pegtl::value_to< &uri::authority > {};
   // userinfo: see below
   template<> struct action< pegtl::uri::host > : pegtl::value_to< &uri::host > {};
   template<> struct action< pegtl::uri::port > : pegtl::value_to< &uri::port > {};
   template<> struct action< pegtl::uri::path_noscheme > : pegtl::value_to< &uri::path > {};
   template<> struct action< pegtl::uri::path_rootless > : pegtl::value_to< &uri::path > {};
   template<> struct action< pegtl::uri::path_absolute > : pegtl::value_to< &uri::path > {};
   template<> struct action< pegtl::uri::path_abempty > : pegtl::value_to< &uri::path > {};
   template<> struct action< pegtl::uri::query > : pegtl::value_to< &uri::query > {};
   template<> struct action< pegtl::uri::fragment > : pegtl::value_to< &uri::fragment > {};
   // clang-format on

   template<>
   struct action< pegtl::uri::opt_userinfo >
   {
      template< typename ActionInput >
      static void apply( const ActionInput& in, uri& u )
      {
         if( !in.empty() ) {
            u.userinfo = std::string( in.begin(), in.size() - 1 );
         }
      }
   };

   template< typename ParseInput >
   uri::uri( ParseInput& in )
   {
      using grammar = pegtl::must< pegtl::uri::URI >;
      pegtl::parse< grammar, action >( in, *this );
   }

}  // namespace example

int main( int argc, char** argv )
{
   try {
      for( int i = 1; i < argc; ++i ) {
         std::cout << "Parsing " << argv[ i ] << std::endl;
         pegtl::argv_input< pegtl::scan::lf_crlf > in( argv, i );
         const example::uri uri( in );
         std::cout << "uri.scheme: " << uri.scheme << std::endl;
         std::cout << "uri.authority: " << uri.authority << std::endl;
         std::cout << "uri.userinfo: " << uri.userinfo << std::endl;
         std::cout << "uri.host: " << uri.host << std::endl;
         std::cout << "uri.port: " << uri.port << std::endl;
         std::cout << "uri.path: " << uri.path << std::endl;
         std::cout << "uri.query: " << uri.query << std::endl;
         std::cout << "uri.fragment: " << uri.fragment << std::endl;
      }
   }
   catch( const std::exception& e ) {
      std::cerr << "error: " << e.what() << std::endl;
      return 1;
   }
   return 0;
}

#endif
