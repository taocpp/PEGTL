// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <algorithm>
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
#include <tao/pegtl/analyze.hpp>
#include <tao/pegtl/contrib/abnf.hpp>
#include <tao/pegtl/contrib/parse_tree.hpp>

namespace tao
{
   namespace TAO_PEGTL_NAMESPACE
   {
      namespace abnf
      {
         using node_ptr = std::unique_ptr< parse_tree::node >;

         namespace
         {
            std::string prefix = "tao::pegtl::";  // NOLINT

            std::set< std::string > keywords = {  // NOLINT
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
               "char16_t",
               "char32_t",
               "class",
               "compl",
               "const",
               "constexpr",
               "const_cast",
               "continue",
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
            rules_t rules_defined;  // NOLINT
            rules_t rules;          // NOLINT

            // clang-format off
            struct one_tag {};
            struct string_tag {};
            struct istring_tag {};
            // clang-format on

            rules_t::reverse_iterator find_rule( rules_t& r, const std::string& v, const rules_t::reverse_iterator& rbegin )
            {
               return std::find_if( rbegin, r.rend(), [&]( const rules_t::value_type& p ) { return TAO_PEGTL_STRCASECMP( p.c_str(), v.c_str() ) == 0; } );
            }

            rules_t::reverse_iterator find_rule( rules_t& r, const std::string& v )
            {
               return find_rule( r, v, r.rbegin() );
            }

            bool append_char( std::string& s, const char c )
            {
               if( !s.empty() ) {
                  s += ", ";
               }
               s += '\'';
               if( c == '\'' || c == '\\' ) {
                  s += '\\';
               }
               s += c;
               s += '\'';
               return std::isalpha( c ) != 0;
            }

            std::string remove_leading_zeroes( const std::string& v )
            {
               const auto pos = v.find_first_not_of( '0' );
               if( pos == std::string::npos ) {
                  return "";
               }
               return v.substr( pos );
            }

            void shift( internal::iterator& it, int delta )
            {
               it.data += delta;
               it.byte += delta;
               it.byte_in_line += delta;
            }

         }  // namespace

         namespace grammar
         {
            // ABNF grammar according to RFC 5234, updated by RFC 7405, with
            // the following differences:
            //
            // To form a C++ identifier from a rulename, all minuses are
            // replaced with underscores.
            //
            // As C++ identifiers are case-sensitive, we remember the "correct"
            // spelling from the first occurrence of a rulename, all other
            // occurrences are automatically changed to that.
            //
            // Certain rulenames are reserved as their equivalent C++ identifier is
            // reserved as a keyword, an alternative token, by the standard or
            // for other, special reasons.
            //
            // When using numerical values (num-val, repeat), the values
            // must be in the range of the corresponsing C++ data type.
            //
            // Remember we are defining a PEG, not a CFG. Simply copying some
            // ABNF from somewhere might lead to surprising results as the
            // alternations are now sequential, using the sor<> rule.
            //
            // PEG also require two extensions: the and-predicate and the
            // not-predicate. They are expressed by '&' and '!' respectively,
            // being allowed (optionally, only one of them) before the
            // repetition. You can use braces for more complex expressions.
            //
            // Finally, instead of the pre-defined CRLF sequence, we accept
            // any type of line ending as a convencience extension:

            // clang-format off
            struct CRLF : sor< abnf::CRLF, CR, LF > {};

            // The rest is according to the RFC(s):
            struct comment_cont : until< CRLF, sor< WSP, VCHAR > > {};
            struct comment : if_must< one< ';' >, comment_cont > {};
            struct c_nl : sor< comment, CRLF > {};
            struct c_wsp : sor< WSP, seq< c_nl, WSP > > {};

            struct rulename : seq< ALPHA, star< ranges< 'a', 'z', 'A', 'Z', '0', '9', '-' > > > {};

            struct quoted_string_cont : until< DQUOTE, print > {};
            struct quoted_string : if_must< DQUOTE, quoted_string_cont > {};
            struct case_insensitive_string : seq< opt< istring< '%', 'i' > >, quoted_string > {};
            struct case_sensitive_string : seq< istring< '%', 's' >, quoted_string > {};
            struct char_val : sor< case_insensitive_string, case_sensitive_string > {};

