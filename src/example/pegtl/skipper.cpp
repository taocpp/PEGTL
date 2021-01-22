// Copyright (c) 2021 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <iostream>
#include <string>

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/skip.hpp>

namespace pegtl = TAO_PEGTL_NAMESPACE;

namespace demo
{
   // define your grammar without concerning yourself with skipping whitespace

   // clang-format off
   struct key : pegtl::identifier {};
   struct value : pegtl::identifier {};
   struct assign : pegtl::one< '=' > {};
   struct grammar : pegtl::seq< key, assign, value, pegtl::eof > {};
   // clang-format on

   // define your actions as usual

   template< typename Rule >
   struct action
   {};

   template<>
   struct action< key >
   {
      template< typename ActionInput >
      static void apply( const ActionInput& in )
      {
         std::cout << "key: '" << in.string() << "'\n";
      }
   };

   template<>
   struct action< value >
   {
      template< typename ActionInput >
      static void apply( const ActionInput& in )
      {
         std::cout << "value: '" << in.string() << "'\n";
      }
   };

   // now specify how and where to skip whitespace

   using skip_how = pegtl::star< pegtl::sor< pegtl::space, pegtl::eol > >;

   // clang-format off
   template< typename > inline constexpr bool where = false;
   template<> inline constexpr bool where< key > = true;
   template<> inline constexpr bool where< value > = true;
   // clang-format on

   // as 'skip<>' can not take 'where' as a template parameter directly, we need to wrap it.
   struct skip_where
   {
      // when to skip
      template< typename Rule >
      static constexpr auto value = where< Rule >;
   };

   template< typename Rule >
   using control = pegtl::skip< skip_how, skip_where >::control< Rule >;

   // The above is a first step, the helper (skip<>) may change in the future
   // to allow separate skipping before and after and also support different skip
   // rules for each rule/set-of-rules. Also, a more convenient way to specify
   // the whole thing.

}  // namespace demo

int main( int argc, char** argv )  // NOLINT(bugprone-exception-escape)
{
   if( argc > 1 ) {
      pegtl::argv_input in( argv, 1 );
      if( pegtl::parse< demo::grammar, demo::action, demo::control >( in ) ) {
         std::cout << "success!" << std::endl;
      }
      else {
         std::cerr << "failure." << std::endl;
      }
   }
}
