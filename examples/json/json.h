/*
 * json.h
 *
 * This grammar is generated using the Grammar.export_c() method and
 * should be used with the cleri module.
 *
 * Source class: JsonGrammar
 * Created at: 2017-06-20 21:53:53
 */
#ifndef CLERI_EXPORT_JSON_H_
#define CLERI_EXPORT_JSON_H_

#include <cleri/cleri.h>

cleri_grammar_t * compile_grammar(void);

enum cleri_grammar_ids {
    CLERI_NONE,   // used for objects with no name
    CLERI_GID_JSON_ARRAY,
    CLERI_GID_JSON_MAP,
    CLERI_GID_JSON_MAP_ITEM,
    CLERI_GID_K_FALSE,
    CLERI_GID_K_NULL,
    CLERI_GID_K_TRUE,
    CLERI_GID_R_FLOAT,
    CLERI_GID_R_INTEGER,
    CLERI_GID_R_STRING,
    CLERI_GID_START,
    CLERI_END // can be used to get the enum length
};

#endif /* CLERI_EXPORT_JSON_H_ */
