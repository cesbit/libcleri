/*
 * olist.h - linked list for keeping cleri objects.
 *
 * author       : Jeroen van der Heijden
 * email        : jeroen@transceptor.technology
 * copyright    : 2016, Transceptor Technology
 *
 * changes
 *  - initial version, 08-03-2016
 *  - refactoring, 17-06-2017
 */
#ifndef CLERI_OLIST_H_
#define CLERI_OLIST_H_

#include <cleri/object.h>

/* typedefs */
typedef struct cleri_object_s cleri_object_t;
typedef struct cleri_olist_s cleri_olist_t;

typedef struct cleri_olist_s
{
    cleri_object_t * cl_obj;
    cleri_olist_t * next;
} cleri_olist_t;

cleri_olist_t * cleri_olist_new(void);
int cleri_olist_append(cleri_olist_t * olist, cleri_object_t * cl_object);
int cleri_olist_append_nref(cleri_olist_t * olist, cleri_object_t * cl_object);
void cleri_olist_free(cleri_olist_t * olist);
void cleri_olist_empty(cleri_olist_t * olist);

#endif /* CLERI_OLIST_H_ */