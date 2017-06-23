/*
 * choice.h - this cleri element can hold other elements and the grammar
 *            has to choose one of them.
 *
 * author       : Jeroen van der Heijden
 * email        : jeroen@transceptor.technology
 * copyright    : 2017, Transceptor Technology
 *
 * changes
 *  - initial version, 08-03-2016
 *  - refactoring, 17-06-2017
 */
#ifndef CLERI_CHOICE_H_
#define CLERI_CHOICE_H_

#include <stddef.h>
#include <inttypes.h>
#include <cleri/cleri.h>
#include <cleri/olist.h>

/* typedefs */
typedef struct cleri_s cleri_t;
typedef struct cleri_olist_s cleri_olist_t;
typedef struct cleri_choice_s cleri_choice_t;

/* public functions */
cleri_t * cleri_choice(uint32_t gid, int most_greedy, size_t len, ...);

/* structs */
struct cleri_choice_s
{
    int most_greedy;
    cleri_olist_t * olist;
};

#endif /* CLERI_CHOICE_H_ */