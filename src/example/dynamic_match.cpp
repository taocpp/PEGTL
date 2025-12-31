// Copyright (c) 2014-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <cassert>
#include <cstring>

#include <iostream>
#include <string>

#include <tao/pegtl.hpp>

#include <tao/pegtl/debug/analyze.hpp>

namespace dynamic
{
   struct long_literal_id
      : tao::pegtl::plus< tao::pegtl::not_one7< '[' > >
   {};

   struct long_literal_open
      : tao::pegtl::seq< tao::pegtl::one< '[' >, long_literal_id, tao::pegtl::one< '[' > >
   {};

   struct long_literal_mark
   {
      using rule_t = long_literal_mark;

      template< tao::pegtl::apply_mode,
                tao::pegtl::rewind_mode,
                template< typename... >
                class Action,
                template< typename... >
                class Control,
                typename ParseInput,
                typename... States >
      static bool match( ParseInput& in, const std::string& id, const std::string& /*unused*/, States&&... /*unused*/ )
      {
         if( in.size( id.size() ) >= id.size() ) {
            if( std::memcmp( in.current(), id.data(), id.size() ) == 0 ) {
               in.template consume< tao::pegtl::eol_unknown_tag >( id.size() );
               return true;
            }
         }
         return false;
      }
   };

   struct long_literal_close
      : tao::pegtl::seq< tao::pegtl::one< ']' >, long_literal_mark, tao::pegtl::one< ']' > >
   {};

   struct long_literal_body
      : tao::pegtl::any7
   {};

   struct grammar
      : tao::pegtl::seq< long_literal_open, tao::pegtl::until< long_literal_close, long_literal_body >, tao::pegtl::eof >
   {};

   template< typename Rule >
   struct action
   {};

   template<>
   struct action< long_literal_id >
   {
      template< typename ActionInput >
      static void apply( const ActionInput& in, std::string& id, const std::string& /*unused*/ )
      {
         id = in.string();
      }
   };

   template<>
   struct action< long_literal_body >
   {
      template< typename ActionInput >
      static void apply( const ActionInput& in, const std::string& /*unused*/, std::string& body )
      {
         body += in.string_view();
      }
   };

}  // namespace dynamic

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Name >
   struct analyze_traits< Name, dynamic::long_literal_mark >
      : analyze_any_traits<>
   {};

}  // namespace TAO_PEGTL_NAMESPACE

int main( int argc, char** argv )  // NOLINT(bugprone-exception-escape)
{
   if( tao::pegtl::analyze< dynamic::grammar >() != 0 ) {
      std::cerr << "cycles without progress detected!" << std::endl;
      return 1;
   }
   for( int i = 1; i < argc; ++i ) {
      std::string id;
      std::string body;

      tao::pegtl::argv_input< tao::pegtl::scan::lf_crlf > in( argv, i );
      if( tao::pegtl::parse< dynamic::grammar, dynamic::action >( in, id, body ) ) {
         std::cout << "long literal id was: " << id << std::endl;
         std::cout << "long literal body was: " << body << std::endl;
      }
      else {
         std::cerr << "parse error for: " << argv[ i ] << std::endl;
         return 1;
      }
   }
   return 0;
}
