// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <iostream>

#include <tao/pegtl.hpp>

#include <tao/pegtl/contrib/json.hpp>

namespace with_subs
{
   namespace internal
   {
      template< typename Rule, typename... Subs >
      struct printer
      {
         static void visit()
         {
            std::cout << TAO_PEGTL_NAMESPACE::internal::demangle< Rule >() << ' ' << sizeof...( Subs ) << std::endl;
         }
      };

   }  // namespace internal

   template< typename Rule >
   void print()
   {
      TAO_PEGTL_NAMESPACE::visit< internal::printer, Rule >();
   }

}  // namespace with_subs

int main()  // NOLINT(bugprone-exception-escape)
{
   with_subs::print< tao::pegtl::json::text >();
   return 0;
}