            struct prose_val_cont : until< one< '>' >, print > {};
            struct prose_val : if_must< one< '<' >, prose_val_cont > {};

            template< char First, typename Digit >
            struct gen_val
            {
               struct value : plus< Digit > {};
               struct range : if_must< one< '-' >, value > {};
               struct next_value : must< value > {};
               struct type : seq< istring< First >, must< value >, sor< range, star< one< '.' >, next_value > > > {};
            };

            using hex_val = gen_val< 'x', HEXDIG >;
            using dec_val = gen_val< 'd', DIGIT >;
            using bin_val = gen_val< 'b', BIT >;

            struct num_val_choice : sor< bin_val::type, dec_val::type, hex_val::type > {};
            struct num_val : if_must< one< '%' >, num_val_choice > {};

            struct alternation;
            struct option_close : one< ']' > {};
            struct option : seq< one< '[' >, pad< must< alternation >, c_wsp >, must< option_close > > {};
            struct group_close : one< ')' > {};
            struct group : seq< one< '(' >, pad< must< alternation >, c_wsp >, must< group_close > > {};
            struct element : sor< rulename, group, option, char_val, num_val, prose_val > {};

            struct repeat : sor< seq< star< DIGIT >, one< '*' >, star< DIGIT > >, plus< DIGIT > > {};
            struct repetition : seq< opt< repeat >, element > {};

            struct and_predicate : if_must< one< '&' >, repetition > {};
            struct not_predicate : if_must< one< '!' >, repetition > {};
            struct predicate : sor< and_predicate, not_predicate, repetition > {};

            struct concatenation : list< predicate, plus< c_wsp > > {};
            struct alternation : list_must< concatenation, pad< one< '/' >, c_wsp > > {};

            struct defined_as_op : sor< string< '=', '/' >, one< '=' > > {};
            struct defined_as : pad< defined_as_op, c_wsp > {};
            struct rule : seq< if_must< rulename, defined_as, alternation >, star< c_wsp >, must< c_nl > > {};
            struct rulelist : until< eof, sor< seq< star< c_wsp >, c_nl >, must< rule > > > {};

            // end of grammar

            template< typename Rule >
            struct error_control : normal< Rule >
            {
               static const std::string error_message;

               template< typename Input, typename... States >
               static void raise( const Input& in, States&&... /*unused*/ )
               {
                  throw parse_error( error_message, in );
               }
            };

            template<> const std::string error_control< comment_cont >::error_message = "unterminated comment";  // NOLINT

            template<> const std::string error_control< quoted_string_cont >::error_message = "unterminated string (missing '\"')";  // NOLINT
            template<> const std::string error_control< prose_val_cont >::error_message = "unterminated prose description (missing '>')";  // NOLINT

            template<> const std::string error_control< hex_val::value >::error_message = "expected hexadecimal value";  // NOLINT
            template<> const std::string error_control< dec_val::value >::error_message = "expected decimal value";  // NOLINT
            template<> const std::string error_control< bin_val::value >::error_message = "expected binary value";  // NOLINT
            template<> const std::string error_control< num_val_choice >::error_message = "expected base specifier (one of 'bBdDxX')";  // NOLINT

            template<> const std::string error_control< option_close >::error_message = "unterminated option (missing ']')";  // NOLINT
            template<> const std::string error_control< group_close >::error_message = "unterminated group (missing ')')";  // NOLINT

            template<> const std::string error_control< repetition >::error_message = "expected element";  // NOLINT
            template<> const std::string error_control< concatenation >::error_message = "expected element";  // NOLINT
            template<> const std::string error_control< alternation >::error_message = "expected element";  // NOLINT

            template<> const std::string error_control< defined_as >::error_message = "expected '=' or '=/'";  // NOLINT
            template<> const std::string error_control< c_nl >::error_message = "unterminated rule";  // NOLINT
            template<> const std::string error_control< rule >::error_message = "expected rule";  // NOLINT
            // clang-format on

         }  // namespace grammar

