# Temporary File



## Tracer

A fundamental tool for grammar development is a *tracer* that prints every step of a parsing run.
It shows exactly which rule was attempted to match where, and what the result was.
The PEGTL provides a tracer that serves both as practical debug tool and as example of the modular nature of the core library that allows for such facilities to be added.

```c++
#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/trace.hpp>

// This example uses the included JSON grammar
#include <tao/pegtl/contrib/json.hpp>

namespace pegtl = tao::pegtl;

using grammar = pegtl::must< pegtl::json::text, pegtl::eof >;

int main( int argc, char** argv )
{
   if( argc != 2 ) return 1;

   pegtl::argv_input in( argv, 1 );
   pegtl::standard_trace< grammar >( in );

   return 0;
}
```

In the example above, each command line parameter is parsed as a JSON string and a trace is printed to `stderr`.

The full example can be found in `src/pegtl/json_trace.cpp`.
For more information see `tao/pegtl/contrib/trace.hpp`.


## Parse Tree / AST

When developing parsers, a common goal after creating the grammar is to generate a [parse tree](https://en.wikipedia.org/wiki/Parse_tree) or an [AST](https://en.wikipedia.org/wiki/Abstract_syntax_tree).

The PEGTL provides a [Parse Tree](Parse-Tree.md) builder that can filter and/or transform tree nodes on-the-fly.
Additionally, a helper is provided to print out the resulting data structure in [DOT](https://en.wikipedia.org/wiki/DOT_(graph_description_language)) format, suitable for creating a graphical representation of the parse tree.

The following example uses a selector to choose which rules generate parse tree nodes, as the graphical representation will usually be too large and confusing when not using a filter and generating nodes for *all* rules.

```c++
#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/parse_tree.hpp>
#include <tao/pegtl/contrib/parse_tree_to_dot.hpp>

// This example uses the included JSON grammar
#include <tao/pegtl/contrib/json.hpp>

namespace pegtl = tao::pegtl;

using grammar = pegtl::must< pegtl::json::text, pegtl::eof >;

template< typename Rule >
using selector = pegtl::parse_tree::selector<
   Rule,
   pegtl::parse_tree::store_content::on<
      pegtl::json::null,
      pegtl::json::true_,
      pegtl::json::false_,
      pegtl::json::number,
      pegtl::json::string,
      pegtl::json::key,
      pegtl::json::array,
      pegtl::json::object,
      pegtl::json::member > >;

int main( int argc, char** argv )
{
   if( argc != 2 ) return 1;

   pegtl::argv_input in( argv, 1 );
   const auto root = parse_tree::parse< grammar, selector >( in );
   if( root ) {
      parse_tree::print_dot( std::cout, *root );
   }

   return 0;
}
```

Running the above program with some example input:

```sh
$ build/src/pegtl/json_parse_tree '{"foo":[true,{}],"bar":[42,null]}' | dot -Tsvg -o json_parse_tree.svg
```

The above will generate an SVG file with a graphical representation of the parse tree.

![JSON Parse Tree](Json-Parse-Tree.svg)

For more information see [Parse Tree](Parse-Tree.md).









--
This page is part of the [PEGTL](https://github.com/taocpp/PEGTL) and its [documentation](README.md).

Copyright (c) 2014-2026 Dr. Colin Hirsch and Daniel Frey<br>
Distributed under the Boost Software License, Version 1.0<br>
See accompanying file [LICENSE_1_0.txt](../LICENSE_1_0.txt) or copy at https://www.boost.org/LICENSE_1_0.txt
