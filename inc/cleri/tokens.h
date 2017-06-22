/*
 * tokens.h - cleri tokens element. (like token but can contain more tokens
 *            in one element)
 *
 * author       : Jeroen van der Heijden
 * email        : jeroen@transceptor.technology
 * copyright    : 2016, Transceptor Technology
 *
 * changes
 *  - initial version, 08-03-2016
 *  - refactoring, 17-06-2017
 */
#ifndef CLERI_TOKENS_H_
#define CLERI_TOKENS_H_

#include <stddef.h>
#include <inttypes.h>
#include <cleri/object.h>

/* typedefs */
typedef struct cleri_object_s cleri_object_t;
typedef struct cleri_tlist_s cleri_tlist_t;
typedef struct cleri_tokens_s cleri_tokens_t;

/* public functions */
cleri_object_t * cleri_tokens(uint32_t gid, const char * tokens);

/* structs */
struct cleri_tlist_s
{
    const char * token;
    size_t len;
    cleri_tlist_t * next;
};

struct cleri_tokens_s
{
    char * tokens;
    char * spaced;
    cleri_tlist_t * tlist;
};

#endif /* CLERI_TOKENS_H_ */