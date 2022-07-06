#include "tao/pegtl/contrib/list_length.hpp"
#include <tao/pegtl.hpp>

#include <iostream>

namespace list_length_example
{
   namespace pegtl = tao::pegtl;

   struct digits : pegtl::plus< pegtl::digit >
   {};

   template< int64_t len >
   struct number_list_with_length_without_padding : pegtl::list< digits, pegtl::one< ',' >, pegtl::one< ' ' >, len >
   {};

   template< int64_t len >
   struct grammar : pegtl::seq< number_list_with_length_without_padding< len >, pegtl::eolf >
   {};

   template< typename Rule >
   struct action
   {};

   template<>
   struct action< digits >
   {
      template< typename ActionInput >
      static void apply( const ActionInput& in, uint64_t& val )
      {
         val += std::stoull( in.string() );
      }
   };

   template< uint64_t len >
   struct action< grammar< len > >
   {
      template< typename... States >
      static void apply( States&&... /*unused*/ )
      {
         std::cout << "Hello from " << len << "." << std::endl;
      }
   };

}  // namespace list_length_example

static std::string generate_correct_input( uint64_t len )
{
   if( len == 0 ) {
      return "";
   }
   if( len == 1 ) {
      return "1";
   }
   return std::to_string( len ) + "," + std::string(     len / 31, ' ' ) + generate_correct_input( len - 1 );
}

static std::string generate_incorrect_input( uint64_t len )
{
   if( len == 0 ) {
      return "0";
   }
   return generate_correct_input( len ) + ",0";
}

#define TEST_CASE( FUN, LEN )                                                                                  \
   do {                                                                                                        \
      auto input_string = FUN( LEN );                                                                          \
      tao::pegtl::memory_input in( input_string.c_str(), "" );                                                 \
      uint64_t sum = 0;                                                                                        \
                                                                                                               \
      if( tao::pegtl::parse< list_length_example::grammar< LEN >, list_length_example::action >( in, sum ) ) { \
         std::cout << "string \"" << input_string << "\" parsed successfully." << std::endl;                   \
         std::cout << "sum is: " << sum << "\n"                                                                \
                   << std::endl;                                                                               \
      }                                                                                                        \
      else {                                                                                                   \
         std::cerr << "Failed to parse \"" << input_string << "\"." << std::endl;                              \
      }                                                                                                        \
   } while( 0 )

int main()
{
   TEST_CASE( generate_correct_input, 0 );
   TEST_CASE( generate_correct_input, 1 );
   TEST_CASE( generate_correct_input, 2 );
   TEST_CASE( generate_correct_input, 3 );
   TEST_CASE( generate_correct_input, 4 );
   TEST_CASE( generate_correct_input, 5 );
   TEST_CASE( generate_correct_input, 6 );
   TEST_CASE( generate_correct_input, 24 );
   TEST_CASE( generate_correct_input, 42 );
   TEST_CASE( generate_correct_input, 100 );

   TEST_CASE( generate_incorrect_input, 0 );
   TEST_CASE( generate_incorrect_input, 1 );
   TEST_CASE( generate_incorrect_input, 2 );
   TEST_CASE( generate_incorrect_input, 3 );
   TEST_CASE( generate_incorrect_input, 4 );
   TEST_CASE( generate_incorrect_input, 5 );
   TEST_CASE( generate_incorrect_input, 6 );
   TEST_CASE( generate_incorrect_input, 24 );
   TEST_CASE( generate_incorrect_input, 42 );
   TEST_CASE( generate_incorrect_input, 100 );
}