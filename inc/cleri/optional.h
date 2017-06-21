/*
 * optional.h - cleri optional element.
 *
 * author       : Jeroen van der Heijden
 * email        : jeroen@transceptor.technology
 * copyright    : 2016, Transceptor Technology
 *
 * changes
 *  - initial version, 08-03-2016
 *  - refactoring, 17-06-2017
 */
#ifndef CLERI_OPTIONAL_H_
#define CLERI_OPTIONAL_H_

#include <inttypes.h>
#include <cleri/object.h>

/* typedefs */
typedef struct cleri_object_s cleri_object_t;
typedef struct cleri_optional_s cleri_optional_t;

/* public functions */
cleri_object_t * cleri_optional(uint32_t gid, cleri_object_t * cl_obj);

/* structs */
struct cleri_optional_s
{
    cleri_object_t * cl_obj;
};

#endif /* CLERI_OPTIONAL_H_ */