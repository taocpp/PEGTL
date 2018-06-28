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

#include "../internal/conditional.hpp"
#include "../internal/demangle.hpp"
#include "../internal/has_apply.hpp"
#include "../internal/has_apply0.hpp"
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

            // if application of the rule succeeded, this method is called
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

            template< typename Control, template< typename... > class Action, typename Input, typename... States >
            struct return_type_control_apply0
            {
               using type = decltype( Control::template apply0< Action >( std::declval< const Input& >(), std::declval< States&& >()... ) );
            };

            template< typename Node, typename Rule, typename... States >
            struct return_type_node_apply0
            {
               using type = decltype( std::declval< Node >().template apply0< Rule, States... >( std::declval< States&& >()... ) );
            };

            template< typename Control, template< typename... > class Action, typename Iterator, typename Input, typename... States >
            struct return_type_control_apply
            {
               using type = decltype( Control::template apply< Action >( std::declval< const Iterator& >(), std::declval< const Input& >(), std::declval< States&& >()... ) );
            };

            template< typename Node, typename Rule, typename ActionInput, typename... States >
            struct return_type_node_apply
            {
               using type = decltype( std::declval< Node >().template apply< Rule, ActionInput, States... >( std::declval< const ActionInput& >(), std::declval< States&& >()... ) );
            };

            template< typename, typename, typename, typename... >
            struct has_node_apply0 : std::false_type
            {
            };

            template< typename Node, typename Rule, typename... Args >
            struct has_node_apply0< Node, Rule, decltype( std::declval< Node >().template apply0< Rule, Args... > ( std::declval< Args >()... ) ), Args... > : std::true_type
            {
            };

            template< typename, typename, typename, typename... >
            struct has_node_apply : std::false_type
            {
            };

            template< typename Node, typename Rule, typename... Args >
            struct has_node_apply< Node, Rule, decltype( std::declval< Node >().template apply< Rule, Args... > ( std::declval< Args >()... ) ), Args... > : std::true_type
            {
            };

            enum class apply_mode : char
            {
               NOTHING = 0,
               VOID_APPLY0 = 1,
               BOOL_APPLY0 = 2,
               VOID_APPLY = 3,
               BOOL_APPLY = 4
            };

            template< typename Rule, template < typename... > class A, typename... States >
            using has_void_apply0 = tao::TAO_PEGTL_NAMESPACE::internal::has_apply0< A< Rule >, void, States... >;
            template< typename Rule, template < typename... > class A, typename... States >
            using has_bool_apply0 = tao::TAO_PEGTL_NAMESPACE::internal::has_apply0< A< Rule >, bool, States... >;
            template< typename Rule, template < typename... > class A, typename ActionInput, typename... States >
            using has_void_apply = tao::TAO_PEGTL_NAMESPACE::internal::has_apply< A< Rule >, void, ActionInput, States... >;
            template< typename Rule, template < typename... > class A, typename ActionInput, typename... States >
            using has_bool_apply = tao::TAO_PEGTL_NAMESPACE::internal::has_apply< A< Rule >, bool, ActionInput, States... >;
            template< typename Rule, template < typename... > class A, typename... States >
            using has_void_or_bool_apply0 = typename std::conditional< has_void_apply0< Rule, A, States... >::value ||
                                                                       has_bool_apply0< Rule, A, States... >::value,
                                                                       std::true_type, std::false_type >::type;
            template< typename Rule, template < typename... > class A, typename ActionInput, typename... States >
            using has_void_or_bool_apply = typename std::conditional< has_void_apply< Rule, A, ActionInput, States... >::value ||
                                                                      has_bool_apply< Rule, A, ActionInput, States... >::value,
                                                                      std::true_type, std::false_type >::type;
            template< typename Rule, template < typename... > class A, typename ActionInput, typename... States >
            using has_void_or_bool_apply0_or_apply = typename std::conditional< has_void_or_bool_apply0< Rule, A, States... >::value ||
                                                                                has_void_or_bool_apply< Rule, A, ActionInput, States... >::value,
                                                                                std::true_type, std::false_type >::type;
            template< typename Rule, template < typename... > class A, typename ActionInput, typename... States >
            using has_void_or_bool_apply0_and_apply = typename std::conditional< has_void_or_bool_apply0< Rule, A, States... >::value &&
                                                                                 has_void_or_bool_apply< Rule, A, ActionInput, States... >::value,
                                                                                 std::true_type, std::false_type >::type;
            template< typename Rule, template < typename... > class A, typename ActionInput, typename... States >
            using valid_action_apply = typename std::conditional< !has_void_or_bool_apply0_and_apply< Rule, A, ActionInput, States... >::value,
                                                                  std::true_type, std::false_type >::type;
            template< typename Rule, template < typename... > class A, typename ActionInput, typename... States >
            using action_apply_mode = typename std::integral_constant< char,
                                                                       static_cast< char >( apply_mode::NOTHING ) +
                                                                       static_cast< char >( apply_mode::VOID_APPLY0 ) * has_void_apply0< Rule, A, States... >::value +
                                                                       static_cast< char >( apply_mode::BOOL_APPLY0 ) * has_bool_apply0< Rule, A, States... >::value +
                                                                       static_cast< char >( apply_mode::VOID_APPLY ) * has_void_apply< Rule, A, ActionInput, States... >::value +
                                                                       static_cast< char >( apply_mode::BOOL_APPLY ) * has_bool_apply< Rule, A, ActionInput, States... >::value >::type;

            template< typename Node, typename Rule, typename... States >
            using has_node_void_apply0 = has_node_apply0< Node, Rule, void, States... >;
            template< typename Node, typename Rule, typename... States >
            using has_node_bool_apply0 = has_node_apply0< Node, Rule, bool, States... >;
            template< typename Node, typename Rule, typename ActionInput, typename... States >
            using has_node_void_apply = has_node_apply< Node, Rule, void, ActionInput, States... >;
            template< typename Node, typename Rule, typename ActionInput, typename... States >
            using has_node_bool_apply = has_node_apply< Node, Rule, bool, ActionInput, States... >;
            template< typename Node, typename Rule, typename... States >
            using has_node_void_or_bool_apply0 = typename std::conditional< has_node_void_apply0< Node, Rule, States... >::value ||
                                                                            has_node_bool_apply0< Node, Rule, States... >::value,
                                                                            std::true_type, std::false_type >::type;
            template< typename Node, typename Rule, typename ActionInput, typename... States >
            using has_node_void_or_bool_apply = typename std::conditional< has_node_void_apply< Node, Rule, ActionInput, States... >::value ||
                                                                           has_node_bool_apply< Node, Rule, ActionInput, States... >::value,
                                                                           std::true_type, std::false_type >::type;
            template< typename Node, typename Rule, typename ActionInput, typename... States >
            using has_node_void_or_bool_apply0_or_apply = typename std::conditional< has_node_void_or_bool_apply0< Node, Rule, States... >::value ||
                                                                                     has_node_void_or_bool_apply< Node, Rule, ActionInput, States... >::value,
                                                                                     std::true_type, std::false_type >::type;
            template< typename Node, typename Rule, typename ActionInput, typename... States >
            using has_node_void_or_bool_apply0_and_apply = typename std::conditional< has_node_void_or_bool_apply0< Node, Rule, States... >::value &&
                                                                                      has_node_void_or_bool_apply< Node, Rule, ActionInput, States... >::value,
                                                                                      std::true_type, std::false_type >::type;
            template< typename Node, typename Rule, typename ActionInput, typename... States >
            using valid_node_apply = typename std::conditional< !has_node_void_or_bool_apply0_and_apply< Node, Rule, ActionInput, States... >::value,
                                                                std::true_type, std::false_type >::type;
            template< typename Node, typename Rule, typename ActionInput, typename... States >
            using node_apply_mode = typename std::integral_constant< char,
                                                                     static_cast< char >( apply_mode::NOTHING ) +
                                                                     static_cast< char >( apply_mode::VOID_APPLY0 ) * has_node_void_apply0< Node, Rule, States... >::value +
                                                                     static_cast< char >( apply_mode::BOOL_APPLY0 ) * has_node_bool_apply0< Node, Rule, States... >::value +
                                                                     static_cast< char >( apply_mode::VOID_APPLY ) * has_node_void_apply< Node, Rule, ActionInput, States... >::value +
                                                                     static_cast< char >( apply_mode::BOOL_APPLY ) * has_node_bool_apply< Node, Rule, ActionInput, States... >::value >::type;

            template< typename Node, template< typename > class A, typename ActionInput, typename... States >
            struct make_action
            {
               template< typename Rule,
                         apply_mode action_apply_mode = apply_mode::NOTHING,
                         apply_mode node_apply_mode = apply_mode::NOTHING >
               struct action;

               template< typename Rule >
               using type = action< Rule,
                                    static_cast< apply_mode >( action_apply_mode< Rule, A, ActionInput, States... >::value ),
                                    static_cast< apply_mode >( node_apply_mode< Node, Rule, ActionInput, States... >::value ) >;
            };

            template< typename Node, template< typename > class A, typename ActionInput, typename... States >
            template< typename Rule >
            struct make_action< Node, A, ActionInput, States... >::action< Rule, apply_mode::NOTHING, apply_mode::NOTHING >
               : nothing< Rule >
            {
            };

            template< typename Node, template< typename > class A, typename ActionInput, typename... States >
            template< typename Rule >
            struct make_action< Node, A, ActionInput, States... >::action< Rule, apply_mode::NOTHING, apply_mode::VOID_APPLY0 >
            {
               static void apply0( state< Node >& state, States&&... st )
               {
                  state.back()->template apply0< Rule >( st... );
               }
            };

            template< typename Node, template< typename > class A, typename ActionInput, typename... States >
            template< typename Rule >
            struct make_action< Node, A, ActionInput, States... >::action< Rule, apply_mode::NOTHING, apply_mode::BOOL_APPLY0 >
            {
               static bool apply0( state< Node >& state, States&&... st )
               {
                  return state.back()->template apply0< Rule >( st... );
               }
            };

            template< typename Node, template< typename > class A, typename ActionInput, typename... States >
            template< typename Rule >
            struct make_action< Node, A, ActionInput, States... >::action< Rule, apply_mode::NOTHING, apply_mode::VOID_APPLY >
            {
               static void apply( const ActionInput& in, state< Node >& state, States&&... st )
               {
                  state.back()->template apply< Rule >( in, st... );
               }
            };

            template< typename Node, template< typename > class A, typename ActionInput, typename... States >
            template< typename Rule >
            struct make_action< Node, A, ActionInput, States... >::action< Rule, apply_mode::NOTHING, apply_mode::BOOL_APPLY >
            {
               static bool apply( const ActionInput& in, state< Node >& state, States&&... st )
               {
                  return state.back()->template apply< Rule >( in, st... );
               }
            };

            template< typename Node, template< typename > class A, typename ActionInput, typename... States >
            template< typename Rule >
            struct make_action< Node, A, ActionInput, States... >::action< Rule, apply_mode::VOID_APPLY0, apply_mode::NOTHING >
            {
               static void apply0( state< Node >& /*state*/, States&&... st )
               {
                  A< Rule >::apply0( st... );
               }
            };

            template< typename Node, template< typename > class A, typename ActionInput, typename... States >
            template< typename Rule >
            struct make_action< Node, A, ActionInput, States... >::action< Rule, apply_mode::VOID_APPLY0, apply_mode::VOID_APPLY0 >
            {
               static void apply0( state< Node >& state, States&&... st )
               {
                  A< Rule >::apply0( st... );
                  state.back()->template apply0< Rule >( st... );
               }
            };

            template< typename Node, template< typename > class A, typename ActionInput, typename... States >
            template< typename Rule >
            struct make_action< Node, A, ActionInput, States... >::action< Rule, apply_mode::VOID_APPLY0, apply_mode::BOOL_APPLY0 >
            {
               static bool apply0( state< Node >& state, States&&... st )
               {
                  A< Rule >::apply0( st... );
                  return state.back()->template apply0< Rule >( st... );
               }
            };

            template< typename Node, template< typename > class A, typename ActionInput, typename... States >
            template< typename Rule >
            struct make_action< Node, A, ActionInput, States... >::action< Rule, apply_mode::VOID_APPLY0, apply_mode::VOID_APPLY >
            {
               static void apply( const ActionInput& in, state< Node >& state, States&&... st )
               {
                  A< Rule >::apply0( st... );
                  state.back()->template apply< Rule >( in, st... );
               }
            };

            template< typename Node, template< typename > class A, typename ActionInput, typename... States >
            template< typename Rule >
            struct make_action< Node, A, ActionInput, States... >::action< Rule, apply_mode::VOID_APPLY0, apply_mode::BOOL_APPLY >
            {
               static bool apply( const ActionInput& in, state< Node >& state, States&&... st )
               {
                  A< Rule >::apply0( st... );
                  return state.back()->template apply< Rule >( in, st... );
               }
            };

            template< typename Node, template< typename > class A, typename ActionInput, typename... States >
            template< typename Rule >
            struct make_action< Node, A, ActionInput, States... >::action< Rule, apply_mode::BOOL_APPLY0, apply_mode::NOTHING >
            {
               static bool apply0( state< Node >& /*state*/, States&&... st )
               {
                  return A< Rule >::apply0( st... );
               }
            };

            template< typename Node, template< typename > class A, typename ActionInput, typename... States >
            template< typename Rule >
            struct make_action< Node, A, ActionInput, States... >::action< Rule, apply_mode::BOOL_APPLY0, apply_mode::VOID_APPLY0 >
            {
               static bool apply0( state< Node >& state, States&&... st )
               {
                  if ( !A< Rule >::apply0( st... ) )
                     return false;
                  state.back()->template apply0< Rule >( st... );
                  return true;
               }
            };

            template< typename Node, template< typename > class A, typename ActionInput, typename... States >
            template< typename Rule >
            struct make_action< Node, A, ActionInput, States... >::action< Rule, apply_mode::BOOL_APPLY0, apply_mode::BOOL_APPLY0 >
            {
               static bool apply0( state< Node >& state, States&&... st )
               {
                  if ( !A< Rule >::apply0( st... ) )
                     return false;
                  return state.back()->template apply0< Rule >( st... );
               }
            };

            template< typename Node, template< typename > class A, typename ActionInput, typename... States >
            template< typename Rule >
            struct make_action< Node, A, ActionInput, States... >::action< Rule, apply_mode::BOOL_APPLY0, apply_mode::VOID_APPLY >
            {
               static bool apply( const ActionInput& in, state< Node >& state, States&&... st )
               {
                  if ( !A< Rule >::apply0( st... ) )
                     return false;
                  state.back()->template apply< Rule >( in, st... );
                  return true;
               }
            };

            template< typename Node, template< typename > class A, typename ActionInput, typename... States >
            template< typename Rule >
            struct make_action< Node, A, ActionInput, States... >::action< Rule, apply_mode::BOOL_APPLY0, apply_mode::BOOL_APPLY >
            {
               static bool apply( const ActionInput& in, state< Node >& state, States&&... st )
               {
                  if ( !A< Rule >::apply0( st... ) )
                     return false;
                  return state.back()->template apply< Rule >( in, st... );
               }
            };

            template< typename Node, template< typename > class A, typename ActionInput, typename... States >
            template< typename Rule >
            struct make_action< Node, A, ActionInput, States... >::action< Rule, apply_mode::VOID_APPLY, apply_mode::NOTHING >
            {
               static void apply( const ActionInput& in, state< Node >& /*state*/, States&&... st )
               {
                  A< Rule >::apply( in, st... );
               }
            };

            template< typename Node, template< typename > class A, typename ActionInput, typename... States >
            template< typename Rule >
            struct make_action< Node, A, ActionInput, States... >::action< Rule, apply_mode::VOID_APPLY, apply_mode::VOID_APPLY0 >
            {
               static void apply( const ActionInput& in, state< Node >& state, States&&... st )
               {
                  A< Rule >::apply( in, st... );
                  state.back()->template apply0< Rule >( st... );
               }
            };

            template< typename Node, template< typename > class A, typename ActionInput, typename... States >
            template< typename Rule >
            struct make_action< Node, A, ActionInput, States... >::action< Rule, apply_mode::VOID_APPLY, apply_mode::BOOL_APPLY0 >
            {
               static bool apply( const ActionInput& in, state< Node >& state, States&&... st )
               {
                  A< Rule >::apply( in, st... );
                  return state.back()->template apply0< Rule >( st... );
               }
            };

            template< typename Node, template< typename > class A, typename ActionInput, typename... States >
            template< typename Rule >
            struct make_action< Node, A, ActionInput, States... >::action< Rule, apply_mode::VOID_APPLY, apply_mode::VOID_APPLY >
            {
               static void apply( const ActionInput& in, state< Node >& state, States&&... st )
               {
                  A< Rule >::apply( in, st... );
                  state.back()->template apply< Rule >( in, st... );
               }
            };

            template< typename Node, template< typename > class A, typename ActionInput, typename... States >
            template< typename Rule >
            struct make_action< Node, A, ActionInput, States... >::action< Rule, apply_mode::VOID_APPLY, apply_mode::BOOL_APPLY >
            {
               static bool apply( const ActionInput& in, state< Node >& state, States&&... st )
               {
                  A< Rule >::apply( in, st... );
                  return state.back()->template apply< Rule >( in, st... );
               }
            };

            template< typename Node, template< typename > class A, typename ActionInput, typename... States >
            template< typename Rule >
            struct make_action< Node, A, ActionInput, States... >::action< Rule, apply_mode::BOOL_APPLY, apply_mode::NOTHING >
            {
               static bool apply( const ActionInput& in, state< Node >& /*state*/, States&&... st )
               {
                  return A< Rule >::apply( in, st... );
               }
            };

            template< typename Node, template< typename > class A, typename ActionInput, typename... States >
            template< typename Rule >
            struct make_action< Node, A, ActionInput, States... >::action< Rule, apply_mode::BOOL_APPLY, apply_mode::VOID_APPLY0 >
            {
               static bool apply( const ActionInput& in, state< Node >& state, States&&... st )
               {
                  if ( !A< Rule >::apply( in, st... ) )
                     return false;
                  state.back()->template apply0< Rule >( st... );
                  return true;
               }
            };

            template< typename Node, template< typename > class A, typename ActionInput, typename... States >
            template< typename Rule >
            struct make_action< Node, A, ActionInput, States... >::action< Rule, apply_mode::BOOL_APPLY, apply_mode::BOOL_APPLY0 >
            {
               static bool apply( const ActionInput& in, state< Node >& state, States&&... st )
               {
                  if ( !A< Rule >::apply( in, st... ) )
                     return false;
                  return state.back()->template apply0< Rule >( st... );
               }
            };

            template< typename Node, template< typename > class A, typename ActionInput, typename... States >
            template< typename Rule >
            struct make_action< Node, A, ActionInput, States... >::action< Rule, apply_mode::BOOL_APPLY, apply_mode::VOID_APPLY >
            {
               static bool apply( const ActionInput& in, state< Node >& state, States&&... st )
               {
                  if ( !A< Rule >::apply( in, st... ) )
                     return false;
                  state.back()->template apply< Rule >( in, st... );
                  return true;
               }
            };

            template< typename Node, template< typename > class A, typename ActionInput, typename... States >
            template< typename Rule >
            struct make_action< Node, A, ActionInput, States... >::action< Rule, apply_mode::BOOL_APPLY, apply_mode::BOOL_APPLY >
            {
               static bool apply( const ActionInput& in, state< Node >& state, States&&... st )
               {
                  if ( !A< Rule >::apply( in, st... ) )
                     return false;
                  return state.back()->template apply< Rule >( in, st... );
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
               static typename return_type_control_apply0< C< Rule >, Action, Input, state< Node >, States... >::type
               apply0( const Input& in, state< Node >& state, States&&... st )
               {
                  return C< Rule >::template apply0< Action >( in, state, st... );
               }

               template< template< typename... > class Action, typename Iterator, typename Input, typename Node, typename... States >
               static typename return_type_control_apply< C< Rule >, Action, Iterator, Input, state< Node >, States... >::type
               apply( const Iterator& begin, const Input& in, state< Node >& state, States&&... st )
               {
                  return C< Rule >::template apply< Action >( begin, in, state, st... );
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
               static typename return_type_control_apply0< C< Rule >, Action, Input, state< Node >, States... >::type
               apply0( const Input& in, state< Node >& state, States&&... st )
               {
                  return C< Rule >::template apply0< Action >( in, state, st... );
               }

               template< template< typename... > class Action, typename Iterator, typename Input, typename Node, typename... States >
               static typename return_type_control_apply< C< Rule >, Action, Iterator, Input, state< Node >, States... >::type
               apply( const Iterator& begin, const Input& in, state< Node >& state, States&&... st )
               {
                  return C< Rule >::template apply< Action >( begin, in, state, st... );
               }
            };

            template< typename >
            struct element
            {
            };

            template< typename >
            struct store_all : std::true_type
            {
            };

         }  // namespace internal

         struct store_content : std::true_type
         {
            template< typename Node, typename... States >
            static void transform( std::unique_ptr< Node >& /*unused*/, States&&... /*unused*/ ) noexcept
            {
            }
         };

         // some nodes don't need to store their content
         struct remove_content : std::true_type
         {
            template< typename Node, typename... States >
            static void transform( std::unique_ptr< Node >& n, States&&... st ) noexcept( noexcept( n->Node::remove_content( st... ) ) )
            {
               n->remove_content( st... );
            }
         };

         // if a node has only one child, replace the node with its child, otherwise apply B
         template< typename B >
         struct fold_one_or : B
         {
            template< typename Node, typename... States >
            static void transform( std::unique_ptr< Node >& n, States&&... st ) noexcept( noexcept( n->children.size(), n->children.front(), B::transform( n, st... ) ) )
            {
               if( n->children.size() == 1 ) {
                  n = std::move( n->children.front() );
               }
               else {
                  B::transform( n, st... );
               }
            }
         };

         // if a node has no children, discard the node, otherwise apply B
         template< typename B >
         struct discard_empty_or : B
         {
            template< typename Node, typename... States >
            static void transform( std::unique_ptr< Node >& n, States&&... st ) noexcept( noexcept( n->children.empty(), B::transform( n, st... ) ) )
            {
               if( n->children.empty() ) {
                  n.reset();
               }
               else {
                  B::transform( n, st... );
               }
            }
         };

         using fold_one = fold_one_or< remove_content >;
         using discard_empty = discard_empty_or< remove_content >;

         template< typename Rule, typename... Collections >
         struct selector : std::false_type
         {
         };

         // TODO: Implement in a non-recursive way
         // TODO: Check for multiple matches (currently: first match wins)
         template< typename Rule, typename Collection, typename... Collections >
         struct selector< Rule, Collection, Collections... >
            : TAO_PEGTL_NAMESPACE::internal::conditional< Collection::template contains< Rule >::value >::template type< typename Collection::type, selector< Rule, Collections... > >
         {
         };

         template< typename Base >
         struct apply
         {
            template< typename... Rules >
            struct to
               : internal::element< Rules >...
            {
               using type = Base;

               template< typename Rule >
               using contains = std::is_base_of< internal::element< Rule >, to >;
            };
         };

         using apply_store_content = apply< store_content >;
         using apply_remove_content = apply< remove_content >;
         using apply_fold_one = apply< fold_one >;
         using apply_discard_empty = apply< discard_empty >;

         template< typename Rule,
                   typename Node,
                   template< typename > class S = internal::store_all,
                   template< typename > class A = nothing,
                   template< typename > class C = normal,
                   typename Input,
                   typename... States >
         std::unique_ptr< Node > parse( Input&& in, States&&... st )
         {
            internal::state< Node > state;
            using ActionInput = typename std::remove_reference<Input>::type::action_t;
            static_assert(internal::valid_action_apply< Rule, A, ActionInput, States... >::value, "Invalid action: must not define both apply0 and apply");
            static_assert(internal::valid_node_apply< Node, Rule, ActionInput, States... >::value, "Invalid node: must not define both apply0 and apply");
            if( !TAO_PEGTL_NAMESPACE::parse< Rule,
                internal::make_action< Node, A, ActionInput, States... >::template type,
                internal::make_control< S, C >::template type >( in, state, st... ) ) {
               return nullptr;
            }
            assert( state.stack.size() == 1 );
            return std::move( state.back() );
         }

         template< typename Rule,
                   template< typename > class S = internal::store_all,
                   template< typename > class A = nothing,
                   template< typename > class C = normal,
                   typename Input,
                   typename... States >
         std::unique_ptr< node > parse( Input&& in, States&&... st )
         {
            return parse< Rule, node, S, A, C >( in, st... );
         }

      }  // namespace parse_tree

   }  // namespace TAO_PEGTL_NAMESPACE

}  // namespace tao

#endif