         template< typename Rule >
         struct selector
            : parse_tree::selector<
                 Rule,
                 parse_tree::apply_store_content::to<
                    grammar::rulename,
                    grammar::prose_val,
                    grammar::hex_val::value,
                    grammar::dec_val::value,
                    grammar::bin_val::value,
                    grammar::hex_val::range,
                    grammar::dec_val::range,
                    grammar::bin_val::range,
                    grammar::hex_val::type,
                    grammar::dec_val::type,
                    grammar::bin_val::type,
                    grammar::repeat,
                    grammar::defined_as_op >,
                 parse_tree::apply_remove_content::to<
                    grammar::option,
                    grammar::and_predicate,
                    grammar::not_predicate,
                    grammar::rule >,
                 parse_tree::apply_fold_one::to<
                    grammar::alternation,
                    grammar::group,
                    grammar::repetition,
                    grammar::concatenation > >
         {
         };

         template<>
         struct selector< grammar::quoted_string > : std::true_type
         {
            static void transform( node_ptr& n )
            {
               shift( n->m_begin, 1 );
               shift( n->m_end, -1 );

               const std::string content = n->content();
               for( const auto c : content ) {
                  if( std::isalpha( c ) != 0 ) {
                     n->id = &typeid( istring_tag );
                     return;
                  }
               }
               if( content.size() == 1 ) {
                  n->id = &typeid( one_tag );
               }
               else {
                  n->id = &typeid( string_tag );
               }
            }
         };

         template<>
         struct selector< grammar::case_sensitive_string > : std::true_type
         {
            static void transform( node_ptr& n )
            {
               n = std::move( n->children.back() );
               if( n->content().size() == 1 ) {
                  n->id = &typeid( one_tag );
               }
               else {
                  n->id = &typeid( string_tag );
               }
            }
         };

         std::string to_string( const node_ptr& n );
         std::string to_string( const std::vector< node_ptr >& v );

         std::string to_string_unwrap_seq( const node_ptr& n )
         {
            if( n->is< grammar::group >() || n->is< grammar::concatenation >() ) {
               return to_string( n->children );
            }
            return to_string( n );
         }

         namespace
         {
            std::string get_rulename( const node_ptr& n )
            {
               assert( n->is< grammar::rulename >() );
               std::string v = n->content();
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
                  throw parse_error( '\'' + n->content() + "' is a reserved rulename", n->begin() );  // NOLINT
               }
               if( print_forward_declarations && find_rule( rules_defined, v ) != rules_defined.rend() ) {
                  std::cout << "struct " << v << ";\n";
               }
               rules.push_back( v );
               return v;
            }

            template< typename T >
            std::string gen_val( const node_ptr& n )
            {
               if( n->children.size() == 2 ) {
                  if( n->children.back()->is< T >() ) {
                     return prefix + "range< " + to_string( n->children.front() ) + ", " + to_string( n->children.back()->children.front() ) + " >";
                  }
               }
               if( n->children.size() == 1 ) {
                  return prefix + "one< " + to_string( n->children ) + " >";
               }
               return prefix + "string< " + to_string( n->children ) + " >";
            }

            struct ccmp
            {
               bool operator()( const std::string& lhs, const std::string& rhs ) const noexcept
               {
                  return TAO_PEGTL_STRCASECMP( lhs.c_str(), rhs.c_str() ) < 0;
               }
            };

            std::map< std::string, parse_tree::node*, ccmp > previous_rules;  // NOLINT

         }  // namespace

