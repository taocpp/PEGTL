// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <type_traits>

#include "test.hpp"

#include <tao/pegtl/contrib/alphabet_constants.hpp>
#include <tao/pegtl/contrib/alphabet_rules.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      static_assert( std::is_same_v< alphabet::rules::a::rule_t, one< alphabet::a >::rule_t > );
      static_assert( std::is_same_v< alphabet::rules::b::rule_t, one< alphabet::b >::rule_t > );
      static_assert( std::is_same_v< alphabet::rules::c::rule_t, one< alphabet::c >::rule_t > );
      static_assert( std::is_same_v< alphabet::rules::d::rule_t, one< alphabet::d >::rule_t > );
      static_assert( std::is_same_v< alphabet::rules::e::rule_t, one< alphabet::e >::rule_t > );
      static_assert( std::is_same_v< alphabet::rules::f::rule_t, one< alphabet::f >::rule_t > );
      static_assert( std::is_same_v< alphabet::rules::g::rule_t, one< alphabet::g >::rule_t > );
      static_assert( std::is_same_v< alphabet::rules::h::rule_t, one< alphabet::h >::rule_t > );
      static_assert( std::is_same_v< alphabet::rules::i::rule_t, one< alphabet::i >::rule_t > );
      static_assert( std::is_same_v< alphabet::rules::j::rule_t, one< alphabet::j >::rule_t > );
      static_assert( std::is_same_v< alphabet::rules::k::rule_t, one< alphabet::k >::rule_t > );
      static_assert( std::is_same_v< alphabet::rules::l::rule_t, one< alphabet::l >::rule_t > );
      static_assert( std::is_same_v< alphabet::rules::m::rule_t, one< alphabet::m >::rule_t > );
      static_assert( std::is_same_v< alphabet::rules::n::rule_t, one< alphabet::n >::rule_t > );
      static_assert( std::is_same_v< alphabet::rules::o::rule_t, one< alphabet::o >::rule_t > );
      static_assert( std::is_same_v< alphabet::rules::p::rule_t, one< alphabet::p >::rule_t > );
      static_assert( std::is_same_v< alphabet::rules::q::rule_t, one< alphabet::q >::rule_t > );
      static_assert( std::is_same_v< alphabet::rules::r::rule_t, one< alphabet::r >::rule_t > );
      static_assert( std::is_same_v< alphabet::rules::s::rule_t, one< alphabet::s >::rule_t > );
      static_assert( std::is_same_v< alphabet::rules::t::rule_t, one< alphabet::t >::rule_t > );
      static_assert( std::is_same_v< alphabet::rules::u::rule_t, one< alphabet::u >::rule_t > );
      static_assert( std::is_same_v< alphabet::rules::v::rule_t, one< alphabet::v >::rule_t > );
      static_assert( std::is_same_v< alphabet::rules::w::rule_t, one< alphabet::w >::rule_t > );
      static_assert( std::is_same_v< alphabet::rules::x::rule_t, one< alphabet::x >::rule_t > );
      static_assert( std::is_same_v< alphabet::rules::y::rule_t, one< alphabet::y >::rule_t > );
      static_assert( std::is_same_v< alphabet::rules::z::rule_t, one< alphabet::z >::rule_t > );

      static_assert( std::is_same_v< alphabet::rules::A::rule_t, one< alphabet::A >::rule_t > );
      static_assert( std::is_same_v< alphabet::rules::B::rule_t, one< alphabet::B >::rule_t > );
      static_assert( std::is_same_v< alphabet::rules::C::rule_t, one< alphabet::C >::rule_t > );
      static_assert( std::is_same_v< alphabet::rules::D::rule_t, one< alphabet::D >::rule_t > );
      static_assert( std::is_same_v< alphabet::rules::E::rule_t, one< alphabet::E >::rule_t > );
      static_assert( std::is_same_v< alphabet::rules::F::rule_t, one< alphabet::F >::rule_t > );
      static_assert( std::is_same_v< alphabet::rules::G::rule_t, one< alphabet::G >::rule_t > );
      static_assert( std::is_same_v< alphabet::rules::H::rule_t, one< alphabet::H >::rule_t > );
      static_assert( std::is_same_v< alphabet::rules::I::rule_t, one< alphabet::I >::rule_t > );
      static_assert( std::is_same_v< alphabet::rules::J::rule_t, one< alphabet::J >::rule_t > );
      static_assert( std::is_same_v< alphabet::rules::K::rule_t, one< alphabet::K >::rule_t > );
      static_assert( std::is_same_v< alphabet::rules::L::rule_t, one< alphabet::L >::rule_t > );
      static_assert( std::is_same_v< alphabet::rules::M::rule_t, one< alphabet::M >::rule_t > );
      static_assert( std::is_same_v< alphabet::rules::N::rule_t, one< alphabet::N >::rule_t > );
      static_assert( std::is_same_v< alphabet::rules::O::rule_t, one< alphabet::O >::rule_t > );
      static_assert( std::is_same_v< alphabet::rules::P::rule_t, one< alphabet::P >::rule_t > );
      static_assert( std::is_same_v< alphabet::rules::Q::rule_t, one< alphabet::Q >::rule_t > );
      static_assert( std::is_same_v< alphabet::rules::R::rule_t, one< alphabet::R >::rule_t > );
      static_assert( std::is_same_v< alphabet::rules::S::rule_t, one< alphabet::S >::rule_t > );
      static_assert( std::is_same_v< alphabet::rules::T::rule_t, one< alphabet::T >::rule_t > );
      static_assert( std::is_same_v< alphabet::rules::U::rule_t, one< alphabet::U >::rule_t > );
      static_assert( std::is_same_v< alphabet::rules::V::rule_t, one< alphabet::V >::rule_t > );
      static_assert( std::is_same_v< alphabet::rules::W::rule_t, one< alphabet::W >::rule_t > );
      static_assert( std::is_same_v< alphabet::rules::X::rule_t, one< alphabet::X >::rule_t > );
      static_assert( std::is_same_v< alphabet::rules::Y::rule_t, one< alphabet::Y >::rule_t > );
      static_assert( std::is_same_v< alphabet::rules::Z::rule_t, one< alphabet::Z >::rule_t > );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
