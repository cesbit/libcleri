/*
 * keyword.h - cleri keyword element
 *
 * author       : Jeroen van der Heijden
 * email        : jeroen@transceptor.technology
 * copyright    : 2016, Transceptor Technology
 *
 * changes
 *  - initial version, 08-03-2016
 *  - refactoring, 17-06-2017
 */
#ifndef CLERI_KEYWORD_H_
#define CLERI_KEYWORD_H_

#include <stddef.h>
#include <inttypes.h>
#include <cleri/cleri.h>

/* typedefs */
typedef struct cleri_s cleri_t;
typedef struct cleri_keyword_s cleri_keyword_t;

/* public functions */
cleri_t * cleri_keyword(uint32_t gid, const char * keyword, int ign_case);

/* structs */
struct cleri_keyword_s
{
    const char * keyword;
    int ign_case;
    size_t len;
};

#endif /* CLERI_KEYWORD_H_ */