/*
 * children.c - linked list for keeping node results.
 */
#include <stdlib.h>
#include <cleri/children.h>

/*
 * Appends a node to children.
 *
 * Returns 0 when successful or -1 in case of an error.
 */
int cleri__children_add(cleri_children_t ** children, cleri_node_t * node)
{
    cleri_children_t * child;
    if (*children == NULL)
    {
        *children = cleri__malloc(cleri_children_t);
        if (*children == NULL)
        {
            return -1;
        }

        (*children)->node = node;
        (*children)->next = NULL;

        return 0;
    }

    child = *children;

    while (child->next != NULL)
    {
        child = child->next;
    }

    child->next = cleri__malloc(cleri_children_t);
    if (child->next == NULL)
    {
        return -1;
    }

    child->next->node = node;
    child->next->next = NULL;

    return 0;
}

/*
 * Destroy children.
 */
void cleri__children_free(cleri_children_t * children)
{
    cleri_children_t * next;
    while (children != NULL)
    {
        next = children->next;
        cleri__node_free(children->node);
        free(children);
        children = next;
    }
}
