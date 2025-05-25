// Copyright (c) 2021-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INPUTS_HPP
#define TAO_PEGTL_INPUTS_HPP

#include <string>
#include <type_traits>

#include "ascii.hpp"
#include "config.hpp"
#include "system.hpp"

#include "internal/choose_container.hpp"
#include "internal/inputs.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Eol = void, typename Source = std::string >
   struct argv_input
      : internal::argv_alias< Eol, Source >
   {
      using internal::argv_alias< Eol, Source >::argv_alias;
   };

   argv_input( char**, const int ) -> argv_input<>;

   using tao_default_eol = TAO_PEGTL_DEFAULT_EOL;  // Details might change.

   template< typename Eol = tao_default_eol, typename Data = char >
   struct base_input
      : internal::base_alias< Eol, Data >
   {
      using internal::base_alias< Eol, Data >::base_alias;
   };

   template< typename Data >
   base_input( const Data*, const Data* ) -> base_input< tao_default_eol, Data >;

   template< typename Data >
   base_input( const Data*, const std::size_t ) -> base_input< tao_default_eol, Data >;

   base_input( std::string& ) -> base_input< tao_default_eol, char >;
   base_input( const std::string& ) -> base_input< tao_default_eol, char >;
   base_input( const std::string_view ) -> base_input< tao_default_eol, char >;

   template< typename Data, typename... Params >
   base_input( std::vector< Data, Params... >& ) -> base_input< tao_default_eol, Data >;

   template< typename Data, typename... Params >
   base_input( const std::vector< Data, Params... >& ) -> base_input< tao_default_eol, Data >;

   template< std::size_t Size >
   base_input( const char ( & )[ Size ] ) -> base_input< tao_default_eol, char >;

   template< typename Data, std::size_t Size >
   base_input( const std::array< Data, Size >& ) -> base_input< tao_default_eol, Data >;

   template< typename Eol = tao_default_eol, typename Data = char, typename InputSource = void, typename ErrorSource = InputSource >
   struct view_input
      : internal::view_alias< Eol, Data, InputSource, ErrorSource >
   {
      using internal::view_alias< Eol, Data, InputSource, ErrorSource >::view_alias;
   };

   template< typename Data >
   view_input( const Data*, const Data* ) -> view_input< tao_default_eol, Data, void, void >;

   template< typename Data >
   view_input( const Data*, const std::size_t ) -> view_input< tao_default_eol, Data, void, void >;

   view_input( std::string& ) -> view_input< tao_default_eol, char, void, void >;
   view_input( const std::string& ) -> view_input< tao_default_eol, char, void, void >;
   view_input( const std::string_view ) -> view_input< tao_default_eol, char, void, void >;

   template< typename Data, typename... Params >
   view_input( std::vector< Data, Params... >& ) -> view_input< tao_default_eol, Data, void, void >;

   template< typename Data, typename... Params >
   view_input( const std::vector< Data, Params... >& ) -> view_input< tao_default_eol, Data, void, void >;

   template< std::size_t Size >
   view_input( const char ( & )[ Size ] ) -> view_input< tao_default_eol, char, void, void >;

   template< typename Data, std::size_t Size >
   view_input( const std::array< Data, Size >& ) -> view_input< tao_default_eol, Data, void, void >;

   template< typename Eol = tao_default_eol, typename Container = std::string, typename InputSource = void, typename ErrorSource = InputSource >
   struct copy_input
      : internal::copy_alias< Eol, Container, InputSource, ErrorSource >
   {
      using internal::copy_alias< Eol, Container, InputSource, ErrorSource >::copy_alias;
   };

   template< typename Data >
   copy_input( const Data*, const Data* ) -> copy_input< tao_default_eol, internal::choose_container_t< Data >, void, void >;

   template< typename Data >
   copy_input( const Data*, const std::size_t ) -> copy_input< tao_default_eol, internal::choose_container_t< Data >, void, void >;

   template< typename Container >
   copy_input( Container&& ) -> copy_input< tao_default_eol, std::decay_t< Container >, void, void >;

   template< typename Container >
   copy_input( const Container& ) -> copy_input< tao_default_eol, std::decay_t< Container >, void, void >;

   template< typename Data, std::size_t Size >
   copy_input( const std::array< Data, Size >& ) -> copy_input< tao_default_eol, internal::choose_container_t< Data >, void, void >;

   template< typename Data >
   copy_input( const std::initializer_list< Data >& ) -> copy_input< tao_default_eol, internal::choose_container_t< Data >, void, void >;

   template< typename Eol = tao_default_eol >
   struct file_input
      : internal::file_alias< Eol >
   {
      using internal::file_alias< Eol >::file_alias;
   };

   template< typename Args >
   file_input( Args&& ) -> file_input< tao_default_eol >;

   template< typename Eol = tao_default_eol >
   struct read_input
      : internal::read_alias< Eol >
   {
      using internal::read_alias< Eol >::read_alias;
   };

   template< typename Args >
   read_input( Args&& ) -> read_input< tao_default_eol >;

