/*
 * ref.c - cleri ref element
 *
 * author       : Jeroen van der Heijden
 * email        : jeroen@transceptor.technology
 * copyright    : 2017, Transceptor Technology
 *
 * changes
 *  - initial version, 20-06-2017
 *
 */
#include <stdlib.h>
#include <assert.h>
#include <cleri/ref.h>

/*
 * Returns NULL in case an error has occurred.
 */
cleri_object_t * cleri_ref(void)
{
    cleri_object_t * cl_object = cleri_object_new(
            0,
            CLERI_TP_REF,
            NULL,
            NULL);

    return cl_object;
}

/*
 * Both ref and cl_obj are not allowed to be NULL.
 */
void cleri_ref_set(cleri_object_t * ref, cleri_object_t * cl_obj)
{
    /* cl_obj should have no other refs */
    assert (ref != NULL &&
            ref->tp == CLERI_TP_REF &&
            cl_obj != NULL &&
            cl_obj->ref == 1);

    /* assign properties, except ref counter */
    ref->gid = cl_obj->gid;
    ref->free_object = cl_obj->free_object;
    ref->parse_object = cl_obj->parse_object;
    ref->tp = cl_obj->tp;
    ref->via = cl_obj->via;

    /* free *cl_obj and set the pointer to the ref object */
    free(cl_obj);
}



