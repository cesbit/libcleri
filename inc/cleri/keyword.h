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
#include <cleri/object.h>

typedef struct cleri_object_s cleri_object_t;

typedef struct cleri_keyword_s
{
    uint32_t gid;
    const char * keyword;
    int ign_case;
    size_t len;
} cleri_keyword_t;

cleri_object_t * cleri_keyword(
        uint32_t gid,
        const char * keyword,
        int ign_case);

#endif /* CLERI_KEYWORD_H_ */