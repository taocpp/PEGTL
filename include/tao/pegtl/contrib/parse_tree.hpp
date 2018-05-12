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
#include "../nothing.hpp"
#include "../parse.hpp"

#include "../internal/demangle.hpp"
#include "../internal/iterator.hpp"

namespace tao
{
   namespace TAO_PEGTL_NAMESPACE
   {
      namespace parse_tree
      {
         template< typename T >
         struct basic_node
         {
            using node_t = T;
            using children_t = std::vector< std::unique_ptr< node_t > >;
            children_t children;

            const std::type_info* id = nullptr;
            std::string source;

            TAO_PEGTL_NAMESPACE::internal::iterator m_begin;
            TAO_PEGTL_NAMESPACE::internal::iterator m_end;

            // each node will be default constructed
            basic_node() = default;

            // no copy/move is necessary
            // (nodes are always owned/handled by a std::unique_ptr)
            basic_node( const basic_node& ) = delete;
            basic_node( basic_node&& ) = delete;

            ~basic_node() = default;

            // no assignment either
            basic_node& operator=( const basic_node& ) = delete;
            basic_node& operator=( basic_node&& ) = delete;

            bool is_root() const noexcept
            {
               return id == nullptr;
            }

            template< typename U >
            bool is() const noexcept
            {
               return id == &typeid( U );
            }

            std::string name() const
            {
               assert( !is_root() );
               return TAO_PEGTL_NAMESPACE::internal::demangle( id->name() );
            }

            position begin() const
            {
               return position( m_begin, source );
            }

            position end() const
            {
               return position( m_end, source );
            }

            bool has_content() const noexcept
            {
               return m_end.data != nullptr;
            }

            std::string content() const
            {
               assert( has_content() );
               return std::string( m_begin.data, m_end.data );
            }

            template< typename... States >
            void remove_content( States&&... /*unused*/ ) noexcept
            {
               m_end.reset();
            }

            // all non-root nodes are initialized by calling this method
            template< typename Rule, typename Input, typename... States >
            void start( const Input& in, States&&... /*unused*/ )
            {
               id = &typeid( Rule );
               source = in.source();
               m_begin = in.iterator();
            }

            // if parsing of the rule succeeded, this method is called
            template< typename Rule, typename Input, typename... States >
            void success( const Input& in, States&&... /*unused*/ ) noexcept
            {
               m_end = in.iterator();
            }

            // if parsing of the rule failed, this method is called
            template< typename Rule, typename Input, typename... States >
            void failure( const Input& /*unused*/, States&&... /*unused*/ ) noexcept
            {
            }

            // if parsing succeeded and the (optional) transform call
            // did not discard the node, it is appended to its parent.
            // note that "child" is the node whose Rule just succeeded
            // and "*this" is the parent where the node should be appended.
            template< typename... States >
            void emplace_back( std::unique_ptr< node_t > child, States&&... /*unused*/ )
            {
               assert( child );
               children.emplace_back( std::move( child ) );
            }
         };

         struct node
            : basic_node< node >
         {
         };

         namespace internal
         {
            template< typename Node >
            struct state
            {
               std::vector< std::unique_ptr< Node > > stack;

               state()
               {
                  emplace_back();
               }

               void emplace_back()
               {
                  stack.emplace_back( std::unique_ptr< Node >( new Node ) );  // NOLINT: std::make_unique requires C++14
               }

               std::unique_ptr< Node >& back() noexcept
               {
                  return stack.back();
               }

               void pop_back() noexcept
               {
                  return stack.pop_back();
               }
            };

            template< typename Node, typename S, typename = void >
            struct transform
            {
               template< typename... States >
               static void call( std::unique_ptr< Node >& /*unused*/, States&&... /*unused*/ ) noexcept
               {
               }
            };

            template< typename Node, typename S >
            struct transform< Node, S, decltype( S::transform( std::declval< std::unique_ptr< Node >& >() ), void() ) >
            {
               template< typename... States >
               static void call( std::unique_ptr< Node >& n, States&&... st ) noexcept( noexcept( S::transform( n ) ) )
               {
                  S::transform( n, st... );
               }
            };

            template< template< typename > class S, template< typename > class C >
            struct make_control
            {
               template< typename Rule, bool >
               struct control;

               template< typename Rule >
               using type = control< Rule, S< Rule >::value >;
            };

            template< typename Control, template< typename... > class Action, typename Input, typename... States >
            struct return_type_apply0
            {
               using type = decltype( Control::template apply0< Action >( std::declval< const Input& >(), std::declval< States&& >()... ) );
            };

            template< typename Control, template< typename... > class Action, typename Iterator, typename Input, typename... States >
            struct return_type_apply
            {
               using type = decltype( Control::template apply< Action >( std::declval< const Iterator& >(), std::declval< const Input& >(), std::declval< States&& >()... ) );
            };

