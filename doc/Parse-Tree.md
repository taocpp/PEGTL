# Parse Tree

## Introduction

The PEGTL supports building a [parse tree](https://en.wikipedia.org/wiki/Parse_tree) / [AST](https://en.wikipedia.org/wiki/Abstract_syntax_tree) by including the following header:

```c++
#include <tao/pegtl/contrib/parse_tree.hpp>
```

This provides

* The basic infrastructure to build a parse tree.
* Builds a full parse tree by default.
* Supports a selector class template to choose which nodes will be stored in the parse tree.
* Each selector specialization for a node may have an optional transformation method which allows modifying the current node (including its child nodes).


> The parse tree / AST part of the PEGTL is currently in active development and serves as a prove-of-concept, expect changes at any time. Try it out, experiment with it, and most importantly let us know what you think of it. We need **your** feedback!

## Full Parse Tree

If you want to get a full parse tree, you call the `tao::pegtl::parse_tree::parse()`-method with only your grammar and an input:

```c++
auto root = tao::pegtl::parse_tree::parse< my_grammar >( in );
```

The result is a `std::unique_ptr< tao::pegtl::parse_tree::node >`. The pointer is empty when the input did not match the grammar, otherwise it contains the root node of the resulting parse tree. Intermediate nodes from rules which did not match will be removed automatically.

## `tao::pegtl::tao::pegtl::parse_tree::node`

The root node, as well as all (nested) child nodes provide the following interface:

```c++
struct node
{
   std::vector< std::unique_ptr< node > > children;
   const std::type_info* id;
   tao::pegtl::internal::iterator begin;
   tao::pegtl::internal::iterator end;
   std::string source;
};
```

The root node contains an empty `id`, i.e. `id == nullptr`. Otherwise, `id` points to the type info of the grammar rule that matched/generated this node. `begin` and `end` denote the position of the match, as well as a `const char*` to the input that matched. See the [`parse_tree.cpp`](https://github.com/taocpp/PEGTL/blob/master/src/example/pegtl/parse_tree.cpp)-example for more information how to output (or otherwise use) the nodes.

> Note that Parse Tree / AST support is still in development, when the above will stabilize it is likely that we won't use `internal::iterator` or store a *copy* of the source in each node. The code currently focuses on functionality, structure, and ease of use - not efficiency.

## Partial Parse Tree

A full parse tree is often too much, one does not need *every* node in a grammar, only the main nodes are important. This is supported by adding another template parameter to the `parse_tree::parse` call. The second template parameter `S` is a class template, for each rule that matched to compiler checks if `S<Rule>::value` is `true`. Hence, you can select the rules to generate nodes from your grammar like this:

```c++
template< typename Rule > struct my_selector : std::false_type {};
template<> struct my_selector< my_rule_1 > : std::true_type {};
template<> struct my_selector< my_rule_2 > : std::true_type {};
template<> struct my_selector< my_rule_3 > : std::true_type {};

// ...

auto root = tao::pegtl::parse_tree::parse< my_grammar, my_selector >( in );
```

The above style is a white-list, where the default is `std::false_type` and you explicitly list those rules which will generate a node. Of course, you can also set the default to `std::true_type` and explicitly list the rules which will *not* generate a node (black-list style).

## Transform

A parse tree, even a partial one, might still be too closely related to the actual grammar. In order to simplify the tree (or otherwise improve its structure), an optional `transform()` method can be added to each specialization. This methods gets passed a modifiable `std::unique_ptr< node >` to the current node from where you have access to the children but not to the parent:

```c++
template<> struct my_selector< my_rule_2 > : std::true_type
{
   static void transform( std::unique_ptr< node >& n )
   {
      // modify n...
   }
};
```

You can transform `n` in almost any way you can imagine, the [`parse_tree.cpp`](https://github.com/taocpp/PEGTL/blob/master/src/example/pegtl/parse_tree.cpp)-example shows two techniques for marking nodes as "content-less" and for actual transformations of the parse tree into an AST.

There is another option not shown in the example: You can call `n.reset()` before returning from `transform()`, this will prevent the node from being added to its parent. It removes the node (as well as all of its children) from the generated parse tree.

Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
