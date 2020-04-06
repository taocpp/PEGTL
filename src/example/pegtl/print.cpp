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
      struct printer
      {
         static void call()
         {
            if constexpr( std::is_same_v< Rule, typename Rule::rule_t > ) {
               std::cout << internal::demangle< Rule >() << std::endl;
            }
            else {
               std::cout << internal::demangle< Rule >() << " = " << internal::demangle< typename Rule::rule_t >() << std::endl;
            }
         }
      };

   }  // namespace internal

   template< typename Rule >
   void print()
   {
      visit< internal::printer, Rule >();
   }

}  // namespace TAO_PEGTL_NAMESPACE

int main( int, char** )  // NOLINT(bugprone-exception-escape)
{
   tao::pegtl::print< tao::pegtl::json::text >();
   return 0;
}
