# Example Tables

The following tables show how groups of related combinators differ by giving examples of their matching behaviour.

We assume that the rule `x` is defined as `one< 'x' >` for all reasonable choices of `x` and that we are in namespace `tao::pegtl`.

## Simple Combinators

| Rule | Input | Result | Matched |
|------|-------|--------|---------|
| `seq< a, b, c >` | "" | false | |
| `opt< a, b, c >` | "" | **true** | "" |
| `strict< a, b, c >` | "" | **true** | "" |
| `partial< a, b, c >` | "" | **true** | "" |
| `if_must< a, b, c >` | "" | false | |
| `seq< a, b, c >` | "a" | false | |
| `opt< a, b, c >` | "a" | **true** | "" |
| `strict< a, b, c >` | "a" | false | |
| `partial< a, b, c >` | "a" | **true** | "a" |
| `if_must< a, b, c >` | "a" | error | |
| `seq< a, b, c >` | "ab" | false | |
| `opt< a, b, c >` | "ab" | **true** | "" |
| `strict< a, b, c >` | "ab" | false | |
| `partial< a, b, c >` | "ab" | **true** | "ab" |
| `if_must< a, b, c >` | "ab" | error | |
| `seq< a, b, c >` | "abc" | **true** | "abc" |
| `opt< a, b, c >` | "abc" | **true** | "abc" |
| `strict< a, b, c >` | "abc" | **true** | "abc" |
| `partial< a, b, c >` | "abc" | **true** | "abc" |
| `if_must< a, b, c >` | "abc" | **true** | "abc" |
| `seq< a, b, c >` | "abca" | **true** | "abc" |
| `opt< a, b, c >` | "abca" | **true** | "abc" |
| `strict< a, b, c >` | "abca" | **true** | "abc" |
| `partial< a, b, c >` | "abca" | **true** | "abc" |
| `if_must< a, b, c >` | "abca" | **true** | "abc" |
| `seq< a, b, c >` | "d" | false | |
| `opt< a, b, c >` | "d" | **true** | "" |
| `strict< a, b, c >` | "d" | **true** | "" |
| `partial< a, b, c >` | "d" | **true** | "" |
| `if_must< a, b, c >` | "d" | false | |
| `seq< a, b, c >` | "ad" | false | |
| `opt< a, b, c >` | "ad" | **true** | "" |
| `strict< a, b, c >` | "ad" | false | |
| `partial< a, b, c >` | "ad" | **true** | "a" |
| `if_must< a, b, c >` | "ad" | error | |
| `seq< a, b, c >` | "abd" | false | |
| `opt< a, b, c >` | "abd" | **true** | "" |
| `strict< a, b, c >` | "abd" | false | |
| `partial< a, b, c >` | "abd" | **true** | "ab" |
| `if_must< a, b, c >` | "abd" | error | |
| `seq< a, b, c >` | "abcd" | **true** | "abc" |
| `opt< a, b, c >` | "abcd" | **true** | "abc" |
| `strict< a, b, c >` | "abcd" | **true** | "abc" |
| `partial< a, b, c >` | "abcd" | **true** | "abc" |
| `if_must< a, b, c >` | "abcd" | **true** | "abc" |

## Iterating Combinators

