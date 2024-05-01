# Rule Behaviour

These tables show how groups of related combinators differ by giving examples of their matching behaviour.

We assume that the rule `x` is defined as `one< 'x' >` for all reasonable choices of `x`.
We also assume that we are in namespace `tao::pegtl`.

Remember that "success" corresponds to a return value of `true`, "local failure" is a return value of `false`, and "global failure" means that an exception, usually of type `parse_error`, is thrown.

## Simple Combinators

| Rule | Input | Result | Matched |
|------|-------|--------|---------|
| `seq< a, b, c >` | "" | local failure | |
| `opt< a, b, c >` | "" | **success** | "" |
| `strict< a, b, c >` | "" | **success** | "" |
| `partial< a, b, c >` | "" | **success** | "" |
| `if_must< a, b, c >` | "" | local failure | |
| `seq< a, b, c >` | "a" | local failure | |
| `opt< a, b, c >` | "a" | **success** | "" |
| `strict< a, b, c >` | "a" | local failure | |
| `partial< a, b, c >` | "a" | **success** | "a" |
| `if_must< a, b, c >` | "a" | global failure | |
| `seq< a, b, c >` | "ab" | local failure | |
| `opt< a, b, c >` | "ab" | **success** | "" |
| `strict< a, b, c >` | "ab" | local failure | |
| `partial< a, b, c >` | "ab" | **success** | "ab" |
| `if_must< a, b, c >` | "ab" | global failure | |
| `seq< a, b, c >` | "abc" | **success** | "abc" |
| `opt< a, b, c >` | "abc" | **success** | "abc" |
| `strict< a, b, c >` | "abc" | **success** | "abc" |
| `partial< a, b, c >` | "abc" | **success** | "abc" |
| `if_must< a, b, c >` | "abc" | **success** | "abc" |
| `seq< a, b, c >` | "d" | local failure | |
| `opt< a, b, c >` | "d" | **success** | "" |
| `strict< a, b, c >` | "d" | **success** | "" |
| `partial< a, b, c >` | "d" | **success** | "" |
| `if_must< a, b, c >` | "d" | local failure | |
| `seq< a, b, c >` | "ad" | local failure | |
| `opt< a, b, c >` | "ad" | **success** | "" |
| `strict< a, b, c >` | "ad" | local failure | |
| `partial< a, b, c >` | "ad" | **success** | "a" |
| `if_must< a, b, c >` | "ad" | global failure | |
| `seq< a, b, c >` | "abd" | local failure | |
| `opt< a, b, c >` | "abd" | **success** | "" |
| `strict< a, b, c >` | "abd" | local failure | |
| `partial< a, b, c >` | "abd" | **success** | "ab" |
| `if_must< a, b, c >` | "abd" | global failure | |

## Iterating Combinators

| Rule | Input | Result | Matched |
|------|-------|--------|---------|
| `plus< a, b, c >` | "" | local failure | |
| `star< a, b, c >` | "" | **success** | "" |
| `star_strict< a, b, c >` | "" | **success** | "" |
| `star_partial< a, b, c >` | "" | **success** | "" |
| `star_must< a, b, c >` | "" | **success** | "" |
| `plus< a, b, c >` | "a" | local failure | |
| `star< a, b, c >` | "a" | **success** | "" |
| `star_strict< a, b, c >` | "a" | local failure | |
| `star_partial< a, b, c >` | "a" | **success** | "a" |
| `star_must< a, b, c >` | "a" | global failure | |
| `plus< a, b, c >` | "ab" | local failure | |
| `star< a, b, c >` | "ab" | **success** | "" |
| `star_strict< a, b, c >` | "ab" | local failure | |
| `star_partial< a, b, c >` | "ab" | **success** | "ab" |
| `star_must< a, b, c >` | "ab" | global failure | |
| `plus< a, b, c >` | "abc" | **success** | "abc" |
| `star< a, b, c >` | "abc" | **success** | "abc" |
| `star_strict< a, b, c >` | "abc" | **success** | "abc" |
| `star_partial< a, b, c >` | "abc" | **success** | "abc" |
| `star_must< a, b, c >` | "abc" | **success** | "abc" |
| `plus< a, b, c >` | "abca" | **success** | "abc" |
| `star< a, b, c >` | "abca" | **success** | "abc" |
| `star_strict< a, b, c >` | "abca" | local failure | |
| `star_partial< a, b, c >` | "abca" | **success** | "abca" |
| `star_must< a, b, c >` | "abca" | global failure | |
| `plus< a, b, c >` | "d" | local failure | |
| `star< a, b, c >` | "d" | **success** | "" |
| `star_strict< a, b, c >` | "d" | **success** | "" |
| `star_partial< a, b, c >` | "d" | **success** | "" |
| `star_must< a, b, c >` | "d" | **success** | "" |
| `plus< a, b, c >` | "ad" | local failure | |
| `star< a, b, c >` | "ad" | **success** | "" |
| `star_strict< a, b, c >` | "ad" | local failure | |
| `star_partial< a, b, c >` | "ad" | **success** | "a" |
| `star_must< a, b, c >` | "ad" | global failure | |
| `plus< a, b, c >` | "abd" | local failure | |
| `star< a, b, c >` | "abd" | **success** | "" |
| `star_strict< a, b, c >` | "abd" | local failure | |
| `star_partial< a, b, c >` | "abd" | **success** | "ab" |
| `star_must< a, b, c >` | "abd" | global failure | |
| `plus< a, b, c >` | "abcd" | **success** | "abc" |
| `star< a, b, c >` | "abcd" | **success** | "abc" |
| `star_strict< a, b, c >` | "abcd" | **success** | "abc" |
| `star_partial< a, b, c >` | "abcd" | **success** | "abc" |
| `star_must< a, b, c >` | "abcd" | **success** | "abc" |
| `plus< a, b, c >` | "abcabc" | **success** | "abcabc" |
| `star< a, b, c >` | "abcabc" | **success** | "abcabc" |
| `star_strict< a, b, c >` | "abcabc" | **success** | "abcabc" |
| `star_partial< a, b, c >` | "abcabc" | **success** | "abcabc" |
| `star_must< a, b, c >` | "abcabc" | **success** | "abcabc" |
| `plus< a, b, c >` | "abcabca" | **success** | "abcabc" |
| `star< a, b, c >` | "abcabca" | **success** | "abcabc" |
| `star_strict< a, b, c >` | "abcabca" | local failure | |
| `star_partial< a, b, c >` | "abcabca" | **success** | "abcabca" |
| `star_must< a, b, c >` | "abcabca" | global failure | |
| `plus< a, b, c >` | "abcabcb" | **success** | "abcabc" |
| `star< a, b, c >` | "abcabcb" | **success** | "abcabc" |
| `star_strict< a, b, c >` | "abcabcb" | **success** | "abcabc" |
| `star_partial< a, b, c >` | "abcabcb" | **success** | "abcabc" |
| `star_must< a, b, c >` | "abcabcb" | **success** | "abcabc" |

This document is part of the [PEGTL](https://github.com/taocpp/PEGTL).

Copyright (c) 2023-2024 Dr. Colin Hirsch and Daniel Frey
Distributed under the Boost Software License, Version 1.0<br>
See accompanying file [LICENSE_1_0.txt](../LICENSE_1_0.txt) or copy at https://www.boost.org/LICENSE_1_0.txt
