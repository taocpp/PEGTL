// Copyright (c) 2014-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_DATA_AND_SIZE_HPP
#define TAO_PEGTL_INTERNAL_DATA_AND_SIZE_HPP

#include <cstddef>
#include <type_traits>

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Data, typename Size = std::uint8_t >
   class data_and_size
   {
   public:
      static_assert( !std::is_pointer_v< Data > );
      static_assert( !std::is_reference_v< Data > );
      static_assert( !std::is_member_pointer_v< Data > );

      data_and_size() noexcept = default;

      data_and_size( const Data d, const Size s ) noexcept
         : m_data( d ),
           m_size( s )
      {}

      [[nodiscard]] Size size() const noexcept
      {
         return m_size;
      }

      [[nodiscard]] Data data() const noexcept
      {
         return m_data;
      }

      [[nodiscard]] explicit operator bool() const noexcept
      {
         return m_size > 0;
      }

      void mask_with( const Data mask ) noexcept
      {
         m_data &= mask;
      }

   private:
      Data m_data;
      Size m_size = 0;
   };

   template< typename Data >
   class data_and_size< Data, void >
   {
   public:
      static_assert( !std::is_pointer_v< Data > );
      static_assert( !std::is_reference_v< Data > );
      static_assert( !std::is_member_pointer_v< Data > );

      data_and_size() noexcept = default;

      explicit data_and_size( const Data* d ) noexcept
         : m_data( d )
      {}

      [[nodiscard]] static std::size_t size() noexcept
      {
         return 1;
      }

      [[nodiscard]] const Data& data() const noexcept
      {
         return *m_data;
      }

      [[nodiscard]] explicit operator bool() const noexcept
      {
         return m_data != nullptr;
      }

   private:
      const Data* m_data = nullptr;
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
