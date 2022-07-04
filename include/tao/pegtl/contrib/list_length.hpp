#ifndef PEGTL_LIST_LENGTH_HPP
#define PEGTL_LIST_LENGTH_HPP

#include "../internal/result_on_found.hpp"
#include "../internal/rules.hpp"

namespace tao::pegtl::list_length
{

   template< uint64_t N, typename Rule, typename Sep >
   struct list_length;

   template< typename Rule, typename Sep >
   struct list_length< 0, Rule, Sep >
      : success
   {};

   template< uint64_t N, typename Rule, typename Sep >
   struct list_length
   {
      using rule_t = list_length;
      using subs_t = type_list< Rule, Sep >;

      template< tao::pegtl::apply_mode A,
                tao::pegtl::rewind_mode M,
                template< typename... >
                class Action,
                template< typename... >
                class Control,
                typename ParseInput,
                typename... States >
      [[nodiscard]] static bool match( ParseInput& in, States&&... st )
      {
         if constexpr( N == 1 ) {
            return Control< Rule >::template match< A, M, Action, Control >( in, std::forward< States >( st )... );
         }
         else {
            auto m = in.template auto_rewind< M >();
            using m_t = decltype( m );
            // clang-format off
            return m( ( Control< Rule >::template match< A, m_t::next_rewind_mode, Action, Control >( in, std::forward< States >( st )... ) ) &&
                      ( Control< Sep >::template match< A, m_t::next_rewind_mode, Action, Control >( in, std::forward< States >( st )... ) ) &&
                      ( Control< list_length< N - 1, Rule, Sep > >::template match< A, m_t::next_rewind_mode, Action, Control >( in, std::forward< States >( st )... ) ) );
            // clang-format on
         }
      }
   };

}  // namespace tao::pegtl::list_length

#endif
