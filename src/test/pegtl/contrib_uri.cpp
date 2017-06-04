// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <cassert>

#include <tao/pegtl.hpp>
#include <tao/pegtl/analyze.hpp>
#include <tao/pegtl/contrib/uri.hpp>

using namespace tao::TAOCPP_PEGTL_NAMESPACE;
using GRAMMAR = must< uri::URI, eof >;

void test( const std::string& s )
{
   memory_input<> in( s, "expect success" );
   parse< GRAMMAR >( in );
}

void fail( const std::string& s )
{
   memory_input<> in( s, "expect exception" );
   try {
      parse< GRAMMAR >( in );
      std::cerr << "FAILED: " << s << std::endl;
      assert( false );
   }
   catch( ... ) {
   }
}

int main( int /*unused*/, char** /*unused*/ )
{
   // ensure the grammar does not contain any obvious problems
   analyze< GRAMMAR >();

   // some unit tests that should work
   test( "http://de.wikipedia.org/wiki/Uniform_Resource_Identifier" );
   test( "ftp://ftp.is.co.za/rfc/rfc1808.txt" );
   test( "file:///C:/Users/Benutzer/Desktop/Uniform%20Resource%20Identifier.html" );
   test( "file:///etc/fstab" );
   test( "geo:48.33,14.122;u=22.5" );
   test( "ldap://[2001:db8::7]/c=GB?objectClass?one" );
   test( "gopher://gopher.floodgap.com" );
   test( "mailto:John.Doe@example.com" );
   test( "sip:911@pbx.mycompany.com" );
   test( "news:comp.infosystems.www.servers.unix" );
   test( "data:text/plain;charset=iso-8859-7,%be%fa%be" );
   test( "tel:+1-816-555-1212" );
   test( "telnet://192.0.2.16:80/" );
   test( "urn:oasis:names:specification:docbook:dtd:xml:4.1.2" );
   test( "git://github.com/rails/rails.git" );
   test( "crid://broadcaster.com/movies/BestActionMovieEver" );
   test( "http://nobody:password@example.org:8080/cgi-bin/script.php?action=submit&pageid=86392001#section_2" );

   // some unit tests that should fail
   fail( "" );

   return 0;
}
