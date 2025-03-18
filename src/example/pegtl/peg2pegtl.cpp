// Copyright (c) 2018-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <algorithm>
#include <exception>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include <cassert>
#include <cctype>
#include <cstdlib>

#if defined( _MSC_VER )
#include <string.h>
#define TAO_PEGTL_STRCASECMP _stricmp
#else
#include <strings.h>
#define TAO_PEGTL_STRCASECMP strcasecmp
#endif

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/parse_tree.hpp>
#include <tao/pegtl/contrib/peg.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   namespace peg
   {
      using node_ptr = std::unique_ptr< parse_tree::node >;

      namespace
      {
         std::string prefix = "TAO_PEGTL_NAMESPACE::";

         std::set< std::string > keywords = {
            "alignas",
            "alignof",
            "and",
            "and_eq",
            "asm",
            "auto",
            "bitand",
            "bitor",
            "bool",
            "break",
            "case",
            "catch",
            "char",
            "char8_t",
            "char16_t",
            "char32_t",
            "class",
            "compl",
            "concept",
            "const",
            "consteval",
            "constexpr",
            "constinit",
            "const_cast",
            "continue",
            "co_await",
            "co_return",
            "co_yield",
            "decltype",
            "default",
            "delete",
            "do",
            "double",
            "dynamic_cast",
            "else",
            "enum",
            "explicit",
            "export",
            "extern",
            "false",
            "float",
            "for",
            "friend",
            "goto",
            "if",
            "inline",
            "int",
            "long",
            "mutable",
            "namespace",
            "new",
            "noexcept",
            "not",
            "not_eq",
            "nullptr",
            "operator",
            "or",
            "or_eq",
            "private",
            "protected",
            "public",
            "register",
            "reinterpret_cast",
            "return",
            "requires",
            "short",
            "signed",
            "sizeof",
            "static",
            "static_assert",
            "static_cast",
            "struct",
            "switch",
            "template",
            "this",
            "thread_local",
            "throw",
            "true",
            "try",
            "typedef",
            "typeid",
            "typename",
            "union",
            "unsigned",
            "using",
            "virtual",
            "void",
            "volatile",
            "wchar_t",
            "while",
            "xor",
            "xor_eq"
         };

         using rules_t = std::vector< std::string >;
         rules_t rules_defined;
         rules_t rules;

         // clang-format off
         struct one_tag {};
         struct string_tag {};
         struct istring_tag {};
         // clang-format on

         rules_t::reverse_iterator find_rule( rules_t& r, const std::string& v, const rules_t::reverse_iterator& rbegin )
         {
            return std::find_if( rbegin, r.rend(), [ & ]( const rules_t::value_type& p ) { return TAO_PEGTL_STRCASECMP( p.c_str(), v.c_str() ) == 0; } );
         }

         rules_t::reverse_iterator find_rule( rules_t& r, const std::string& v )
         {
            return find_rule( r, v, r.rbegin() );
         }

      }  // namespace

#if defined( __cpp_exceptions )
      // Using must_if<> we define a control class which is used for
      // the parsing run instead of the default control class.
      //
      // This improves the errors reported to the user.
      //
      // The following turns local errors into global errors, i.e.
      // if one of the rules for which a custom error message is
      // defined fails, it throws a parse_error exception (aka global
      // failure) instead of returning false (aka local failure).

      // clang-format off
      template< typename > inline constexpr const char* error_message = nullptr;

      template<> inline constexpr auto error_message< Char > = "unterminated character literal";
      template<> inline constexpr auto error_message< Expression > = "unterminated expression";
      template<> inline constexpr auto error_message< Grammar > = "unterminated grammar";
      template<> inline constexpr auto error_message< Range > = "unterminated range";

      // clang-format on

      struct error
      {
         template< typename Rule >
         static constexpr auto message = error_message< Rule >;
      };

      template< typename Rule >
      using control = must_if< error >::control< Rule >;
#else
      template< typename Rule >
      using control = normal< Rule >;
#endif

      // Since we are going to generate a parse tree, we define a
      // selector that decides which rules will be included in our
      // parse tree, which rules will be omitted from the parse tree,
      // and which of the nodes will store the matched content.
      // Additionally, some nodes will fold when they have exactly
      // one child node. (see fold_one below)

      template< typename Rule >
      struct selector
         : pegtl::parse_tree::selector<
              Rule,
              pegtl::parse_tree::store_content::on<
                 Definition,
                 Prefix,
                 Suffix,
                 Sequence,
                 Expression,
                 Class,
                 Literal,
                 Identifier,
                 IdentStart,
                 Range,
                 Char,
                 AND,
                 NOT,
                 QUESTION,
                 STAR,
                 PLUS,
                 DOT >,
              pegtl::parse_tree::fold_one::on< IdentCont > >
      {
         template< typename... States >
         static void transform( node_ptr& n )
         {
            // As we use the PEG grammar taken directly from the original PEG
            // paper, some nodes may have excess content from nodes not included
            // in the parse tree (e.g. Comment, Space, etc).

            if( !n->children.empty() ) {
               n->m_end = n->children.back()->m_end;
            }
         }
      };

      std::string to_string( const node_ptr& n );
      std::string to_string( const std::vector< node_ptr >& v );

      namespace
      {
         std::string get_rulename( const node_ptr& n )
         {
            assert( n->is_type< Identifier >() );
            std::string v = n->string();
            std::replace( v.begin(), v.end(), '-', '_' );
            return v;
         }

         std::string get_rulename( const node_ptr& n, const bool print_forward_declarations )
         {
            std::string v = get_rulename( n );
            const auto it = find_rule( rules, v );
            if( it != rules.rend() ) {
               return *it;
            }
            if( keywords.count( v ) != 0 || v.find( "__" ) != std::string::npos ) {
#if defined( __cpp_exceptions )
               throw parse_error( '\'' + n->string() + "' is a reserved rulename", n->begin() );
#else
               std::cerr << '\'' + n->string() + "' is a reserved rulename" << std::endl;
               std::terminate();
#endif
            }
            if( print_forward_declarations && find_rule( rules_defined, v ) != rules_defined.rend() ) {
               std::cout << "struct " << v << ";\n";
            }
            rules.push_back( v );
            return v;
         }

         struct ccmp
         {
            bool operator()( const std::string& lhs, const std::string& rhs ) const noexcept
            {
               return TAO_PEGTL_STRCASECMP( lhs.c_str(), rhs.c_str() ) < 0;
            }
         };

         std::map< std::string, parse_tree::node*, ccmp > previous_rules;

      }  // namespace

      template<>
      struct selector< Definition >
         : std::true_type
      {
         template< typename... States >
         static void transform( node_ptr& n )
         {
            const auto idname = get_rulename( n->children.front() );
            assert( n->children.back()->is_type< Expression >() );
            if( !previous_rules.try_emplace( idname, n.get() ).second ) {
#if defined( __cpp_exceptions )
               throw parse_error( "identifier '" + idname + "' is already defined", n->begin() );
#else
               std::cerr << "identifier '" + idname + "' is already defined" << std::endl;
               std::terminate();
#endif
            }
         }
      };

      // Finally, the generated parse tree for each node is converted to
      // a C++ source code string.

      struct stringifier
      {
         using function_t = std::string ( * )( const node_ptr& n );
         function_t default_ = nullptr;

         std::map< std::string_view, function_t > map_;

         template< typename T >
         void add( const function_t& f )
         {
            map_.try_emplace( demangle< T >(), f );
         }

         std::string operator()( const node_ptr& n ) const
         {
            const auto it = map_.find( n->type );
            if( it != map_.end() ) {
               return it->second( n );
            }
            return default_( n );
         }
      };

      stringifier make_stringifier()
      {
         stringifier nrv;
         nrv.default_ = []( const node_ptr& n ) -> std::string {
#if defined( __cpp_exceptions )
            throw parse_error( "missing to_string() for " + std::string( n->type ), n->begin() );
#else
            std::cerr << "missing to_string() for " + std::string( n->type ) << std::endl;
            std::terminate();
#endif
         };
         nrv.add< Identifier >( []( const node_ptr& n ) { return get_rulename( n, true ); } );

         nrv.add< Definition >( []( const node_ptr& n ) {
            return "struct " + get_rulename( n->children.front(), false ) + " : " + to_string( n->children.back() ) + " {};";
         } );

         nrv.add< Char >( []( const node_ptr& n ) {
            if (n->string_view() == "\\[") {
               return std::string("'['");
            } else if (n->string_view() == "\\]") {
               return std::string("']'");
            } else if (n->string_view() == "\\") {
               return std::string("'\\'");
            } else if (n->string_view() == "'") {
               return "\'\\" + n->string() + '\'';
            }

            return '\'' + n->string() + '\'';
         } );

         nrv.add< Sequence >( []( const node_ptr& n ) {
            if( n->children.size() == 1 ) {
               return to_string( n->children.front() );
            }

            return prefix + "seq< " + to_string( n->children ) + " >";
         } );

         nrv.add< Expression >( []( const node_ptr& n ) {
            if( n->children.size() == 1 ) {
               return to_string( n->children.front() );
            }

            return prefix + "sor< " + to_string( n->children ) + " >";
         } );

         nrv.add< Range >( []( const node_ptr& n ) {
            if( n->children.size() == 1 ) {
               return prefix + "one< " + to_string( n->children.front() ) + " >";
            }

            return prefix + "range< " + to_string( n->children.front() ) + ", " + to_string( n->children.back() ) + " >";
         } );

         nrv.add< Class >( []( const node_ptr& n ) {
            if( n->children.size() == 1 ) {
               return to_string( n->children.front() );
            }

            return prefix + "sor < " + to_string( n->children ) + " >";
         } );

         nrv.add< Literal >( []( const node_ptr& n ) {
            if( n->children.size() == 1 ) {
               return prefix + "one< " + to_string( n->children.front() ) + " >";
            }

            return prefix + "string< " + to_string( n->children ) + " >";
         } );

         nrv.add< Prefix >( []( const node_ptr& n ) {
            const auto sub = to_string( n->children.back() );

            if( n->children.front()->is_type< AND >() ) {
               return prefix + "at< " + sub + " >";
            }

            if( n->children.front()->is_type< NOT >() ) {
               return prefix + "not_at< " + sub + " >";
            }

            assert( n->children.size() == 1 );
            return sub;
         } );

         nrv.add< Suffix >( []( const node_ptr& n ) {
            const auto sub = to_string( n->children.front() );

            if( n->children.back()->is_type< QUESTION >() ) {
               return prefix + "opt< " + sub + " >";
            }

            if( n->children.back()->is_type< STAR >() ) {
               return prefix + "star< " + sub + " >";
            }

            if( n->children.back()->is_type< PLUS >() ) {
               return prefix + "plus< " + sub + " >";
            }

            assert( n->children.size() == 1 );
            return sub;
         } );

         nrv.add< DOT >( []( const node_ptr& ) {
            return prefix + "any";
         } );

         return nrv;
      }

      std::string to_string( const node_ptr& n )
      {
         static stringifier s = make_stringifier();
         return s( n );
      }

      std::string to_string( const std::vector< node_ptr >& v )
      {
         std::string result;
         for( const auto& c : v ) {
            if( !result.empty() ) {
               result += ", ";
            }
            result += to_string( c );
         }
         return result;
      }

   }  // namespace peg

}  // namespace TAO_PEGTL_NAMESPACE

