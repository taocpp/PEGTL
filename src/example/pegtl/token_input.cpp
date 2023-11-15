// Copyright (c) 2020-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <string>
#include <vector>

#include <tao/pegtl.hpp>

#include <tao/pegtl/contrib/analyze_traits.hpp>

// This file contains some experiments towards generalising inputs to
// represent sequences of arbitrary objects; it's not very complete, but
// it does get a minimal example up-and-running. One main limitation is
// that nothing that throws a parse_error can be used because positions
// aren't supported by the token_parse_input.

namespace TAO_PEGTL_NAMESPACE
{
   template< typename ParseInput >
   class token_action_input
   {
   public:
      using input_t = ParseInput;
      using value_t = typename ParseInput::value_t;
      using rewind_position_t = typename ParseInput::rewind_position_t;

      token_action_input( const rewind_position_t& in_begin, const ParseInput& in_input ) noexcept
         : m_begin( in_begin ),
           m_input( in_input )
      {}

      token_action_input( const token_action_input& ) = delete;
      token_action_input( token_action_input&& ) = delete;

      ~token_action_input() = default;

      token_action_input& operator=( const token_action_input& ) = delete;
      token_action_input& operator=( token_action_input&& ) = delete;

      [[nodiscard]] const rewind_position_t& rewind_position() const noexcept
      {
         return m_begin;
      }

      [[nodiscard]] const ParseInput& input() const noexcept
      {
         return m_input;
      }

      [[nodiscard]] const auto& begin() const noexcept
      {
         return m_begin;
      }

      [[nodiscard]] decltype( auto ) end() const noexcept
      {
         return m_input.rewind_position();
      }

      [[nodiscard]] bool empty() const noexcept
      {
         return begin() == end();
      }

      [[nodiscard]] std::size_t size() const noexcept
      {
         return std::size_t( end() - begin() );
      }

   protected:
      const rewind_position_t m_begin;
      const ParseInput& m_input;
   };

   template< typename T, typename Source = std::string >
   class token_parse_input
   {
   public:
      using value_t = T;
      using source_t = Source;
      using rewind_position_t = internal::basic_small_position< T >;

      using action_t = token_action_input< token_parse_input >;

      template< typename S >
      token_parse_input( const T* in_begin, const T* in_end, S&& in_source )
         : m_begin( in_begin ),
           m_current( in_begin ),
           m_end( in_end ),
           m_source( std::forward< S >( in_source ) )
      {}

      template< typename S >
      token_parse_input( const std::vector< T >& in_vector, S&& in_source )
         : token_parse_input( in_vector.data(), in_vector.data() + in_vector.size(), std::forward< S >( in_source ) )
      {}

      token_parse_input( const token_parse_input& ) = delete;
      token_parse_input( token_parse_input&& ) = delete;

      ~token_parse_input() = default;

      token_parse_input& operator=( const token_parse_input& ) = delete;
      token_parse_input& operator=( token_parse_input&& ) = delete;

      void discard() const noexcept {}

      void require( const std::size_t /*unused*/ ) const noexcept {}

      [[nodiscard]] const T* current() const noexcept
      {
         return m_current;
      }

      [[nodiscard]] const T* begin() const noexcept
      {
         return m_begin;
      }

      [[nodiscard]] const T* end( const std::size_t /*unused*/ = 0 ) const noexcept
      {
         return m_end;
      }

      [[nodiscard]] std::size_t byte() const noexcept
      {
         return std::size_t( m_current - m_begin );  // We don't return the byte offset even if this function is still called 'byte'.
      }

      template< rewind_mode M >
      [[nodiscard]] internal::rewind_guard< M, token_parse_input > make_rewind_guard() noexcept
      {
         return internal::rewind_guard< M, token_parse_input >( this );
      }

      [[nodiscard]] rewind_position_t rewind_position() const noexcept
      {
         return rewind_position_t( m_current );
      }

      void rewind_position( const rewind_position_t& data ) noexcept
      {
         m_current = data.data;
      }

      void bump( const std::size_t in_count = 1 ) noexcept
      {
         m_current += in_count;
      }

      void restart()
      {
         m_current = m_begin;
      }

      [[nodiscard]] const Source& source() const noexcept
      {
         return this->m_source;
      }

      [[nodiscard]] bool empty() const noexcept
      {
         return m_current == m_end;
      }

      [[nodiscard]] std::size_t size( const std::size_t /*unused*/ = 0 ) const noexcept
      {
         return std::size_t( m_end - m_current );
      }

      [[nodiscard]] T peek_token( const std::size_t offset = 0 ) const noexcept
      {
         return m_current[ offset ];
      }

   private:
      const T* const m_begin;
      const T* m_current;
      const T* const m_end;
      const Source m_source;
   };

   namespace internal
   {
      template< typename Type, Type Value >
      struct token_type
      {
         using rule_t = token_type;
         using subs_t = empty_list;

         template< typename ParseInput >
         static bool match( ParseInput& in )
         {
            if( ( !in.empty() ) && ( in.peek_token().type == Value ) ) {
               in.bump( 1 );
               return true;
            }
            return false;
         }
      };

   }  // namespace internal

   template< auto Value >
   using token_type = internal::token_type< decltype( Value ), Value >;

   template< typename Name, typename Type, Type Value >
   struct analyze_traits< Name, internal::token_type< Type, Value > >
      : analyze_any_traits<>
   {};

}  // namespace TAO_PEGTL_NAMESPACE

using namespace TAO_PEGTL_NAMESPACE;

enum my_type
{
   a,
   b,
   c,
   d,
   e,
   f
};

struct my_token
{
   my_type type;
   std::string data;
};

template< typename Rule >
struct my_action
   : nothing< Rule >
{};

template<>
struct my_action< eof >
{
   static void apply0()
   {
      std::cout << "We have eof!" << std::endl;
   }
};

template<>
struct my_action< token_type< my_type::b > >
{
   template< typename ActionInput >
   static void apply( const ActionInput& /*unused*/ )
   {
      std::cout << "We have a token of type 'b'!" << std::endl;
   }
};

struct my_grammar
   : seq< plus< token_type< my_type::b > >, eof >
{};

int main()
{
   const std::vector< my_token > v{
      { my_type::b, "" },
      { my_type::b, "" }
   };

   token_parse_input< my_token > in( v, __FUNCTION__ );
   if( !parse< my_grammar, my_action >( in ) ) {
      return 1;
   }

   return 0;
}
