// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <cstddef>

#include <tao/pegtl.hpp>

#include <tao/pegtl/control/state_control.hpp>

namespace pegtl = TAO_PEGTL_NAMESPACE;

namespace
{
   struct grammar : pegtl::seq< pegtl::ascii::alpha, pegtl::ascii::digit, pegtl::eof > {};

   struct observer
   {
      std::size_t events = 0;

      template< typename Rule >
      static constexpr bool enable = true;

      template< typename Rule, typename ParseInput, typename... States >
      void start( [[maybe_unused]] const ParseInput& in, [[maybe_unused]] States&&... st ) noexcept
      {
         ++events;
      }

      template< typename Rule, typename ParseInput, typename... States >
      void success( [[maybe_unused]] const ParseInput& in, [[maybe_unused]] States&&... st ) noexcept
      {
         ++events;
      }

      template< typename Rule, typename ParseInput, typename... States >
      void failure( [[maybe_unused]] const ParseInput& in, [[maybe_unused]] States&&... st ) noexcept
      {
         ++events;
      }

      template< typename Rule, typename ParseInput, typename... States >
      void unwind( [[maybe_unused]] const ParseInput& in, [[maybe_unused]] States&&... st ) noexcept
      {
         ++events;
      }
   };

}  // namespace

int main()
{
   pegtl::text_view_input<> in( "a1" );
   observer state;  // NOLINT(misc-const-correctness) -- Mutated through state_control.
   const bool result = pegtl::parse< grammar, pegtl::nothing, pegtl::state_control_n< pegtl::normal >::template type >( in, state );
   return ( result && ( state.events != 0 ) ) ? 0 : 1;
}
