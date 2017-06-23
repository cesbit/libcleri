/*
 * node.h - node is created while parsing a string. a node old the result
 *          for one element.
 *
 * author       : Jeroen van der Heijden
 * email        : jeroen@transceptor.technology
 * copyright    : 2016, Transceptor Technology
 *
 * changes
 *  - initial version, 08-03-2016
 *  - refactoring, 17-06-2017
 */
#ifndef CLERI_NODE_H_
#define CLERI_NODE_H_

#include <stddef.h>
#include <inttypes.h>
#include <cleri/cleri.h>
#include <cleri/children.h>

/* typedefs */
typedef struct cleri_s cleri_t;
typedef struct cleri_children_s cleri_children_t;
typedef struct cleri_node_s cleri_node_t;

/* public macro function */
#define cleri_node_has_children(__node) \
    (__node->children != NULL && __node->children->node != NULL)

/* pricate functions */
cleri_node_t * cleri__node_new(
        cleri_t * cl_obj,
        const char * str,
        size_t len);
void cleri__node_free(cleri_node_t * node);

/* private use as empty node */
extern cleri_node_t * CLERI_EMPTY_NODE;

/* structs */
struct cleri_node_s
{
    /* public */
    const char * str;
    size_t len;
    cleri_t * cl_obj;
    cleri_children_t * children;

    /* private */
    uint_fast8_t ref;
    int64_t result;
};

#endif /* CLERI_NODE_H_ */