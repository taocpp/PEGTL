// Copyright (c) 2021-2023 Kelvin Hammond
// Copyright (c) 2021-2026 Dr. Colin Hirsch and Daniel Frey
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
#include <tao/pegtl/example/iri.hpp>

namespace pegtl = TAO_PEGTL_NAMESPACE;

namespace example
{
   struct iri
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
      explicit iri( ParseInput& in );
   };

   // clang-format off
   template< typename Rule > struct action {};

   template<> struct action< pegtl::iri::scheme > : pegtl::value_to< &iri::scheme > {};
   template<> struct action< pegtl::iri::iauthority > : pegtl::value_to< &iri::authority > {};
   // iuserinfo: see below
   template<> struct action< pegtl::iri::ihost > : pegtl::value_to< &iri::host > {};
   template<> struct action< pegtl::iri::port > : pegtl::value_to< &iri::port > {};
   template<> struct action< pegtl::iri::ipath_noscheme > : pegtl::value_to< &iri::path > {};
   template<> struct action< pegtl::iri::ipath_rootless > : pegtl::value_to< &iri::path > {};
   template<> struct action< pegtl::iri::ipath_absolute > : pegtl::value_to< &iri::path > {};
   template<> struct action< pegtl::iri::ipath_abempty > : pegtl::value_to< &iri::path > {};
   template<> struct action< pegtl::iri::iquery > : pegtl::value_to< &iri::query > {};
   template<> struct action< pegtl::iri::ifragment > : pegtl::value_to< &iri::fragment > {};
   // clang-format on

   template<>
   struct action< pegtl::iri::opt_iuserinfo >
   {
      template< typename ActionInput >
      static void apply( const ActionInput& in, iri& i )
      {
         if( !in.empty() ) {
            i.userinfo = std::string( in.begin(), in.size() - 1 );
         }
      }
   };

   template< typename ParseInput >
   iri::iri( ParseInput& in )
   {
      using grammar = pegtl::must< pegtl::iri::IRI >;
      pegtl::parse< grammar, action >( in, *this );
   }

}  // namespace example

int main( int argc, char** argv )
{
   try {
      for( int i = 1; i < argc; ++i ) {
         std::cout << "Parsing " << argv[ i ] << std::endl;
         pegtl::argv_input< pegtl::scan::lf_crlf > in( argv, i );
         const example::iri iri( in );
         std::cout << "iri.scheme: " << iri.scheme << std::endl;
         std::cout << "iri.authority: " << iri.authority << std::endl;
         std::cout << "iri.userinfo: " << iri.userinfo << std::endl;
         std::cout << "iri.host: " << iri.host << std::endl;
         std::cout << "iri.port: " << iri.port << std::endl;
         std::cout << "iri.path: " << iri.path << std::endl;
         std::cout << "iri.query: " << iri.query << std::endl;
         std::cout << "iri.fragment: " << iri.fragment << std::endl;
      }
   }
   catch( const std::exception& e ) {
      std::cerr << "error: " << e.what() << std::endl;
      return 1;
   }
   return 0;
}

#endif
