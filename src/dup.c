/*
 * dup.c - this cleri element can be used to duplicate an element.
 *
 * author       : Jeroen van der Heijden
 * email        : jeroen@transceptor.technology
 * copyright    : 2017, Transceptor Technology
 *
 * changes
 *  - initial version, 21-06-2017
 *
 */
 #include <cleri/dup.h>

static void DUP_free(cleri_t * cl_object);

/*
 * Duplicate a libcleri object.
 * Note: a pointer to the original object->via (element) is used.
 */
cleri_t * cleri_dup(uint32_t gid, cleri_t * cl_obj)
{
    cleri_dup_t * dup = (cleri_dup_t *) malloc(sizeof(cleri_dup_t));
    if (dup != NULL)
    {
        dup->gid = gid;
        dup->ref = 1;
        dup->tp = cl_obj->tp;
        dup->via = cl_obj->via;
        dup->free_object = &DUP_free;
        dup->parse_object = cl_obj->parse_object;
        cleri_incref(cl_obj);
        dup->dup = cl_obj;
    }
    return (cleri_t *) dup;
}

static void DUP_free(cleri_t * cl_object)
{
    cleri_dup_t * dup = (cleri_dup_t *) cl_object;
    cleri_free(dup->dup);
}