            template< template< typename > class S, template< typename > class C >
            template< typename Rule >
            struct make_control< S, C >::control< Rule, false >
               : C< Rule >
            {
               template< typename Input, typename Node, typename... States >
               static void start( const Input& in, state< Node >& state, States&&... st )
               {
                  C< Rule >::start( in, st... );
                  state.emplace_back();
               }

               template< typename Input, typename Node, typename... States >
               static void success( const Input& in, state< Node >& state, States&&... st )
               {
                  C< Rule >::success( in, st... );
                  auto n = std::move( state.back() );
                  state.pop_back();
                  for( auto& c : n->children ) {
                     state.back()->children.emplace_back( std::move( c ) );
                  }
               }

               template< typename Input, typename Node, typename... States >
               static void failure( const Input& in, state< Node >& state, States&&... st ) noexcept( noexcept( C< Rule >::failure( in, st... ) ) )
               {
                  C< Rule >::failure( in, st... );
                  state.pop_back();
               }

               template< typename Input, typename Node, typename... States >
               static void raise( const Input& in, state< Node >& /*unused*/, States&&... st )
               {
                  C< Rule >::raise( in, st... );
               }

               template< template< typename... > class Action, typename Input, typename Node, typename... States >
               static typename return_type_apply0< C< Rule >, Action, Input, States... >::type apply0( const Input& in, state< Node >& /*unused*/, States&&... st )
               {
                  return C< Rule >::template apply0< Action >( in, st... );
               }

               template< template< typename... > class Action, typename Iterator, typename Input, typename Node, typename... States >
               static typename return_type_apply< C< Rule >, Action, Iterator, Input, States... >::type apply( const Iterator& begin, const Input& in, state< Node >& /*unused*/, States&&... st )
               {
                  return C< Rule >::template apply< Action >( begin, in, st... );
               }
            };

            template< template< typename > class S, template< typename > class C >
            template< typename Rule >
            struct make_control< S, C >::control< Rule, true >
               : C< Rule >
            {
               template< typename Input, typename Node, typename... States >
               static void start( const Input& in, state< Node >& state, States&&... st )
               {
                  C< Rule >::start( in, st... );
                  state.emplace_back();
                  state.back()->template start< Rule >( in, st... );
               }

               template< typename Input, typename Node, typename... States >
               static void success( const Input& in, state< Node >& state, States&&... st )
               {
                  C< Rule >::success( in, st... );
                  auto n = std::move( state.back() );
                  state.pop_back();
                  n->template success< Rule >( in, st... );
                  transform< Node, S< Rule > >::call( n, st... );
                  if( n ) {
                     state.back()->emplace_back( std::move( n ), st... );
                  }
               }

               template< typename Input, typename Node, typename... States >
               static void failure( const Input& in, state< Node >& state, States&&... st ) noexcept( noexcept( C< Rule >::failure( in, st... ) ) && noexcept( std::declval< node& >().template failure< Rule >( in, st... ) ) )
               {
                  C< Rule >::failure( in, st... );
                  state.back()->template failure< Rule >( in, st... );
                  state.pop_back();
               }

               template< typename Input, typename Node, typename... States >
               static void raise( const Input& in, state< Node >& /*unused*/, States&&... st )
               {
                  C< Rule >::raise( in, st... );
               }

               template< template< typename... > class Action, typename Input, typename Node, typename... States >
               static typename return_type_apply0< C< Rule >, Action, Input, States... >::type apply0( const Input& in, state< Node >& /*unused*/, States&&... st )
               {
                  return C< Rule >::template apply0< Action >( in, st... );
               }

               template< template< typename... > class Action, typename Iterator, typename Input, typename Node, typename... States >
               static typename return_type_apply< C< Rule >, Action, Iterator, Input, States... >::type apply( const Iterator& begin, const Input& in, state< Node >& /*unused*/, States&&... st )
               {
                  return C< Rule >::template apply< Action >( begin, in, st... );
               }
            };

            template< typename >
            struct store_all : std::true_type
            {
            };

         }  // namespace internal

         // some nodes don't need to store their content
         struct remove_content : std::true_type
         {
            template< typename Node, typename... States >
            static void transform( std::unique_ptr< Node >& n, States&&... st ) noexcept( noexcept( n->Node::remove_content( st... ) ) )
            {
               n->remove_content( st... );
            }
         };

         // if a node has only one child, replace the node with its child
         struct fold_one : std::true_type
         {
            template< typename Node, typename... States >
            static void transform( std::unique_ptr< Node >& n, States&&... /*unused*/ ) noexcept( noexcept( n->children.size(), n->children.front() ) )
            {
               if( n->children.size() == 1 ) {
                  n = std::move( n->children.front() );
               }
            }
         };

         // if a node has no children, discard the node
         struct discard_empty : std::true_type
         {
            template< typename Node, typename... States >
            static void transform( std::unique_ptr< Node >& n, States&&... /*unused*/ ) noexcept( noexcept( n->children.empty() ) )
            {
               if( n->children.empty() ) {
                  n.reset();
               }
            }
         };

         template< typename Rule, typename Node, template< typename > class S = internal::store_all, template< typename > class C = normal, typename Input, typename... States >
         std::unique_ptr< Node > parse( Input&& in, States&&... st )
         {
            internal::state< Node > state;
            if( !TAO_PEGTL_NAMESPACE::parse< Rule, nothing, internal::make_control< S, C >::template type >( in, state, st... ) ) {
               return nullptr;
            }
            assert( state.stack.size() == 1 );
            return std::move( state.back() );
         }

         template< typename Rule, template< typename > class S = internal::store_all, template< typename > class C = normal, typename Input, typename... States >
         std::unique_ptr< node > parse( Input&& in, States&&... st )
         {
            return parse< Rule, node, S, C >( in, st... );
         }

      }  // namespace parse_tree

   }  // namespace TAO_PEGTL_NAMESPACE

}  // namespace tao

#endif
