/*
 * dup.h - this cleri element can be used to duplicate an element.
 *
 * author       : Jeroen van der Heijden
 * email        : jeroen@transceptor.technology
 * copyright    : 2017, Transceptor Technology
 *
 * changes
 *  - initial version, 21-06-2017
 *
 */
#ifndef CLERI_DUP_H_
#define CLERI_DUP_H_

#include <stddef.h>
#include <inttypes.h>
#include <cleri/object.h>

/* typedefs */
typedef struct cleri_object_s cleri_object_t;
typedef struct cleri_dup_s cleri_dup_t;

/* public functions */
cleri_object_t * cleri_dup(uint32_t gid, cleri_object_t * cl_obj);

/* structs */
// cleri_dup_t is defined in object.h

#endif /* CLERI_DUP_H_ */