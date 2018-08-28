#ifndef CLERI_EXAMPLE_TREE_H_
#define CLERI_EXAMPLE_TREE_H_

#include <stdio.h>
#include <cleri/cleri.h>
#include "../buffer.h"

#define JTAB 4

// private
int prt_JSON(char * string);
const char * strtp(cleri_tp tp);
int get_children(cleri_children_t * child, const char * orig, buffer_t * buf);

// public
void prt_tree(cleri_grammar_t * grammar, const char * str);

#endif /* CLERI_EXAMPLE_TREE_H_ */
