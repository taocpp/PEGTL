# Input Reference

The reference documention for all inputs.

### Included

All inputs except for the buffer inputs for incremental parsing are automatically included with `tao/pegtl.hpp`.
The buffer inputs are documented together with the other facilities related to incremental parsing.

### Text Inputs

The text inputs us a `text_position` that tracks column and line numbers in addition to the count of parsed objects (usually of type `char`).
The other inputs also have an `Eol` template parameter but only use that for the `eol` and `eolf` rules, not for position tracking.

### Namespaces

All inputs reside in namespace `tao::pegtl`.
This default can be changed via the macro `TAO_PEGTL_NAMESPACE` in `tao/pegtl/config.hpp`.


## Contents

* [Argv Input](#argv-input)
* [Base Input](#base-input)
* [View Input](#view-input)
* [Copy Input](#copy-input)
* [File Input](#file-input)
* [Read Input](#read-input)
* [Mmap Input](#mmap-input)
* [Text View Input](#text-view-input)
* [Text Copy Input](#text-copy-input)
* [Text File Input](#text-file-input)
* [Text Read Input](#text-read-input)
* [Text Mmap Input](#text-mmap-input)


## Argv Input


## Base input


## View Input


## Copy Input


## File Input


## Read Input


## Mmap Input


## Text View Input


## Text Copy Input


## Text File Input


## Text Read Input


## Text Mmap Input

---

This document is part of the [PEGTL](https://github.com/taocpp/PEGTL).

Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey<br>
Distributed under the Boost Software License, Version 1.0<br>
See accompanying file [LICENSE_1_0.txt](../LICENSE_1_0.txt) or copy at https://www.boost.org/LICENSE_1_0.txt
