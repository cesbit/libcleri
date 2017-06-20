/*
 * expecting.h - holds elements which the grammar expects at one position.
 *               this can be used for suggestions.
 *
 * author       : Jeroen van der Heijden
 * email        : jeroen@transceptor.technology
 * copyright    : 2016, Transceptor Technology
 *
 * changes
 *  - initial version, 08-03-2016
 *  - refactoring, 17-06-2017
 */
#ifndef CLERI_EXPECTING_H_
#define CLERI_EXPECTING_H_

#include <stdlib.h>
#include <inttypes.h>
#include <cleri/olist.h>
#include <cleri/object.h>

#define CLERI__EXP_MODE_OPTIONAL 0
#define CLERI__EXP_MODE_REQUIRED 1

/* typedefs */
typedef struct cleri_object_s cleri_object_t;
typedef struct cleri_olist_s cleri_olist_t;
typedef struct cleri_expecting_modes_s cleri_exp_modes_t;
typedef struct cleri_expecting_s cleri_expecting_t;

/* private functions */
cleri_expecting_t * cleri__expecting_new(const char * str);
int cleri__expecting_update(
        cleri_expecting_t * expecting,
        cleri_object_t * cl_obj,
        const char * str);
int cleri__expecting_set_mode(
        cleri_expecting_t * expecting,
        const char * str,
        int mode);
void cleri__expecting_free(cleri_expecting_t * expecting);
void cleri__expecting_combine(cleri_expecting_t * expecting);

/* structs */
struct cleri_expecting_modes_s
{
    int mode;
    const char * str;
    struct cleri_expecting_modes_s * next;
};

struct cleri_expecting_s
{
    const char * str;
    cleri_olist_t * required;
    cleri_olist_t * optional;
    cleri_exp_modes_t * modes;
};

#endif /* CLERI_EXPECTING_H_ */