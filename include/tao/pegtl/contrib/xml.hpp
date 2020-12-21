// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_CONTRIB_XML_HPP
#define TAO_PEGTL_CONTRIB_XML_HPP

namespace TAO_PEGTL_NAMESPACE::xml10
{
   // https://www.w3.org/TR/2008/REC-xml-20081126/
   // https://www.w3.org/TR/xml-names/

   // [3] S ::= (#x20 | #x9 | #xD | #xA)+
   struct S_plus : plus< one< 0x20, 0x09, 0x0d, 0x0a > >
   {};
   struct S_star : star< one< 0x20, 0x09, 0x0d, 0x0a > >
   {};

   // [2] Char ::= #x9 | #xA | #xD | [#x20-#xD7FF] | [#xE000-#xFFFD] | [#x10000-#x10FFFF]
   struct Char : utf8::predicates< utf8::one< 0x09, 0x0a, 0x0d >,
                                   utf8::range< 0x20, 0xd7ff >,
                                   utf8::range< 0xe000, 0xfffd >,
                                   utf8::range< 0x10000, 0x10ffff > >
   {};

   // [4] NameStartChar ::= ":" | [A-Z] | "_" | [a-z] | [#xC0-#xD6] | [#xD8-#xF6] | [#xF8-#x2FF] | [#x370-#x37D] | [#x37F-#x1FFF] | [#x200C-#x200D] | [#x2070-#x218F] | [#x2C00-#x2FEF] | [#x3001-#xD7FF] | [#xF900-#xFDCF] | [#xFDF0-#xFFFD] | [#x10000-#xEFFFF]
   struct NameStartChar : utf8::predicates < utf8::one< ':', '_' >,
      utf8::range< 'a', 'z' >,
      utf8::range< 'A', 'Z' >,
      utf8::range< 0xc0, 0xd6 >,
      utf8::range< 0xd8, 0xf6 >,
      utf8::range< 0xf8, 0x2ff >,
      utf8::range< 0x370, 0x37d >,
      utf8::range< 0x37f, 0x1fff >,
      utf8::range< 0x200c, 0x200d >,
      utf8::range< 0x2070, 0x218f >,
      utf8::range< 0x2c00, 0x2fef >,
      utf8::range< 0x3001, 0xd7ff >,
      utf8::range< 0xf900, 0xfdcf >,
      utf8::range< 0xfdf0, 0xfffd >,
      utf8::range< 0x10000, 0xeffff >
   {};

   // [4a] NameChar ::= NameStartChar | "-" | "." | [0-9] | #xB7 | [#x0300-#x036F] | [#x203F-#x2040]
   struct NameChar : predicates< NameStartChar,
                                 utf8::one< '-', '.' >,
                                 utf8::range< '0', '9' >,
                                 utf8::range< 0x0300, 0x036f >,
                                 utf8::range< 0x203f, 0x2040 > >
   {};

   // [5] Name ::= NameStartChar (NameChar)*
   struct Name : seq< NameStartChar, plus< NameChar > >
   {};

   // [6] Names ::= Name (#x20 Name)*
   struct Names : list< Name, one< 0x20 > >
   {};  // TODO: list_must?

   // [7] Nmtoken ::= (NameChar)+
   struct Nmtoken : plus< NameChar >
   {};

   // [8] Nmtokens ::= Nmtoken (#x20 Nmtoken)*
   struct Nmtokens : list< Nmtoken, one< 0x20 > >
   {};  // TODO: list_must?

   // [68] EntityRef ::= '&' Name ';'
   struct EntityRef : seq< one< '&' >, Name, one< ';' > >
   {};

   // [69] PEReference ::= '%' Name ';'
   struct PEReference : seq< one< '%' >, Name, one< ';' > >
   {};  // TODO: if_must?

   // [66] CharRef ::= '&#' [0-9]+ ';' | '&#x' [0-9a-fA-F]+ ';'
   struct CharRef : seq< string< '&', '#' >, if_then_else< one< 'x' >, plus< xdigit >, plus< digit > >, one< ';' > >
   {};  // TODO: if_must?

   // [67] Reference ::= EntityRef | CharRef
   struct Reference : sor< EntityRef, CharRef >
   {};

   // TODO: Check for other restrictions between quotation marks for all relevant rules.

   template< char Q, typename... Rs >
   struct quoted1 : seq< one< Q >, until< one< Q >, Rs... > >
   {};  // TODO: if_must?

   template< typename... Rs >
   struct quoted2 : sor< quoted1< '\'', Rs... >, quoted1< '"', Rs... > >
   {};

