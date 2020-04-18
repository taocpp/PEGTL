// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <iostream>

#include <tao/pegtl.hpp>

#include <tao/pegtl/contrib/json.hpp>
#include <tao/pegtl/contrib/visit_rt.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   namespace internal
   {
      template< typename Rule, typename... >
      struct printer_cout
      {
         static void visit()
         {
            std::cout << demangle< Rule >() << std::endl;
         }
      };

   }  // namespace internal

   template< typename Rule >
   void print_cout_rt()
   {
      visit_rt< Rule, internal::printer_cout >();
   }

}  // namespace TAO_PEGTL_NAMESPACE

int main()  // NOLINT(bugprone-exception-escape)
{
   tao::pegtl::print_cout_rt< tao::pegtl::json::text >();
   return 0;
}
