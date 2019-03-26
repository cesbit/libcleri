/*
 * kwcache.c - holds keyword regular expression result while parsing.
 */
#define PCRE2_CODE_UNIT_WIDTH 8

#include <stdlib.h>
#include <pcre2.h>
#include <string.h>
#include <cleri/kwcache.h>
#include <assert.h>

#define NOT_FOUND UINT16_MAX

static void kwcache__kw_match(
        uint16_t * kwcache,
        cleri_parse_t * pr,
        const char * str);

/*
 * Returns NULL in case an error has occurred.
 */
uint16_t * cleri__kwcache_new(const char * str)
{
    size_t n = strlen(str);
    uint16_t * kwcache = cleri__mallocn(n, uint16_t);
    if (kwcache != NULL)
    {
        memset(kwcache, 0xff, n * sizeof(uint16_t));
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
    uint16_t * len;
    if (*str == '\0')
    {
        return 0;
    }

    len = &pr->kwcache[str - pr->str];

    if (*len == NOT_FOUND)
    {
        kwcache__kw_match(len, pr, str);
    }

    return *len;
}

/*
 * This function will set kwcache->len if a match is found.
 */
static void kwcache__kw_match(
        uint16_t * kwcache,
        cleri_parse_t * pr,
        const char * str)
{
    int pcre_exec_ret;

    pcre_exec_ret = pcre2_match(
                pr->re_keywords,
                (PCRE2_SPTR8) str,
                PCRE2_ZERO_TERMINATED,
                0,                     // start looking at this point
                0,                     // OPTIONS
                pr->match_data,
                NULL);

    *kwcache = pcre_exec_ret < 0
        ? 0
        : pcre2_get_ovector_pointer(pr->match_data)[1];
}
