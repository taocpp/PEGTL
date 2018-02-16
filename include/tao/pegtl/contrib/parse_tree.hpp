// Copyright (c) 2017-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_CONTRIB_PARSE_TREE_HPP
#define TAO_PEGTL_CONTRIB_PARSE_TREE_HPP

#include <cassert>
#include <memory>
#include <type_traits>
#include <typeinfo>
#include <utility>
#include <vector>

#include "../config.hpp"
#include "../normal.hpp"
#include "../parse.hpp"

#include "../internal/iterator.hpp"

namespace tao
{
   namespace TAO_PEGTL_NAMESPACE
   {
      namespace parse_tree
      {
         struct node
         {
            std::vector< std::unique_ptr< node > > children;
            const std::type_info* id = nullptr;
            internal::iterator begin;
            internal::iterator end;
            std::string source;
         };

         namespace internal
         {
            class state
            {
            private:
               std::vector< std::unique_ptr< node > > stack;

            public:
               state()
               {
                  emplace_back();
               }

               void emplace_back()
               {
                  stack.emplace_back( std::unique_ptr< node >( new node ) );  // NOLINT: std::make_unique requires C++14
               }

               std::unique_ptr< node >& back() noexcept
               {
                  return stack.back();
               }

               void pop_back() noexcept
               {
                  return stack.pop_back();
               }
            };

            template< typename T, typename = void >
            struct transform
            {
               static void call( std::unique_ptr< node >& /*unused*/ ) noexcept
               {
               }
            };

            template< typename T >
            struct transform< T, decltype( T::transform( std::declval< std::unique_ptr< node >& >() ), void() ) >
            {
               static void call( std::unique_ptr< node >& n )
               {
                  T::transform( n );
               }
            };

            template< template< typename > class S >
            struct parse_tree
            {
               template< typename Rule, bool = S< Rule >::value >
               struct builder;
            };

            template< template< typename > class S >
            template< typename Rule >
            struct parse_tree< S >::builder< Rule, false >
               : normal< Rule >
            {
            };

            template< template< typename > class S >
            template< typename Rule >
            struct parse_tree< S >::builder< Rule, true >
               : normal< Rule >
            {
               template< typename Input >
               static void start( const Input& in, state& st )
               {
                  st.emplace_back();
                  st.back()->begin = in.iterator();
                  st.back()->source = in.source();
                  st.back()->id = &typeid( Rule );
               }

               template< typename Input >
               static void success( const Input& in, state& st )
               {
                  auto n = std::move( st.back() );
                  n->end = in.iterator();
                  st.pop_back();
                  transform< S< Rule > >::call( n );
                  if( n ) {
                     st.back()->children.emplace_back( std::move( n ) );
                  }
               }

               template< typename Input >
               static void failure( const Input& /*unused*/, state& st ) noexcept
               {
                  st.pop_back();
               }
            };

         }  // namespace internal

         template< typename >
         struct store_false : std::false_type
         {
         };

         template< typename >
         struct store_true : std::true_type
         {
         };

         template< template< typename > class S = store_true >
         struct make_builder
         {
            template< typename Rule >
            struct type
               : internal::parse_tree< S >::template builder< Rule >
            {
            };
         };

         template< typename Grammar, template< typename > class S = store_true, typename Input >
         std::pair< bool, std::unique_ptr< node > > parse( Input& in )
         {
            internal::state st;
            const bool result = TAO_PEGTL_NAMESPACE::parse< Grammar, nothing, make_builder< S >::template type >( in, st );
            return { result, std::move( st.back() ) };
         }

      }  // namespace parse_tree

   }  // namespace TAO_PEGTL_NAMESPACE

}  // namespace tao

#endif
