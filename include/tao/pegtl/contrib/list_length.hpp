#ifndef PEGTL_LIST_LENGTH_HPP
#define PEGTL_LIST_LENGTH_HPP

#include "tao/pegtl/internal/result_on_found.hpp"
#include "tao/pegtl/internal/rules.hpp"

namespace tao::pegtl
{
   namespace internal
   {
      template< typename Rule, typename Sep, int64_t len >
      struct list_length;

      template< typename Rule, typename Sep >
      struct list_length< Rule, Sep, -1 >
         : list< Rule, Sep >
      {};

      template< typename Rule, typename Sep >
      struct list_length< Rule, Sep, 0 >
         : success
      {};

      template< typename Rule, typename Sep, int64_t len >
      struct list_length
      {
         static_assert( len > -2, "The length should be non-negative, or -1 for dynamic-length list." );

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
            if constexpr( len == 1 ) {
               return Control< Rule >::template match< A, M, Action, Control >( in, std::forward< States >( st )... );
            }
            else {
               auto m = in.template auto_rewind< M >();
               using m_t = decltype( m );
               // clang-format off
               return m( ( Control< Rule >::template match< A, m_t::next_rewind_mode, Action, Control >( in, std::forward< States >( st )... ) ) &&
                         ( Control< Sep >::template match< A, m_t::next_rewind_mode, Action, Control >( in, std::forward< States >( st )... ) ) &&
                         ( Control< list_length< Rule, Sep, len - 1 > >::template match< A, m_t::next_rewind_mode, Action, Control >( in, std::forward< States >( st )... ) ) );
               // clang-format on
            }
         }
      };

      template< std::size_t Min, std::size_t Max, typename Rule, typename Sep >
      struct list_min_max;

      template< typename Rule, typename Sep >
      struct list_min_max< 0, 0, Rule, Sep >
         : success
      {};

      template< std::size_t Max, typename Rule, typename Sep >
      struct list_min_max< 0, Max, Rule, Sep >
         : opt< list_min_max< 1, Max, Rule, Sep > >
      {
         static_assert( Max > 0 );
      };

      template< std::size_t Min, std::size_t Max, typename Rule, typename Sep >
      struct list_min_max
         : seq< Rule, rep_min_max< Min - 1, Max - 1, seq< Sep, Rule > > >
      {
         static_assert( Min > 0 );
      };

   }  // namespace internal

   static constexpr int64_t dynamic = -1;

   // clang-format off
   template< typename Rule, typename Sep, typename Pad = void, int64_t len = dynamic > struct list : internal::list_length< Rule, internal::pad< Sep, Pad >, len > {};
   template< typename Rule, typename Sep, typename Pad > struct list< Rule, Sep, Pad, dynamic > : internal::list< Rule, internal::pad< Sep, Pad > > {};
   template< typename Rule, typename Sep > struct list< Rule, Sep, void, dynamic > : internal::list< Rule, Sep > {};
   template< typename Rule, typename Sep, int64_t len > struct list< Rule, Sep, void, len > : internal::list_length< Rule, Sep, len > {};

   template< std::size_t Min, std::size_t Max, typename Rule, typename Sep, typename Pad = void > struct list_min_max : internal::list_min_max< Min, Max, Rule, internal::pad< Sep, Pad > > {};
   template< std::size_t Min, std::size_t Max, typename Rule, typename Sep > struct list_min_max< Min, Max, Rule, Sep, void > : internal::list_min_max< Min, Max, Rule, Sep > {};
   //clang-format on

}  // namespace tao::pegtl

#endif
