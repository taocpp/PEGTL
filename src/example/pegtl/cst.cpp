#include <cassert>
#include <string_view>
#include <vector>

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/abnf.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   namespace cst
   {
      template< typename Data, typename Position >
      struct node
      {
         Position pos;
         std::string_view rule;
         std::span< const Data > data;
         std::vector< std::unique_ptr< node > > children;
      };

      namespace internal
      {
         template< typename Data, typename Position >
         struct state
         {
            state()
            {
               stack.emplace_back( new node );
            }

            std::vector< std::size_t > rewind;
            std::vector< std::unique_ptr< node > > stack;
         };

         template< typename Rule >
         struct control
         {
            template< typename ParseInput, typename State >
            static void prep_rewind( const ParseInput& /*unused*/, State& st )
            {
               st.rewind.emplace_back( st.stack.size() );
            }

            template< typename ParseInput, typename State >
            static void will_rewind( const ParseInput& /*unused*/, State& st )
            {
               assert( !st.rewind.empty() );
               assert( st.stack.size() >= st.rewind.back() );
               st.stack.resize( st.rewind.back() );
               st.rewind.pop_back();
            }

            template< typename ParseInput, typename State >
            static void wont_rewind( const ParseInput& /*unused*/, State& st )
            {
               assert( !st.rewind.empty() );
               st.rewind.pop_back();
            }
         };

      }  // namespace internal

      template< typename Rule, typename ParseInput >
      [[nodiscard]] std::unique_ptr< typename ParseInput::data_t > parse( ParseInput& in )
      {
         internal::state< typename ParseInput::data_t, typename ParseInput::error_position_t > st;
         if( !TAO_PEGTL_NAMESPACE::parse< Rule, ..., rewind_control< control >::type >( in, st ) ) {
            return nullptr;
         }
         assert( st.rewind.empty() );
         assert( st.stack.size() == 1 );
         return std::move( st.stack.back() );
      }

   }  // namespace cst

   namespace ast
   {
   }  // namespace ast

   namespace tst
   {
   }  // namespace tst

}  // namespace TAO_PEGTL_NAMESPACE

int main( int argc, char** argv )
{
   return 0;
}
