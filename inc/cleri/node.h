/*
 * node.h - node is created while parsing a string. a node old the result
 *          for one element.
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

/* private functions */
cleri_node_t * cleri__node_new(cleri_t * cl_obj, const char * str, size_t len);
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
    void * data;    /* free to use by the user */

    /* private */
    size_t ref;
    int64_t result;     /* DEPRECATED */
};

#endif /* CLERI_NODE_H_ */