/*
 * kwcache.c - holds keyword regular expression result while parsing.
 */
#define PCRE2_CODE_UNIT_WIDTH 8

#include <stdlib.h>
#include <pcre2.h>
#include <string.h>
#include <cleri/kwcache.h>

static void kwcache__kw_match(
        cleri_kwcache_t * kwcache,
        cleri_parse_t * pr,
        const char * str);

/*
 * Returns NULL in case an error has occurred.
 */
cleri_kwcache_t * cleri__kwcache_new(void)
{
    cleri_kwcache_t * kwcache = cleri__malloc(cleri_kwcache_t);
    if (kwcache != NULL)
    {
        kwcache->len = 0;
        kwcache->str = NULL;
        kwcache->next = NULL;
    }
    return kwcache;
}

/*
 * Returns 0 when no kw_match is found, -1 when an error has occurred, or the
 * new kwcache->len value.
 */
ssize_t cleri__kwcache_match(
        cleri_parse_t * pr,
        const char * str)
{
    cleri_kwcache_t * kwcache = pr->kwcache;
    if (kwcache->str != NULL)
    {
        while (1)
        {
            if (str == kwcache->str)
            {
                return kwcache->len;
            }

            if (kwcache->next == NULL)
            {
                break;
            }
            kwcache = kwcache->next;
        }
        kwcache->next = cleri__malloc(cleri_kwcache_t);
        if (kwcache->next == NULL)
        {
            return -1;
        }
        kwcache = kwcache->next;
        kwcache->len = 0;
        kwcache->next = NULL;
    }

    kwcache->str = str;
    kwcache__kw_match(kwcache, pr, str);
    return kwcache->len;
}

/*
 * Destroy kwcache. (parsing NULL is allowed)
 */
void cleri__kwcache_free(cleri_kwcache_t * kwcache)
{
    cleri_kwcache_t * next;
    while (kwcache != NULL)
    {
        next = kwcache->next;
        free(kwcache);
        kwcache = next;
    }
}

/*
 * This function will set kwcache->len if a match is found.
 */
static void kwcache__kw_match(
        cleri_kwcache_t * kwcache,
        cleri_parse_t * pr,
        const char * str)
{
    int pcre_exec_ret;

    PCRE2_SIZE * ovector;

    pcre_exec_ret = pcre2_match(
                pr->re_keywords,
                (PCRE2_SPTR8) str,
                strlen(str),
                0,                     // start looking at this point
                0,                     // OPTIONS
                pr->match_data,
                NULL);

    if (pcre_exec_ret < 0)
    {
        return;
    }

    ovector = pcre2_get_ovector_pointer(pr->match_data);
    kwcache->len = ovector[1];


}