| Rule | Input | Result | Matched |
|------|-------|--------|---------|
| `plus< a, b, c >` | "" | false | |
| `star< a, b, c >` | "" | **true** | "" |
| `star_strict< a, b, c >` | "" | **true** | "" |
| `star_partial< a, b, c >` | "" | **true** | "" |
| `star_must< a, b, c >` | "" | **true** | "" |
| `plus< a, b, c >` | "a" | false | |
| `star< a, b, c >` | "a" | **true** | "" |
| `star_strict< a, b, c >` | "a" | false | |
| `star_partial< a, b, c >` | "a" | **true** | "a" |
| `star_must< a, b, c >` | "a" | error | |
| `plus< a, b, c >` | "ab" | false | |
| `star< a, b, c >` | "ab" | **true** | "" |
| `star_strict< a, b, c >` | "ab" | false | |
| `star_partial< a, b, c >` | "ab" | **true** | "ab" |
| `star_must< a, b, c >` | "ab" | error | |
| `plus< a, b, c >` | "abc" | **true** | "abc" |
| `star< a, b, c >` | "abc" | **true** | "abc" |
| `star_strict< a, b, c >` | "abc" | **true** | "abc" |
| `star_partial< a, b, c >` | "abc" | **true** | "abc" |
| `star_must< a, b, c >` | "abc" | **true** | "abc" |
| `plus< a, b, c >` | "abca" | **true** | "abc" |
| `star< a, b, c >` | "abca" | **true** | "abc" |
| `star_strict< a, b, c >` | "abca" | false | |
| `star_partial< a, b, c >` | "abca" | **true** | "abca" |
| `star_must< a, b, c >` | "abca" | error | |
| `plus< a, b, c >` | "d" | false | |
| `star< a, b, c >` | "d" | **true** | "" |
| `star_strict< a, b, c >` | "d" | **true** | "" |
| `star_partial< a, b, c >` | "d" | **true** | "" |
| `star_must< a, b, c >` | "d" | **true** | "" |
| `plus< a, b, c >` | "ad" | false | |
| `star< a, b, c >` | "ad" | **true** | "" |
| `star_strict< a, b, c >` | "ad" | false | |
| `star_partial< a, b, c >` | "ad" | **true** | "a" |
| `star_must< a, b, c >` | "ad" | error | |
| `plus< a, b, c >` | "abd" | false | |
| `star< a, b, c >` | "abd" | **true** | "" |
| `star_strict< a, b, c >` | "abd" | false | |
| `star_partial< a, b, c >` | "abd" | **true** | "ab" |
| `star_must< a, b, c >` | "abd" | error | |
| `plus< a, b, c >` | "abcd" | **true** | "abc" |
| `star< a, b, c >` | "abcd" | **true** | "abc" |
| `star_strict< a, b, c >` | "abcd" | **true** | "abc" |
| `star_partial< a, b, c >` | "abcd" | **true** | "abc" |
| `star_must< a, b, c >` | "abcd" | **true** | "abc" |
| `plus< a, b, c >` | "abcabc" | **true** | "abcabc" |
| `star< a, b, c >` | "abcabc" | **true** | "abcabc" |
| `star_strict< a, b, c >` | "abcabc" | **true** | "abcabc" |
| `star_partial< a, b, c >` | "abcabc" | **true** | "abcabc" |
| `star_must< a, b, c >` | "abcabc" | **true** | "abcabc" |
| `plus< a, b, c >` | "abcabca" | **true** | "abcabc" |
| `star< a, b, c >` | "abcabca" | **true** | "abcabc" |
| `star_strict< a, b, c >` | "abcabca" | false | |
| `star_partial< a, b, c >` | "abcabca" | **true** | "abcabca" |
| `star_must< a, b, c >` | "abcabca" | error | |
| `plus< a, b, c >` | "abcabcb" | **true** | "abcabc" |
| `star< a, b, c >` | "abcabcb" | **true** | "abcabc" |
| `star_strict< a, b, c >` | "abcabcb" | **true** | "abcabc" |
| `star_partial< a, b, c >` | "abcabcb" | **true** | "abcabc" |
| `star_must< a, b, c >` | "abcabcb" | **true** | "abcabc" |

This document is part of the [PEGTL](https://github.com/taocpp/PEGTL).

Copyright (c) 2023 Dr. Colin Hirsch and Daniel Frey
Distributed under the Boost Software License, Version 1.0<br>
See accompanying file [LICENSE_1_0.txt](../LICENSE_1_0.txt) or copy at https://www.boost.org/LICENSE_1_0.txt