         template<>
         struct selector< grammar::rule > : std::true_type
         {
            static void transform( node_ptr& n )
            {
               const auto rname = get_rulename( n->children.front() );
               assert( n->children.at( 1 )->is< grammar::defined_as_op >() );
               const auto op = n->children.at( 1 )->content();
               // when we insert a normal rule, we need to check for duplicates
               if( op == "=" ) {
                  if( !previous_rules.insert( { rname, n.get() } ).second ) {
                     throw parse_error( "rule '" + rname + "' is already defined", n->begin() );  // NOLINT
                  }
               }
               // if it is an "incremental alternation", we need to consolidate the assigned alternations
               else if( op == "=/" ) {
                  const auto p = previous_rules.find( rname );
                  if( p == previous_rules.end() ) {
                     throw parse_error( "incremental alternation '" + rname + "' without previous rule definition", n->begin() );  // NOLINT
                  }
                  auto& previous = p->second->children.back();

                  // if the previous rule does not assign an alternation, create an intermediate alternation and move its assignee into it.
                  if( !previous->is< abnf::grammar::alternation >() ) {
                     node_ptr s( new parse_tree::node );
                     s->id = &typeid( abnf::grammar::alternation );
                     s->source = previous->source;
                     s->m_begin = previous->m_begin;
                     s->m_end = previous->m_end;
                     s->children.emplace_back( std::move( previous ) );
                     previous = std::move( s );
                  }

                  // append all new options to the previous rule's assignee (which always is an alternation now)
                  previous->m_end = n->children.back()->m_end;

                  // if the new rule itself contains an alternation, append the individual entries...
                  if( n->children.back()->is< abnf::grammar::alternation >() ) {
                     for( auto& e : n->children.back()->children ) {
                        previous->children.emplace_back( std::move( e ) );
                     }
                  }
                  // ...otherwise add the node itself as another option.
                  else {
                     previous->children.emplace_back( std::move( n->children.back() ) );
                  }
                  n.reset();
               }
               else {
                  throw parse_error( "invalid operator '" + op + "', this should not happen!", n->begin() );  // NOLINT
               }
            }
         };

         struct stringifier
         {
            using function_t = std::string ( * )( const node_ptr& n );
            function_t default_ = nullptr;

            std::map< const std::type_info*, function_t > map_;

            template< typename T >
            void add( const function_t& f )
            {
               map_.insert( { &typeid( T ), f } );
            }

            std::string operator()( const node_ptr& n ) const
            {
               const auto it = map_.find( n->id );
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
               throw parse_error( "missing to_string() for " + n->name(), n->begin() );  // NOLINT
            };

            nrv.add< grammar::rulename >( []( const node_ptr& n ) { return get_rulename( n, true ); } );

            nrv.add< grammar::rule >( []( const node_ptr& n ) {
               return "struct " + get_rulename( n->children.front(), false ) + " : " + to_string( n->children.back() ) + " {};";
            } );

            nrv.add< string_tag >( []( const node_ptr& n ) {
               const std::string content = n->content();
               std::string s;
               for( const auto c : content ) {
                  append_char( s, c );
               }
               return prefix + "string< " + s + " >";
            } );

            nrv.add< istring_tag >( []( const node_ptr& n ) {
               const std::string content = n->content();
               std::string s;
               for( const auto c : content ) {
                  append_char( s, c );
               }
               return prefix + "istring< " + s + " >";
            } );

            nrv.add< one_tag >( []( const node_ptr& n ) {
               const std::string content = n->content();
               std::string s;
               for( const auto c : content ) {
                  append_char( s, c );
               }
               return prefix + "one< " + s + " >";
            } );

            nrv.add< grammar::hex_val::value >( []( const node_ptr& n ) { return "0x" + n->content(); } );
            nrv.add< grammar::dec_val::value >( []( const node_ptr& n ) { return n->content(); } );
            nrv.add< grammar::bin_val::value >( []( const node_ptr& n ) {
               unsigned long long v = 0;
               const char* p = n->m_begin.data;
               // TODO: Detect overflow
               do {
                  v <<= 1;
                  v |= ( *p++ & 1 );
               } while( p != n->m_end.data );
               std::ostringstream o;
               o << v;
               return o.str();
            } );

            nrv.add< grammar::hex_val::type >( []( const node_ptr& n ) { return gen_val< grammar::hex_val::range >( n ); } );
            nrv.add< grammar::dec_val::type >( []( const node_ptr& n ) { return gen_val< grammar::dec_val::range >( n ); } );
            nrv.add< grammar::bin_val::type >( []( const node_ptr& n ) { return gen_val< grammar::bin_val::range >( n ); } );

            nrv.add< grammar::alternation >( []( const node_ptr& n ) { return prefix + "sor< " + to_string( n->children ) + " >"; } );
            nrv.add< grammar::option >( []( const node_ptr& n ) { return prefix + "opt< " + to_string( n->children ) + " >"; } );
            nrv.add< grammar::group >( []( const node_ptr& n ) { return prefix + "seq< " + to_string( n->children ) + " >"; } );

            nrv.add< grammar::prose_val >( []( const node_ptr& n ) { return "/* " + n->content() + " */"; } );

