// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <cstddef>

#include <tao/pegtl.hpp>

#include <tao/pegtl/debug/analyze.hpp>
#include <tao/pegtl/debug/trace.hpp>
#include <tao/pegtl/debug/visit.hpp>

namespace pegtl = TAO_PEGTL_NAMESPACE;

namespace
{
   struct word : pegtl::plus< pegtl::ascii::alpha > {};
   struct grammar : pegtl::seq< word, pegtl::eof > {};

   template< typename Rule >
   struct visitor
   {
      static void visit( std::size_t& count ) noexcept
      {
         ++count;
      }
   };

}  // namespace

int main()  // NOLINT(bugprone-exception-escape) -- Deliberately instantiates throwing parse paths.
{
   std::size_t rules = 0;
   pegtl::visit< grammar, visitor >( rules );

   pegtl::text_view_input<> in( "lint" );
   const bool result = pegtl::standard_trace< grammar >( in );
   return ( result && ( rules != 0 ) && ( pegtl::analyze< grammar >() == 0 ) ) ? 0 : 1;
}
