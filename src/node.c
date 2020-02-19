/*
 * node.c - node is created while parsing a string. a node old the result
 *          for one element.
 */
#include <cleri/node.h>
#include <stdlib.h>

static cleri_node_t CLERI__EMPTY_NODE = {
        .children=NULL,
        .cl_obj=NULL,
        .len=0,
        .str=NULL
};

cleri_node_t * CLERI_EMPTY_NODE = &CLERI__EMPTY_NODE;

/*
 * Returns NULL in case an error has occurred.
 */
cleri_node_t * cleri__node_new(cleri_t * cl_obj, const char * str, size_t len)
{
    cleri_node_t * node = cleri__malloc(cleri_node_t);

    if (node != NULL)
    {
        node->cl_obj = cl_obj;
        node->ref = 1;

        node->str = str;
        node->len = len;
        node->children = NULL;
    }
    return node;
}
