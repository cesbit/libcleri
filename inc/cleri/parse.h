/*
 * parser.h - this contains the start for parsing a string to a grammar.
 *
 * author       : Jeroen van der Heijden
 * email        : jeroen@transceptor.technology
 * copyright    : 2016, Transceptor Technology
 *
 * changes
 *  - initial version, 08-03-2016
 *  - refactoring, 17-06-2017
 */
#ifndef CLERI_PARSE_H_
#define CLERI_PARSE_H_

#include <stddef.h>
#include <stdbool.h>
#include <cleri/cleri.h>
#include <cleri/grammar.h>
#include <cleri/node.h>
#include <cleri/expecting.h>
#include <cleri/kwcache.h>
#include <cleri/rule.h>

/* typedefs */
typedef struct cleri_s cleri_t;
typedef struct cleri_grammar_s cleri_grammar_t;
typedef struct cleri_node_s cleri_node_t;
typedef struct cleri_expecting_s cleri_expecting_t;
typedef struct cleri_kwcache_s cleri_kwcache_t;
typedef struct cleri_rule_store_s cleri_rule_store_t;
typedef struct cleri_parse_s cleri_parse_t;

/* public functions */
cleri_parse_t * cleri_parse(cleri_grammar_t * grammar, const char * str);
void cleri_parse_free(cleri_parse_t * pr);
void cleri_parse_expect_start(cleri_parse_t * pr);

/* private functions */
cleri_node_t * cleri__parse_walk(
        cleri_parse_t * pr,
        cleri_node_t * parent,
        cleri_t * cl_obj,
        cleri_rule_store_t * rule,
        int mode);

/* structs */
struct cleri_parse_s
{
    int is_valid;
    size_t pos;
    const char * str;
    cleri_node_t * tree;
    const cleri_olist_t * expect;
    cleri_expecting_t * expecting;
    pcre * re_keywords;
    pcre_extra * re_kw_extra;
    cleri_kwcache_t * kwcache;
};

#endif /* CLERI_PARSE_H_ */