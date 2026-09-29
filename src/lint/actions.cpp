// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <cstddef>

#include <tao/pegtl.hpp>

#include <tao/pegtl/action/limit_consume.hpp>

namespace pegtl = TAO_PEGTL_NAMESPACE;

namespace
{
   struct letter : pegtl::ascii::alpha {};
   struct number : pegtl::ascii::digit {};
   struct grammar : pegtl::seq< letter, number, pegtl::eof > {};

   struct state
   {
      std::size_t applications = 0;
   };

   template< typename Rule >
   struct action
      : pegtl::nothing< Rule >
   {};

   template<>
   struct action< letter >
   {
      template< typename ActionInput >
      static void apply( const ActionInput& in, state& st )
      {
         st.applications += in.size();
      }
   };

   template<>
   struct action< number >
   {
      static bool apply0( state& st )
      {
         ++st.applications;
         return true;
      }
   };

   template< typename Rule >
   struct limited_action
      : pegtl::nothing< Rule >
   {};

   template<>
   struct limited_action< grammar >
      : pegtl::limit_consume< 2 >
   {};

}  // namespace

int main()  // NOLINT(bugprone-exception-escape) -- Deliberately instantiates throwing parse paths.
{
   pegtl::text_view_input<> in( "a1" );
   state st;
   const bool with_actions = pegtl::parse< grammar, action >( in, st );

   pegtl::text_view_input<> limited( "a1" );
   const bool with_limit = pegtl::parse< grammar, limited_action >( limited );
   return ( with_actions && with_limit && ( st.applications == 2 ) ) ? 0 : 1;
}
