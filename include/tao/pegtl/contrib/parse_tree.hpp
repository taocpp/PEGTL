// Copyright (c) 2017-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_CONTRIB_PARSE_TREE_HPP
#define TAO_PEGTL_CONTRIB_PARSE_TREE_HPP

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

         class state
         {
         private:
            std::vector< std::unique_ptr< node > > stack;

         public:
            state()
            {
               emplace_back();
            }

            const node& root() const noexcept
            {
               return *stack.front();
            }

            std::unique_ptr< node >& back() noexcept
            {
               return stack.back();
            }

            void pop_back() noexcept
            {
               return stack.pop_back();
            }

            void emplace_back()
            {
               stack.emplace_back( std::unique_ptr< node >( new node ) );  // NOLINT: std::make_unique requires C++14
            }
         };

      }  // namespace parse_tree

      namespace internal
      {
         template< typename T, typename = void >
         struct transform
         {
            static void call( std::unique_ptr< parse_tree::node >& /*unused*/ ) noexcept
            {
            }
         };

         template< typename T >
         struct transform< T, decltype( T::transform( std::declval< std::unique_ptr< parse_tree::node >& >() ), void() ) >
         {
            static void call( std::unique_ptr< parse_tree::node >& n )
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
            static void start( const Input& in, TAO_PEGTL_NAMESPACE::parse_tree::state& s )
            {
               s.emplace_back();
               s.back()->begin = in.iterator();
               s.back()->source = in.source();
               s.back()->id = &typeid( Rule );
            }

            template< typename Input >
            static void success( const Input& in, TAO_PEGTL_NAMESPACE::parse_tree::state& s )
            {
               auto n = std::move( s.back() );
               n->end = in.iterator();
               s.pop_back();
               transform< S< Rule > >::call( n );
               if( n ) {
                  s.back()->children.emplace_back( std::move( n ) );
               }
            }

            template< typename Input >
            static void failure( const Input& /*unused*/, TAO_PEGTL_NAMESPACE::parse_tree::state& s ) noexcept
            {
               s.pop_back();
            }
         };

      }  // namespace internal

      namespace parse_tree
      {
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
         std::pair< bool, state > parse( Input& in )
         {
            std::pair< bool, state > result;
            result.first = TAO_PEGTL_NAMESPACE::parse< Grammar, nothing, make_builder< S >::template type >( in, result.second );
            return result;
         }

      }  // namespace parse_tree

   }  // namespace TAO_PEGTL_NAMESPACE

}  // namespace tao

#endif