            nrv.add< grammar::and_predicate >( []( const node_ptr& n ) {
               assert( n->children.size() == 1 );
               return prefix + "at< " + to_string_unwrap_seq( n->children.front() ) + " >";
            } );

            nrv.add< grammar::not_predicate >( []( const node_ptr& n ) {
               assert( n->children.size() == 1 );
               return prefix + "not_at< " + to_string_unwrap_seq( n->children.front() ) + " >";
            } );

            nrv.add< grammar::concatenation >( []( const node_ptr& n ) {
               assert( !n->children.empty() );
               return prefix + "seq< " + to_string( n->children ) + " >";
            } );

            nrv.add< grammar::repetition >( []( const node_ptr& n ) -> std::string {
               assert( n->children.size() == 2 );
               const auto content = to_string_unwrap_seq( n->children.back() );
               const auto rep = n->children.front()->content();
               const auto star = rep.find( '*' );
               if( star == std::string::npos ) {
                  const auto v = remove_leading_zeroes( rep );
                  if( v.empty() ) {
                     throw parse_error( "repetition of zero not allowed", n->begin() );  // NOLINT
                  }
                  return prefix + "rep< " + v + ", " + content + " >";
               }
               const auto min = remove_leading_zeroes( rep.substr( 0, star ) );
               const auto max = remove_leading_zeroes( rep.substr( star + 1 ) );
               if( ( star != rep.size() - 1 ) && max.empty() ) {
                  throw parse_error( "repetition maximum of zero not allowed", n->begin() );  // NOLINT
               }
               if( min.empty() && max.empty() ) {
                  return prefix + "star< " + content + " >";
               }
               if( !min.empty() && max.empty() ) {
                  if( min == "1" ) {
                     return prefix + "plus< " + content + " >";
                  }
                  return prefix + "rep_min< " + min + ", " + content + " >";
               }
               if( min.empty() && !max.empty() ) {
                  if( max == "1" ) {
                     return prefix + "opt< " + content + " >";
                  }
                  return prefix + "rep_max< " + max + ", " + content + " >";
               }
               unsigned long long min_val;
               unsigned long long max_val;
               {
                  std::stringstream s;
                  s.str( min );
                  s >> min_val;
                  s.clear();
                  s.str( max );
                  s >> max_val;
               }
               if( min_val > max_val ) {
                  throw parse_error( "repetition minimum which is greater than the repetition maximum not allowed", n->begin() );  // NOLINT
               }
               if( ( min_val == 1 ) && ( max_val == 1 ) ) {
                  // note: content can not be used here!
                  return to_string( n->children.back() );
               }
               const auto min_element = ( min_val == 1 ) ? content : ( prefix + "rep< " + min + ", " + content + " >" );
               if( min_val == max_val ) {
                  return min_element;
               }
               std::ostringstream os;
               os << ( max_val - min_val );
               const auto max_element = prefix + ( ( max_val - min_val == 1 ) ? "opt< " : ( "rep_opt< " + os.str() + ", " ) ) + content + " >";
               return prefix + "seq< " + min_element + ", " + max_element + " >";
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

      }  // namespace abnf

   }  // namespace TAO_PEGTL_NAMESPACE

}  // namespace tao

int main( int argc, char** argv )
{
   using namespace tao::TAO_PEGTL_NAMESPACE;  // NOLINT

   if( argc != 2 ) {
      analyze< abnf::grammar::rulelist >();
      std::cerr << "Usage: " << argv[ 0 ] << " SOURCE" << std::endl;
      return 1;
   }

   file_input<> in( argv[ 1 ] );
   try {
      const auto root = parse_tree::parse< abnf::grammar::rulelist, abnf::selector, nothing, abnf::grammar::error_control >( in );

      for( const auto& rule : root->children ) {
         abnf::rules_defined.push_back( abnf::get_rulename( rule->children.front() ) );
      }

      for( const auto& rule : root->children ) {
         std::cout << abnf::to_string( rule ) << std::endl;
      }
   }
   catch( const parse_error& e ) {
      const auto p = e.positions.front();
      std::cerr << e.what() << std::endl
                << in.line_as_string( p ) << std::endl
                << std::string( p.byte_in_line, ' ' ) << '^' << std::endl;
   }

   return 0;
}
