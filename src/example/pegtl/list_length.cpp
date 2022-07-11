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

namespace list_min_max_example
{
   namespace pegtl = tao::pegtl;

   struct digits : pegtl::plus< pegtl::digit >
   {};

   template< std::size_t Min, std::size_t Max >
   struct list_min_max_expression : pegtl::list_min_max< Min, Max, digits, pegtl::one< ',' >, pegtl::one< ' ' > >
   {};

   template< std::size_t M >
   struct grammar : pegtl::seq< list_min_max_expression< M, M + 1 >, pegtl::eolf >
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

   template< uint64_t M >
   struct action< grammar< M > >
   {
      template< typename... States >
      static void apply( States&&... /*unused*/ )
      {
         std::cout << "Hello from " << M << "." << std::endl;
      }
   };

}  // namespace list_min_max_example

static std::string generate_correct_input( uint64_t len )
{
   if( len == 0 ) {
      return "";
   }
   if( len == 1 ) {
      return "1";
   }
   return std::to_string( len ) + "," + std::string( len / 31, ' ' ) + generate_correct_input( len - 1 );
}

static std::string generate_incorrect_input1( uint64_t len )
{
   if( len == 0 ) {
      return "0";
   }
   return generate_correct_input( len ) + ",0";
}

static std::string generate_incorrect_input2( uint64_t len )
{
   if( len == 0 ) {
      return "0,1";
   }
   return generate_correct_input( len ) + ",0,1";
}

#define TEST_CASE( NAMESPACE, FUN, LEN )                                                          \
   do {                                                                                           \
      auto input_string = FUN( LEN );                                                             \
      tao::pegtl::memory_input in( input_string.c_str(), "" );                                    \
      uint64_t sum = 0;                                                                           \
                                                                                                  \
      if( tao::pegtl::parse< NAMESPACE::grammar< LEN >, NAMESPACE::action >( in, sum ) ) {        \
         std::cout << "From " << #NAMESPACE << ":\n";                                             \
         std::cout << "string \"" << input_string << "\" parsed successfully." << std::endl;      \
         std::cout << "sum is: " << sum << "\n"                                                   \
                   << std::endl;                                                                  \
      }                                                                                           \
      else {                                                                                      \
         std::cerr << #NAMESPACE << ": Failed to parse \"" << input_string << "\"." << std::endl; \
      }                                                                                           \
   } while( 0 )

int main()
{
   TEST_CASE( list_length_example, generate_correct_input, 0 );
   TEST_CASE( list_length_example, generate_correct_input, 1 );
   TEST_CASE( list_length_example, generate_correct_input, 2 );
   TEST_CASE( list_length_example, generate_correct_input, 3 );
   TEST_CASE( list_length_example, generate_correct_input, 4 );
   TEST_CASE( list_length_example, generate_correct_input, 5 );
   TEST_CASE( list_length_example, generate_correct_input, 6 );
   TEST_CASE( list_length_example, generate_correct_input, 24 );
   TEST_CASE( list_length_example, generate_correct_input, 42 );
   TEST_CASE( list_length_example, generate_correct_input, 100 );

   TEST_CASE( list_length_example, generate_incorrect_input1, 0 );
   TEST_CASE( list_length_example, generate_incorrect_input1, 1 );
   TEST_CASE( list_length_example, generate_incorrect_input1, 2 );
   TEST_CASE( list_length_example, generate_incorrect_input1, 3 );
   TEST_CASE( list_length_example, generate_incorrect_input1, 4 );
   TEST_CASE( list_length_example, generate_incorrect_input1, 5 );
   TEST_CASE( list_length_example, generate_incorrect_input1, 6 );
   TEST_CASE( list_length_example, generate_incorrect_input1, 24 );
   TEST_CASE( list_length_example, generate_incorrect_input1, 42 );
   TEST_CASE( list_length_example, generate_incorrect_input1, 100 );

   TEST_CASE( list_length_example, generate_incorrect_input2, 0 );
   TEST_CASE( list_length_example, generate_incorrect_input2, 1 );
   TEST_CASE( list_length_example, generate_incorrect_input2, 2 );
   TEST_CASE( list_length_example, generate_incorrect_input2, 3 );
   TEST_CASE( list_length_example, generate_incorrect_input2, 4 );
   TEST_CASE( list_length_example, generate_incorrect_input2, 5 );
   TEST_CASE( list_length_example, generate_incorrect_input2, 6 );
   TEST_CASE( list_length_example, generate_incorrect_input2, 24 );
   TEST_CASE( list_length_example, generate_incorrect_input2, 42 );
   TEST_CASE( list_length_example, generate_incorrect_input2, 100 );

   TEST_CASE( list_min_max_example, generate_correct_input, 0 );
   TEST_CASE( list_min_max_example, generate_correct_input, 1 );
   TEST_CASE( list_min_max_example, generate_correct_input, 2 );
   TEST_CASE( list_min_max_example, generate_correct_input, 3 );
   TEST_CASE( list_min_max_example, generate_correct_input, 4 );
   TEST_CASE( list_min_max_example, generate_correct_input, 5 );
   TEST_CASE( list_min_max_example, generate_correct_input, 6 );
   TEST_CASE( list_min_max_example, generate_correct_input, 24 );
   TEST_CASE( list_min_max_example, generate_correct_input, 42 );
   TEST_CASE( list_min_max_example, generate_correct_input, 100 );

   // works
   TEST_CASE( list_min_max_example, generate_incorrect_input1, 0 );
   TEST_CASE( list_min_max_example, generate_incorrect_input1, 1 );
   TEST_CASE( list_min_max_example, generate_incorrect_input1, 2 );
   TEST_CASE( list_min_max_example, generate_incorrect_input1, 3 );
   TEST_CASE( list_min_max_example, generate_incorrect_input1, 4 );
   TEST_CASE( list_min_max_example, generate_incorrect_input1, 5 );
   TEST_CASE( list_min_max_example, generate_incorrect_input1, 6 );
   TEST_CASE( list_min_max_example, generate_incorrect_input1, 24 );
   TEST_CASE( list_min_max_example, generate_incorrect_input1, 42 );
   TEST_CASE( list_min_max_example, generate_incorrect_input1, 100 );

   // fails
   TEST_CASE( list_min_max_example, generate_incorrect_input2, 0 );
   TEST_CASE( list_min_max_example, generate_incorrect_input2, 1 );
   TEST_CASE( list_min_max_example, generate_incorrect_input2, 2 );
   TEST_CASE( list_min_max_example, generate_incorrect_input2, 3 );
   TEST_CASE( list_min_max_example, generate_incorrect_input2, 4 );
   TEST_CASE( list_min_max_example, generate_incorrect_input2, 5 );
   TEST_CASE( list_min_max_example, generate_incorrect_input2, 6 );
   TEST_CASE( list_min_max_example, generate_incorrect_input2, 24 );
   TEST_CASE( list_min_max_example, generate_incorrect_input2, 42 );
   TEST_CASE( list_min_max_example, generate_incorrect_input2, 100 );
}