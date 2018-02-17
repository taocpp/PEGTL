# Parse Tree

## Introduction

The PEGTL supports building a [parse tree](https://en.wikipedia.org/wiki/Parse_tree) / [AST](https://en.wikipedia.org/wiki/Abstract_syntax_tree) by including the following header:

```c++
#include <tao/pegtl/contrib/parse_tree.hpp>
```

It provides

* The basic infrastructure to build a parse tree.
* Builds a full parse tree by default.
* A default node class to hold parse tree information.
* Supports custom node classes.
* Supports a selector class template to choose which nodes will be stored in the parse tree.
* Each selector specialization for a node may have an optional transformation method which allows modifying the current node (including its child nodes).

> The parse tree / AST part of the PEGTL is currently in active development and serves as a prove-of-concept, expect changes at any time. Try it out, experiment with it, and most importantly let us know what you think of it. We need **your** feedback!

## Full Parse Tree

If you want to get a full parse tree, you call the `tao::pegtl::parse_tree::parse()`-method with only your grammar and an input:

```c++
auto root = tao::pegtl::parse_tree::parse< my_grammar >( in );
```

The result is a `std::unique_ptr< tao::pegtl::parse_tree::node >`. The pointer is empty when the input did not match the grammar, otherwise it contains the root node of the resulting parse tree. Intermediate nodes from rules which did not match will be removed automatically.

## Partial Parse Tree

A full parse tree is often too much, one does not need *every* node in a grammar, only the main nodes are important. This is supported by adding another template parameter to the `parse_tree::parse` call. The second template parameter `S` is a class template. For each rule that matched, the compiler checks whether `S<Rule>::value` is `true`. You can then select the rules that should generate nodes from your grammar like this:

```c++
template< typename Rule > struct my_selector : std::false_type {};
template<> struct my_selector< my_rule_1 > : std::true_type {};
template<> struct my_selector< my_rule_2 > : std::true_type {};
template<> struct my_selector< my_rule_3 > : std::true_type {};

// ...

auto root = tao::pegtl::parse_tree::parse< my_grammar, my_selector >( in );
```

The above style is a white-list, where the default is `std::false_type` and you explicitly list those rules which will generate a node. Of course, you can also set the default to `std::true_type` and explicitly list the rules which will *not* generate a node (black-list style).

## Transforming Nodes

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

There is another option not shown in the example: You can call `n.reset()` before returning from `transform()`. This prevents the node from being added to its parent. It removes the node (as well as all of its children) from the generated parse tree.

## `tao::pegtl::parse_tree::node`

This is the default node class used by `tao::pegtl::parse_tree::parse` if no custom node class is specified. In that case, the root node, as well as all (nested) child nodes provide the following interface:

```c++
struct node
{
   std::vector< std::unique_ptr< node > > children;

   bool is_root() const noexcept();

   // precondition from here on: !is_root()

   std::string name() const;

   bool has_content() const noexcept();
   std::string content() const;  // precondition: has_content()

   std::string source() const;

   // useful for transform:
   void remove_content();
};
```

The name is the demangled name of the rule. By default all nodes (except the root node) can provide the content that matched, i.e. the part of the input that the rule the node was created for matched. You only need to check `has_content()` if you previously used `remove_content()` in your transform methods.

See the [`parse_tree.cpp`](https://github.com/taocpp/PEGTL/blob/master/src/example/pegtl/parse_tree.cpp)-example for more information how to output (or otherwise use) the nodes.

## Custom Node Class

If you need more control over how data is stored/handled in the nodes, you can provide your own node class. You can add it to the parse call as an additional template parameter after the rule:

```c++
auto r1 = tao::pegtl::parse_tree::parse< my_grammar, my_node >( in );
auto r2 = tao::pegtl::parse_tree::parse< my_grammar, my_node, my_selector >( in );
```

Note that `my_node` is a class type while `my_selector` is a class template. If you provide your own node class, it must provide the following interface:

```c++
struct my_node
{
   // it must be default constructible
   my_node() = default;

   // no copy/move is necessary
   // (nodes are always owned/handled by a std::unique_ptr)
   my_node( const my_node& ) = delete;
   my_node( my_node&& ) = delete;

   // it must be destructible
   ~my_node() = default;

   // no assignment necessary
   my_node& operator=( const my_node& ) = delete;
   my_node& operator=( my_node&& ) = delete;

   // all non-root nodes are initialized by calling this method
   template< typename Rule, typename Input >
   void start( const Input& in );

   // if parsing of the rule succeeded, this method is called
   template< typename Rule, typename Input >
   void success( const Input& in );

   // if parsing succeeded and the (optional) transform call
   // did not discard the node, it is appended to its parent.
   // note that "child" is the node whose Rule just succeeded
   // and *this is the parent where the node should be appended.
   void append( std::unique_ptr< my_node > child );
};
```

Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
