// Copyright (c) 2021-2023 Kelvin Hammond
// Copyright (c) 2021-2025 Dr. Colin Hirsch and Daniel Frey
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

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/iri.hpp>

namespace pegtl = TAO_PEGTL_NAMESPACE;

struct IRI
{
   std::string scheme;
   std::string authority;
   std::string userinfo;
   std::string host;
   std::string port;
   std::string path;
   std::string query;
   std::string fragment;

   explicit IRI( pegtl::argv_input< pegtl::scan::lf_crlf >& in );
};

namespace iri
{
   template< std::string IRI::*Field >
   struct bind
   {
      template< typename ActionInput >
      static void apply( const ActionInput& in, IRI& iri )
      {
         iri.*Field = in.string();
      }
   };

   // clang-format off
   template< typename Rule > struct action {};

   template<> struct action< pegtl::iri::scheme > : bind< &IRI::scheme > {};
   template<> struct action< pegtl::iri::iauthority > : bind< &IRI::authority > {};
   // userinfo: see below
   template<> struct action< pegtl::iri::ihost > : bind< &IRI::host > {};
   template<> struct action< pegtl::iri::port > : bind< &IRI::port > {};
   template<> struct action< pegtl::iri::ipath_noscheme > : bind< &IRI::path > {};
   template<> struct action< pegtl::iri::ipath_rootless > : bind< &IRI::path > {};
   template<> struct action< pegtl::iri::ipath_absolute > : bind< &IRI::path > {};
   template<> struct action< pegtl::iri::ipath_abempty > : bind< &IRI::path > {};
   template<> struct action< pegtl::iri::iquery > : bind< &IRI::query > {};
   template<> struct action< pegtl::iri::ifragment > : bind< &IRI::fragment > {};
   // clang-format on

   template<>
   struct action< pegtl::iri::opt_iuserinfo >
   {
      template< typename ActionInput >
      static void apply( const ActionInput& in, IRI& iri )
      {
         if( !in.empty() ) {
            iri.userinfo = std::string( in.begin(), in.size() - 1 );
         }
      }
   };

}  // namespace iri

IRI::IRI( pegtl::argv_input< pegtl::scan::lf_crlf >& in )
{
   using grammar = pegtl::must< pegtl::iri::IRI >;
   pegtl::parse< grammar, iri::action >( in, *this );
}

int main( int argc, char** argv )
{
   try {
      for( int i = 1; i < argc; ++i ) {
         std::cout << "Parsing " << argv[ i ] << std::endl;
         pegtl::argv_input< pegtl::scan::lf_crlf > in( argv, i );
         const IRI iri( in );
         std::cout << "IRI.scheme: " << iri.scheme << std::endl;
         std::cout << "IRI.authority: " << iri.authority << std::endl;
         std::cout << "IRI.userinfo: " << iri.userinfo << std::endl;
         std::cout << "IRI.host: " << iri.host << std::endl;
         std::cout << "IRI.port: " << iri.port << std::endl;
         std::cout << "IRI.path: " << iri.path << std::endl;
         std::cout << "IRI.query: " << iri.query << std::endl;
         std::cout << "IRI.fragment: " << iri.fragment << std::endl;
      }
   }
   catch( const std::exception& e ) {
      std::cerr << "error: " << e.what() << std::endl;
      return 1;
   }
   return 0;
}

#endif
