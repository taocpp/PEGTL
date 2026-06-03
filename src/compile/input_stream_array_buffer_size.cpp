// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <tao/pegtl/stream/inputs.hpp>

namespace pegtl = TAO_PEGTL_NAMESPACE;

#if TAO_PEGTL_COMPILE_ACCEPT
using input = pegtl::array_cstring_input< pegtl::tao_stream_eol, void, void, 8, 4 >;
#else
// include/tao/pegtl/stream/array_buffer.hpp
// static_assert( Chunk < Size );
using input = pegtl::array_cstring_input< pegtl::tao_stream_eol, void, void, 8, 8 >;
#endif

int main()
{
   (void)sizeof( input );
   return 0;
}
