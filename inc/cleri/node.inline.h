#ifndef CLERI_NODE_INLINE_H_
#define CLERI_NODE_INLINE_H_

#include <cleri/node.h>
#include <cleri/children.h>

/*
 * Destroy node. (parsing NULL is allowed)
 */
static inline void cleri__node_free(cleri_node_t * node)
{
    /* node can be NULL or this could be an CLERI_EMPTY_NODE */
    if (node == NULL || node == CLERI_EMPTY_NODE || --node->ref)
    {
        return;
    }
    cleri__children_free(node->children);
    free(node);
}

#endif  /* CLERI_NODE_INLINE_H_  */