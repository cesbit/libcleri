/*
 * parse.c - this contains everything for parsing a string to a grammar.
 */
#include <cleri/expecting.h>
#include <cleri/parse.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

/*
 * Return a parse result. In case of a memory allocation error the return value
 * will be NULL.
 */
cleri_parse_t * cleri_parse2(
    cleri_grammar_t * grammar,
    const char * str,
    int flags)
{
    cleri_node_t * nd;
    cleri_parse_t * pr;
    const char * end;
    const char * test;
    bool at_end = true;

    /* prepare parsing */
    pr = cleri__malloc(cleri_parse_t);
    if (pr == NULL)
    {
        return NULL;
    }

    pr->str = str;
    pr->tree = NULL;
    pr->kwcache = NULL;
    pr->expecting = NULL;
    pr->is_valid = 0;

    if (    (pr->tree = cleri__node_new(NULL, str, 0)) == NULL ||
            (pr->kwcache = cleri__kwcache_new(str)) == NULL ||
            (pr->expecting = cleri__expecting_new(str, flags)) == NULL)
    {
        cleri_parse_free(pr);
        return NULL;
    }

    pr->re_keywords = grammar->re_keywords;
    pr->match_data = grammar->match_data;

    /* do the actual parsing */
    nd = cleri__parse_walk(
            pr,
            pr->tree,
            grammar->start,
            NULL,
            CLERI__EXP_MODE_REQUIRED);

    /* When is_valid is -1, an allocation error has occurred. */
    if (pr->is_valid == -1)
    {
        cleri_parse_free(pr);
        return NULL;
    }

    pr->is_valid = nd != NULL;

    /* process the parse result */
    end = pr->tree->str + pr->tree->len;

    /* check if we are at the end of the string */
    for (test = end; *test; test++)
    {
        if (!isspace(*test))
        {
            at_end = false;
            pr->is_valid = false;
            break;
        }
    }

    pr->pos = (pr->is_valid || (
                !at_end && nd && (
                    !pr->expecting->required ||
                    pr->expecting->str == pr->str))) ?
            pr->tree->len : (size_t) (pr->expecting->str - pr->str);

    if (pr->expecting->required && !at_end && nd)
    {
        if (cleri__expecting_set_mode(
                pr->expecting,
                end,
                CLERI__EXP_MODE_REQUIRED) == -1 ||
            cleri__expecting_update(
                pr->expecting,
                CLERI_END_OF_STATEMENT,
                end) == -1)
        {
            cleri_parse_free(pr);
            return NULL;
        }
        cleri__expecting_combine(pr->expecting);
    }

    cleri__olist_unique(pr->expecting->required);
    pr->expect = pr->expecting->required;

    return pr;
}

/*
 * Destroy parser. (parsing NULL is allowed)
 */
void cleri_parse_free(cleri_parse_t * pr)
{
    cleri__node_free(pr->tree);
    cleri__kwcache_free(pr->kwcache);
    if (pr->expecting != NULL)
    {
        cleri__expecting_free(pr->expecting);
    }
    free(pr);
}

/*
 * Reset expect to start
 */
void cleri_parse_expect_start(cleri_parse_t * pr)
{
    pr->expect = pr->expecting->required;
}

/*
 * Print parse result to a string. The return value is equal to the snprintf
 * function. Argument `translate_cb` maybe NULL or a function which may return
 * a string based on the `cleri_t`. This allows for returning nice strings for
 * regular expressions. The function may return NULL if it has no translation
 * for the given regular expression.
 */
int cleri_parse_strn(
    char * s,
    size_t n,
    cleri_parse_t * pr,
    cleri_translate_t * translate)
{
    int rc, count = 0;
    size_t i, m;
    cleri_t * o;
    const char * expect;
    const char * template;
    if (pr == NULL)
    {
        return snprintf(s, n,
            "no parse result, a possible reason might be that the maximum "
            "recursion depth of %d has been reached", MAX_RECURSION_DEPTH);
    }
    if (pr->is_valid)
    {
        return snprintf(s, n, "parsed successfully");
    }
    /* make sure expecting is at start */
    cleri_parse_expect_start(pr);

    rc = snprintf(s, n, "error at position %zd", pr->pos);
    if (rc < 0)
    {
        return rc;
    }
    i = rc;

    while (pr->expect)
    {
        o = pr->expect->cl_obj;
        if (!translate || !(expect = (*translate)(o))) switch(o->tp)
        {
        case CLERI_TP_END_OF_STATEMENT:
            expect = "end_of_statement";
            break;
        case CLERI_TP_KEYWORD:
            expect = o->via.keyword->keyword;
            break;
        case CLERI_TP_TOKENS:
            expect = o->via.tokens->spaced;
            break;
        case CLERI_TP_TOKEN:
            expect = o->via.token->token;
            break;
        default:
            expect = "";  /* continue */
        }

        if (*expect == '\0')
        {
            /* ignore empty strings */
            pr->expect = pr->expect->next;
            continue;
        }

        /* make sure len is not greater than the maximum size */
        m = (i < n) ? n - i : 0;

        /* we use count = 0 to print the first one, then for the others
         * a comma prefix and the last with -or-
         *
         * TODO: could be improved since the last `or` might never be added
         */
        template = !count++
            ? ", expecting: %s"
            : pr->expect->next
            ? ", %s"
            : " or %s";
        rc = snprintf(s + i, m, template, expect);
        if (rc < 0)
        {
            return rc;
        }
        i += rc;

        pr->expect = pr->expect->next;
    }
    return (int) i;
}

/*
 * Walk a parser object.
 * (recursive function, called from each parse_object function)
 * Returns a node or NULL. (In case of error one should check pr->is_valid)
 */
cleri_node_t * cleri__parse_walk(
        cleri_parse_t * pr,
        cleri_node_t * parent,
        cleri_t * cl_obj,
        cleri_rule_store_t * rule,
        int mode)
{
    const char * str = parent->str + parent->len;

    /* set parent len to next none white space char */
    for (; isspace(*str); ++str, ++parent->len);

    /* set expecting mode */
    if (cleri__expecting_set_mode(pr->expecting, parent->str, mode) == -1)
    {
        pr->is_valid = -1;
        return NULL;
    }

    /* note that the actual node is returned or NULL but we do not
     * actually need the node. (boolean true/false would be enough)
     */
    return (*cl_obj->parse_object)(pr, parent, cl_obj, rule);
}
