// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <tao/pegtl.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   namespace internal
   {
      template< typename F, F U >
      struct function;

      template< typename ParseInput, typename... States, bool ( *U )( ParseInput&, States... ) >
      struct function< bool ( * )( ParseInput&, States... ), U >
      {
         template< pegtl::apply_mode A,
                   pegtl::rewind_mode M,
                   template< typename... >
                   class Action,
                   template< typename... >
                   class Control >
         [[nodiscard]] static bool match( ParseInput& in, States... st ) noexcept( noexcept( U( in, st... ) ) )
         {
            return U( in, st... );
         }
      };

      template< typename F, F U >
      inline constexpr bool enable_control< function< F, U > > = false;

   }  // namespace internal

   template< auto F >
   struct function
      : internal::function< decltype( F ), F >
   {};

}  // namespace TAO_PEGTL_NAMESPACE

[[nodiscard]] bool func1( TAO_PEGTL_NAMESPACE::argv_input<>& /*unused*/, int /*unused*/, char*& /*unused*/, const double& /*unused*/ )
{
   return true;
}

struct rule1 : TAO_PEGTL_NAMESPACE::function< func1 >
{};

int main( int argc, char** argv )  // NOLINT(bugprone-exception-escape)
{
   char c = 'a';
   double d = 42.0;

   for( int i = 1; i < argc; ++i ) {
      TAO_PEGTL_NAMESPACE::argv_input in( argv, i );
      TAO_PEGTL_NAMESPACE::parse< TAO_PEGTL_NAMESPACE::must< rule1 > >( in, i, &c, d );
   }
   return 0;
}
