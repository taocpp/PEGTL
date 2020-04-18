// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <iostream>

#include <tao/pegtl.hpp>

#include <tao/pegtl/contrib/json.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   namespace internal
   {
      template< typename Rule >
      struct printer_cout
      {
         static void visit()
         {
            std::cout << demangle< Rule >()
                      << " => "
                      << demangle< typename Rule::rule_t >()
                      << std::endl;
         }
      };

   }  // namespace internal

   template< typename Rule >
   void print_cout()
   {
      visit< Rule, internal::printer_cout >();
   }

}  // namespace TAO_PEGTL_NAMESPACE

int main()  // NOLINT(bugprone-exception-escape)
{
   tao::pegtl::print_cout< tao::pegtl::json::text >();
   return 0;
}
