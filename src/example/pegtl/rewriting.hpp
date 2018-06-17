#ifndef TAO_PEGTL_REWRITING_HPP
#define TAO_PEGTL_REWRITING_HPP

#include <tao/pegtl/contrib/parse_tree.hpp>

namespace tao
{
   namespace pegtl
   {
      namespace parse_tree
      {
         struct c1
         {
         };

         struct cn
         {
         };

         template< typename T, typename... Cs >
         struct t
         {
            bool match( const std::shared_ptr< node >& n, std::vector< std::vector< std::shared_ptr< node > > >& m )
            {
               if( n->is< T >() ) {

                  return true;
               }
               return false;
            }
         };

         std::shared_ptr< node > rewrite( const std::shared_ptr< node >& root, const std::set< rewrite_rule >& rules )
         {
         }
      }
   }
}

#endif