   // [9] EntityValue ::= '"' ([^%&"] | PEReference | Reference)* '"'
   //                   | "'" ([^%&'] | PEReference | Reference)* "'"
   struct EntityValue : quoted2< sor< PEReference, Reference, not_one< '%', '&' > > >
   {};  // NOTE: The until in quoted eats the trailing quote so it doesn't need to be excluded in the not_one. // TODO: If (PE)Reference use if_must the not_one can be replaced with any.

   // [10] AttValue ::= '"' ([^<&"] | Reference)* '"'
   //                |  "'" ([^<&'] | Reference)* "'"
   struct AttValue : quoted2< sor< Reference, not_one< '<', '&' > > >
   {};  // NOTE: The until in quoted eats the trailing quote so it doesn't need to be excluded in the not_one. // TODO: If Reference uses if_must the '&' can be removed from the not_one.

   // [11] SystemLiteral ::= ('"' [^"]* '"') | ("'" [^']* "'")
   struct SystemLiteral : quoted2<>
   {};

   // [13] PubidChar ::= #x20 | #xD | #xA | [a-zA-Z0-9] | [-'()+,./:=?;!*#@$_%]
   struct PubidChar : predicates< one< 0x20, 0x0d, 0x0a, '-', '\'', '(', ')', '+', ',', '.', '/', ':', '=', '?', ';', '!', '*', '#', '@', '$', '_', '%' >, alnum >
   {};

   // [12] PubidLiteral ::= '"' PubidChar* '"' | "'" (PubidChar - "'")* "'"
   struct PubidLiteral : quoted2< PubidChar >
   {};  // NOTE: Any ' would match the terminating one< '\'' > of the until in quoted.

   // [15] Comment ::= '<!--' ((Char - '-') | ('-' (Char - '-')))* '-->'
   struct Comment : seq< string< '<', '!', '-', '-' >, until< two< '-' > >, one< '>' > >
   {};

   // [17] PITarget ::= Name - (('X' | 'x') ('M' | 'm') ('L' | 'l'))
   struct PITarget : minus< Name, istring< 'x', 'm', 'l' > >
   {};

   // [16] PI ::= '<?' PITarget (S (Char* - (Char* '?>' Char*)))? '?>'
   struct PI : seq< string< '<', '?' >, PITarget, if_then_else< S_plus, until< string< '?', '>' >, Char >, string< '?', '>' > > >
   {};  // TODO: if_must?

   // [19] CDStart ::= '<![CDATA['
   struct CDStart : string< '<', '!', '[', 'C', 'D', 'A', 'T', 'A', '[' >
   {};

   // [21] CDEnd ::= ']]>'
   struct CDEnd : string< ']', ']', '>' >
   {};

   // [20] CData ::= (Char* - (Char* ']]>' Char*))
   // [18] CDSect ::= CDStart CData CDEnd
   struct CDSect : seq< CDStart, until< CDEnd, Char > >
   {};  // TODO: if_must?

   // [26] VersionNum ::= '1.' [0-9]+
   struct VersionNum : seq< string< '1', '.' >, plus< digit > >
   {};

   // [25] Eq ::= S? '=' S?
   struct Eq : seq< S_star, one< '=' >, S_star >
   {};

   // [24] VersionInfo ::= S 'version' Eq ("'" VersionNum "'" | '"' VersionNum '"')
   struct VersionInfoNum : sor< seq< one< '"' >, VersionNum, one< '"' > >,
                                seq< one< '\'' >, VersionNum, one< '\'' > > >
   {};
   struct VersionInfo : seq< S_plus, string< 'v', 'e', 'r', 's', 'i', 'o', 'n' >, Eq, VersionInfoNum >
   {};

   struct choice;
   struct seq_;

   // [48] cp ::= (Name | choice | seq) ('?' | '*' | '+')?
   struct cp : seq< sor< Name, choice, seq_ >, opt< one< '?', '*', '+' > > >
   {};

   // [50] seq ::= '(' S? cp ( S? ',' S? cp )* S? ')'
   struct seq_ : seq< one< '(' >, S_star, cp, star< S_star, one< ',' >, S_star, cp >, S_star, one< ')' > >
   {};

   // [49] choice ::= '(' S? cp ( S? '|' S? cp )+ S? ')'
   struct choice : seq< one< '(' >, S_star, cp, plus< S_star, one< '|' >, S_star, cp >, S_star, one< ')' > >
   {};

   // [47] children ::= (choice | seq) ('?' | '*' | '+')?
   struct children : seq< sor< choice, seq_ >, opt< one< '?', '*', '+' > > >
   {};

   // [51] Mixed ::= '(' S? '#PCDATA' (S? '|' S? Name)* S? ')*'
   //              | '(' S? '#PCDATA' S? ')'
   struct Mixed : seq< one< '(' >, S_star, string< '#', 'P', 'C', 'D', 'A', 'T', 'A' >, S_star, sor< star< one< '|' >, S_star, Name, S_star >, one< ')' > > >
   {};

