#include <cleri/parse.h>
#include <stdlib.h>

static char * parse_str(
    cleri_parse_t * pr,
    cleri_translate_t * translate) __attribute__((unused));

static char * parse_str(cleri_parse_t * pr, cleri_translate_t * translate)
{
    size_t sz;
    int i = cleri_parse_strn(NULL, 0, pr, translate);
    if (i < 0)
    {
        return NULL;
    }
    sz = i + 1;
    char * s = malloc(sz);
    if (s)
    {
        i = cleri_parse_strn(s, sz, pr, translate);
        if (i < 0 || i >= (int) sz)
        {
            free(s);
            return NULL;
        }
    }
    return s;
}

#define _assert_is_valid(__grammar, __str) \
{ \
    cleri_parse_t * __pr = cleri_parse(__grammar, __str); \
    _assert (__pr); \
    _assert (__pr->is_valid); \
    cleri_parse_free(__pr); \
}

#define _assert_is_not_valid(__grammar, __str) \
{ \
    cleri_parse_t * __pr = cleri_parse(__grammar, __str); \
    _assert (__pr); \
    _assert (!__pr->is_valid); \
    cleri_parse_free(__pr); \
}

#define _assert_parse_str(__grammar, __str, __expect, __translate) \
{ \
    cleri_parse_t * __pr = cleri_parse(__grammar, __str); \
    _assert (__pr); \
    char * __s = parse_str(__pr, __translate); \
    _assert (__s); \
    if (strcmp(__s, __expect) != 0) printf("\n\ngot: `%s`\n", __s); \
    _assert (strcmp(__s, __expect) == 0); \
    free(__s); \
    cleri_parse_free(__pr); \
}
