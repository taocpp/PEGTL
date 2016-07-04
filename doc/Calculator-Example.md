# Calculator Example

The PEGTL includes an example that shows how to implement a calculator that parses arithmetic expressions given as command line arguments. Here is the full source code of [`examples/calculator.cc`](https://github.com/ColinH/PEGTL/blob/master/examples/calculator.cc).

The implementation uses `long` integers as data type for all calculations, and understands all binary arithmetic operators from the C programming language.
In addition, round brackets can be used to change the evaluation order.

```sh
$ build/examples/calculator  "2 + 3 * -7"  "(2 + 3) * 7"
-19
35
```

In this example the grammar takes a bit of a second place behind the infrastructure for the actions required to actually evaluate the arithmetic expressions.
The basic approach is "shift-reduce", which is very close to a stack machine, which is a model often well suited to PEGTL grammar actions:
Some actions merely push something onto a stack, while other actions apply some functions to the objects on the stack, usually reducing its size.

Copyright (c) 2014-2016 Dr. Colin Hirsch and Daniel Frey
