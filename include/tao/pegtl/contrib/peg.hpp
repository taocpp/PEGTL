// Copyright (c) 2021 Daniel Deptford
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_CONTRIB_PEG_HPP
#define TAO_PEGTL_CONTRIB_PEG_HPP

#include <tao/pegtl.hpp>

namespace TAO_PEGTL_NAMESPACE::peg
{
   // PEG grammar from https://pdos.csail.mit.edu/~baford/packrat/popl04/peg-popl04.pdf

   // clang-format off
   struct AND;
   struct Char;
   struct Class;
   struct CLOSE;
   struct Comment;
   struct Definition;
   struct DOT;
   struct EndOfFile;
   struct EndOfLine;
   struct Expression;
   struct QUESTION;
   struct IdentCont;
   struct Identifier;
   struct IdentStart;
   struct LEFTARROW;
   struct Literal;
   struct NOT;
   struct OPEN;
   struct PLUS;
   struct Prefix;
   struct Primary;
   struct Range;
   struct Sequence;
   struct SLASH;
   struct Space;
   struct Spacing;
   struct STAR;
   struct Suffix;

   struct Grammar : tao::pegtl::seq<Spacing, tao::pegtl::plus<Definition>, EndOfFile> {};
   struct Definition : tao::pegtl::seq<Identifier, LEFTARROW, Expression> {};
   struct Expression : tao::pegtl::seq<
     Sequence,
     tao::pegtl::star<
       tao::pegtl::seq<
         SLASH,
         Sequence
       >
     >
   > {};
   struct Sequence : tao::pegtl::star<Prefix> {};
   struct Prefix : tao::pegtl::seq<
     tao::pegtl::opt<
       tao::pegtl::sor<
         AND,
         NOT
       >
     >,
     Suffix
   > {};

   struct Suffix : tao::pegtl::seq<
     Primary,
     tao::pegtl::opt<
       tao::pegtl::sor<
         QUESTION,
         STAR,
         PLUS
       >
     >
   > {};

   struct Primary : tao::pegtl::sor<
     tao::pegtl::seq<Identifier, tao::pegtl::not_at<LEFTARROW> >,
     tao::pegtl::seq<OPEN, Expression, CLOSE>,
     Literal,
     Class,
     DOT> {};

   struct Identifier : tao::pegtl::seq<IdentStart, tao::pegtl::star<IdentCont>, Spacing> {};

   struct IdentStart : tao::pegtl::ranges< 'a', 'z', 'A', 'Z', '_' > {};

   struct IdentCont : tao::pegtl::sor<
     IdentStart,
     tao::pegtl::range<'0','9'>
   > {};

   struct Literal : tao::pegtl::sor<
     tao::pegtl::seq<
       tao::pegtl::one<'\''>,
       tao::pegtl::star<
          tao::pegtl::seq<
            tao::pegtl::not_at<
              tao::pegtl::one<'\''>
            >,
            Char
          >
       >,
       tao::pegtl::one<'\''>,
       Spacing
     >,
     tao::pegtl::seq<
       tao::pegtl::one<'\"'>,
       tao::pegtl::star<
         tao::pegtl::seq<
            tao::pegtl::not_at<tao::pegtl::one<'\"'> >,
            Char
          >
       >,
       tao::pegtl::one<'\"'>,
       Spacing
     >
   > {};

   struct Class : tao::pegtl::seq<
     tao::pegtl::one<'['>,
     tao::pegtl::star<
       tao::pegtl::seq<
         tao::pegtl::not_at<tao::pegtl::one<']'> >,
         Range
       >
     >,
     tao::pegtl::one<']'>,
     Spacing
   > {};

   struct Range : tao::pegtl::sor<
     tao::pegtl::seq<
       Char,
       tao::pegtl::one<'-'>,
       Char>,
     Char
   > {};

   struct Char : tao::pegtl::sor<
     tao::pegtl::seq<
       tao::pegtl::one<'\\'>,
       tao::pegtl::one<'n','r','t','\'','\"','[',']','\\'> >,
     tao::pegtl::seq<
       tao::pegtl::one<'\\'>,
       tao::pegtl::range<'0','2'>,
       tao::pegtl::range<'0','7'>,
       tao::pegtl::range<'0','7'> >,
     tao::pegtl::seq<
       tao::pegtl::one<'\\'>,
       tao::pegtl::range<'0','7'>,
       tao::pegtl::opt<tao::pegtl::range<'0','7'> > >,
     tao::pegtl::seq<
       tao::pegtl::not_at<tao::pegtl::one<'\\'> >,
       tao::pegtl::any>
     > {};

   struct LEFTARROW : tao::pegtl::seq<tao::pegtl::string<'<','-'>, Spacing> {};
   struct SLASH : tao::pegtl::seq<tao::pegtl::one<'/'>, Spacing> {};
   struct AND : tao::pegtl::seq<tao::pegtl::one<'&'>, Spacing> {};
   struct NOT : tao::pegtl::seq<tao::pegtl::one<'!'>, Spacing> {};
   struct QUESTION : tao::pegtl::seq<tao::pegtl::one<'?'>, Spacing> {};
   struct STAR : tao::pegtl::seq<tao::pegtl::one<'*'>, Spacing> {};
   struct PLUS : tao::pegtl::seq<tao::pegtl::one<'+'>, Spacing> {};
   struct OPEN : tao::pegtl::seq<tao::pegtl::one<'('>, Spacing> {};
   struct CLOSE : tao::pegtl::seq<tao::pegtl::one<')'>, Spacing> {};
   struct DOT : tao::pegtl::seq<tao::pegtl::one<'.'>, Spacing> {};

   struct Spacing : tao::pegtl::star<tao::pegtl::sor<Space, Comment> > {};
   struct Comment :
   tao::pegtl::seq<
     tao::pegtl::one<'#'>,
     tao::pegtl::star<
       tao::pegtl::seq<
         tao::pegtl::not_at<EndOfLine>,
         tao::pegtl::any
       >
     >,
     EndOfLine
   > {};

   struct Space : tao::pegtl::sor<tao::pegtl::one<' '>, tao::pegtl::one<'\t'>, EndOfLine> {};
   struct EndOfLine : tao::pegtl::sor<tao::pegtl::string<'\r','\n'>, tao::pegtl::one<'\n'>, tao::pegtl::one<'\r'> > {};
   struct EndOfFile : tao::pegtl::eof {};
   // clang-format on

}  // namespace TAO_PEGTL_NAMESPACE::peg

#endif
