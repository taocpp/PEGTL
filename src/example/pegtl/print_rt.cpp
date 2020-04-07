// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <iostream>

#include <tao/pegtl.hpp>
#include <tao/pegtl/visit_rt.hpp>

#include <tao/pegtl/contrib/json.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Rule >
   struct printer_rt
   {
      static void call()
      {
         std::cout << internal::demangle< Rule >() << std::endl;
      }
   };

   template< typename Rule >
   void print_rt()
   {
      visit_rt< printer_rt, Rule >();
   }

}  // namespace TAO_PEGTL_NAMESPACE

int main( int, char** )  // NOLINT(bugprone-exception-escape)
{
   tao::pegtl::print_rt< tao::pegtl::json::text >();
   return 0;
}
