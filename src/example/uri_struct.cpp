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

   template< std::string uri::* Field >
   struct bind
   {
      template< typename ActionInput >
      static void apply( const ActionInput& in, uri& u )
      {
         u.*Field = in.string();
      }
   };

   // clang-format off
   template< typename Rule > struct action {};

   template<> struct action< pegtl::uri::scheme > : bind< &uri::scheme > {};
   template<> struct action< pegtl::uri::authority > : bind< &uri::authority > {};
   // userinfo: see below
   template<> struct action< pegtl::uri::host > : bind< &uri::host > {};
   template<> struct action< pegtl::uri::port > : bind< &uri::port > {};
   template<> struct action< pegtl::uri::path_noscheme > : bind< &uri::path > {};
   template<> struct action< pegtl::uri::path_rootless > : bind< &uri::path > {};
   template<> struct action< pegtl::uri::path_absolute > : bind< &uri::path > {};
   template<> struct action< pegtl::uri::path_abempty > : bind< &uri::path > {};
   template<> struct action< pegtl::uri::query > : bind< &uri::query > {};
   template<> struct action< pegtl::uri::fragment > : bind< &uri::fragment > {};
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