#if defined( TAO_PEGTL_MMAP_AVAILABLE )
   template< typename Eol = tao_default_eol, typename Data = char >
   struct mmap_input
      : internal::mmap_alias< Eol, Data >
   {
      using internal::mmap_alias< Eol, Data >::mmap_alias;
   };

   template< typename Args >
   mmap_input( Args&& ) -> mmap_input< tao_default_eol, char >;
#endif

   template< typename Eol = tao_default_eol, typename Data = char, typename InputSource = void, typename ErrorSource = InputSource >
   struct text_view_input
      : internal::choose_alias< Eol, internal::lazy_view_alias< Eol, Data, InputSource, ErrorSource >, internal::text_view_alias< Eol, Data, InputSource, ErrorSource > >
   {
      using internal::choose_alias< Eol, internal::lazy_view_alias< Eol, Data, InputSource, ErrorSource >, internal::text_view_alias< Eol, Data, InputSource, ErrorSource > >::choose_alias;
   };

   template< typename Data >
   text_view_input( const Data*, const Data* ) -> text_view_input< tao_default_eol, Data, void, void >;

   template< typename Data >
   text_view_input( const Data*, const std::size_t ) -> text_view_input< tao_default_eol, Data, void, void >;

   text_view_input( std::string& ) -> text_view_input< tao_default_eol, char, void, void >;
   text_view_input( const std::string& ) -> text_view_input< tao_default_eol, char, void, void >;
   text_view_input( const std::string_view ) -> text_view_input< tao_default_eol, char, void, void >;

   template< typename Data, typename... Params >
   text_view_input( std::vector< Data, Params... >& ) -> text_view_input< tao_default_eol, Data, void, void >;

   template< typename Data, typename... Params >
   text_view_input( const std::vector< Data, Params... >& ) -> text_view_input< tao_default_eol, Data, void, void >;

   template< std::size_t Size >
   text_view_input( const char ( & )[ Size ] ) -> text_view_input< tao_default_eol, char, void, void >;

   template< typename Data, std::size_t Size >
   text_view_input( const std::array< Data, Size >& ) -> text_view_input< tao_default_eol, Data, void, void >;

   template< typename Eol = tao_default_eol, typename Container = std::string, typename InputSource = void, typename ErrorSource = InputSource >
   struct text_copy_input
      : internal::choose_alias< Eol, internal::lazy_copy_alias< Eol, Container, InputSource, ErrorSource >, internal::text_copy_alias< Eol, Container, InputSource, ErrorSource > >
   {
      using internal::choose_alias< Eol, internal::lazy_copy_alias< Eol, Container, InputSource, ErrorSource >, internal::text_copy_alias< Eol, Container, InputSource, ErrorSource > >::choose_alias;
   };

   template< typename Data >
   text_copy_input( const Data*, const Data* ) -> text_copy_input< tao_default_eol, internal::choose_container_t< Data >, void, void >;

   template< typename Data >
   text_copy_input( const Data*, const std::size_t ) -> text_copy_input< tao_default_eol, internal::choose_container_t< Data >, void, void >;

   template< typename Container >
   text_copy_input( Container&& ) -> text_copy_input< tao_default_eol, std::decay_t< Container >, void, void >;

   template< typename Container >
   text_copy_input( const Container& ) -> text_copy_input< tao_default_eol, std::decay_t< Container >, void, void >;

   template< typename Data, std::size_t Size >
   text_copy_input( const std::array< Data, Size >& ) -> text_copy_input< tao_default_eol, internal::choose_container_t< Data >, void, void >;

   template< typename Data >
   text_copy_input( const std::initializer_list< Data >& ) -> text_copy_input< tao_default_eol, internal::choose_container_t< Data >, void, void >;

   template< typename Eol = tao_default_eol >
   struct text_file_input
      : internal::choose_alias< Eol, internal::lazy_file_alias< Eol >, internal::text_file_alias< Eol > >
   {
      using internal::choose_alias< Eol, internal::lazy_file_alias< Eol >, internal::text_file_alias< Eol > >::choose_alias;
   };

   template< typename Args >
   text_file_input( Args&& ) -> text_file_input< tao_default_eol >;

   template< typename Eol = tao_default_eol >
   struct text_read_input
      : internal::choose_alias< Eol, internal::lazy_read_alias< Eol >, internal::text_read_alias< Eol > >
   {
      using internal::choose_alias< Eol, internal::lazy_read_alias< Eol >, internal::text_read_alias< Eol > >::choose_alias;
   };

   template< typename Args >
   text_read_input( Args&& ) -> text_read_input< tao_default_eol >;

#if defined( TAO_PEGTL_MMAP_AVAILABLE )
   template< typename Eol = tao_default_eol, typename Data = char >
   struct text_mmap_input
      : internal::choose_alias< Eol, internal::lazy_mmap_alias< Eol, Data >, internal::text_mmap_alias< Eol, Data > >
   {
      using internal::choose_alias< Eol, internal::lazy_mmap_alias< Eol, Data >, internal::text_mmap_alias< Eol, Data > >::choose_alias;
   };

   template< typename Args >
   text_mmap_input( Args&& ) -> text_mmap_input< tao_default_eol >;
#endif

}  // namespace TAO_PEGTL_NAMESPACE

// FCOV_EXCL_FILE
#endif
