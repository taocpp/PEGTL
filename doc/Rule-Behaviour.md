# Rule Behaviour

These tables show how groups of related combinators differ by giving examples of their matching behaviour.

We assume that the rule `x` is defined as `one< 'x' >` for all reasonable choices of `x`.
We also assume that we are in namespace `tao::pegtl`.

In these tables a quoted string indicates that the rule matched on the input and shows which part of the input was consumed.
The letter 'f' stands for a "local failure" where the rule returns `false` while the letter 'E' is for a "global failure" where the rule throws an exception.

## Simple Combinators

|  | "" | "a" | "ab" | "abc" | "d" | "ad" | "abd" |
|--|--|--|--|--|--|--|--|
| `seq< a, b, c >` | f | f | f | "abc" | f | f | f |
| `opt< a, b, c >` | "" | "" | "" | "abc" | "" | "" | "" |
| `strict< a, b, c >` | "" | f | f | "abc" | "" | f | f |
| `partial< a, b, c >` | "" | "a" | "ab" | "abc" | "" | "a" | "ab" |
| `if_must< a, b, c >` | f | E | E | "abc" | f | E | E |

## Iterating Combinators

|  | "" | "a" | "ab" | "abc" | "d" | "ad" | "abd" |
|--|--|--|--|--|--|--|--|
| `plus< a, b, c >` | f | f | f | "abc" | f | f | f |
| `star< a, b, c >` | "" | "" | "" | "abc" | "" | "" | "" |
| `star_strict< a, b, c >` | "" | f | f | "abc" | "" | f | f |
| `star_partial< a, b, c >` | "" | "a" | "ab" | "abc" | "" | "a" | "ab" |
| `star_must< a, b, c >` | "" | E | E | "abc" | "" | E | E |

|  | "abca" | "abcd" | "abcabc" | "abcabca" | "abcabcb" |
|--|--|--|--|--|--|
| `plus< a, b, c >` | "abc" | "abc" | "abcabc" | "abcabc" | "abcabc" |
| `star< a, b, c >` | "abc" | "abc" | "abcabc" | "abcabc" | "abcabc" |
| `star_strict< a, b, c >` | f | "abc" | "abcabc" | f | "abcabc" |
| `star_partial< a, b, c >` | "abca" | "abc" | "abcabc" | "abcabca" | "abcabc" |
| `star_must< a, b, c >` | E | "abc" | "abcabc" | E | "abcabc" |

This document is part of the [PEGTL](https://github.com/taocpp/PEGTL).

Copyright (c) 2023-2024 Dr. Colin Hirsch and Daniel Frey
Distributed under the Boost Software License, Version 1.0<br>
See accompanying file [LICENSE_1_0.txt](../LICENSE_1_0.txt) or copy at https://www.boost.org/LICENSE_1_0.txt
