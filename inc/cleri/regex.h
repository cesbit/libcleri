/*
 * regex.h - cleri regular expression element.
 *
 * author       : Jeroen van der Heijden
 * email        : jeroen@transceptor.technology
 * copyright    : 2016, Transceptor Technology
 *
 * changes
 *  - initial version, 08-03-2016
 *  - refactoring, 17-06-2017
 */
#ifndef CLERI_REGEX_H_
#define CLERI_REGEX_H_

#include <pcre.h>
#include <stddef.h>
#include <inttypes.h>
#include <cleri/object.h>

/* typedefs */
typedef struct cleri_object_s cleri_object_t;
typedef struct cleri_regex_s cleri_regex_t;

/* public functions */
cleri_object_t * cleri_regex(uint32_t gid, const char * pattern);

/* structs */
struct cleri_regex_s
{
    pcre * regex;
    pcre_extra * regex_extra;
};

#endif /* CLERI_REGEX_H_ */