int main( int argc, char** argv )  // NOLINT(bugprone-exception-escape)
{
   using namespace TAO_PEGTL_NAMESPACE;

   if( argc != 2 ) {
      std::cerr << "Usage: " << argv[ 0 ] << " SOURCE\n";
      return 1;
   }

   if( !std::filesystem::exists( argv[ 1 ] ) ) {
      std::cerr << "Missing source grammar file " << argv[ 1 ] << '\n';
      return 1;
   }

   file_input in( argv[ 1 ] );
#if defined( __cpp_exceptions )
   try {
      const auto root = parse_tree::parse< peg::Grammar, peg::selector, nothing, peg::control >( in );

      for( const auto& rule : root->children ) {
         peg::rules_defined.push_back( peg::get_rulename( rule->children.front() ) );
      }

      for( const auto& rule : root->children ) {
         std::cout << peg::to_string( rule ) << '\n';
      }
   }
   catch( const parse_error& e ) {
      const auto& p = e.position_object();
      std::cerr << e.what() << '\n'
                << in.line_at( p ) << '\n'
                << std::setw( int( p.column ) ) << '^' << '\n';
   }
#else
   if( const auto root = parse_tree::parse< peg::Grammar, peg::selector, nothing, peg::control >( in ) ) {
      for( const auto& rule : root->children ) {
         peg::rules_defined.push_back( peg::get_rulename( rule->children.front() ) );
      }

      for( const auto& rule : root->children ) {
         std::cout << peg::to_string( rule ) << '\n';
      }
   }
   else {
      std::cerr << "error occurred" << std::endl;
      return 1;
   }
#endif

   return 0;
}
