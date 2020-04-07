// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <iostream>

#include <tao/pegtl.hpp>

#include <tao/pegtl/contrib/json.hpp>

namespace without_subs
{
   namespace internal
   {
      template< typename Rule, typename... >
      struct printer
      {
         static void call()
         {
            std::cout << TAO_PEGTL_NAMESPACE::internal::demangle< Rule >() << std::endl;
         }
      };

   }  // namespace internal

   template< typename Rule >
   void print()
   {
      TAO_PEGTL_NAMESPACE::visit< internal::printer, Rule >();
   }

}  // namespace without_subs

int main( int, char** )  // NOLINT(bugprone-exception-escape)
{
   without_subs::print< tao::pegtl::json::text >();
   return 0;
}
