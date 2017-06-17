/*
 * repeat.h - cleri regular repeat element.
 *
 * author       : Jeroen van der Heijden
 * email        : jeroen@transceptor.technology
 * copyright    : 2016, Transceptor Technology
 *
 * changes
 *  - initial version, 08-03-2016
 *  - refactoring, 17-06-2017
 */
#ifndef CLERI_REPEAT_H_
#define CLERI_REPEAT_H_

#include <stddef.h>
#include <stdbool.h>
#include <inttypes.h>
#include <cleri/object.h>

typedef struct cleri_object_s cleri_object_t;

typedef struct cleri_repeat_s
{
    uint32_t gid;
    struct cleri_object_s * cl_obj;
    size_t min;
    size_t max;
} cleri_repeat_t;


cleri_object_t * cleri_repeat(
        uint32_t gid,
        struct cleri_object_s * cl_obj,
        size_t min,
        size_t max);

#endif /* CLERI_REPEAT_H_ */