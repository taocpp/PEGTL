// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <iostream>

#include <tao/pegtl.hpp>

#include <tao/pegtl/contrib/json.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   namespace internal
   {
      template< typename Rule, typename... Subs >
      struct printer_with_subs
      {
         static void call()
         {
            std::cout << internal::demangle< Rule >() << ' ' << sizeof...( Subs ) << std::endl;
         }
      };

   }  // namespace internal

   template< typename Rule >
   void print_with_subs()
   {
      visit_with_subs< internal::printer_with_subs, Rule >();
   }

}  // namespace TAO_PEGTL_NAMESPACE

int main( int, char** )  // NOLINT(bugprone-exception-escape)
{
   tao::pegtl::print_with_subs< tao::pegtl::json::text >();
   return 0;
}
