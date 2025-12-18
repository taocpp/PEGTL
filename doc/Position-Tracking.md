# Position Tracking

During a parsing run the PEGTL tracks progress by updating a pointer to the current input object (or past the last input object for end-of-file).


For error reporting this notion of the current position in the input is frequently insufficient since presenting the user with "parse error at address 0x16f45b998" 



Lazy vs. Eager vs. Trivial





Static vs. dynamic position information.


Line Endings

For tracking line numbers in text-based inputs there are three tracking modes that determine how line endings are detected.

### Lazy Tracking



### Eager Scan Tracking

The eager scan tracking mode scans the input consumed by every successful rule match for a single code unit that is recognized as line ending.
The position information is part of the inputand is kept up-to-date during the parsing run.

To choose this tracking mode use a rule from the `scan` sub-namespace as `Eol` template parameter of the Input.
These namespace are e.g. `tao::pegtl::(ascii::)::scan` or `tao::pegtl::utf8::scan`.

These rules define the type alias `eol_char_rule` that must be the rule that matches the single code unit that is recognized as line ending.

Note that this tracking mode greatly restricts the choice of line endings.
For example accepting both a single LF or a single CR as per `(ascii::)cr_lf` is not permissible because there is no single code unit that can be scanned for.

Accepting both a single CR or a CRLF pair as per `(ascii::)cr_crlf` is not permissible even though there is a single code unit that can be scanned for, the CR.
That is because if some rule matches and consumes a CR then the line number is incremnted and the column number is reset.
If however the next rule matches and consumes an LF then that consumed input is scanned separately and will -- wrongly -- increment the column number.

Fortunately the common case of either a single LF or a CRLF pair as per `(ascii::)lf_crlf` *does* work with eager scan tracking as the line number is only incremented, and the column number only reset, after the LF has been consumed.
And matching the CR of a CRLF pair will increment the column number which is only slightly weird and does not lead to wrong column numbers down the line, no pun intended.

Note that in some cases the PEGTL can determine at compile time that a rule can not match the line ending code unit and will optimize away the scanning of the consumed input.
See `tao/pegtl/internal/text_scan_traits.hpp` for details.

### Eager Rule Tracking

The eager rule tracking mode **only** recognizes a line ending when the `eol` rule matches (or the `eolf` rule matches on a non-empty input).
Like for eager scan tracking the position information is part of the input and is kept up-to-date during the parsing run.

This tracking mode is very efficient and easy to understand.
There are no requirements or restrictions on the `Eol` template parameter of the Input beyond the obvious -- must not itself use `eol` or `eolf`.

An important disadvantage is that line endings in the input that are incidentally matched by any *other* rule do not increment the line number.
Consequently a grammar that is used with this tracking mode must be carefully designed to prevent other rules from matching a line ending.



| Lazy | Eager Scan | Eager Rule |
| ---- | ---------- | ---------- |


File Names



---

This document is part of the [PEGTL](https://github.com/taocpp/PEGTL).

Copyright (c) 2024-2025 Dr. Colin Hirsch and Daniel Frey<br>
Distributed under the Boost Software License, Version 1.0<br>
See accompanying file [LICENSE_1_0.txt](../LICENSE_1_0.txt) or copy at https://www.boost.org/LICENSE_1_0.txt
