# C Left-Right Parser (libcleri)
Languange parser for the C program language.

---------------------------------------
  * [Installation](#installation)
  * [Related projects](#related-projects)
  * [Quick usage](#quick-usage)
  * [API](#api)
    * [cleri_object_t](#cleri_object_t)
    * [cleri_grammar_t](#cleri_grammar_t)
    * [cleri_parse_t](#cleri_parse_t)
    * [cleri_node_t](#cleri_node_t)
    * [cleri_children_t](#cleri_children_t)
  * [Elements](#elements)
    * [Forward reference](#forward-reference)
    * [cleri_dup_t](#cleri_dup_t)
    * [cleri_keyword_t](#cleri_keyword_t)
    * [cleri_regex_t](#cleri_regex_t)
    * [cleri_choice_t](#cleri_choice_t)
    * [cleri_sequence_t](#cleri_sequence_t)
    * [cleri_optional_t](#cleri_optional_t)
  * [Miscellaneous functions](#miscellaneous-functions)

---------------------------------------

## Installation
>Note: libcleri requires [pcre](http://www.pcre.org/)

Install debug or release version, in this example we will install the release version.
```
$ cd Release
```

Compile libcleri
```
$ make all
```

Install libcleri
```
$ sudo make install
```

> Note: run `sudo make uninstall` for removal.

## Related projects
- [pyleri](https://github.com/transceptor-technology/pyleri): Python parser (can export grammar to pyleri, cleri and jsleri)
- [jsleri](https://github.com/transceptor-technology/jsleri): JavaScript parser
- [goleri](https://github.com/transceptor-technology/goleri): Go parser

## Quick usage
>The recommended way to create a static grammar is to use [pyleri](https://github.com/transceptor-technology/pyleri) for
>writing the language and from there export the language to libcleri or other languages.

This is a simple example using libcleri:
```c
#include <stdio.h>
#include <cleri/object.h>

void test_str(cleri_grammar_t * grammar, const char * str)
{
    cleri_parse_t * pr = cleri_parse(grammar, str);
    printf("Test string '%s': %s\n", str, pr->is_valid ? "true" : "false");
    cleri_parse_free(pr);
}

int main(void)
{
    /* define grammar */
    cleri_object_t * k_hi = cleri_keyword(0, "hi", 0);
    cleri_object_t * r_name = cleri_regex(0, "^(?:\"(?:[^\"]*)\")+");
    cleri_object_t * start = cleri_sequence(0, 2, k_hi, r_name);

    /* compile grammar */
    cleri_grammar_t * my_grammar = cleri_grammar(start, NULL);

    /* test some strings */
    test_str(my_grammar, "hi \"Iris\"");  // true
    test_str(my_grammar, "bye \"Iris\""); // false

    /* cleanup grammar */
    cleri_grammar_free(my_grammar);

    return 0;
}
```

## API

### `cleri_object_t`
Cleri object is the base object for each element.

*Public members*
- `uint32_t gid`: Global Identifier for the element. This GID is not required and
as a rule it should be set to 0 if not used. You can use the GID for identifiying
an element in a parse result. When exporting a Pyleri grammar, each named element
will have set the GID be default. (readonly)
- `cleri_object_tp tp`: Type for the object. (readonly)
    - `CLERI_TP_SEQUENCE`
    - `CLERI_TP_OPTIONAL`
    - `CLERI_TP_CHOICE`
    - `CLERI_TP_LIST`
    - `CLERI_TP_REPEAT`
    - `CLERI_TP_PRIO`
    - `CLERI_TP_RULE`
    - `CLERI_TP_THIS`
    - `CLERI_TP_KEYWORD`
    - `CLERI_TP_TOKEN`
    - `CLERI_TP_TOKENS`
    - `CLERI_TP_REGEX`
    - `CLERI_TP_END_OF_STATEMENT`
- `cleri_object_via_t via`: Object. (readonly)
    - `cleri_sequence_t * sequence`
    - `cleri_optional_t * optional`
    - `cleri_choice_t * choice`
    - `cleri_list_t * list`
    - `cleri_repeat_t * repeat`
    - `cleri_prio_t * prio`
    - `cleri_rule_t * rule`
    - `cleri_keyword_t * keyword`
    - `cleri_regex_t * regex`
    - `cleri_token_t * token`
    - `cleri_tokens_t * tokens`
    - `void * dummy` (place holder, this, eof)

#### `cleri_object_t * cleri_object_new(uint32_t gid, cleri_object_tp tp, cleri_free_object_t free_object, cleri_parse_object_t parse_object)`
Create and return a new cleri object. A unique gid is not required but can help
you wih identifiying the element in a [parse result](#cleri_parse_t). As a rule
you should assign 0 in case no specific  This function should only be used in case
you want to create your own custom element.

#### `void cleri_object_incref(cleri_object_t * cl_object)`
Increment the reference counter for an object. Should only be used in case you
want to write your own custom element.

#### `void cleri_object_decref(cleri_object_t * cl_object)`
Decrement the reference counter for an object. If no references are left the
object will be destroyed. Do not use this function after the element has
successfully been added to another element or grammar. Should only be used in
case you want to write your own custom element.

#### `int cleri_object_free(cleri_object_t * cl_object)`
Decrement referenct counter for a cleri object. When the no more references are
left the object will be destroyed. Use this function to cleanup after errors
have occurred. Do not use this function after the element has successfully been
added to another element or grammar.

Example strict error handling:
```c
cleri_grammar_t * compile_grammar(void)
{
    cleri_object_t * k_hello = cleri_keyword(0, "hello", 0);
    if (k_hello == NULL) {
        return NULL;
    }
    cleri_object_t * k_world = cleri_keyword(0, "world", 0);
    if (k_world == NULL) {
        cleri_object_free(k_hello); // must cleanup k_hello
        return NULL;
    }
    cleri_object_t * hello_world = cleri_sequence(0, 2, k_hello, k_world);
    if (start == NULL) {
        cleri_object_free(k_hello);
        cleri_object_free(k_world);
        return NULL;
    }
    cleri_object_t * opt = cleri_optional(0, hello_world);
    if (opt == NULL) {
        /* we now must only cleanup hello_world since this sequence will
        * cleanup both keywords too. */
        cleri_object_free(hello_world);
        return NULL;
    }
    cleri_grammar_t * grammar = cleri_grammar(opt, NULL);
    if (grammar == NULL) {
        cleri_object_free(opt);
    }
    /* when your program has finished, the grammar including all elements can
     * be destroyed using cleri_grammar_free() */
    return grammar;
}
```
>Note: Usually grammar is only compiled at the startup of your program so
>memory allocation errors during the grammar creation are unlikely to occur.
>If NULL is parsed as an argument instead of an element, then the function
>to which the argument is parsed to, will return NULL. Following this
>chain the final grammar returns NULL in case somewhere an error has occurred.
>In this case you should usually abort the program.

### `cleri_grammar_t`
Compiled libcleri grammar.

*No public members*

#### `cleri_grammar_t * cleri_grammar(cleri_object_t * start, const char * re_keywords)`
Create and return a compiled grammar. Argument `start` must be the entry element
for the grammar. Argument `re_keywords` should be a regular expression starting
with character `^` for matching keywords in a grammar. When a grammar is created,
each defined [keyword](#cleri_keyword_t) should match this regular expression.
`re_keywords` is allowed to be `NULL` in which case the defualt
`CLERI_DEFAULT_RE_KEYWORDS` is used.

#### `void cleri_grammar_free(cleri_grammar_t * grammar)`
Cleanup grammar. This will also destroy all elements which are used by the
grammar. Make sure all parse results are destroyed before destroying the grammar
because a [parse result](#cleri_parse_t) depends on elements from the grammar.

### `cleri_parse_t`
Parse result containing the parse tree and other information about the parse
result.

*Public members*
- `int cleri_parse_t.is_valid`: Boolean. Value is 1 (TRUE) in case the parse string is valid or 0 (FALSE) if not. (readonly)
- `size_t cleri_parse_t.pos`: Position in the string to where the string was successfully parsed. This value is (readonly)
equal to the length of the string in case `cleri_parse_t.is_valid` is TRUE. (readonly)
- `const char * cleri_parse_t.str`: Pointer to the provided string. (readonly)
- `cleri_node_t * tree`: Parse tree. (see [cleri_node_t](#cleri_node_t) and [cleri_children_t](#cleri_children_t)) (readonly)
- `const cleri_olist_t * expect`: Linked list to possible elements at position `cleri_parse_t.pos` in `cleri_parse_t.str`.
(see [cleri_olist_t](#cleri_olist_t) for more information)

#### `cleri_parse_t * cleri_parse(cleri_grammar_t * grammar, const char * str)`
Create and return a parse result. The parse result contains pointers to the
provided string (`str`) so make sure the string is available while using the
parse result.

#### `void cleri_parse_free(cleri_parse_t * pr)`
Cleanup a parse result.

#### `void cleri_parse_expect_start(cleri_parse_t * pr)`
Can be used to reset the expect list to start. Usually you are not required to
use this function since the expect list is already at the start position.

### `cleri_node_t`
Node obeject. A parse result has a parse tree which consists of nodes. Each node
may have children.

*Public members*
- `const char * cleri_node_t.str`: Pointer to the position in the parse string where this node starts. (readonly)
- `size_t cleri_node_t.len`: Length of the string which is applicable for this node. (readonly)
- `cleri_object_t * cleri_node_t.cl_obj`: Element from the grammar which matches this node. (readonly)
- `cleri_children_t * cleri_node_t.children`: Optional children for this node. (readonly)

#### `bool cleri_node_has_children(cleri_node_t * node)`
Macro function for checking if a node has children.

### `cleri_children_t`
Children from a node in a linked list.

*Public members*
- `cleri_node_t * cleri_children_t.node`: Child node. (readonly)
- `struct cleri_children_s * cleri_children_t.next`: Next child node or `NULL` if there are no other childs. (readonly)

Example looping over all children within a node:
```c
/* we asume having a node (cleri_node_t*) */
if (cleri_node_has_children(node)) {
    cleri_children_t * child = node->children;
    while (child != NULL) {
        // do something with child->node
        child = child->next;
    }
}
```

### `cleri_olist_t`
Linked list holding libcleri objects. A `cleri_olist_t` type is used for
expected elements in a parse result.

*Public members*
- `cleri_object_t * cl_obj`: Object (holding an element, readonly)
- `cleri_olist_t * next`: Next object. (readonly)

Example looping over `cleri_parse_t.expect`:
```c
/* we asume having a pr (cleri_parse_t*)
 *
 * Notes:
 *    pr->expect is NULL if nothing is expected and it is save to
 *    change pr->expect. If required the linked list can be reset to start
 *    using cleri_parse_expect_start(). */
while (pr->expect != NULL) {
    // do something with pr->expect->cl_obj
    pr->expect = pr->expect->next;
}
```
## Elements
Elements are objects used to define a grammar.

### `Forward reference`
Forward reference to a libcleri object. There is no specific type for a
reference.

>Warning: A reference is not protected against testing the same position in
>in a string. This could potentially lead to an infinite loop.
>For example:
>```c
>cleri_ref_set(ref, cleri_optional(0, ref)); // DON'T DO THIS
>```
>Use [prio](#cleri_prio_t) if such recursive constrution is required.

#### `cleri_object_t * cleri_ref(void)`
Create and return a new [object](#cleri_object_t) as reference element.
Once the reference is created, it can be used as element in you grammar. Do not
forget to actualle set the reference using `cleri_ref_set()`.

#### `void cleri_ref_set(cleri_object_t * ref, cleri_object_t * cl_obj)`
Set a reference. For every created forward reference, this function must be
called exactly once. Argument `ref` must be created with `cleri_ref()`. Argument
`cl_obj` cannot be used outside the reference. Since the reference becomes
the `cl_obj`, it is the reference you should use.

Example
```c
/* define grammar */
cleri_object_t * ref = cleri_ref();
cleri_object_t * choice = cleri_choice(
    0, 0, 2, cleri_keyword(0, "ni", 0), ref);

cleri_ref_set(ref, cleri_sequence(
    0,
    3,
    cleri_token(0, "["),
    cleri_list(0, choice, cleri_token(0, ","), 0, 0, 0),
    cleri_token(0, "]")));

/* create grammar */
cleri_grammar_t * grammar = cleri_grammar(ref, NULL);

/* parse some test string */
cleri_parse_t * pr = cleri_parse(grammar, "[ni, ni, [ni, [], [ni, ni]]]");
printf("Valid: %s\n", pr->is_valid ? "true" : "false"); // true

/* cleanup */
cleri_parse_free(pr);
cleri_grammar_free(grammar);
```

### `cleri_dup_t`
Duplicate an object. The type is an extension to `cleri_object_t`.

#### `cleri_object_t * cleri_dup(uint32_t gid, cleri_object_t * cl_obj)`
Duplicate a libcleri object with a different gid but using the same element.

>Note: Only the object is duplicated. The element (`cleri_object_via_t via`)
>is a pointer to the original object.

The following [pyleri](https://github.com/transceptor-technology/pyleri) code
will use `cleri_dup()` when exported to c:
```python
elem = Repeat(obj, mi=1, ma=1)
```

Use the code below if you want similar behavior without duplication:
```python
elem = Sequence(obj)
```

### `cleri_keyword_t`
Keyword element. The parser needs a match with the keyword.

Type (`cleri_object_t.tp`): `CLERI_TP_KEYWORD`

*Public members*
- `const char * cleri_keyword_t.keyword`: Contains the keyword string. (readonly)
- `int cleri_keyword_t.ign_case`: Boolean. (readonly)
- `size_t cleri_keyword_t.len`: Length of the keyword string. (readonly)

#### `cleri_object_t * cleri_keyword(uint32_t gid, const char * keyword, int ign_case)`
Create and return a new [object](#cleri_object_t) containing a keyword element.
Argument `ign_case` can be set to 1 for a case insensitive keyword match.

Example:
```c
/* define case insensitive keyword */
cleri_object_t * k_tictactoe = cleri_keyword(
    0,                  // gid, not used in this example
    "tic-tac-toe",      // keyword
    1);                 // case insensitive

/* create grammar with custom keyword regular expression match */
cleri_grammar_t * grammar = cleri_grammar(k_tictactoe, "^[A-Za-z-]+");

/* parse some test string */
cleri_parse_t * pr = cleri_parse(grammar, "Tic-Tac-Toe");
printf("Valid: %s\n", pr->is_valid ? "true" : "false"); // true

/* cleanup */
cleri_parse_free(pr);
cleri_grammar_free(grammar);
```

### `cleri_regex_t`
Regular expression element. The parser needs a match with the regular
expression.

*No public members*

#### `cleri_object_t * cleri_regex(uint32_t gid, const char * pattern)`
Create and return a new [object](#cleri_object_t) containing a regular
expression element. Argument `pattern` should contain the regular expression.
Each pattern must start with character `^` and the pattern should be checked
before calling this function.

See [Quick usage](#quick-usage) for a `cleri_regex_t` example.

### `cleri_choice_t`
Choice element. The parser must choose one of the child elements.

*Public members*
- `int cleri_choice_t.most_greedy`: Boolean. (readonly)
- `cleri_olist_t * cleri_choice_t.olist`: Children. (readonly)

#### `cleri_object_t * cleri_choice(uint32_t gid, int most_greedy, size_t len, ...)`
Create and return a new [object](#cleri_object_t) containing a choice element.
Argument `most_greedy` can be set to 1 in which case the parser will select the
most greedy match. When 0 the parser will select the first match.

Example:
```c
/* define grammar */
cleri_object_t * k_hello = cleri_keyword(0, "hello", 0);
cleri_object_t * k_goodbye = cleri_keyword(0, "goodbye", 0);
cleri_object_t * choice = cleri_choice(
    0,                      // gid, not used in this example
    0,                      // stop at first match
    2,                      // number of elements
    k_hello, k_goodbye);    // elements

/* create grammar */
cleri_grammar_t * grammar = cleri_grammar(choice, NULL);

/* parse some test string */
cleri_parse_t * pr = cleri_parse(grammar, "goodbye");
printf("Valid: %s\n", pr->is_valid ? "true" : "false"); // true

/* cleanup */
cleri_parse_free(pr);
cleri_grammar_free(grammar);
```

### `cleri_sequence_t`
Sequence element. The parser must match each element in the specified order.

*Public members*
- `cleri_olist_t * cleri_sequence_t.olist`: Elements. (readonly)

#### `cleri_object_t * cleri_sequence(uint32_t gid, size_t len, ...)`
Create and return a new [object](#cleri_object_t) containing a sequence element.

Example:
```c
cleri_object_t * sequence = cleri_sequence(
    0,                              // gid, not used in the example
    3,                              // number of elements
    cleri_keyword(0, "Tic", 0),     // first element
    cleri_keyword(0, "Tac", 0),     // second element
    cleri_keyword(0, "Toe", 0));    // third element

/* create grammar */
cleri_grammar_t * grammar = cleri_grammar(sequence, NULL);

/* parse some test string */
cleri_parse_t * pr = cleri_parse(grammar, "Tic Tac Toe");
printf("Valid: %s\n", pr->is_valid ? "true" : "false"); // true

/* cleanup */
cleri_parse_free(pr);
cleri_grammar_free(grammar);
```

### `cleri_optional_t`
Optional element. The parser looks for an optional element.

*Public members*
- `cleri_object_t * cleri_optional_t.cl_obj`: Optional element. (readonly)

#### `cleri_object_t * cleri_optional(uint32_t gid, cleri_object_t * cl_obj)`
Create and return a new [object](#cleri_object_t) containing an optional element.

Example:
```c
/* define grammar */
cleri_object_t * k_hello = cleri_keyword(0, "hello", 0);
cleri_object_t * k_there = cleri_keyword(0, "there", 0);
cleri_object_t * optional = cleri_optional(
    0,                  // gid, not used in this example
    k_there);           // optional element
cleri_object_t * greet = cleri_sequence(
    0,                  // gid, not used in this example
    2,                  // number of elements
    k_hello, optional); // elements

/* create grammar */
cleri_grammar_t * grammar = cleri_grammar(greet, NULL);

/* parse some test string */
cleri_parse_t * pr = cleri_parse(grammar, "hello");
printf("Valid: %s\n", pr->is_valid ? "true" : "false"); // true

/* cleanup */
cleri_parse_free(pr);
cleri_grammar_free(grammar);
```

### `cleri_prio_t`
Prio element. The parser must match one element. Inside the prio element it
is possible to use `CLERI_THIS` which is a reference to itself.

>Note: Use a [forward reference](#forward-reference) when possible.
>A prio is required when the same position in a string is potentially checked
>more than once.

*Public members*
- `cleri_olist_t * cleri_sequence_t.olist`: Elements. (readonly)

#### `cleri_object_t * cleri_prio(uint32_t gid, size_t len, ...)`
Create and return a new [object](#cleri_object_t) containing a prio element.

Example:
```c
/*
 * define grammar.
 *
 * Note: The third and fourth element are using a reference to the prio
 *       element at the same position in the string as the prio element.
 *       This is why a forward reference cannot be used for this example.
 */
cleri_object_t * prio = cleri_prio(
    0,                              // gid, not used in the example
    4,                              // number of elements
    cleri_keyword(0, "ni", 0),      // first element
    cleri_sequence(0, 3,            // second element
        cleri_token(0, "("),
        CLERI_THIS,
        cleri_token(0, ")")),
    cleri_sequence(0, 3,            // third element
        CLERI_THIS,
        cleri_keyword(0, "or", 0),
        CLERI_THIS),
    cleri_sequence(0, 3,            // fourth element
        CLERI_THIS,
        cleri_keyword(0, "and", 0),
        CLERI_THIS));

/* create grammar */
cleri_grammar_t * grammar = cleri_grammar(prio, NULL);

/* parse some test string */
cleri_parse_t * pr = cleri_parse(grammar, "(ni or ni) and (ni or ni)");
printf("Valid: %s\n", pr->is_valid ? "true" : "false"); // true

/* cleanup */
cleri_parse_free(pr);
cleri_grammar_free(grammar);
```

### `cleri_repeat_t`
Repeat element. The parser must math at least `cleri_repeat_t.min` elements and
at most `cleri_repeat_t.max`. An unlimited amount is allowed in case `cleri_repeat_t.max`
is set to 0 (zero).

*Public members*
- `cleri_object_t * cleri_repeat_t.cl_obj`: Element to repeat.
- `size_t cleri_repeat_t.min`: Minimum times an element is expected. (readonly)
- `size_t cleri_repeat_t.max`: Maximum times an element is expected or 0 for unlimited. (readonly)

#### `cleri_object_t * cleri_repeat(uint32_t gid, cleri_object_t * cl_obj, size_t min, size_t max)`
Create and return a new [object](#cleri_object_t) containing a repeat element.
Argument `max` should be greater or equal to `min` or 0.

Example:
```c

```

### Miscellaneous functions
#### `const char * siridb_version(void)`
Returns the version of libsiridb.