   // [46] contentspec ::= 'EMPTY' | 'ANY' | Mixed | children
   struct contentspec : sor< string< 'E', 'M', 'P', 'T', 'Y' >, string< 'A', 'N', 'Y' >, Mixed, children >
   {};

   // [45] elementdecl ::= '<!ELEMENT' S Name S contentspec S? '>
   struct elementdecl : seq< string< '<', '!', 'E', 'L', 'E', 'M', 'E', 'N', 'T' >, S_plus, Name, S_plus, contentspec, S_star, one< '>' > >
   {};

   // [29] markupdecl ::= elementdecl | AttlistDecl | EntityDecl | NotationDecl | PI | Comment
   struct markupdecl : sor< elementdecl, AttlistDecl, EntityDecl, NotationDecl, PI, Comment >
   {};

   // [28a] DeclSep ::= PEReference | S
   struct DeclSep : sor< PEReference, S_plus >
   {};

   // [28b] intSubset ::= (markupdecl | DeclSep)*
   struct intSubset : star< sor< markupdecl, DeclSep > >
   {};

   // [28] doctypedecl ::= '<!DOCTYPE' S Name (S ExternalID)? S? ('[' intSubset ']' S?)? '>'
   struct doctypedecl : seq< string< '<', '!', 'D', 'O', 'C', 'T', 'Y', 'P', 'E' >, S_plus, Name, opt< S_plus, ExternalID >, S_star, opt< one< '[' >, intSubset, one< ']' >, S_star >, one< '>' > >
   {};

   // [32] SDDecl ::= S 'standalone' Eq (("'" ('yes' | 'no') "'") | ('"' ('yes' | 'no') '"'))
   struct SDDecl_yes_no : sor< string< '"', 'y', 'e', 's', '"' >, string< '"', 'n', 'o', '"' >, string< '\'', 'y', 'e', 's', '\'' >, string< '\'', 'n', 'o', '\'' > >
   {};
   struct SDDecl : seq< S_plus, string< 's', 't', 'a', 'n', 'd', 'a', 'l', 'o', 'n', 'e' >, Eq, SDDecl_yes_no >
   {};

   // [23] XMLDecl ::= '<?xml' VersionInfo EncodingDecl? SDDecl? S? '?>'
   struct XMLDecl : seq< string< '<', '?', 'x', 'm', 'l' >, VersionInfo, opt< EncodingDecl >, opt< SDDecl >, S_star, string< '?', '>' > >
   {};  // TODO: if_must?

   // [27] Misc ::= Comment | PI | S
   struct Misc : sor< Comment, PI, S_plus >
   {};

   // [22] prolog ::= XMLDecl? Misc* (doctypedecl Misc*)?
   struct prolog : seq< opt< XMLDecl >, star< Misc >, opt< doctypedecl, star< Misc > > >
   {};

   // [31] extSubsetDecl ::= ( markupdecl | conditionalSect | DeclSep)*
   struct extSubsetDecl : star< sor< markupdecl, conditionalSect, DeclSep > >
   {};

   // [30] extSubset ::= TextDecl? extSubsetDecl
   struct extSubset : seq< opt< TextDecl >, extSubsetDecl >
   {};

   // [41] Attribute ::= Name Eq AttValue
   struct Attribute : seq< Name, Eq, AttValue >
   {};

   // [40] STag ::= '<' Name (S Attribute)* S? '>'
   struct STag : seq< one< '<' >, Name, star< S_plus, Attribute >, S_star, one< '>' > >
   {};

   // [42] ETag ::= '</' Name S? '>'
   struct ETag : seq< string< '<', '/' >, Name, S_star, one< '>' > >
   {};

   // [43] content ::= CharData? ((element | Reference | CDSect | PI | Comment) CharData?)*
   struct content : seq< opt< CharData >, star< sor< element, Reference, CDSect, PI, Comment >, opt< CharData > > >
   {};

   // [44] EmptyElemTag ::= '<' Name (S Attribute)* S? '/>'
   struct EmptyElemTag : seq< one< '<' >, Name, star< S_plus, Attribute >, S_star, string< '/', '>' > >
   {};

   // [39] element ::= EmptyElemTag
   //                | STag content ETag
   struct element : sor< EmptyElemTag, seq< STag, content, ETag > >
   {};  // TODO: For a real parser merge EmptyElemTag and STag and disambiguate at the end of them.

   // [14] CharData ::= [^<&]* - ([^<&]* ']]>' [^<&]*)
   struct CharData : until< at< string< ']', ']', '>' > >, not_one< '<', '&' > >
   {};

   // [1] document ::= prolog element Misc*
   struct document : seq< prolog, element, star< Misc > >
   {};

}  // namespace TAO_PEGTL_NAMESPACE::xml10

#endif
