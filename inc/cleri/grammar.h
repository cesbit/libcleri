/*
 * grammar.h - this should contain the 'start' or your grammar.
 *
 * author       : Jeroen van der Heijden
 * email        : jeroen@transceptor.technology
 * copyright    : 2016, Transceptor Technology
 *
 * changes
 *  - initial version, 08-03-2016
 *  - refactoring, 17-06-2017
 */
#ifndef CLERI_GRAMMAR_H_
#define CLERI_GRAMMAR_H_

#include <cleri/object.h>
#include <cleri/olist.h>
#include <pcre.h>

#define CLERI_DEFAULT_RE_KEYWORDS "^\\w+"

/* typedefs */
typedef struct cleri_object_s cleri_object_t;
typedef struct cleri_grammar_s cleri_grammar_t;

/* public functions */
cleri_grammar_t * cleri_grammar(
        cleri_object_t * start,
        const char * re_keywords);
void cleri_grammar_free(cleri_grammar_t * grammar);

/* structs */
struct cleri_grammar_s
{
    cleri_object_t * start;
    pcre * re_keywords;
    pcre_extra * re_kw_extra;
};

#endif /* CLERI_GRAMMAR_H_ */