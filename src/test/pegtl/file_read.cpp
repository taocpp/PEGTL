// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include "test.hpp"

#include "verify_file.hpp"

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      namespace test1
      {
         class file_function_wrapper
         {
         public:
            explicit file_function_wrapper( const std::string& in_source )
               : m_source( in_source )
            {
            }

            template< typename Rule,
                      template< typename... > class Action = nothing,
                      template< typename... > class Control = normal,
                      typename... States >
            bool parse( States&&... st )
            {
               return parse_file< Rule, Action, Control >( m_source, st... );
            }

            template< typename Rule,
                      template< typename... > class Action = nothing,
                      template< typename... > class Control = normal,
                      typename Outer,
                      typename... States >
            bool parse_nested( const Outer& oi, States&&... st )
            {
               return parse_file_nested< Rule, Action, Control >( oi, m_source, st... );
            }

            const std::string& source() const
            {
               return m_source;
            }

         private:
            const std::string m_source;
         };

      }  // namespace test1
      void unit_test()
      {
         verify_file< read_parser >();
         verify_file< test1::file_function_wrapper >();
      }

   }  // namespace TAOCPP_PEGTL_NAMESPACE

}  // namespace tao

#include "main.hpp"
