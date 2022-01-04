/*
 * kwmatch.c - cleri keyword match element.
 */
#include <cleri/kwmatch.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

static void kwmatch__free(cleri_t * cl_object);

static cleri_node_t *  kwmatch__parse(
        cleri_parse_t * pr,
        cleri_node_t * parent,
        cleri_t * cl_obj,
        cleri_rule_store_t * rule);

/*
 * Returns a regex object or NULL in case of an error.
 *
 * Argument pattern must start with character '^'. Be sure to check the pattern
 * for the '^' character before calling this function.
 *
 * Warning: this function could write to stderr in case the pattern could not
 * be compiled.
 */
cleri_t * cleri_kwmatch(uint32_t gid)
{
    cleri_t * cl_object;

    cl_object = cleri_new(
            gid,
            CLERI_TP_KWMATCH,
            &kwmatch__free,
            &kwmatch__parse);

    if (cl_object == NULL)
    {
        return NULL;
    }

    return cl_object;
}

/*
 * Destroy regex object.
 */
static void kwmatch__free(cleri_t * cl_object __attribute__((unused))) { }

/*
 * Returns a node or NULL. In case of an error, pr->is_valid is set to -1
 */
static cleri_node_t *  kwmatch__parse(
        cleri_parse_t * pr,
        cleri_node_t * parent,
        cleri_t * cl_obj,
        cleri_rule_store_t * rule __attribute__((unused)))
{
    const char * str = parent->str + parent->len;
    cleri_node_t * node;
    size_t kw_size = cleri__kwcache_match(pr, str);

    if (kw_size == 0)
    {
        if (cleri__expecting_update(pr->expecting, cl_obj, str) == -1)
        {
            pr->is_valid = -1; /* error occurred */
        }
        return NULL;
    }

    if ((node = cleri__node_new(cl_obj, str, kw_size)) != NULL)
    {
        parent->len += node->len;
        cleri__node_add(parent, node);
    }
    else
    {
        pr->is_valid = -1;  /* error occurred */
    }

    return node;
}
