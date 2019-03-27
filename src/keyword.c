/*
 * keyword.c - cleri keyword element.
 */
#include <stdlib.h>
#include <string.h>
#include <cleri/keyword.h>
#include <assert.h>

static void keyword__free(cleri_t * cl_object);

static cleri_node_t * keyword__parse(
        cleri_parse_t * pr,
        cleri_node_t * parent,
        cleri_t * cl_obj,
        cleri_rule_store_t * rule);

/*
 * Keywords must match the `keyword` regular expression defined by the grammar,
 * and should have more than 0 and less than 255 characters.
 *
 * Returns NULL in case an error has occurred.
 */
cleri_t * cleri_keyword(uint32_t gid, const char * keyword, int ign_case)
{
    size_t n = strlen(keyword);
    assert (n > 0 && n < UINT8_MAX);

    cleri_t * cl_object = cleri_new(
            gid,
            CLERI_TP_KEYWORD,
            &keyword__free,
            &keyword__parse);

    if (cl_object == NULL)
    {
        return NULL;
    }

    cl_object->via.keyword = cleri__malloc(cleri_keyword_t);

    if (cl_object->via.tokens == NULL)
    {
        free(cl_object);
        return NULL;
    }

    cl_object->via.keyword->keyword = keyword;
    cl_object->via.keyword->ign_case = ign_case;
    cl_object->via.keyword->len = n;

    return cl_object;
}

/*
 * Destroy keyword object.
 */
static void keyword__free(cleri_t * cl_object)
{
    free(cl_object->via.keyword);
}

/*
 * Returns a node or NULL. In case or an error, pr->is_valid is set to -1.
 */
static cleri_node_t * keyword__parse(
        cleri_parse_t * pr,
        cleri_node_t * parent,
        cleri_t * cl_obj,
        cleri_rule_store_t * rule __attribute__((unused)))
{
    ssize_t match_len;
    cleri_node_t * node = NULL;
    const char * str = parent->str + parent->len;

    if ((match_len = cleri__kwcache_match(pr, str)) < 0)
    {
        pr->is_valid = -1; /* error occurred */
        return NULL;
    }

    if (match_len == (ssize_t) cl_obj->via.keyword->len &&
       (
           strncmp(cl_obj->via.keyword->keyword, str, match_len) == 0 ||
           (
               cl_obj->via.keyword->ign_case &&
               strncasecmp(cl_obj->via.keyword->keyword, str, match_len) == 0
           )
       ))
    {
        if ((node = cleri__node_new(cl_obj, str, match_len)) != NULL)
        {
            parent->len += node->len;
            cleri__children_add(&parent->children, node);
        }
    }
    else
    {
        /* Update expecting */
        if (cleri__expecting_update(pr->expecting, cl_obj, str) == -1)
        {
            /* error occurred, node is already NULL */
            pr->is_valid = -1;
        }
    }
    return node;
}
