// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_ANALYSIS_ANALYZE_CYCLES_HH
#define PEGTL_ANALYSIS_ANALYZE_CYCLES_HH

#include <cassert>

#include <map>
#include <set>

#include <utility>
#include <iostream>

#include "grammar_info.hh"
#include "insert_guard.hh"

namespace pegtl
{
   namespace analysis
   {
      class analyze_cycles_impl
      {
      protected:
         explicit
         analyze_cycles_impl( const bool verbose )
               : m_verbose( verbose ),
                 m_problems( 0 )
         { }

         const bool m_verbose;
         unsigned m_problems;
         grammar_info m_info;
         std::set< std::string > m_stack;
         std::map< std::string, bool > m_cache;
         std::map< std::string, bool > m_results;

         const std::map< std::string, rule_info >::const_iterator find( const std::string & name ) const
         {
            const auto iter = m_info.map.find( name );
            assert( iter != m_info.map.end() );
            return iter;
         }

         bool work( const std::map< std::string, rule_info >::const_iterator & start, const bool accum )
         {
            const auto j = m_cache.find( start->first );

            if ( j != m_cache.end() ) {
               return j->second;
            }
            const auto g = make_insert_guard( m_stack, start->first );

            if ( g ) {
               switch ( start->second.type ) {
                  case rule_type::PLUS:
                  {
                     bool a = false;
                     for ( std::size_t i = 0; i < start->second.rules.size(); ++i ) {
                        a |= work( find( start->second.rules[ i ] ), accum || a );
                     }
                     if ( ! a ) {
                        ++m_problems;
                        if ( m_verbose ) {
                           std::cout << "problem: rules without progress detected in rule of type plus " << start->first << std::endl;
                        }
                     }
                     return m_cache[ start->first ] = a;
                  }
                  case rule_type::STAR:
                  {
                     bool a = false;
                     for ( std::size_t i = 0; i < start->second.rules.size(); ++i ) {
                        a |= work( find( start->second.rules[ i ] ), accum || a );
                     }
                     if ( ! a ) {
                        ++m_problems;
                        if ( m_verbose ) {
                           std::cout << "problem: rules without progress detected in rule of type star " << start->first << std::endl;
                        }
                     }
                     return m_cache[ start->first ] = false;
                  }
                  case rule_type::SEQ:
                  {
                     bool a = false;
                     for ( std::size_t i = 0; i < start->second.rules.size(); ++i ) {
                        a |= work( find( start->second.rules[ i ] ), accum || a );
                     }
                     return m_cache[ start->first ] = a;
                  }
                  case rule_type::SOR:
                  {
                     bool a = true;
                     for ( std::size_t i = 0; i < start->second.rules.size(); ++i ) {
                        a &= work( find( start->second.rules[ i ] ), accum );
                     }
                     return m_cache[ start->first ] = a;
                  }
                  case rule_type::ANY:
                  {
                     for ( std::size_t i = 0; i < start->second.rules.size(); ++i ) {
                        work( find( start->second.rules[ i ] ), accum );
                     }
                     return m_cache[ start->first ] = true;
                  }
                  case rule_type::OPT:
                  {
                     for ( std::size_t i = 0; i < start->second.rules.size(); ++i ) {
                        work( find( start->second.rules[ i ] ), accum );
                     }
                     return m_cache[ start->first ] = false;
                  }
                  case rule_type::UNTIL:
                  {
                     bool a = false;
                     for ( std::size_t i = 1; i < start->second.rules.size(); ++i ) {
                        a |= work( find( start->second.rules[ i ] ), accum || a );
                     }
                     if ( ! a ) {
                        ++m_problems;
                        if ( m_verbose ) {
                           std::cout << "problem: rules without progress detected in until " << start->first << std::endl;
                        }
                     }
                     return m_cache[ start->first ] = work( find( start->second.rules[ 0 ] ), accum );
                  }
                  case rule_type::IF:
                  {
                     assert( start->second.rules.size() == 3 );
                     const bool c = work( find( start->second.rules[ 0 ] ), accum );  // Cond
                     const bool t = work( find( start->second.rules[ 1 ] ), accum || c );  // Rule
                     const bool e = work( find( start->second.rules[ 2 ] ), accum );  // Else
                     return m_cache[ start->first ] = ( c || t ) && e;
                  }
               }
               assert( false );
            }
            if ( ! accum ) {
               ++m_problems;
               if ( m_verbose ) {
                  std::cout << "problem: cycle without progress detected at class " << start->first << std::endl;
               }
               return m_cache[ start->first ] = false;
            }
            return m_cache[ start->first ] = accum;  // Cycle with progress.
         }
      };

      template< typename Grammar >
      class analyze_cycles
            : private analyze_cycles_impl
      {
      public:
         explicit
         analyze_cycles( const bool verbose )
               : analyze_cycles_impl( verbose )
         {
            Grammar::analyze_t::template insert< Grammar >( m_info );
         }

         std::size_t problems()
         {
            for ( auto i = m_info.map.begin(); i != m_info.map.end(); ++i ) {
               m_results[ i->first ] = work( i, false );
               m_cache.clear();
            }
            return m_problems;
         }

         template< typename Rule >
         bool consumes() const
         {
            const auto i = m_results.find( internal::demangle< Rule >() );
            assert( i != m_results.end() );
            return i->second;
         }
      };

   } // analysis

} // pegtl

#endif
