// Copyright (c) 2017-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_CONTRIB_PARSE_TREE_HPP
#define TAOCPP_PEGTL_INCLUDE_CONTRIB_PARSE_TREE_HPP

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
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      namespace parse_tree
      {
         struct node
         {
            std::vector< std::unique_ptr< node > > children;
            const std::type_info* id = nullptr;
            internal::iterator begin;
            internal::iterator end;
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

         template< template< typename > class S, template< typename > class C >
         struct parse_tree
         {
            template< typename Rule, bool = S< Rule >::value, bool = C< Rule >::value >
            struct builder;
         };

         template< template< typename > class S, template< typename > class C >
         template< typename Rule >
         struct parse_tree< S, C >::builder< Rule, false, false >
            : normal< Rule >
         {
         };

         template< template< typename > class S, template< typename > class C >
         template< typename Rule >
         struct parse_tree< S, C >::builder< Rule, true, true >
            : normal< Rule >
         {
            static_assert( sizeof( Rule ) == 0, "error: both S<Rule>::value and C<Rule>::value are true" );
         };

         template< template< typename > class S, template< typename > class C >
         template< typename Rule >
         struct parse_tree< S, C >::builder< Rule, true, false >
            : normal< Rule >
         {
            template< typename Input >
            static void start( const Input& in, TAOCPP_PEGTL_NAMESPACE::parse_tree::state& s )
            {
               s.emplace_back();
               s.back()->begin = in.iterator();
            }

            template< typename Input >
            static void success( const Input& /*unused*/, TAOCPP_PEGTL_NAMESPACE::parse_tree::state& s )
            {
               auto n = std::move( s.back() );
               n->id = &typeid( Rule );
               s.pop_back();
               s.back()->children.emplace_back( std::move( n ) );
               transform< S< Rule > >::call( s.back()->children.back() );
            }

            template< typename Input >
            static void failure( const Input& /*unused*/, TAOCPP_PEGTL_NAMESPACE::parse_tree::state& s ) noexcept
            {
               s.pop_back();
            }
         };

         template< template< typename > class S, template< typename > class C >
         template< typename Rule >
         struct parse_tree< S, C >::builder< Rule, false, true >
            : normal< Rule >
         {
            template< typename Input >
            static void start( const Input& in, TAOCPP_PEGTL_NAMESPACE::parse_tree::state& s )
            {
               s.emplace_back();
               s.back()->begin = in.iterator();
            }

            template< typename Input >
            static void success( const Input& in, TAOCPP_PEGTL_NAMESPACE::parse_tree::state& s )
            {
               auto n = std::move( s.back() );
               n->id = &typeid( Rule );
               n->end = in.iterator();
               s.pop_back();
               s.back()->children.emplace_back( std::move( n ) );
               transform< C< Rule > >::call( s.back()->children.back() );
            }

            template< typename Input >
            static void failure( const Input& /*unused*/, TAOCPP_PEGTL_NAMESPACE::parse_tree::state& s ) noexcept
            {
               s.pop_back();
            }
         };

         template< typename >
         struct default_store_simple : std::false_type
         {
         };

         template< typename >
         struct default_store_content : std::true_type
         {
         };

      }  // namespace internal

      namespace parse_tree
      {
         template< template< typename > class S, template< typename > class C >
         struct make_builder
         {
            template< typename Rule >
            struct type
               : internal::parse_tree< S, C >::template builder< Rule >
            {
            };
         };

         template< typename Grammar, template< typename > class S, template< typename > class C, typename Input >
         state parse( Input& in )
         {
            state s;
            TAOCPP_PEGTL_NAMESPACE::parse< Grammar, nothing, make_builder< S, C >::template type >( in, s );
            return s;
         }

      }  // namespace parse_tree

   }  // namespace TAOCPP_PEGTL_NAMESPACE

}  // namespace tao

#endif
