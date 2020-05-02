// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_CONTRIB_ANALYZE_HPP
#define TAO_PEGTL_CONTRIB_ANALYZE_HPP

#include <cassert>
#include <cstddef>
#include <iostream>
#include <map>
#include <set>
#include <stdexcept>
#include <string_view>
#include <utility>
#include <vector>

#include "../config.hpp"
#include "../demangle.hpp"

#include "analyze_traits.hpp"

#include "internal/set_stack_guard.hpp"

#include "../internal/dependent_false.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   namespace internal
   {
      struct analyze_entry
      {
         explicit analyze_entry( const analyze_type in_type ) noexcept
            : type( in_type )
         {}

         const analyze_type type;
         std::vector< std::string_view > subs;
      };

      class analyze_cycles_impl
      {
      public:
         analyze_cycles_impl( analyze_cycles_impl&& ) = delete;
         analyze_cycles_impl( const analyze_cycles_impl& ) = delete;

         ~analyze_cycles_impl() = default;

         void operator=( analyze_cycles_impl&& ) = delete;
         void operator=( const analyze_cycles_impl& ) = delete;

         [[nodiscard]] std::size_t problems()
         {
            for( auto i = m_info.begin(); i != m_info.end(); ++i ) {
               m_results[ i->first ] = work( i, false );
               m_cache.clear();
            }
            return m_problems;
         }

         template< typename Rule >
         [[nodiscard]] bool consumes() const
         {
            return m_results.at( demangle< Rule >() );
         }

      protected:
         explicit analyze_cycles_impl( const bool verbose ) noexcept
            : m_verbose( verbose ),
              m_problems( 0 )
         {}

         [[nodiscard]] std::map< std::string_view, analyze_entry >::const_iterator find( const std::string_view name ) const noexcept
         {
            const auto iter = m_info.find( name );
            assert( iter != m_info.end() );
            return iter;
         }

         [[nodiscard]] bool work( const std::map< std::string_view, analyze_entry >::const_iterator& start, const bool accum )
         {
            if( const auto j = m_cache.find( start->first ); j != m_cache.end() ) {
               return j->second;
            }
            if( const auto g = set_stack_guard( m_stack, start->first ) ) {
               switch( start->second.type ) {
                  case analyze_type::any: {
                     bool a = false;
                     for( const auto& r : start->second.subs ) {
                        a = a || work( find( r ), accum || a );
                     }
                     return m_cache[ start->first ] = true;
                  }
                  case analyze_type::opt: {
                     bool a = false;
                     for( const auto& r : start->second.subs ) {
                        a = a || work( find( r ), accum || a );
                     }
                     return m_cache[ start->first ] = false;
                  }
                  case analyze_type::seq: {
                     bool a = false;
                     for( const auto& r : start->second.subs ) {
                        a = a || work( find( r ), accum || a );
                     }
                     return m_cache[ start->first ] = a;
                  }
                  case analyze_type::sor: {
                     bool a = true;
                     for( const auto& r : start->second.subs ) {
                        a = a && work( find( r ), accum );
                     }
                     return m_cache[ start->first ] = a;
                  }
               }
               assert( false );  // LCOV_EXCL_LINE
            }
            if( !accum ) {
               ++m_problems;
               if( m_verbose ) {
                  std::cerr << "problem: cycle without progress detected at rule class " << start->first << std::endl;  // LCOV_EXCL_LINE
               }
            }
            return m_cache[ start->first ] = accum;
         }

         const bool m_verbose;

         std::size_t m_problems;

         std::map< std::string_view, analyze_entry > m_info;
         std::set< std::string_view > m_stack;
         std::map< std::string_view, bool > m_cache;
         std::map< std::string_view, bool > m_results;
      };

      template< typename Name >
      std::string_view analyze_insert( std::map< std::string_view, analyze_entry >& info )
      {
         using Traits = analyze_traits< Name, typename Name::rule_t >;

         const auto [ i, b ] = info.try_emplace( demangle< Name >(), Traits::type_v );
         if( b ) {
            analyze_insert_impl( typename Traits::subs_t(), i->second.subs, info );
         }
         return i->first;
      }

      template< typename... Subs >
      void analyze_insert_impl( type_list< Subs... > /*unused*/, std::vector< std::string_view >& subs, std::map< std::string_view, analyze_entry >& info )
      {
         ( subs.emplace_back( analyze_insert< Subs >( info ) ), ... );
      }

      template< typename Grammar >
      class analyze_cycles
         : public analyze_cycles_impl
      {
      public:
         explicit analyze_cycles( const bool verbose )
            : analyze_cycles_impl( verbose )
         {
            analyze_insert< Grammar >( m_info );
         }
      };

   }  // namespace internal

   template< typename Grammar >
   [[nodiscard]] std::size_t analyze( const bool verbose = true )
   {
      return internal::analyze_cycles< Grammar >( verbose ).problems();
   }

}  // namespace TAO_PEGTL_NAMESPACE

#endif
