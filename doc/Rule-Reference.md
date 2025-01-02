# Rule Reference

The reference documentation for all rules and combinators.

For how to implement your own rules and combinators see ... TODO.

For additional and experimental rules and combinators see ... TODO.

#### Namespaces

All rules reside in namespace `tao::pegtl` or a sub-namespace of `tao::pegtl`.
This default can be changed via the macro `TAO_PEGTL_NAMESPACE` in `tao/pegtl/config.hpp`.

TODO: Consistent namespace usage in equivalent rules.

#### Equivalence

Some rules are documented as being *equivalent* to a combination of other rules.
This equivalence is with respect to which inputs are matched, but is **not** (necessarily) how the rule is implemented.

For rules other than `must<>` that contain "must" in their name, rule equivalence shows which rule will be used to call the control class' `raise()` function when certain sub-rules fail to match.

#### Implementation

The "meta data and implementation mapping" section of each rule's description shows both how the rule is implemented and what the [meta data](Meta-Data-and-Visit.md) looks like.
When the list of sub-rules is empty then the definition of `subs_t` is omitted from the description.

Remember that the default [control](Control-and-Debug.md) `tao::pegtl::normal` does **not** call control functions for rules in the `tao::pegtl::internal` namespace.

#### Parameter Packs

The documentation will use [(template parameter) packs](https://en.cppreference.com/w/cpp/language/parameter_pack) when zero-or-more or one-or-more of a (template) parameter are allowed.
For example `seq< R... >` accepts zero-or-more template parameters.
In the zero case, i.e. `seq<>`, we also say `R` is "empty", otherwise `R` is "non-empty".

#### End Of Line Rules

Rules that can be used for end-of-line scanning mode and/or lazy end-of-line tracking are documented as being "also available in the `scan` and/or `lazy` sub-namespace".
The different end-of-line modes that can be chosen for an input are documented in [TODO].


## Contents

* [Atomic](#atomic)
* [ASCII](#ascii)
* [Unicode](#unicode)
* [Binary](#binary)
* [Member](#member)
* [Combinators](#combinators)
* [Convenience](#convenience)
* [Exceptional](#exceptional)
* [Controlling](#controlling)
* [Incremental](#incremental)
* [Compatibility](#compatibility)
* [ICU Support](#icu-support)
  * [Basic ICU Rules](#basic-icu-rules)
  * [ICU Rules for Binary Properties](#icu-rules-for-binary-properties)
  * [ICU Rules for Enumerated Properties](#icu-rules-for-enumerated-properties)
  * [ICU Rules for Value Properties](#icu-rules-for-value-properties)
* [Index](#index)


## Atomic

Atomic rules do not rely on other rules (but might have non-rule template parameters).

These rules are in namespace `tao::pegtl`.

###### `bof`

* Succeeds at "beginning-of-file", i.e. when the input is at its start.
* Does not consume input.
* Requires an input `in` with an `in.start()` member function, and/or
* requires an input `in` where `in.direct_position().count` is available.
* [Meta data] and [implementation] mapping:
  - `bof::rule_t` is `internal::bof`

###### `bol`

* Succeeds at "beginning-of-line", i.e. when the input's `column()` member function returns `1`.
* Does not consume input.
* Requires an input with eager text position tracking, more precisely:
* Requires an input `in` where `in.direct_position().column` is available.
* [Meta data] and [implementation] mapping:
  - `bol::rule_t` is `internal::bol`

###### `consume< Num >`

* Succeeds if the input contains at least `Num` further objects, and
* unconditionally consumes `Num` objects from the input.
* Limited to the buffer size when using an [Incremental Input].
* [Meta data] and [implementation] mapping:
  - `consume< 0 >::rule_t` is `internal::success`
  - `consume< N >::rule_t` is `internal::consume< N >`

###### `eof`

* Succeeds at "end-of-file", i.e. when the input is empty (all input has been consumed).
* Does not consume input.
* [Meta data] and [implementation] mapping:
  - `eof::rule_t` is `internal::eof`

###### `eol`

* Matches a single end-of-line as defined by the input.
* The default definition of what constitutes a lines ending is system dependent.
  - On Unix-like systems a Unix line ending is matched, a single 'LF'.
  - On Windows the MS-DOS line ending 'CR LF' is also accepted.
* Requires an input with an `eol_rule` type definition.
* [Meta data] and [implementation] mapping:
  - `eol::rule_t` is `internal::eol`

Note that the default behaviour can be changed either by defining `TAO_PEGTL_DEFAULT_EOL` before `tao/pegtl/system.hpp` is (indirectly) included or by supplying an end-of-line rule as template parameter to the input.

###### `eolf`

* Matches a single end-of-line or the end-of-file.
* Requires an input with an `eol_rule` type definition.
* [Equivalent] to `sor< eof, eol >`.
* [Meta data] and [implementation] mapping:
  - `eolf::rule_t` is `internal::eolf`

###### `everything`

* Matches and consumes the entire input.
* Limited by the buffer size when using an [Incremental Input].
* [Equivalent] to `until< eof, any >`.
* [Meta data] and [implementation] mapping:
  - `everything::rule_t` is `internal::everything`

###### `failure`

* Rule that never succeeds.
* Does not consume input.
* [Meta data] and [implementation] mapping:
  - `failure::rule_t` is `internal::failure`

###### `function< F >`

* Delegates matching to the function `F`.
* For details see `tao/pegtl/rules.hpp` and `tao/pegtl/internal/function.hpp`.

###### `success`

* Rule that always succeeds.
* Does not consume input.
* [Meta data] and [implementation] mapping:
  - `success::rule_t` is `internal::success`


## ASCII

The ASCII rules operate on any input of integral or enum type of size 1.

Unless noted otherwise they do *not* restrict the range of matched values to 7-bit ASCII values.
For example rules like `ascii::any` or `ascii::not_one< 'a' >` will match *all* possible byte values, and *all* possible byte values excluding `'a'`, respectively.
The additional rules [`any7`](#any7), [`many7`](#many7), [`not_one7`](#not_one7-c-), [`not_ione7`](#not_ione7-c-) and [`not_range7`](#not_range7-c-d-) are similar to their counterparts without the trailing `7` but only match bytes with a high bit of zero.

It is possible to match UTF-8 multi-byte characters with the non-seven ASCII rules.
For example the Euro sign code point `U+20AC`, which is encoded by the UTF-8 sequence `E2 82 AC`, is matched by both `ascii::string< 0xe2, 0x82, 0xac >` and `utf8::one< 0x20ac >`.
On closer inspection one would find that both rules are implemented via `internal::ascii_string< 0xe2, 0x82, 0xac >`.

These rules are in the *inline* namespace `tao::pegtl::ascii`.

For all ASCII rules the template parameters representing characters are of type `char`.

###### `alnum`

* Matches and consumes a single ASCII alphabetic or numeric character.
* [Equivalent] to `(ascii::)ranges< 'a', 'z', 'A', 'Z', '0', '9' >`.

###### `alpha`

* Matches and consumes a single ASCII alphabetic character.
* [Equivalent] to `(ascii::)ranges< 'a', 'z', 'A', 'Z' >`.

###### `any`

* Matches and consumes any single byte, including all ASCII characters.
* [Equivalent] to `(ascii::)many< 1 >`.
* [Meta data] and [implementation] mapping:
  - `(ascii::)any::rule_t` is `internal::any< internal::peek_char >`

###### `any7`

* Matches and consumes any single "true" ASCII character that fits into 7 bits.
* [Equivalent] to `(ascii::)range< 0, 127 >`.
* [Meta data] and [implementation] mapping:
  - `(ascii::)any7::rule_t` is `internal::any< internal::peek_seven >`

###### `blank`

* Matches and consumes a single ASCII horizontal space or horizontal tabulator character.
* [Equivalent] to `(ascii::)one< ' ', '\t' >`.

###### `cntrl`
* Matches and consumes a single ASCII control character.
* [Equivalent] to `(ascii::)ranges< 0, 31, 127 >`.

###### `cr`

* Matches and consumes a single *carriage return* of value `13` or `0x0d`.
* [Equivalent] to `(ascii::)one< '\r' >`.
* Also available in the `scan` sub-namespace.
* Also available in the `lazy` sub-namespace.

###### `cr_crlf`

* Matches and consumes an carriage return optionally followed by a line feed.
* [Equivalent] to `seq< (ascii::)cr, opt< (ascii::)lf > >`.
* [Equivalent] to `sor< (ascii::)crlf, (ascii::)cr >`.
* Also available in the `scan` sub-namespace.
* Also available in the `lazy` sub-namespace.

###### `cr_lf`

* Matches and consumes an carriage return **or** line feed.
* [Equivalent] to `(ascii::)one< '\r', '\n' >`.
* Also available in the `lazy` sub-namespace.

###### `cr_lf_crlf`

* Matches and consumes an carriage return **and/or** line feed.
* [Equivalent] to `sor< (ascii::)crlf, (ascii::)cr_lf >`.
* Also available in the `lazy` sub-namespace.

###### `crlf`

* Matches and consumes an carriage return followed by a line feed.
* [Equivalent] to `(ascii::)string< '\r', '\n' >`.
* Also available in the `lazy` sub-namespace.

###### `digit`

* Matches and consumes a single ASCII decimal digit.
* [Equivalent] to `(ascii::)range< '0', '9' >`.

###### `ellipsis`

* Matches and consumes three dots.
* [Equivalent] to `(ascii::)three< '.' >`.

###### `esc`

* Matches and consumes a single ASCII escape character of value `27` or `0x1b`.
* [Equivalent] to `(ascii::)one< '\e' >`.

###### `ff`

* Matches and consumes a single ASCII form feed (new page) character of value `12` or `0x0c`.
* [Equivalent] to `(ascii::)one< '\f' >`.

###### `graph`

* Matches and consumes a single ASCII character traditionally defined as "printable but not space".
* [Equivalent] to `(ascii::)range< '33', '126' >`.

###### `ht`

* Matches and consumes a single ASCII horizontal tab character of value `9`.
* [Equivalent] to `(ascii::)one< '\t' >`.

###### `identifier_first`

* Matches and consumes a single ASCII character permissible as first character of a C identifier.
* [Equivalent] to `(ascii::)ranges< 'a', 'z', 'A', 'Z', '_' >`.
* [Meta data] and [implementation] mapping:
  - `ascii::identifier_first::rule_t` is `internal::ranges< internal::peek_char, 'a', 'z', 'A', 'Z', '_' >`

###### `identifier_other`

* Matches and consumes a single ASCII character permissible as subsequent character of a C identifier.
* [Equivalent] to `(ascii::)ranges< 'a', 'z', 'A', 'Z', '0', '9', '_' >`.
* [Meta data] and [implementation] mapping:
  - `ascii::identifier_first::rule_t` is `internal::ranges< internal::peek_char, 'a', 'z', 'A', 'Z', '0', '9', '_' >`

###### `identifier`

* Matches and consumes an ASCII identifier as defined for the C programming language.
* [Equivalent] to `seq< (ascii::)identifier_first, star< (ascii::)identifier_other > >`.
* [Meta data] and [implementation] mapping is subject to change.

###### `ione< C... >`

* Similar to `(ascii::)one< C... >` but:
* For ASCII letters the match is case insensitive.
* [Meta data] and [implementation] mapping:
  - `ascii::ione<>::rule_t` is `internal::failure`
  - `ascii::ione< C... >::rule_t` is `internal::tester< internal::ione< internal::peek_char, C... > >`.

###### `istring< C... >`

* Similar to `(ascii::)string< C... >`, but:
* For ASCII letters the match is case insensitive.
* [Meta data] and [implementation] mapping:
  - `ascii::istring<>::rule_t` is `internal::success`
  - `ascii::istring< C... >::rule_t` is `internal::ascii_istring< C... >`

###### `keyword< C... >`

* Matches and consumes a non-empty string not followed by an identifier character.
* [Equivalent] to `seq< (ascii::)string< C... >, not_at< (ascii::)identifier_other > >`.
* `C` must be a non-empty character pack.
* [Meta data] and [implementation] mapping:
  - `ascii::keyword< C... >::rule_t` is `internal::seq< internal::ascii_string< C... >, internal::not_at< internal::ranges< internal::peek_char, 'a', 'z', 'A', 'Z', '0', '9', '_' > > >`

###### `lf`

* Matches and consumes a single *line feed* of value `10` or `0x0a`.
* [Equivalent] to `(ascii::)one< '\n' >`.
* Also available in the `scan` sub-namespace.
* Also available in the `lazy` sub-namespace.

###### `lf_crlf`

* Matches and consumes a line feed optionally preceded by a carriage return.
* [Equivalent] to `sor< (ascii::)lf, (ascii::)crlf >`.
* Also available in the `scan` sub-namespace.
* Also available in the `lazy` sub-namespace.

###### `lower`

* Matches and consumes a single ASCII lower-case alphabetic character.
* [Equivalent] to `(ascii::)range< 'a', 'z' >`.

###### `many< Num >`

* Succeeds when the input contains at least `Num` further bytes.
* Consumes these `Num` bytes from the input.
* [Equivalent] to `(ascii::)rep< Num, any >`.
* [Meta data] and [implementation] mapping:
  - `ascii::many< 0 >::rule_t` is `internal::success`
  - `ascii::many< Num >::rule_t` is `internal::many< Num, internal::peek_char >`

###### `many7< Num >`

* True ASCII version of `many` only matches input bytes between 0 and 127.
* [Equivalent] to `rep< Num, (ascii::)any7 >`.
* [Meta data] and [implementation] mapping:
  - `ascii::many7< 0 >::rule_t` is `internal::success`
  - `ascii::many7< Num >::rule_t` is `internal::many< Num, internal::peek_seven >`

###### `not_ione< C... >`

* Similar to `(ascii::)not_one< C... >` but:
* For ASCII letters the match is case insensitive.
* [Meta data] and [implementation] mapping:
  - `ascii::not_ione<>::rule_t` is `internal::any< internal::peek_char >`
  - `ascii::not_ione< C... >::rule_t` is `internal::tester< internal::not_ione< internal::peek_char, Cs... > >`.

###### `not_one< C... >`

* Succeeds when the input is not empty, and:
* `C` is an empty character pack or the next input byte is **not** one of `C...`.
* Consumes one byte on success.
* [Meta data] and [implementation] mapping:
  - `ascii::not_one<>::rule_t` is `internal::any< internal::peek_char >`
  - `ascii::not_one< C... >::rule_t` is `internal::tester< internal::not_one< internal::peek_char, Cs... > >`.

###### `not_one7< C... >`

* True ASCII version of `not_one` only matches input bytes between 0 and 127.
* [Meta data] and [implementation] mapping:
  - `ascii::not_one<>::rule_t` is `internal::any< internal::peek_seven >`
  - `ascii::not_one< C... >::rule_t` is `internal::tester< internal::not_one< internal::peek_seven, Cs... > >`.

###### `not_range< C, D >`

* Succeeds when the input is not empty, and:
* The next input byte is **not** in the closed range `C ... D`.
* Consumes one byte on success.
* [Meta data] and [implementation] mapping:
  - `ascii::not_range< C, C >::rule_t` is `internal::tester< internal::one< internal::peek_char, C > >`.
  - `ascii::not_range< C, D >::rule_t` is `internal::tester< internal::range< internal::peek_char, C, D > >`.

###### `not_range7< C, D >`

* True ASCII version of `not_range` only matches input bytes between 0 and 127.
* [Meta data] and [implementation] mapping:
  - `ascii::not_range< C, C >::rule_t` is `internal::tester< internal::one< internal::peek_seven, C > >`.
  - `ascii::not_range< C, D >::rule_t` is `internal::tester< internal::range< internal::peek_seven, C, D > >`.

###### `nul`

* Matches and consumes an ASCII *nul* character of value `0`.
* [Equivalent] to `(ascii::)one< '\0' >`.

###### `odigit`

* Matches and consumes a single ASCII octal digit.
* [Equivalent] to `(ascii::)range< '0', '7' >`.

###### `one< C... >`

* Succeeds when the input is not empty, and:
* The next input byte is one of `C...`.
* Consumes one byte on success.
* Fails if `C` is an empty character pack.
* [Meta data] and [implementation] mapping:
  - `ascii::not_one<>::rule_t` is `internal::failure`
  - `ascii::not_one< C... >::rule_t` is `internal::tester< internal::one< internal::peek_char, C... > >`.

###### `print`

* Matches and consumes any single ASCII character traditionally defined as *printable*.
* [Equivalent] to `(ascii::)range< 32, 126 >`.

###### `range< C, D >`

* Succeeds when the input is not empty, and:
* The next input byte is in the closed range `C ... D`.
* Consumes one byte on success.
* [Meta data] and [implementation] mapping:
  - `ascii::range< C, C >::rule_t` is `internal::tester< internal::one< internal::peek_char, C > >`.
  - `ascii::range< C, D >::rule_t` is `internal::tester< internal::range< internal::peek_char, C, D > >`.

###### `ranges< C1, D1, C2, D2, ... >`
###### `ranges< C1, D1, C2, D2, ..., E >`

* [Equivalent] to `sor< (ascii::)range< C1, D1 >, (ascii::)range< C2, D2 >, ... >`.
* [Equivalent] to `sor< (ascii::)range< C1, D1 >, (ascii::)range< C2, D2 >, ..., (ascii::)one< E > >`.
* [Meta data] and [implementation] mapping:
  - `ascii::ranges<>::rule_t` is `internal::failure`
  - `ascii::ranges< E >::rule_t` is `internal::tester< internal::one< internal::peek_char, E > >`.
  - `ascii::ranges< C, D >::rule_t` is `internal::tester< internal::range< internal::peek_char, C, D > >`.
  - `ascii::ranges< C... >::rule_t` is `internal::tester< internal::ranges< internal::peek_char, C... > >`.

###### `shebang`

* [Equivalent] to `if_must< (ascii::)string< '#', '!' >, until< eolf > >`.
* [Meta data] and [implementation] mapping:
  - `ascii::shebang::rule_t` is `internal::seq< false, internal::ascii_string< '#', '!' >, internal::until< internal::eolf > >`
  - `ascii::shebang::subs_t` is `type_list< internal::ascii_string< '#', '!' >, internal::until< internal::eolf > >`

###### `sp`

* Matches and consumes a single ASCII space character of value `32` or `0x20`.
* [Equivalent] to `(ascii::)one< ' ' >`.

###### `space`

* Matches and consumes a single space, line feed, carriage return, horizontal tab, vertical tab or form feed.
* [Equivalent] to `(ascii::)one< ' ', '\n', '\r', '\t', '\v', '\f' >`.

###### `string< C... >`

* Matches and consumes a string, a sequence of bytes or ASCII characters.
* [Equivalent] to `seq< (ascii::)one< C >... >`.
* [Meta data] and [implementation] mapping:
  - `ascii::string<>::rule_t` is `internal::success`
  - `ascii::string< C >:rule_t` is `internal::tester< internal::one< internal::peek_char, C > >`
  - `ascii::string< C... >::rule_t` is `internal::ascii_string< C... >`

###### `TAO_PEGTL_ISTRING( "..." )`

* Macro where `TAO_PEGTL_ISTRING( "foo" )` yields `(ascii::)istring< 'f', 'o', 'o' >`.
* The argument must be a string literal.
* Works for strings up to 512 bytes of length (excluding trailing `'\0'`).
* Strings may contain embedded `'\0'`.

###### `TAO_PEGTL_KEYWORD( "..." )`

* Macro where `TAO_PEGTL_KEYWORD( "foo" )` yields `(ascii::)keyword< 'f', 'o', 'o' >`.
* The argument must be a string literal.
* Works for keywords up to 512 bytes of length (excluding trailing `'\0'`).
* Strings may contain embedded `'\0'`.

###### `TAO_PEGTL_STRING( "..." )`

* Macro where `TAO_PEGTL_STRING( "foo" )` yields `(ascii::)string< 'f', 'o', 'o' >`.
* The argument must be a string literal.
* Works for strings up to 512 bytes of length (excluding trailing `'\0'`).
* Strings may contain embedded `'\0'`.

###### `three< C >`

* Succeeds when the input contains at least three bytes, and:
* These three input bytes are all `C`.
* Consumes three bytes on success.
* [Meta data] and [implementation] mapping:
  - `ascii::three< C >::rule_t` is `internal::ascii_string< C, C, C >`

###### `two< C >`

* Succeeds when the input contains at least two bytes, and:
* These two input bytes are both `C`.
* Consumes two bytes on success.
* [Meta data] and [implementation] mapping:
  - `ascii::two< C >::rule_t` is `internal::ascii_string< C, C >`

###### `upper`

* Matches and consumes a single ASCII upper-case alphabetic character.
* [Equivalent] to `(ascii::)range< 'A', 'Z' >`.

###### `vt`

* Matches and consumes a single ASCII vertical tab of value `11` or `0x0b`.
* [Equivalent] to `(ascii::)one< '\v' >`.

###### `xdigit`

* Matches and consumes a single ASCII hexadecimal digit character.
* [Equivalent] to `(ascii::)ranges< '0', '9', 'a', 'f', 'A', 'F' >`.


## Unicode

These rules are available in multiple versions,

* in namespace `tao::pegtl::utf8` for UTF-8 inputs,
* in namespace alias `tao::pegtl::utf16` for native-endian UTF-16 inputs,
* in namespace alias `tao::pegtl::utf32` for native-endian UTF-32 inputs.
* in namespace `tao::pegtl::utf16_be` for big-endian UTF-16 inputs,
* in namespace `tao::pegtl::utf16_le` for little-endian UTF-16 inputs,
* in namespace `tao::pegtl::utf32_be` for big-endian UTF-32 inputs,
* in namespace `tao::pegtl::utf32_le` for little-endian UTF-32 inputs,
* in namespace `tao::pegtl::unicode` for native-endian input-adaptive rules.

Only the UTF-8 rules are included with the default header `<tao/pegtl.hpp>`.
For the other rules the following header files need to be included as required.

* `tao/pegtl/utf16.hpp`
* `tao/pegtl/utf32.hpp`
* `tao/pegtl/unicode.hpp`

Remember that the `column` field of the input positions counts the *number of input objects* since the last line ending, not the actual column!

A Unicode code point is considered *valid* when it is in the range `0` to `0x10ffff`.

* The UTF-8 rules work on the same inputs as the ASCII rules whose `data_t` is an 8-bit integer or enum type.
* The UTF-16 rules work with inputs whose `data_t` is either an 8-bit or a 16-bit integer or enum type.
* The UTF-32 rules work with inputs whose `data_t` is either an 8-bit or a 32-bit integer or enum type.

In the following descriptions the parameter N stands for the size of the encoding of the next Unicode code point in the input.

* For UTF-8 the rules are multi-byte-sequence-aware and N is either 1, 2, 3 or 4.
* For UTF-16 the rules are surrogate-pair-aware and N is either 2 or 4 for 8-bit inputs, and either 1 or 2 for 16-bit inputs.
* For UTF-32 N is always 4 for 8-bit inputs, and always 1 for 32-bit inputs.

The input-adaptive rules use UTF-8 on 8-bit inputs, native-endian UTF-16 on 16-bit inputs, and native-endian UTF-32 on 32bit inputs.

Note that the Unicode rules only match UTF-16 surrogates as part of a valid UTF-16 surrogate pair.

For all Unicode rules the template parameters representing code points are of type `char32_t`.

###### `any`

* Succeeds when the input is not empty, and:
* The next N input objects encode a valid Unicode code point.
* Consumes the N input objects on success.

###### `bom`

* Matches and consumes a *byte-order mark* of value `U+FEFF`.

###### `cr`

* Matches and consumes a single *carriage return* of value `13` or `U+000D`.
* Also available in the `scan` sub-namespace.
* Also available in the `lazy` sub-namespace.

###### `cr_crlf`

* Matches and consumes an carriage return optionally followed by a line feed.
* Also available in the `scan` sub-namespace.
* Also available in the `lazy` sub-namespace.

###### `cr_lf`

* Matches and consumes an carriage return **or** line feed.
* Also available in the `lazy` sub-namespace.

###### `cr_lf_crlf`

* Matches and consumes an carriage return **and/or** line feed.
* Also available in the `lazy` sub-namespace.

###### `crlf`

* Matches and consumes an carriage return followed by a line feed.
* Also available in the `lazy` sub-namespace.

###### `eol1`

* Match any single-code-point Unicode end-of-line character:
* Carriage return, line feed, form feed, vertical tab, next line, or line or paragraph separator.

###### `eolu`

* Match any Unicode end-of-line sequence, i.e. `crlf` or `eol1`.
* Also available in the `lazy` sub-namespace.

###### `lf`

* Matches and consumes a single *line feed* of value `10` or `U+000A`.
* Also available in the `scan` sub-namespace.
* Also available in the `lazy` sub-namespace.

###### `lf_crlf`

* Matches and consumes a line feed optionally preceded by a carriage return.
* Also available in the `scan` sub-namespace.
* Also available in the `lazy` sub-namespace.

###### `ls`

* Matches and consumes a single Unicode *line separator* of value `U+2028`.
* Also available in the `scan` sub-namespace.
* Also available in the `lazy` sub-namespace.

###### `many< Num >`

* Succeeds when the input contains at least `Num` further code points.
* Consumes these `Num` code points from the input.
* [Equivalent] to `rep< Num, utf_::any >`.

###### `not_one< C... >`

* Succeeds when the input is not empty, and:
* The next N input objects encode a valid Unicode code point, and:
* `C` is an empty character pack or the input code point is **not** one of the given code points `C...`.
* Consumes the N input objects on success.

###### `not_range< C, D >`

* Succeeds when the input is not empty, and:
* The next N input objects encode a valid Unicode code point, and:
* The input code point `B` satisfies `B < C || D < B`.
* Consumes the N input objects on success.

###### `one< C... >`

* Succeeds when the input is not empty, and:
* The next N input objects encode a valid Unicode code point, and:
* `C` is a non-empty character pack and the input code point is one of the given code points `C...`.
* Consumes the N input objects on success.

###### `nel`

* Matches and consumes a single Unicode *next line* of value `U+0085`.
* Also available in the `scan` sub-namespace.
* Also available in the `lazy` sub-namespace.

###### `ps`

* Matches and consumes a single Unicode *paragraph separator* of value `U+2029`.
* Also available in the `scan` sub-namespace.
* Also available in the `lazy` sub-namespace.

###### `range< C, D >`

* Succeeds when the input is not empty, and:
* The next N input objects encode a valid Unicode code point, and:
* The input code point `B` satisfies `C <= B && B <= D`.
* Consumes the N input objects on success.

###### `ranges< C1, D1, C2, D2, ... >`

* [Equivalent] to `sor< utf_::range< C1, D1 >, utf_::range< C2, D2 >, ... >`.

###### `ranges< C1, D1, C2, D2, ..., E >`

* [Equivalent] to `sor< utf_::range< C1, D1 >, utf_::range< C2, D2 >, ..., utf_::one< E > >`.

###### `string< C... >`

* [Equivalent] to `seq< utf_::one< C >... >`.
* [Meta data] and [implementation] mapping:
  * `utf_::string<>::rule_t` is `internal::success`.
  * `utf_::string< C... >::rule_t` is `internal::seq< internal::one< internal::peek_, C >... >`.
  * `utf8::string< C >::rule_t` is `internal::tester< internal::one< internal::peek_utf8, C > >`.
  * `utf8::string< C... >::rule_t` is `internal::ascii_string< U... >` where `U...` is the UTF-8 encoding of `C...`.


## Binary

These rules are available in multiple versions,

* in namespace `tao::pegtl::int8` for `std::int8_t` values,
* in namespace `tao::pegtl::uint8` for `std::uint8_t` values,
* in namespace `tao::pegtl::int16_be` for big-endian `std::int16_t` values,
* in namespace `tao::pegtl::int16_le` for little-endian `std::int16_t` values,
* in namespace `tao::pegtl::int32_be` for big-endian `std::int32_t` values,
* in namespace `tao::pegtl::int32_le` for little-endian `std::int32_t` values,
* in namespace `tao::pegtl::int64_be` for big-endian `std::int64_t` values,
* in namespace `tao::pegtl::int64_le` for little-endian `std::int64_t` values,
* in namespace `tao::pegtl::uint16_be` for big-endian `std::uint16_t` values,
* in namespace `tao::pegtl::uint16_le` for little-endian `std::uint16_t` values,
* in namespace `tao::pegtl::uint32_be` for big-endian `std::uint32_t` values,
* in namespace `tao::pegtl::uint32_le` for little-endian `std::uint32_t` values,
* in namespace `tao::pegtl::uint64_be` for big-endian `std::uint64_t` values,
* in namespace `tao::pegtl::uint64_le` for little-endian `std::uint64_t` values,
* in namespace `tao::pegtl::enums_be` for big-endian enumeration type values,
* in namespace `tao::pegtl::enums_le` for little-endian enumeration type values,
* in namespace alias `tao::pegtl::int16` for native-endian `std::int16_t` values,
* in namespace alias `tao::pegtl::int32` for native-endian `std::int32_t` values,
* in namespace alias `tao::pegtl::int64` for native-endian `std::int64_t` values,
* in namespace alias `tao::pegtl::uint16` for native-endian `std::uint16_t` values,
* in namespace alias `tao::pegtl::uint32` for native-endian `std::uint32_t` values,
* in namespace alias `tao::pegtl::uint64` for native-endian `std::uint64_t` values.
* in namespace alias `tao::pegtl::enums` for native-endian enumeration type values.

The binary rules are **not** automatically included with `<tao/pegtl.hpp>.
To make them available the following header files need to be included as required.

* `tao/pegtl/int8.hpp`
* `tao/pegtl/uint8.hpp`
* `tao/pegtl/int16.hpp`
* `tao/pegtl/uint16.hpp`
* `tao/pegtl/int32.hpp`
* `tao/pegtl/uint32.hpp`
* `tao/pegtl/int64.hpp`
* `tao/pegtl/uint64.hpp`
* `tao/pegtl/enums.hpp`

These rules operate both on byte-sized inputs and on inputs whose data size corresponds to the size of the matched integers or enums.

* The 8-bit rules work on the same inputs as the ASCII rules whose `data_t` is an 8-bit integer or enum type.
* The 16-bit rules work with inputs whose `data_t` is either an 8-bit or a 16-bit integer or enum type.
* The 32-bit rules work with inputs whose `data_t` is either an 8-bit or a 32-bit integer or enum type.
* The 64-bit rules work with inputs whose `data_t` is either an 8-bit or a 64-bit integer or enum type.

The enum rules behave like integers of the same size.

In the following descriptions the parameter N stands for the size of the encoding of the next integer value according to the size of integer matched by the rule in question.

* For 8-bit rules N is always 1.
* For 16-bit rules N is 2 for 8-bit inputs and 1 for 16-bit inputs.
* For 32-bit rules N is 4 for 8-bit inputs and 1 for 32-bit inputs.
* For 64-bit rules N is 8 for 8-bit inputs and 1 for 64-bit inputs.

The enum rules again behave like integers of the same size.

The template parameters of an enum rule **must** all be of the same enum type.

Only the `not_one`, `not_range`, `one`, `range`, `ranges` and `string` rules are available for enums.

The term *input value* indicates an integer or enum value of the appropriate size read from the input.

###### `any`

* Succeeds when the input contains at least N objects.
* Consumes N objects on success.

###### `many< Num >`

* Succeeds when the input contains at least `Num` times N objects.
* Consumes these `Num` * N objects from the input.
* [Equivalent] to `rep< N, any >`.

###### `mask_not_one< M, C... >`

* Succeeds when the input contains at least N objects, and:
* `C` is an empty pack *or* the (endian adjusted) input value masked with `M` is **not** one of the values `C...`.
* Consumes N objects on success.

###### `mask_not_range< M, C, D >`

* Succeeds when the input contains at least N objects, and:
* The (endian adjusted) input value `b` satisfies `( b & M ) < C || D < ( b & M )`.
* Consumes N objects on success.

###### `mask_one< M, C... >`

* Succeeds when the input contains at least N objects, and:
* `C` is a non-empty pack and the (endian adjusted) input value masked with `M` is one of the given values `C...`.
* Consumes N objects on success.

###### `mask_range< M, C, D >`

* Succeeds when the input contains at least N objects, and:
* The (endian adjusted) input value `b` satisfies `C <= ( b & M ) && ( b & M ) <= D`.
* Consumes N objects on success.

###### `mask_ranges< M, C1, D1, C2, D2, ... >`

* [Equivalent] to `sor< mask_range< M, C1, D1 >, mask_range< M, C2, D2 >, ... >`.

###### `mask_ranges< M, C1, D1, C2, D2, ..., E >`

* [Equivalent] to `sor< mask_range< M, C1, D1 >, mask_range< M, C2, D2 >, ..., mask_one< M, E > >`.

###### `mask_string< M, C... >`

* [Equivalent] to `seq< mask_one< M, C >... >`.

###### `not_one< C... >`

* Succeeds when the input contains at least N objects, and:
* `C` is an empty pack or the (endian adjusted) input value is **not** one of the given values `C...`.
* Consumes N objects on success.

###### `not_range< C, D >`

* Succeeds when the input contains at least N objects, and:
* The (endian adjusted) input value `b` satisfies `b < C || D < b`.
* Consumes N objects on success.

###### `one< C... >`

* Succeeds when the input contains at least N objects, and:
* `C` is a non-empty pack and the (endian adjusted) input value is one of the given values `C...`.
* Consumes N objects on success.

###### `range< C, D >`

* Succeeds when the input contains at least N objects, and:
* The (endian adjusted) input value `b` satisfies `C <= b && b <= D`.
* Consumes N objects on success.

###### `ranges< C1, D1, C2, D2, ... >`

* [Equivalent] to `sor< range< C1, D1 >, range< C2, D2 >, ... >`.

###### `ranges< C1, D1, C2, D2, ..., E >`

* [Equivalent] to `sor< range< C1, D1 >, range< C2, D2 >, ..., one< E > >`.

###### `string< C... >`

* [Equivalent] to `seq< one< C >... >`.


## Member

The member rules apply their matching condition to a value derived from the next object in the input.

The first template parameter to the member rules must be a member pointer, a member function pointer to a "getter" function, or a function pointer to a global "getter".

More precisely, for inputs representing sequences of objects of type `T`, the permissible types of the first template parameter `M` are as follows; the functions can be `noexcept( true )` or `noexcept( false )`.

* `U T::*` -- pointer to data member of type `U`.
* `const U T::*` -- pointer to data member of type `const U`.
* `U* T::*` -- pointer to data member of type `U*`.
* `const U* T::*` -- pointer to data member of type `const U*`.
* `U* const T::*` -- pointer to data member of type `U* const`.
* `const U* const T::*` -- pointer to data member of type `const U* const`.
* `U (T::*)() const` -- pointer to const member function that returns a `U`.
* `const U& (T::*)() const` -- pointer to const member function that returns a `const U&`.
* `const U* (T::*)() const` -- pointer to const member function that returns a `const U*`.
* `U (*)( const T& )` -- pointer to function that takes a `const T&` and returns a `U`.
* `const U& (*)( const T& )` -- pointer to function that takes a `const T&` and returns a `const U&`.
* `const U* (*)( const T& )` -- pointer to function that takes a `const T&` and returns a `const U*`.

Here `U` is assumed to be neither a pointer nor a reference type.
Subsequent non-type template parameters, if any, must be of type `U` as that is the type of the object **extracted** from the next input object of type `T` on which the match condition is tested.

The member rules are **not** automatically included with `<tao/pegtl.hpp>`.
To make them available it is necessary to include `tao/pegtl/member.hpp`.

These rules are in namespace `tao::pegtl::member`.

###### `function< M, F >`

* Apply the function `F` to the extracted object of type `U`.
* The function `F` must take a `U` or `const U&` as first argument, and
* if possible the function `F` is called with all State objects as additional arguments.
* The function `F` must return a `bool` with the result of the match attempt.
* The function `F` can be `noexcept( true )` or `noexcept( false )`.

###### `nested< M, R >`

* Performs a nested parsing run with rule `R` on the extracted object.
* The extracted object must be suited to construct a `view_input`.

###### `not_one< M, U... >`

* Succeeds when the input contains at least 1 object, and:
* The object extracted from the next input object is **not** one of the the values `U...`.
* Consumes N objects on success.

###### `not_range< M, U, V >`

* Succeeds when the input contains at least 1 object, and:
* The object `u` extracted from the next input object satisfies `u < U || V < u`.
* Consumes 1 object on success.

###### `one< M, U... >`

* Succeeds when the input contains at least 1 object, and:
* The object extracted from the next input object is one of the the values `U...`.
* Consumes N objects on success.

###### `range< M, U, V >`

* Succeeds when the input contains at least 1 object, and:
* The object `u` extracted from the next input object satisfies `U <= u && u <= V`.
* Consumes 1 object on success.

###### `ranges< M, U1, V1, U2, V2, ... >`

* [Equivalent] to `sor< range< M, U1, V1 >, range< M, U2, V2 >, ... >`.

###### `ranges< M, U1, V1, U2, V2, ..., W >`

* [Equivalent] to `sor< range< M, U1, V1 >, range< M, U2, V2 >, ..., one< M, W > >`.

###### `string< M, U... >`

* [Equivalent] to `seq< one< M, U >... >`.


## Combinators

These rules correspond to the classical [**PEG**](https://en.wikipedia.org/wiki/Parsing_expression_grammar) combinators or operators.

These rules are in namespace `tao::pegtl`.

###### `at< R... >`

* [PEG] **and-predicate** &*e*
* Succeeds if and only if `seq< R... >` would succeed.
* Consumes nothing independent of result.
* Disables all actions while matching `R...`.
* [Meta data] and [implementation] mapping:
  - `at<>::rule_t` is `internal::success`
  - `at< R >::rule_t` is `internal::at< R >`
  - `at< R >::subs_t` is `type_list< R >`
  - `at< R... >::rule_t` is `internal::at< internal::seq< R... > >`
  - `at< R... >::subs_t` is `type_list< internal::seq< R... > >`

###### `not_at< R... >`

* [PEG] **not-predicate** !*e*
* Succeeds if and only if `seq< R... >` would **not** succeed.
* Consumes nothing independent of result.
* Disables all actions whiel matching `R...`.
* [Meta data] and [implementation] mapping:
  - `not_at<>::rule_t` is `internal::failure`
  - `not_at< R >::rule_t` is `internal::not_at< R >`
  - `not_at< R >::subs_t` is `type_list< R >`
  - `not_at< R... >::rule_t` is `internal::not_at< internal::seq< R... > >`
  - `not_at< R... >::subs_t` is `type_list< internal::seq< R... > >`

###### `opt< R... >`

* [PEG] **optional** *e*?
* Optional `seq< R... >`, i.e. attempt to match `seq< R... >` and always return success:
* The return value of `opt< R... >` does *not* depend on whether `R...` matched.
* [Equivalent] to `sor< seq< R... >, success >`.
* [Meta data] and [implementation] mapping:
  - `opt<>::rule_t` is `internal::success`
  - `opt< R >::rule_t` is `internal::opt< R >`
  - `opt< R >::subs_t` is `type_list< R >`
  - `opt< R... >::rule_t` is `internal::opt< internal::seq< R... > >`
  - `opt< R... >::subs_t` is `type_list< internal::seq< R... > >`

###### `plus< R... >`

* [PEG] **one-or-more** *e*+
* Matches `seq< R... >` as often as possible and succeeds if it matches at least once.
* [Equivalent] to `seq< R..., star< R... > >`.
* `R` must be a non-empty rule pack.
* [Meta data] and [implementation] mapping:
  - `plus< R >::rule_t` is `internal::plus< R >`
  - `plus< R >::subs_t` is `type_list< R >`
  - `plus< R... >::rule_t` is `internal::plus< internal::seq< R... > >`
  - `plus< R... >::subs_t` is `type_list< internal::seq< R... > >`

###### `seq< R... >`

* [PEG] **sequence** *e*<sub>1</sub> *e*<sub>2</sub>
* Sequence or *conjunction* of rules.
* Matches the given rules `R...` in the given order.
* Fails and stops matching when one of the given rules fails.
* Consumes everything that the rules `R...` consumed.
* Succeeds if `R` is an empty rule pack.
* [Meta data] and [implementation] mapping:
  - `seq<>::rule_t` is `internal::success`
  - `seq< R >::rule_t` is `internal::seq< R >`
  - `seq< R >::subs_t` is `type_list< R >`
  - `seq< R... >::rule_t` is `internal::seq< R... >`
  - `seq< R... >::subs_t` is `type_list< R... >`

###### `sor< R... >`

* [PEG] **ordered choice** *e*<sub>1</sub> / *e*<sub>2</sub>
* Choice or *disjunction* of rules.
* Matches the given rules `R...` in the given order.
* Succeeds and stops matching when one of the given rules succeeds.
* Consumes whatever the first rule that succeeded consumed.
* Fails if `R` is an empty rule pack.
* [Meta data] and [implementation] mapping:
  - `sor<>::rule_t` is `internal::failure`
  - `sor< R >::rule_t` is `internal::sor< R >`
  - `sor< R >::subs_t` is `type_list< R >`
  - `sor< R... >::rule_t` is `internal::sor< R... >`
  - `sor< R... >::subs_t` is `type_list< R... >`

###### `star< R... >`

* [PEG] **zero-or-more** *e**
* Matches `seq< R... >` as often as possible and always succeeds.
* [Equivalent] to `opt< plus< R... > >`.
* `R` must be a non-empty rule pack.
* [Meta data] and [implementation] mapping:
  - `star< R >::rule_t` is `internal::star< R >`
  - `star< R >::subs_t` is `type_list< R >`
  - `star< R... >::rule_t` is `internal::star< internal::seq< R... > >`
  - `star< R... >::subs_t` is `type_list< internal::seq< R... > >`


## Convenience

These rules implement common rule combinations for more concise and/or efficient grammars.

These rules are in namespace `tao::pegtl`.

###### `if_then_else< R, S, T >`

* Attempts to match either `S` or `T` depending on whether `R` matched.
* [Equivalent] to `sor< seq< R, S >, seq< not_at< R >, T > >`.
* [Meta data] and [implementation] mapping:
  - `if_then_else< R, S, T >::rule_t` is `internal::if_then_else< R, S, T>`
  - `if_then_else< R, S, T >::subs_t` is `type_list< R, S, T >`

###### `list< R, S >`

* Matches a non-empty list of `R` separated by `S`.
* [Equivalent] to `seq< R, star< S, R > >`.
* [Meta data] and [implementation] mapping:
  - `list< R, S >::rule_t` is `internal::seq< R, internal::star< S, R > >`
  - `list< R, S >::subs_t` is `type_list< R, internal::star< S, R > >`

###### `list< R, S, P >`

* Matches a non-empty list of `R` separated by `S` where each `S` can be padded by `P`.
* [Equivalent] to `seq< R, star< pad< S, P >, R > >`.
* [Meta data] and [implementation] mapping:
  - `list< R, S, P >::rule_t` is `internal::seq< R, internal::star< internal::pad< S, P >, R > >`
  - `list< R, S, P >::subs_t` is `type_list< R, internal::star< internal::pad< S, P >, R > >`

###### `list_tail< R, S >`

* Matches a non-empty list of `R` separated by `S` with optional trailing `S`.
* [Equivalent] to `seq< list< R, S >, opt< S > >`.
* [Equivalent] to `seq< R, star_partial< S, R > >`.
* [Meta data] and [implementation] mapping:
  - `list_tail< R, S >::rule_t` is `internal::seq< R, internal::star_partial< S, R > >`
  - `list_tail< R, S >::subs_t` is `type_list< R, internal::star_partial< S, R > >`

###### `list_tail< R, S, P >`

* Matches a non-empty list of `R` separated by `S` with optional trailing `S` and padding `P` inside the list.
* [Equivalent] to `seq< list< R, S, P >, opt< star< P >, S > >`.
* [Equivalent] to `seq< R, star_partial< padl< S, P >, padl< R, P > > >`.
* [Meta data] and [implementation] mapping:
  - `list_tail< R, S, P >::rule_t` is `internal::seq< R, internal::star_partial< internal::padl< S, P >, internal::padl< R, P > > >`
  - `list_tail< R, S, P >::subs_t` is `type_list< R, internal::star_partial< internal::padl< S, P >, internal::padl< R, P > > >`

###### `minus< M, S >`

* Succeeds if `M` matches, and `S` does *not* match *all* of the input that `M` matched.
* [Equivalent] to `rematch< M, not_at< S, eof > >`.
* [Meta data] and [implementation] mapping:
  - `minus< M, S >::rule_t` is `internal::rematch< M, internal::not_at< S, internal::eof > >`
  - `minus< M, S >::subs_t` is `type_list< M, internal::not_at< S, internal::eof > >`

Note that `S` is ignored by the [grammar analysis](Grammar-Analysis.md).

###### `pad< R, S, T = S >`

* Matches an `R` that can be padded by arbitrary many `S` on the left and `T` on the right.
* [Equivalent] to `seq< star< S >, R, star< T > >`.
* [Meta data] and [implementation] mapping:
  - `pad< R, S, T >::rule_t` is `internal::seq< internal::star< S >, R, internal::star< T > >`
  - `pad< R, S, T >::subs_t` is `type_list< internal::star< S >, R, internal::star< T > >`

A common mistake is to forget about the implicit `star` and use e.g. `star< blank >` for `S` (and `T`).
This attempts to match `star< star< S > >` which is an infinite loop without progress.
The PEGTL [grammar analysis](Grammar-Analysis.md) catches this mistake.

###### `pad_opt< R, P >`

* Matches an optional `R` that can be padded by arbitrary many `P`, or just arbitrary many `P`.
* [Equivalent] to `seq< star< P >, opt< R, star< P > > >`.
* [Meta data] and [implementation] mapping:
  - `pad_opt< R, P >::rule_t` is `internal::seq< internal::star< P >, internal::opt< R, internal::star< P > > >`
  - `pad_opt< R, P >::subs_t` is `type_list< internal::star< P >, internal::opt< R, internal::star< P > > >`

###### `partial< R... >`

* Similar to `opt< R... >` with one important difference:
* Does *not* rewind the input after a partial match of `R...`.
* Attempts to match the given rules `R...` in the given order.
* Succeeds and stops matching when one of the given rules fails;
* succeds when all of the given rules succeed.
* Consumes everything that the successful rules of `R...` consumed.
* `R` must be a non-empty rule pack.
* [Equivalent] to `opt< R >` when `R...` is a single rule.
* [Equivalent] to `opt< R1, opt< R2, opt< ...` when `R` is `R1`, `R2`, ...
* [Meta data] and [implementation] mapping:
  - `partial< R... >::rule_t` is `internal::partial< R... >`
  - `partial< R... >::subs_t` is `type_list< R... >`

###### `rematch< R, S... >`

* Succeeds if `R` matches, and each `S` matches the input that `R` matched.
* [Meta data] and [implementation] mapping:
  - `rematch< R, S... >::rule_t` is `internal::rematch< R, S... >`
  - `rematch< R, S... >::subs_t` is `type_list< R, S... >`

Note that the rules in `S...` do *not* need to match *all* of the input matched by `R`.
(Which is why [`minus`](#minus-m-s-) uses `eof` in its implementation).

Note that the `S...` are ignored in the grammar analysis.

###### `rep< Num, R... >`

* Matches `seq< R... >` for `Num` times without checking for further matches.
* [Equivalent] to `seq< seq< R... >, ..., seq< R... > >` where `seq< R... >` is repeated `Num` times.
* [Meta data] and [implementation] mapping:
  - `rep< 0, R... >::rule_t` is `internal::success`
  - `rep< N >::rule_t` is `internal::success`
  - `rep< N, R >::rule_t` is `internal::rep< N, R >`
  - `rep< N, R >::subs_t` is `type_list< R >`
  - `rep< N, R... >::rule_t` is `internal::rep< N, internal::seq< R... > >`
  - `rep< N, R... >::subs_t` is `type_list< internal::seq< R... > >`

###### `rep_max< Max, R... >`

* Matches `seq< R... >` for at most `Max` times and verifies that it doesn't match more often.
* [Equivalent] to `rep_min_max< 0, Max, R... >`.
* [Meta data] and [implementation] mapping:
  - `rep_max< 0, R >::rule_t` is `internal::not_at< R >`
  - `rep_max< 0, R >::subs_t` is `type_list< R >`
  - `rep_max< 0, R... >::rule_t` is `internal::not_at< internal::seq< R... > >`
  - `rep_max< 0, R... >::subs_t` is `type_list< internal::seq< R... > >`
  - `rep_max< Max >::rule_t` is `internal::failure`
  - `rep_max< Max, R >::rule_t` is `internal::rep_min_max< 0, Max, R >`
  - `rep_max< Max, R >::subs_t` is `type_list< R >`
  - `rep_max< Max, R... >::rule_t` is `internal::rep_min_max< 0, Max, internal::seq< R... > >`
  - `rep_max< Max, R... >::subs_t` is `type_list< internal::seq< R... > >`

###### `rep_min< Min, R... >`

* Matches `seq< R... >` as often as possible and succeeds if it matches at least `Min` times.
* [Equivalent] to `seq< rep< Min, R... >, star< R... > >`.
* `R` must be a non-empty rule pack.
* [Meta data] and [implementation] mapping:
  - `rep_min< Min, R... >::rule_t` is `internal::seq< internal::rep< Min, R... >, internal::star< R... > >`
  - `rep_min< Min, R... >::subs_t` is `type_list< internal::rep< Min, R... >, internal::star< R... > >`

###### `rep_min_max< Min, Max, R... >`

* Matches `seq< R... >` for `Min` to `Max` times and verifies that it doesn't match more often.
* [Equivalent] to `seq< rep< Min, R... >, rep_opt< Max - Min, R... >, not_at< R... > >`.
* [Meta data] and [implementation] mapping:
  - `rep_min_max< 0, 0, R >::rule_t` is `internal::not_at< R >`
  - `rep_min_max< 0, 0, R >::subs_t` is `type_list< R >`
  - `rep_min_max< 0, 0, R... >::rule_t` is `internal::not_at< internal::seq< R... > >`
  - `rep_min_max< 0, 0, R... >::subs_t` is `type_list< internal::seq< R... > >`
  - `rep_min_max< Min, Max >::rule_t` is `internal::failure`
  - `rep_min_max< Min, Max, R >::rule_t` is `internal::rep_min_max< Min, Max, R >`
  - `rep_min_max< Min, Max, R >::subs_t` is `type_list< R >`
  - `rep_min_max< Min, Max, R... >::rule_t` is `internal::rep_min_max< Min, Max, internal::seq< R... > >`
  - `rep_min_max< Min, Max, R... >::subs_t` is `type_list< internal::seq< R... > >`

###### `rep_opt< Num, R... >`

* Matches `seq< R... >` for zero to `Num` times without check for further matches.
* [Equivalent] to `rep< Num, opt< R... > >`.
* [Meta data] and [implementation] mapping:
  - `rep_opt< 0, R... >::rule_t` is `internal::success`
  - `rep_opt< Num >::rule_t` is `internal::success`
  - `rep_opt< Num, R... >::rule_t` is `internal::seq< internal::rep< Num, R... >, internal::star< R... > >`
  - `rep_opt< Num, R... >::subs_t` is `type_list< internal::rep< Num, R... >, internal::star< R... > >`

###### `star_partial< R... >`

* Similar to `star< R... >` with one important difference:
* The final iteration does *not* rewind the input after a partial match of `R...`.
* `R` must be a non-empty rule pack.
* [Meta data] and [implementation] mapping:
  - `star_partial< R... >::rule_t` is `internal::star_partial< R... >`
  - `star_partial< R... >::subs_t` is `type_list< R... >`

###### `star_strict< R... >`

* Similar to `star< R... >` with one important difference:
* A partial match of `R...` lets `star_strict` fail locally.
* `R` must be a non-empty rule pack.
* [Meta data] and [implementation] mapping:
  - `star_strict< R... >::rule_t` is `internal::star_strict< R... >`
  - `star_strict< R... >::subs_t` is `type_list< R... >`

###### `strict< R... >`

* Similar to `opt< R... >` with one important difference:
* A partial match of `R...` lets `strict` fail locally.
* [Equivalent] to `sor< not_at< R1 >, seq< R... > >` if `R1` is the first rule of `R...`.
* `R` must be a non-empty rule pack.
* [Meta data] and [implementation] mapping:
  - `strict< R... >::rule_t` is `internal::strict< R... >`
  - `strict< R... >::subs_t` is `type_list< R... >`

###### `until< R >`

* Consumes all input until `R` matches.
* [Equivalent] to `until< R, consume< 1 > >`.
* [Meta data] and [implementation] mapping:
  - `until< R >::rule_t` is `internal::until< R >`
  - `until< R >::subs_t` is `type_list< R >`

###### `until< R, S... >`

* Matches `seq< S... >` as long as `at< R >` does not match and succeeds when `R` matches.
* [Equivalent] to `seq< star< not_at< R >, S... >, R >`.
* See the previous entry for `until< R >`, i.e. when `S...` is empty.
* [Meta data] and [implementation] mapping:
  - `until< R, S >::rule_t` is `internal::until< R, S >`
  - `until< R, S >::subs_t` is `type_list< R, S >`
  - `until< R, S... >::rule_t` is `internal::until< R, internal::seq< S... > >`
  - `until< R, S... >::subs_t` is `type_list< R, internal::seq< S... > >`


## Controlling

These rules manage and change the [control](Control-and-Debug.md), [action and states](Actions-and-States.md) during a parsing run.

These rules are in namespace `tao::pegtl`.

###### `action< A, R... >`

* [Equivalent] to `seq< R... >`, but:
* Uses the given class template `A` as [action](Actions-and-States.md).
* Does *not* change whether actions are enabled or disabled!
* [Meta data] and [implementation] mapping:
  - `action< A >::rule_t` is `internal::success`
  - `action< A, R >::rule_t` is `internal::action< A, R >`
  - `action< A, R >::subs_t` is `type_list< R >`
  - `action< A, R... >::rule_t` is `internal::action< A, internal::seq< R... > >`
  - `action< A, R... >::subs_t` is `type_list< internal::seq< R... > >`

###### `control< C, R... >`

* [Equivalent] to `seq< R... >`, but:
* Uses the given class template `C` as [control](Control-and-Debug.md).
* [Meta data] and [implementation] mapping:
  - `control< C >::rule_t` is `internal::success`
  - `control< C, R >::rule_t` is `internal::control< C, R >`
  - `control< C, R >::subs_t` is `type_list< R >`
  - `control< C, R... >:rule_t` is `internal::control< C, internal::seq< R... > >`
  - `control< C, R... >:subs_t` is `type_list< internal::seq< R... > >`

###### `disable< R... >`

* [Equivalent] to `seq< R... >`, but:
* Disables all actions while parsing `R...`.
* [Meta data] and [implementation] mapping:
  - `disable<>::rule_t` is `internal::success`
  - `disable< R >::rule_t` is `internal::disable<, R >`
  - `disable< R >::subs_t` is `type_list< R >`
  - `disable< R... >::rule_t` is `internal::disable< internal::seq< R... > >`
  - `disable< R... >::subs_t` is `type_list< internal::seq< R... > >`

###### `enable< R... >`

* [Equivalent] to `seq< R... >`, but:
* Enables all actions (if any) while parsing `R...`.
* [Meta data] and [implementation] mapping:
  - `enable<>::rule_t` is `internal::success`
  - `enable< R >::rule_t` is `internal::enable< R >`
  - `enable< R >::subs_t` is `type_list< R >`
  - `enable< R... >::rule_t` is `internal::enable< internal::seq< R... > >`
  - `enable< R... >::subs_t` is `type_list< internal::seq< R... > >`

###### `state< S, R... >`

* [Equivalent] to `seq< R... >`, but:
* Replaces all state arguments with a new instance `s` of type `S`.
* `s` is constructed with the input and all previous states as arguments, or
* `s` is default constructed if `S` has no constructor for the above clause.
* If `R...` succeeds then `s.success()` is called with the input (after the match) and all previous states as arguments.
* [Meta data] and [implementation] mapping:
  - `state< S >::rule_t` is `internal::success`
  - `state< S, R >::rule_t` is `internal::state< S, R >`
  - `state< S, R >::subs_t` is `type_list< R >`
  - `state< S, R... >::rule_t` is `internal::state< S, internal::seq< R... > >`
  - `state< S, R... >::subs_t` is `type_list< internal::seq< R... > >`


## Exceptional

These rules throw and/or catch exceptions and are only available when compiling with exception support.

These rules are in namespace `tao::pegtl`.

###### `if_must< R, S... >`

* Attempts to match `R` and depending on the result proceeds with either `must< S... >` or `failure`.
* [Equivalent] to `seq< R, must< S... > >`.
* [Equivalent] to `if_then_else< R, must< S... >, failure >`.
* [Meta data] and [implementation] mapping:
  - `if_must< R >::rule_t` is `internal::if_must< false, R >`
  - `if_must< R >::subs_t` is `type_list< R >`
  - `if_must< R, S... >::rule_t` is `internal::if_must< false, R, S... >`
  - `if_must< R, S... >::subs_t` is `type_list< R, internal::must< S... > >`

Note that the `false` template parameter to `internal::if_must` corresponds to the `failure` in the equivalent description using `if_then_else`.

###### `if_must_else< R, S, T >`

* Attempts to match `R` and depending on the result proceeds with either `must< S >` or `must< T >`.
* [Equivalent] to `if_then_else< R, must< S >, must< T > >`.
* [Meta data] and [implementation] mapping:
  - `if_must_else< R, S, T >::rule_t` is `internal::if_then_else< R, internal::must< S >, internal::must< T > >`
  - `if_must_else< R, S, T >::subs_t` is `type_list< R, internal::must< S >, internal::must< T > >`

###### `list_must< R, S >`

* Matches a non-empty list of `R` separated by `S`.
* Similar to `list< R, S >`, but if there is an `S` it **must** be followed by an `R`.
* [Equivalent] to `seq< R, star< if_must< S, R > > >`.
* [Meta data] and [implementation] mapping:
  - `list_must< R, S >::rule_t` is `internal::seq< R, internal::star< S, internal::must< R > > >`
  - `list_must< R, S >::subs_t` is `type_list< R, internal::star< S, internal::must< R > > >`

###### `list_must< R, S, P >`

* Matches a non-empty list of `R` separated by `S` where each `S` can be padded by `P`.
* Similar to `list< R, S, P >`, but if there is an `S` it **must** be followed by an `R`.
* [Equivalent] to `seq< R, star< if_must< pad< S, P >, R > > >`.
* [Meta data] and [implementation] mapping:
  - `list_must< R, S, P >::rule_t` is `internal::seq< R, internal::star< internal::pad< S, P >, internal::must< R > > >`
  - `list_must< R, S, P >::subs_t` is `type_list< R, internal::star< internal::pad< S, P >, internal::must< R > > >`

###### `must< R... >`

* [Equivalent] to `seq< R... >`, but:
* Converts local failure of `R...` into global failure.
* Calls `raise< R >` for the `R` that failed.
* [Equivalent] to `seq< sor< R, raise< R > >... >`.
* [Meta data] and [implementation] mapping:
  - `must<>::rule_t` is `internal::success`
  - `must< R >::rule_t` is `internal::must< R >`
  - `must< R >::subs_t` is `type_list< R >`
  - `must< R... >::rule_t` is `internal::seq< internal::must< R >... >::rule_t`
  - `must< R... >::subs_t` is `type_list< internal::must< R... > >`

Note that `must` uses a different pattern to handle multiple sub-rules compared to the other `seq`-equivalent rules (which use `rule< seq< R... > >` rather than `seq< rule< R >... >`).

###### `opt_must< R, S... >`

* [Equivalent] to `opt< if_must< R, S... > >`.
* [Equivalent] to `if_then_else< R, must< S... >, success >`.
* [Meta data] and [implementation] mapping:
  - `opt_must< R >::rule_t` is `internal::if_must< true, R >`
  - `opt_must< R >::subs_t` is `type_list< R >`
  - `opt_must< R, S... >::rule_t` is `internal::if_must< true, R, S... >`
  - `opt_must< R, S... >::subs_t` is `type_list< R, internal::must< S... > >`

Note that the `true` template parameter to `internal::if_must` corresponds to the `success` in the equivalent description using `if_then_else`.

###### `raise< T >`

* Generates a *global failure*.
* Calls the control-class' `Control< T >::raise()` static member function.
* `T` *can* be a rule, but it does not have to be a rule.
* Does not consume input.
* [Meta data] and [implementation] mapping:
  - `raise< T >::rule_t` is `internal::raise< T >`

###### `raise_message< C... >`

* Generates a *global failure* with the message given by `C...`.
* Calls the control-class' `Control< raise_message< C... > >::raise()` static member function.
* Does not consume input.
* [Meta data] and [implementation] mapping:
  - `raise_message< C... >::rule_t` is `internal::raise< raise_message< C... > >`

###### `star_must< R, S... >`

* [Equivalent] to `star< if_must< R, S... > >`.
* [Meta data] and [implementation] mapping:
  - `star_must< R >::rule_t` is `internal::star< internal::if_must< false, R > >`
  - `star_must< R >::subs_t` is `type_list< internal::if_must< false, R > >`
  - `star_must< R, S... >::rule_t` is `internal::star< internal::if_must< false, R, S... > >`
  - `star_must< R, S... >::subs_t` is `type_list< internal::if_must< false, R, S... > >`

###### `try_catch_any_raise_nested< R... >`

* [Equivalent] to `seq< R... >`, but:
* Catches exceptions of any type via `catch( ... )` and:
* Throws a new exception with the caught one as nested exception.
* Throws via `Control< R >::raise_nested()` when `R...` is a single rule.
* Throws via `Control< internal::seq< R... > >::raise_nested()` when `R...` is more than one rule.
* [Meta data] and [implementation] mapping:
  - `try_catch_any_raise_nested<>::rule_t` is `internal::success`
  - `try_catch_any_raise_nested< R >::rule_t` is `internal::try_catch_raise_nested< void, R >`
  - `try_catch_any_raise_nested< R >::subs_t` is `type_list< R >`
  - `try_catch_any_raise_nested< R... >::rule_t` is `internal::try_catch_raise_nested< void, internal::seq< R... > >`
  - `try_catch_any_raise_nested< R... >::subs_t` is `type_list< internal::seq< R... > >`

###### `try_catch_any_return_false< E, R... >`

* [Equivalent] to `seq< R... >`, but:
* Catches exceptions of any type via `catch( ... )`, and:
* Converts the global failure (exception) into a local failure (return value `false`).
* [Meta data] and [implementation] mapping:
  - `try_catch_any_return_false< E >::rule_t` is `internal::success`
  - `try_catch_any_return_false< E, R >::rule_t` is `internal::try_catch_return_false< void, R >`
  - `try_catch_any_return_false< E, R >::subs_t` is `type_list< R >`
  - `try_catch_any_return_false< E, R... >::rule_t` is `internal::try_catch_return_false< void, internal::seq< R... > >`
  - `try_catch_any_return_false< E, R... >::subs_t` is `type_list< internal::seq< R... > >`

###### `try_catch_raise_nested< R... >`

* [Equivalent] to `seq< R... >`, but:
* Catches exceptions of type `tao::pegtl::parse_error_base` (or derived), and:
* Throws a new exception with the caught one as nested exception.
* Throws via `Control< R >::raise_nested()` when `R...` is a single rule.
* Throws via `Control< internal::seq< R... > >::raise_nested()` when `R...` is more than one rule.
* [Meta data] and [implementation] mapping:
  - `try_catch_raise_nested<>::rule_t` is `internal::success`
  - `try_catch_raise_nested< R >::rule_t` is `internal::try_catch_raise_nested< parse_error_base, R >`
  - `try_catch_raise_nested< R >::subs_t` is `type_list< R >`
  - `try_catch_raise_nested< R... >::rule_t` is `internal::try_catch_raise_nested< parse_error_base, internal::seq< R... > >`
  - `try_catch_raise_nested< R... >::subs_t` is `type_list< internal::seq< R... > >`

###### `try_catch_return_false< R... >`

* [Equivalent] to `seq< R... >`, but:
* Catches exceptions of type `tao::pegtl::parse_error_base` (or derived), and:
* Converts the global failure (exception) into a local failure (return value `false`).
* [Meta data] and [implementation] mapping:
  - `try_catch_return_false<>::rule_t` is `internal::success`
  - `try_catch_return_false< R >::rule_t` is `internal::try_catch_return_false< parse_error_base, R >`
  - `try_catch_return_false< R >::subs_t` is `type_list< R >`
  - `try_catch_return_false< R... >::rule_t` is `internal::try_catch_return_false< parse_error_base, internal::seq< R... > >`
  - `try_catch_return_false< R... >::subs_t` is `type_list< internal::seq< R... > >`

###### `try_catch_std_raise_nested< R... >`

* [Equivalent] to `seq< R... >`, but:
* Catches exceptions of type `std::exception` (or derived), and:
* Throws a new exception with the caught one as nested exception.
* Throws via `Control< R >::raise_nested()` when `R...` is a single rule.
* Throws via `Control< internal::seq< R... > >::raise_nested()` when `R...` is more than one rule.
* [Meta data] and [implementation] mapping:
  - `try_catch_std_raise_nested<>::rule_t` is `internal::success`
  - `try_catch_std_raise_nested< R >::rule_t` is `internal::try_catch_raise_nested< std::exception, R >`
  - `try_catch_std_raise_nested< R >::subs_t` is `type_list< R >`
  - `try_catch_std_raise_nested< R... >::rule_t` is `internal::try_catch_raise_nested< std::exception, internal::seq< R... > >`
  - `try_catch_std_raise_nested< R... >::subs_t` is `type_list< internal::seq< R... > >`

###### `try_catch_std_return_false< R... >`

* [Equivalent] to `seq< R... >`, but:
* Catches exceptions of type `std::exception` (or derived), and:
* Converts the global failure (exception) into a local failure (return value `false`).
* [Meta data] and [implementation] mapping:
  - `try_catch_std_return_false< E >::rule_t` is `internal::success`
  - `try_catch_std_return_false< E, R >::rule_t` is `internal::try_catch_return_false< std::exception, R >`
  - `try_catch_std_return_false< E, R >::subs_t` is `type_list< R >`
  - `try_catch_std_return_false< E, R... >::rule_t` is `internal::try_catch_return_false< std::exception, internal::seq< R... > >`
  - `try_catch_std_return_false< E, R... >::subs_t` is `type_list< internal::seq< R... > >`

###### `try_catch_type_raise_nested< E, R... >`

* [Equivalent] to `seq< R... >`, but:
* Catches exceptions of type `E` (or derived), and:
* Throws a new exception with the caught one as nested exception.
* Throws via `Control< R >::raise_nested()` when `R...` is a single rule.
* Throws via `Control< internal::seq< R... > >::raise_nested()` when `R...` is more than one rule.
* [Meta data] and [implementation] mapping:
  - `try_catch_type_raise_nested< E >::rule_t` is `internal::success`
  - `try_catch_type_raise_nested< E, R >::rule_t` is `internal::try_catch_raise_nested< E, R >`
  - `try_catch_type_raise_nested< E, R >::subs_t` is `type_list< R >`
  - `try_catch_type_raise_nested< E, R... >::rule_t` is `internal::try_catch_raise_nested< E, internal::seq< R... > >`
  - `try_catch_type_raise_nested< E, R... >::subs_t` is `type_list< internal::seq< R... > >`

###### `try_catch_type_return_false< E, R... >`

* [Equivalent] to `seq< R... >`, but:
* Catches exceptions of type `E` (or derived), and:
* Converts the global failure (exception) into a local failure (return value `false`).
* [Meta data] and [implementation] mapping:
  - `try_catch_type_return_false< E >::rule_t` is `internal::success`
  - `try_catch_type_return_false< E, R >::rule_t` is `internal::try_catch_return_false< E, R >`
  - `try_catch_type_return_false< E, R >::subs_t` is `type_list< R >`
  - `try_catch_type_return_false< E, R... >::rule_t` is `internal::try_catch_return_false< E, internal::seq< R... > >`
  - `try_catch_type_return_false< E, R... >::subs_t` is `type_list< internal::seq< R... > >`

###### `TAO_PEGTL_RAISE_MESSAGE( "..." )`

* Macro where `TAO_PEGTL_RAISE_MESSAGE( "foo" )` yields `raise_message< 'f', 'o', 'o' >`.
* The argument must be a string literal.
* Works for strings up to 512 bytes of length (excluding trailing `'\0'`).


## Incremental

These rules are used in conjunction with [Incremental Input].

Like the other incremental input facilities these rules are **not** automatically included with `<tao/pegtl.hpp>`.
To make them available either include `<tao/pegtl/buffer.hpp>` or the individual include files from the `buffer` subdirectory.

###### `discard`

* [Equivalent] to `success`, but:
* Calls the input's `discard()` member function.
* Must not be used where backtracking to before the `discard` might occur and/or nested within a rule for which an action with input can be called.
* See [Incremental Input] for details.
* [Meta data] and [implementation] mapping:
  - `discard::rule_t` is `discard`

###### `is_buffer`

* Succeeds if the current parsing run uses a buffer input for incremental parsing.
* [Meta data] and [implementation] mapping:
  - `is_buffer::rule_t` is `is_buffer`

###### `require< Num >`

* Succeeds if at least `Num` further input bytes are available,
* will attempt to read enough bytes into the buffer when [Incremental Input] is used.
* [Meta data] and [implementation] mapping:
  - `require< 0 >::rule_t` is `internal::success`
  - `require< N >::rule_t` is `require< N >`


## Compatibility

These rules replicate the intrusive way action invocations were part of the grammar in the PEGTL 0.x.

The actions for these rules are classes, rather than the class templates required by `parse()` and `action<>`.
These rules respect the current `apply_mode`, but do **not** use the Control class to invoke the actions.

These rules are in namespace `tao::pegtl`.

###### `apply< A... >`

* Calls `A::apply()` for all `A`, in order, with an empty input and all states as arguments.
* If any `A::apply()` has a boolean return type and returns `false`, no further `A::apply()` calls are made and the result is equivalent to `failure`, otherwise:
* [Equivalent] to `success` wrt. parsing.
* [Meta data] and [implementation] mapping:
  - `apply< A... >::rule_t` is `internal::apply< A... >`

###### `apply0< A... >`

* Calls `A::apply0()` for all `A`, in order, with all states as arguments.
* If any `A::apply0()` has a boolean return type and returns `false`, no further `A::apply0()` calls are made and the result is equivalent to `failure`, otherwise:
* [Equivalent] to `success` wrt. parsing.
* [Meta data] and [implementation] mapping:
  - `apply0< A... >::rule_t` is `internal::apply0< A... >`

###### `if_apply< R, A... >`

* [Equivalent] to `seq< R, apply< A... > >` wrt. parsing, but also:
* If `R` matches, calls `A::apply()`, for all `A`, in order, with the input matched by `R` and all states as arguments.
* If any `A::apply()` has a boolean return type and returns `false`, no further `A::apply()` calls are made.
* [Meta data] and [implementation] mapping:
  - `if_apply< R, A... >::rule_t` is `internal::if_apply< R, A... >`
  - `if_apply< R, A... >::subs_t` is `type_list< R >`


## ICU Support

These rules depend on the [International Components for Unicode (ICU)](http://icu-project.org/) that provide the means to match specific Unicode character properties.

Because of this external library dependency the ICU rules are **not** automatically included with `tao/pegtl.hpp`.

The convenience ICU rules are supplied for all properties found in ICU version 3.4.
Users of later versions can use the basic rules manually or create their own convenience rules derived from the basic rules for additional enumeration values found in those later versions of the ICU library.

Just as the other Unicode rules, the ICU-based rules are available in multiple versions,

* in namespace `tao::pegtl::utf8::icu` for UTF-8 encoded inputs,
* in namespace alias `tao::pegtl::utf16::icu` for native-endian UTF-16 inputs,
* in namespace alias `tao::pegtl::utf32::icu` for native-endian UTF-32 inputs.
* in namespace `tao::pegtl::utf16_be::icu` for big-endian UTF-16 encoded inputs,
* in namespace `tao::pegtl::utf16_le::icu` for little-endian UTF-16 encoded inputs,
* in namespace `tao::pegtl::utf32_be::icu` for big-endian UTF-32 encoded inputs,
* in namespace `tao::pegtl::utf32_le::icu` for little-endian UTF-32 encoded inputs,
* in namespace `tao::pegtl::unicode::icu` for native-endian input-adaptive rules.

To use these rules it is necessary to provide an include path to the ICU library, to link the application against `libicu`, and to manually include one or more of the following header files:

* `tao/pegtl/icu/utf8.hpp`
* `tao/pegtl/icu/utf16.hpp`
* `tao/pegtl/icu/utf32.hpp`
* `tao/pegtl/icu/unicode.hpp`

### Basic ICU Rules

Each of the above namespaces provides two basic rules for matching binary properties and property value matching for enum properties.

###### `binary_property< P, V >`

* `P` is a binary property defined by ICU, see [`UProperty`](http://icu-project.org/apiref/icu4c/uchar_8h.html).
* `V` is a boolean value.
* Succeeds when the input is not empty, and:
* The next N input objects encode a valid unicode code point, and:
* The code point's property `P`, i.e. [`u_hasBinaryProperty( cp, P )`](http://icu-project.org/apiref/icu4c/uchar_8h.html), equals `V`.
* Consumes the N input objects on success.

###### `binary_property< P >`

* Identical to `binary_property< P, true >`.

###### `property_value< P, V >`

* `P` is an enumerated property defined by ICU, see [`UProperty`](http://icu-project.org/apiref/icu4c/uchar_8h.html).
* `V` is an integer value.
* Succeeds when the input is not empty, and:
* The next N input objects encode a valid unicode code point, and:
* The code point's property `P`, i.e. [`u_getIntPropertyValue( cp, P )`](http://icu-project.org/apiref/icu4c/uchar_8h.html), equals `V`.
* Consumes the N input objects on success.

### ICU Rules for Binary Properties

Convenience wrappers for binary properties.

###### `alphabetic`

* [Equivalent] to `binary_property< UCHAR_ALPHABETIC >`.

###### `ascii_hex_digit`

* [Equivalent] to `binary_property< UCHAR_ASCII_HEX_DIGIT >`.

###### `bidi_control`

* [Equivalent] to `binary_property< UCHAR_BIDI_CONTROL >`.

###### `bidi_mirrored`

* [Equivalent] to `binary_property< UCHAR_BIDI_MIRRORED >`.

###### `case_sensitive`

* [Equivalent] to `binary_property< UCHAR_CASE_SENSITIVE >`.

###### `dash`

* [Equivalent] to `binary_property< UCHAR_DASH >`.

###### `default_ignorable_code_point`

* [Equivalent] to `binary_property< UCHAR_DEFAULT_IGNORABLE_CODE_POINT >`.

###### `deprecated`

* [Equivalent] to `binary_property< UCHAR_DEPRECATED >`.

###### `diacritic`

* [Equivalent] to `binary_property< UCHAR_DIACRITIC >`.

###### `extender`

* [Equivalent] to `binary_property< UCHAR_EXTENDER >`.

###### `full_composition_exclusion`

* [Equivalent] to `binary_property< UCHAR_FULL_COMPOSITION_EXCLUSION >`.

###### `grapheme_base`

* [Equivalent] to `binary_property< UCHAR_GRAPHEME_BASE >`.

###### `grapheme_extend`

* [Equivalent] to `binary_property< UCHAR_GRAPHEME_EXTEND >`.

###### `grapheme_link`

* [Equivalent] to `binary_property< UCHAR_GRAPHEME_LINK >`.

###### `hex_digit`

* [Equivalent] to `binary_property< UCHAR_HEX_DIGIT >`.

###### `hyphen`

* [Equivalent] to `binary_property< UCHAR_HYPHEN >`.

###### `id_continue`

* [Equivalent] to `binary_property< UCHAR_ID_CONTINUE >`.

###### `id_start`

* [Equivalent] to `binary_property< UCHAR_ID_START >`.

###### `ideographic`

* [Equivalent] to `binary_property< UCHAR_IDEOGRAPHIC >`.

###### `ids_binary_operator`

* [Equivalent] to `binary_property< UCHAR_IDS_BINARY_OPERATOR >`.

###### `ids_trinary_operator`

* [Equivalent] to `binary_property< UCHAR_IDS_TRINARY_OPERATOR >`.

###### `join_control`

* [Equivalent] to `binary_property< UCHAR_JOIN_CONTROL >`.

###### `logical_order_exception`

* [Equivalent] to `binary_property< UCHAR_LOGICAL_ORDER_EXCEPTION >`.

###### `lowercase`

* [Equivalent] to `binary_property< UCHAR_LOWERCASE >`.

###### `math`

* [Equivalent] to `binary_property< UCHAR_MATH >`.

###### `nfc_inert`

* [Equivalent] to `binary_property< UCHAR_NFC_INERT >`.

###### `nfd_inert`

* [Equivalent] to `binary_property< UCHAR_NFD_INERT >`.

###### `nfkc_inert`

* [Equivalent] to `binary_property< UCHAR_NFKC_INERT >`.

###### `nfkd_inert`

* [Equivalent] to `binary_property< UCHAR_NFKD_INERT >`.

###### `noncharacter_code_point`

* [Equivalent] to `binary_property< UCHAR_NONCHARACTER_CODE_POINT >`.

###### `pattern_syntax`

* [Equivalent] to `binary_property< UCHAR_PATTERN_SYNTAX >`.

###### `pattern_white_space`

* [Equivalent] to `binary_property< UCHAR_PATTERN_WHITE_SPACE >`.

###### `posix_alnum`

* [Equivalent] to `binary_property< UCHAR_POSIX_ALNUM >`.

###### `posix_blank`

* [Equivalent] to `binary_property< UCHAR_POSIX_BLANK >`.

###### `posix_graph`

* [Equivalent] to `binary_property< UCHAR_POSIX_GRAPH >`.

###### `posix_print`

* [Equivalent] to `binary_property< UCHAR_POSIX_PRINT >`.

###### `posix_xdigit`

* [Equivalent] to `binary_property< UCHAR_POSIX_XDIGIT >`.

###### `quotation_mark`

* [Equivalent] to `binary_property< UCHAR_QUOTATION_MARK >`.

###### `radical`

* [Equivalent] to `binary_property< UCHAR_RADICAL >`.

###### `s_term`

* [Equivalent] to `binary_property< UCHAR_S_TERM >`.

###### `segment_starter`

* [Equivalent] to `binary_property< UCHAR_SEGMENT_STARTER >`.

###### `soft_dotted`

* [Equivalent] to `binary_property< UCHAR_SOFT_DOTTED >`.

###### `terminal_punctuation`

* [Equivalent] to `binary_property< UCHAR_TERMINAL_PUNCTUATION >`.

###### `unified_ideograph`

* [Equivalent] to `binary_property< UCHAR_UNIFIED_IDEOGRAPH >`.

###### `uppercase`

* [Equivalent] to `binary_property< UCHAR_UPPERCASE >`.

###### `variation_selector`

* [Equivalent] to `binary_property< UCHAR_VARIATION_SELECTOR >`.

###### `white_space`

* [Equivalent] to `binary_property< UCHAR_WHITE_SPACE >`.

###### `xid_continue`

* [Equivalent] to `binary_property< UCHAR_XID_CONTINUE >`.

###### `xid_start`

* [Equivalent] to `binary_property< UCHAR_XID_START >`.

### ICU Rules for Enumerated Properties

Convenience wrappers for enumerated properties.

###### `bidi_class< V >`

* `V` is of type `UCharDirection`.
* [Equivalent] to `property_value< UCHAR_BIDI_CLASS, V >`.

###### `block< V >`

* `V` is of type `UBlockCode`.
* [Equivalent] to `property_value< UCHAR_BLOCK, V >`.

###### `decomposition_type< V >`

* `V` is of type `UDecompositionType`.
* [Equivalent] to `property_value< UCHAR_DECOMPOSITION_TYPE, V >`.

###### `east_asian_width< V >`

* `V` is of type `UEastAsianWidth`.
* [Equivalent] to `property_value< UCHAR_EAST_ASIAN_WIDTH, V >`.

###### `general_category< V >`

* `V` is of type `UCharCategory`.
* [Equivalent] to `property_value< UCHAR_GENERAL_CATEGORY, V >`.

###### `grapheme_cluster_break< V >`

* `V` is of type `UGraphemeClusterBreak`.
* [Equivalent] to `property_value< UCHAR_GRAPHEME_CLUSTER_BREAK, V >`.

###### `hangul_syllable_type< V >`

* `V` is of type `UHangulSyllableType`.
* [Equivalent] to `property_value< UCHAR_HANGUL_SYLLABLE_TYPE, V >`.

###### `joining_group< V >`

* `V` is of type `UJoiningGroup`.
* [Equivalent] to `property_value< UCHAR_JOINING_GROUP, V >`.

###### `joining_type< V >`

* `V` is of type `UJoiningType`.
* [Equivalent] to `property_value< UCHAR_JOINING_TYPE, V >`.

###### `line_break< V >`

* `V` is of type `ULineBreak`.
* [Equivalent] to `property_value< UCHAR_LINE_BREAK, V >`.

###### `numeric_type< V >`

* `V` is of type `UNumericType`.
* [Equivalent] to `property_value< UCHAR_NUMERIC_TYPE, V >`.

###### `sentence_break< V >`

* `V` is of type `USentenceBreak`.
* [Equivalent] to `property_value< UCHAR_SENTENCE_BREAK, V >`.

###### `word_break< V >`

* `V` is of type `UWordBreakValues`.
* [Equivalent] to `property_value< UCHAR_WORD_BREAK, V >`.

### ICU Rules for Value Properties

Convenience wrappers for enumerated properties that return a value instead of an actual `enum`.

###### `canonical_combining_class< V >`

* `V` is of type `std::uint8_t`.
* [Equivalent] to `property_value< UCHAR_CANONICAL_COMBINING_CLASS, V >`.

###### `lead_canonical_combining_class< V >`

* `V` is of type `std::uint8_t`.
* [Equivalent] to `property_value< UCHAR_LEAD_CANONICAL_COMBINING_CLASS, V >`.

###### `trail_canonical_combining_class< V >`

* `V` is of type `std::uint8_t`.
* [Equivalent] to `property_value< UCHAR_TRAIL_CANONICAL_COMBINING_CLASS, V >`.


## Index

* [`action< A, R... >`](#action-a-r-) <sup>[(controlling)](#controlling)</sup>
* [`alnum`](#alnum) <sup>[(ascii)](#ascii)</sup>
* [`alpha`](#alpha) <sup>[(ascii)](#ascii)</sup>
* [`alphabetic`](#alphabetic) <sup>[(icu rules)](#icu-rules-for-binary-properties)</sup>
* [`any`](#any) <sup>[(ascii)](#ascii)</sup>
* [`any`](#any-1) <sup>[(unicode)](#unicode)</sup>
* [`any`](#any-2) <sup>[(binary)](#binary)</sup>
* [`any7`](#any7) <sup>[(ascii)](#ascii)</sup>
* [`apply< A... >`](#apply-a-) <sup>[(compat)](#compatibility)</sup>
* [`apply0< A... >`](#apply0-a-) <sup>[(compat)](#compatibility)</sup>
* [`ascii_hex_digit`](#ascii_hex_digit) <sup>[(icu rules)](#icu-rules-for-binary-properties)</sup>
* [`at< R... >`](#at-r-) <sup>[(combinators)](#combinators)</sup>
* [`bidi_class< V >`](#bidi_class-v-) <sup>[(icu rules)](#icu-rules-for-enumerated-properties)</sup>
* [`bidi_control`](#bidi_control) <sup>[(icu rules)](#icu-rules-for-binary-properties)</sup>
* [`bidi_mirrored`](#bidi_mirrored) <sup>[(icu rules)](#icu-rules-for-binary-properties)</sup>
* [`binary_property< P >`](#binary_property-p-) <sup>[(icu rules)](#basic-icu-rules)</sup>
* [`binary_property< P, V >`](#binary_property-p-v-) <sup>[(icu rules)](#basic-icu-rules)</sup>
* [`blank`](#blank) <sup>[(ascii)](#ascii)</sup>
* [`block< V >`](#block-v-) <sup>[(icu rules)](#icu-rules-for-enumerated-properties)</sup>
* [`bof`](#bof) <sup>[(atomic)](#atomic)</sup>
* [`bol`](#bol) <sup>[(atomic)](#atomic)</sup>
* [`bom`](#bom) <sup>[(unicode)](#unicode)</sup>
* [`canonical_combining_class< V >`](#canonical_combining_class-v-) <sup>[(icu rules)](#icu-rules-for-value-properties)</sup>
* [`case_sensitive`](#case_sensitive) <sup>[(icu rules)](#icu-rules-for-binary-properties)</sup>
* [`cntrl`](#cntrl) <sup>[(ascii)](#ascii)</sup>
* [`consume< Num >`](#consume-num-) <sup>[(atomic)](#atomic)</sup>
* [`control< C, R... >`](#control-c-r-) <sup>[(controlling)](#controlling)</sup>
* [`cr`](#cr) <sup>[(ascii)](#ascii)</sup>
* [`cr`](#cr-1) <sup>[(unicode)](#unicode)</sup>
* [`cr_crlf`](#cr_crlf) <sup>[(ascii)](#ascii)</sup>
* [`cr_crlf`](#cr_crlf-1) <sup>[(unicode)](#unicode)</sup>
* [`cr_lf`](#cr_lf) <sup>[(ascii)](#ascii)</sup>
* [`cr_lf`](#cr_lf-1) <sup>[(unicode)](#unicode)</sup>
* [`cr_lf_crlf`](#cr_lf_crlf) <sup>[(ascii)](#ascii)</sup>
* [`cr_lf_crlf`](#cr_lf_crlf-1) <sup>[(unicode)](#unicode)</sup>
* [`crlf`](#crlf) <sup>[(ascii)](#ascii)</sup>
* [`crlf`](#crlf-1) <sup>[(unicode)](#unicode)</sup>
* [`dash`](#dash) <sup>[(icu rules)](#icu-rules-for-binary-properties)</sup>
* [`decomposition_type< V >`](#decomposition_type-v-) <sup>[(icu rules)](#icu-rules-for-enumerated-properties)</sup>
* [`default_ignorable_code_point`](#default_ignorable_code_point) <sup>[(icu rules)](#icu-rules-for-binary-properties)</sup>
* [`deprecated`](#deprecated) <sup>[(icu rules)](#icu-rules-for-binary-properties)</sup>
* [`diacritic`](#diacritic) <sup>[(icu rules)](#icu-rules-for-binary-properties)</sup>
* [`digit`](#digit) <sup>[(ascii)](#ascii)</sup>
* [`disable< R... >`](#disable-r-) <sup>[(controlling)](#controlling)</sup>
* [`discard`](#discard) <sup>[(buffer rules)](#buffer-rules)</sup>
* [`east_asian_width< V >`](#east_asian_width-v-) <sup>[(icu rules)](#icu-rules-for-enumerated-properties)</sup>
* [`enable< R... >`](#enable-r-) <sup>[(controlling)](#controlling)</sup>
* [`eof`](#eof) <sup>[(atomic)](#atomic)</sup>
* [`eol`](#eol) <sup>[(atomic)](#atomic)</sup>
* [`eol1`](#eol1) <sup>[(unicode)](#unicode)</sup>
* [`eolf`](#eolf) <sup>[(atomic)](#atomic)</sup>
* [`eolu`](#eolu) <sup>[(unicode)](#unicode)</sup>
* [`esc`](#esc) <sup>[(ascii)](#ascii)</sup>
* [`everything`](#everything) <sup>[(atomic)](#atomic)</sup>
* [`extender`](#extender) <sup>[(icu rules)](#icu-rules-for-binary-properties)</sup>
* [`failure`](#failure) <sup>[(atomic)](#atomic)</sup>
* [`ff`](#ff) <sup>[(ascii)](#ascii)</sup>
* [`full_composition_exclusion`](#full_composition_exclusion) <sup>[(icu rules)](#icu-rules-for-binary-properties)</sup>
* [`function< F >`](#function-f-) <sup>[(atomic)](#atomic)</sup>
* [`function< M, F >`](#function-m-f-) <sup>[(member)](#member)</sup>
* [`general_category< V >`](#general_category-v-) <sup>[(icu rules)](#icu-rules-for-enumerated-properties)</sup>
* [`graph`](#graph) <sup>[(ascii)](#ascii)</sup>
* [`grapheme_base`](#grapheme_base) <sup>[(icu rules)](#icu-rules-for-binary-properties)</sup>
* [`grapheme_cluster_break< V >`](#grapheme_cluster_break-v-) <sup>[(icu rules)](#icu-rules-for-enumerated-properties)</sup>
* [`grapheme_extend`](#grapheme_extend) <sup>[(icu rules)](#icu-rules-for-binary-properties)</sup>
* [`grapheme_link`](#grapheme_link) <sup>[(icu rules)](#icu-rules-for-binary-properties)</sup>
* [`hangul_syllable_type< V >`](#hangul_syllable_type-v-) <sup>[(icu rules)](#icu-rules-for-enumerated-properties)</sup>
* [`hex_digit`](#hex_digit) <sup>[(icu rules)](#icu-rules-for-binary-properties)</sup>
* [`ht`](#ht) <sup>[(ascii)](#ascii)</sup>
* [`hyphen`](#hyphen) <sup>[(icu rules)](#icu-rules-for-binary-properties)</sup>
* [`id_continue`](#id_continue) <sup>[(icu rules)](#icu-rules-for-binary-properties)</sup>
* [`id_start`](#id_start) <sup>[(icu rules)](#icu-rules-for-binary-properties)</sup>
* [`identifier_first`](#identifier_first) <sup>[(ascii)](#ascii)</sup>
* [`identifier_other`](#identifier_other) <sup>[(ascii)](#ascii)</sup>
* [`identifier`](#identifier) <sup>[(ascii)](#ascii)</sup>
* [`ideographic`](#ideographic) <sup>[(icu rules)](#icu-rules-for-binary-properties)</sup>
* [`ids_binary_operator`](#ids_binary_operator) <sup>[(icu rules)](#icu-rules-for-binary-properties)</sup>
* [`ids_trinary_operator`](#ids_trinary_operator) <sup>[(icu rules)](#icu-rules-for-binary-properties)</sup>
* [`if_apply< R, A... >`](#if_apply-r-a-) <sup>[(compat)](#compatibility)</sup>
* [`if_must< R, S... >`](#if_must-r-s-) <sup>[(exceptional)](#exceptional)</sup>
* [`if_must_else< R, S, T >`](#if_must_else-r-s-t-) <sup>[(exceptional)](#exceptional)</sup>
* [`if_then_else< R, S, T >`](#if_then_else-r-s-t-) <sup>[(convenience)](#convenience)</sup>
* [`ione< C... >`](#ione-c-) <sup>[(ascii)](#ascii)</sup>
* [`is_buffer`](#is_buffer) <sup>[(buffer rules)](#buffer-rules)</sup>
* [`istring< C... >`](#istring-c-) <sup>[(ascii)](#ascii)</sup>
* [`join_control`](#join_control) <sup>[(icu rules)](#icu-rules-for-binary-properties)</sup>
* [`joining_group< V >`](#joining_group-v-) <sup>[(icu rules)](#icu-rules-for-enumerated-properties)</sup>
* [`joining_type< V >`](#joining_type-v-) <sup>[(icu rules)](#icu-rules-for-enumerated-properties)</sup>
* [`keyword< C... >`](#keyword-c-) <sup>[(ascii)](#ascii)</sup>
* [`lead_canonical_combining_class< V >`](#lead_canonical_combining_class-v-) <sup>[(icu rules)](#icu-rules-for-value-properties)</sup>
* [`lf`](#lf) <sup>[(ascii)](#ascii)</sup>
* [`lf`](#lf-1) <sup>[(unicode)](#unicode)</sup>
* [`lfcr`](#lfcr) <sup>[(ascii)](#ascii)</sup>
* [`lfcr`](#lfcr-1) <sup>[(unicode)](#unicode)</sup>
* [`line_break< V >`](#line_break-v-) <sup>[(icu rules)](#icu-rules-for-enumerated-properties)</sup>
* [`list< R, S >`](#list-r-s-) <sup>[(convenience)](#convenience)</sup>
* [`list< R, S, P >`](#list-r-s-p-) <sup>[(convenience)](#convenience)</sup>
* [`list_must< R, S >`](#list_must-r-s-) <sup>[(exceptional)](#exceptional)</sup>
* [`list_must< R, S, P >`](#list_must-r-s-p-) <sup>[(exceptional)](#exceptional)</sup>
* [`list_tail< R, S >`](#list_tail-r-s-) <sup>[(convenience)](#convenience)</sup>
* [`list_tail< R, S, P >`](#list_tail-r-s-p-) <sup>[(convenience)](#convenience)</sup>
* [`logical_order_exception`](#logical_order_exception) <sup>[(icu rules)](#icu-rules-for-binary-properties)</sup>
* [`lower`](#lower) <sup>[(ascii)](#ascii)</sup>
* [`lowercase`](#lowercase) <sup>[(icu rules)](#icu-rules-for-binary-properties)</sup>
* [`ls`](#ls) <sup>[(unicode)](#unicode)</sup>
* [`many< Num >`](#many-num-) <sup>[(ascii)](#ascii)</sup>
* [`many< Num >`](#many-num--1) <sup>[(unicode)](#unicode)</sup>
* [`many< Num >`](#many-num--2) <sup>[(binary)](#binary)</sup>
* [`many7< Num >`](#many7-num-) <sup>[(ascii)](#ascii)</sup>
* [`mask_not_one< M, C... >`](#mask_not_one-m-c-) <sup>[(binary)](#binary)</sup>
* [`mask_not_range< M, C, D >`](#mask_not_range-m-c-d-) <sup>[(binary)](#binary)</sup>
* [`mask_one< M, C... >`](#mask_one-m-c-) <sup>[(binary)](#binary)</sup>
* [`mask_range< M, C, D >`](#mask_range-m-c-d-) <sup>[(binary)](#binary)</sup>
* [`mask_ranges< M, C1, D1, C2, D2, ... >`](#mask_ranges-m-c1-d1-c2-d2--) <sup>[(binary)](#binary)</sup>
* [`mask_ranges< M, C1, D1, C2, D2, ..., E >`](#mask_ranges-m-c1-d1-c2-d2--e-) <sup>[(binary)](#binary)</sup>
* [`mask_string< M, C... >`](#mask_string-m-c-) <sup>[(binary)](#binary)</sup>
* [`math`](#math) <sup>[(icu rules)](#icu-rules-for-binary-properties)</sup>
* [`minus< M, S >`](#minus-m-s-) <sup>[(convenience)](#convenience)</sup>
* [`must< R... >`](#must-r-) <sup>[(exceptional)](#exceptional)</sup>
* [`nel`](#nel) <sup>[(unicode)](#unicode)</sup>
* [`nested< M, R >`](#nested-m-r-) <sup>[(member)](#member)</sup>
* [`nfc_inert`](#nfc_inert) <sup>[(icu rules)](#icu-rules-for-binary-properties)</sup>
* [`nfd_inert`](#nfd_inert) <sup>[(icu rules)](#icu-rules-for-binary-properties)</sup>
* [`nfkc_inert`](#nfkc_inert) <sup>[(icu rules)](#icu-rules-for-binary-properties)</sup>
* [`nfkd_inert`](#nfkd_inert) <sup>[(icu rules)](#icu-rules-for-binary-properties)</sup>
* [`noncharacter_code_point`](#noncharacter_code_point) <sup>[(icu rules)](#icu-rules-for-binary-properties)</sup>
* [`not_at< R... >`](#not_at-r-) <sup>[(combinators)](#combinators)</sup>
* [`not_ione< C... >`](#not_ione-c-) <sup>[(ascii)](#ascii)</sup>
* [`not_one< C... >`](#not_one-c-) <sup>[(ascii)](#ascii)</sup>
* [`not_one< C... >`](#not_one-c--1) <sup>[(unicode)](#unicode)</sup>
* [`not_one< C... >`](#not_one-c--2) <sup>[(binary)](#binary)</sup>
* [`not_one< M, U... >`](#not_one-m-u-) <sup>[(member)](#member)</sup>
* [`not_one7< C... >`](#not_one7-c-) <sup>[(ascii)](#ascii)</sup>
* [`not_range< C, D >`](#not_range-c-d-) <sup>[(ascii)](#ascii)</sup>
* [`not_range< C, D >`](#not_range-c-d--1) <sup>[(unicode)](#unicode)</sup>
* [`not_range< C, D >`](#not_range-c-d--2) <sup>[(binary)](#binary)</sup>
* [`not_range< M, U, V >`](#not_range-m-u-v-) <sup>[(member)](#member)</sup>
* [`not_range7< C, D >`](#not_range7-c-d-) <sup>[(ascii)](#ascii)</sup>
* [`nul`](#nul) <sup>[(ascii)](#ascii)</sup>
* [`numeric_type< V >`](#numeric_type-v-) <sup>[(icu rules)](#icu-rules-for-enumerated-properties)</sup>
* [`one< C... >`](#one-c-) <sup>[(ascii)](#ascii)</sup>
* [`one< C... >`](#one-c--1) <sup>[(unicode)](#unicode)</sup>
* [`one< C... >`](#one-c--2) <sup>[(binary)](#binary)</sup>
* [`one< M, U... >`](#one-m-u-) <sup>[(member)](#member)</sup>
* [`opt< R... >`](#opt-r-) <sup>[(combinators)](#combinators)</sup>
* [`opt_must< R, S...>`](#opt_must-r-s-) <sup>[(exceptional)](#exceptional)</sup>
* [`pad< R, S, T = S >`](#pad-r-s-t--s-) <sup>[(convenience)](#convenience)</sup>
* [`pad_opt< R, P >`](#pad_opt-r-p-) <sup>[(convenience)](#convenience)</sup>
* [`partial< R... >`](#partial-r-) <sup>[(convenience)](#convenience)</sup>
* [`pattern_syntax`](#pattern_syntax) <sup>[(icu rules)](#icu-rules-for-binary-properties)</sup>
* [`pattern_white_space`](#pattern_white_space) <sup>[(icu rules)](#icu-rules-for-binary-properties)</sup>
* [`plus< R... >`](#plus-r-) <sup>[(combinators)](#combinators)</sup>
* [`posix_alnum`](#posix_alnum) <sup>[(icu rules)](#icu-rules-for-binary-properties)</sup>
* [`posix_blank`](#posix_blank) <sup>[(icu rules)](#icu-rules-for-binary-properties)</sup>
* [`posix_graph`](#posix_graph) <sup>[(icu rules)](#icu-rules-for-binary-properties)</sup>
* [`posix_print`](#posix_print) <sup>[(icu rules)](#icu-rules-for-binary-properties)</sup>
* [`posix_xdigit`](#posix_xdigit) <sup>[(icu rules)](#icu-rules-for-binary-properties)</sup>
* [`print`](#print) <sup>[(ascii)](#ascii)</sup>
* [`property_value< P, V >`](#property_value-p-v-) <sup>[(icu rules)](#basic-icu-rules)</sup>
* [`ps`](#ps) <sup>[(unicode)](#unicode)</sup>
* [`quotation_mark`](#quotation_mark) <sup>[(icu rules)](#icu-rules-for-binary-properties)</sup>
* [`radical`](#radical) <sup>[(icu rules)](#icu-rules-for-binary-properties)</sup>
* [`raise< T >`](#raise-t-) <sup>[(exceptional)](#exceptional)</sup>
* [`raise_message< C... >`](#raise_message-c-) <sup>[(exceptional)](#exceptional)</sup>
* [`range< C, D >`](#range-c-d-) <sup>[(ascii)](#ascii)</sup>
* [`range< C, D >`](#range-c-d--1) <sup>[(unicode)](#unicode)</sup>
* [`range< C, D >`](#range-c-d--2) <sup>[(binary)](#binary)</sup>
* [`range< M, U, V >`](#range-m-u-v-) <sup>[(member)](#member)</sup>
* [`ranges< C1, D1, C2, D2, ... >`](#ranges-c1-d1-c2-d2--) <sup>[(ascii)](#ascii)</sup>
* [`ranges< C1, D1, C2, D2, ... >`](#ranges-c1-d1-c2-d2---1) <sup>[(unicode)](#unicode)</sup>
* [`ranges< C1, D1, C2, D2, ... >`](#ranges-c1-d1-c2-d2---2) <sup>[(binary)](#binary)</sup>
* [`ranges< M, U1, V1, U2, V2, ... >`](#ranges-m-u1-v1-u2-v2--) <sup>[(member)](#member)</sup>
* [`ranges< C1, D1, C2, D2, ..., E >`](#ranges-c1-d1-c2-d2--e-) <sup>[(ascii)](#ascii)</sup>
* [`ranges< C1, D1, C2, D2, ..., E >`](#ranges-c1-d1-c2-d2--e--1) <sup>[(unicode)](#unicode)</sup>
* [`ranges< C1, D1, C2, D2, ..., E >`](#ranges-c1-d1-c2-d2--e--2) <sup>[(binary)](#binary)</sup>
* [`ranges< M, U1, V1, U2, V2, ..., W >`](#ranges-m-u1-v1-u2-v2--w-) <sup>[(member)](#member)</sup>
* [`rematch< R, S... >`](#rematch-r-s-) <sup>[(convenience)](#convenience)</sup>
* [`rep< Num, R... >`](#rep-num-r-) <sup>[(convenience)](#convenience)</sup>
* [`rep_max< Max, R... >`](#rep_max-max-r-) <sup>[(convenience)](#convenience)</sup>
* [`rep_min< Min, R... >`](#rep_min-min-r-) <sup>[(convenience)](#convenience)</sup>
* [`rep_min_max< Min, Max, R... >`](#rep_min_max-min-max-r-) <sup>[(convenience)](#convenience)</sup>
* [`rep_opt< Num, R... >`](#rep_opt-num-r-) <sup>[(convenience)](#convenience)</sup>
* [`require< Num >`](#require-num-) <sup>[(buffer-rules)](#buffer-rules)</sup>
* [`s_term`](#s_term) <sup>[(icu rules)](#icu-rules-for-binary-properties)</sup>
* [`segment_starter`](#segment_starter) <sup>[(icu rules)](#icu-rules-for-binary-properties)</sup>
* [`sentence_break< V >`](#sentence_break-v-) <sup>[(icu rules)](#icu-rules-for-enumerated-properties)</sup>
* [`seq< R... >`](#seq-r-) <sup>[(combinators)](#combinators)</sup>
* [`seven`](#seven) <sup>[(ascii)](#ascii)</sup>
* [`shebang`](#shebang) <sup>[(ascii)](#ascii)</sup>
* [`soft_dotted`](#soft_dotted) <sup>[(icu rules)](#icu-rules-for-binary-properties)</sup>
* [`sor< R... >`](#sor-r-) <sup>[(combinators)](#combinators)</sup>
* [`sp`](#sp) <sup>[(ascii)](#ascii)</sup>
* [`space`](#space) <sup>[(ascii)](#ascii)</sup>
* [`star< R... >`](#star-r-) <sup>[(combinators)](#combinators)</sup>
* [`star_must< R, S... >`](#star_must-r-s-) <sup>[(exceptional)](#exceptional)</sup>
* [`star_partial< R... >`](#star_partial-r-) <sup>[(convenience)](#convenience)</sup>
* [`star_strict< R... >`](#star_strict-r-) <sup>[(convenience)](#convenience)</sup>
* [`state< S, R... >`](#state-s-r-) <sup>[(controlling)](#controlling)</sup>
* [`strict< R... >`](#strict-r-) <sup>[(convenience)](#convenience)</sup>
* [`string< C... >`](#string-c-) <sup>[(ascii)](#ascii)</sup>
* [`string< C... >`](#string-c--1) <sup>[(unicode)](#unicode)</sup>
* [`string< C... >`](#string-c--2) <sup>[(binary)](#binary)</sup>
* [`string< M, U... >`](#string-m-u-) <sup>[(member)](#member)</sup>
* [`success`](#success) <sup>[(atomic)](#atomic)</sup>
* [`TAO_PEGTL_ISTRING( "..." )`](#tao_pegtl_istring--) <sup>[(ascii)](#ascii)</sup>
* [`TAO_PEGTL_KEYWORD( "..." )`](#tao_pegtl_keyword--) <sup>[(ascii)](#ascii)</sup>
* [`TAO_PEGTL_RAISE_MESSAGE( "..." )`](#tao_pegtl_raise_message--) <sup>[(exceptional)](#exceptional)</sup>
* [`TAO_PEGTL_STRING( "..." )`](#tao_pegtl_string--) <sup>[(ascii)](#ascii)</sup>
* [`terminal_punctuation`](#terminal_punctuation) <sup>[(icu rules)](#icu-rules-for-binary-properties)</sup>
* [`three< C >`](#three-c-) <sup>[(ascii)](#ascii)</sup>
* [`trail_canonical_combining_class< V >`](#trail_canonical_combining_class-v-) <sup>[(icu rules)](#icu-rules-for-value-properties)</sup>
* [`try_catch_any_raise_nested< R... >`](#try_catch_any_raise_nested-r-) <sup>[(exceptional)](#exceptional)</sup>
* [`try_catch_any_return_false< R... >`](#try_catch_any_return_false-r-) <sup>[(exceptional)](#exceptional)</sup>
* [`try_catch_raise_nested< R... >`](#try_catch_raise_nested-r-) <sup>[(exceptional)](#exceptional)</sup>
* [`try_catch_return_false< R... >`](#try_catch_return_false-r-) <sup>[(exceptional)](#exceptional)</sup>
* [`try_catch_std_raise_nested< R... >`](#try_catch_std_raise_nested-r-) <sup>[(exceptional)](#exceptional)</sup>
* [`try_catch_std_return_false< R... >`](#try_catch_std_return_false-r-) <sup>[(exceptional)](#exceptional)</sup>
* [`try_catch_type_raise_nested< E, R... >`](#try_catch_type_raise_nested-e-r-) <sup>[(exceptional)](#exceptional)</sup>
* [`try_catch_type_return_false< E, R... >`](#try_catch_type_return_false-e-r-) <sup>[(exceptional)](#exceptional)</sup>
* [`two< C >`](#two-c-) <sup>[(ascii)](#ascii)</sup>
* [`unified_ideograph`](#unified_ideograph) <sup>[(icu rules)](#icu-rules-for-binary-properties)</sup>
* [`until< R >`](#until-r-) <sup>[(convenience)](#convenience)</sup>
* [`until< R, S... >`](#until-r-s-) <sup>[(convenience)](#convenience)</sup>
* [`upper`](#upper) <sup>[(ascii)](#ascii)</sup>
* [`uppercase`](#uppercase) <sup>[(icu rules)](#icu-rules-for-binary-properties)</sup>
* [`variation_selector`](#variation_selector) <sup>[(icu rules)](#icu-rules-for-binary-properties)</sup>
* [`vt`](#vt) <sup>[(ascii)](#ascii)</sup>
* [`white_space`](#white_space) <sup>[(icu rules)](#icu-rules-for-binary-properties)</sup>
* [`word_break< V >`](#word_break-v-) <sup>[(icu rules)](#icu-rules-for-enumerated-properties)</sup>
* [`xdigit`](#xdigit) <sup>[(ascii)](#ascii)</sup>
* [`xid_continue`](#xid_continue) <sup>[(icu rules)](#icu-rules-for-binary-properties)</sup>
* [`xid_start`](#xid_start) <sup>[(icu rules)](#icu-rules-for-binary-properties)</sup>

---

This document is part of the [PEGTL](https://github.com/taocpp/PEGTL).

Copyright (c) 2014-2024 Dr. Colin Hirsch and Daniel Frey<br>
Distributed under the Boost Software License, Version 1.0<br>
See accompanying file [LICENSE_1_0.txt](../LICENSE_1_0.txt) or copy at https://www.boost.org/LICENSE_1_0.txt

[ASCII rules]: #ascii
[Equivalent]: #equivalence
[implementation]: #implementation
[Incremental Input]: Inputs-and-Parsing.md#incremental-input
[Meta data]: Meta-Data-and-Visit.md
[PEG]: https://en.wikipedia.org/wiki/Parsing_expression_grammar
