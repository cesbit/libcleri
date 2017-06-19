/*
 * grammar.c - this should contain the 'start' or your grammar.
 *
 * author       : Jeroen van der Heijden
 * email        : jeroen@transceptor.technology
 * copyright    : 2016, Transceptor Technology
 *
 * changes
 *  - initial version, 08-03-2016
 *
 */
#include <cleri/grammar.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcre.h>

/*
 * Returns a grammar object or NULL in case of an error.
 *
 * Warning: this function could write to stderr in case the re_keywords could
 * not be compiled.
 */
cleri_grammar_t * cleri_grammar(
        cleri_object_t * start,
        const char * re_keywords)
{
    const char * re_kw = (re_keywords == NULL) ?
            CLERI_DEFAULT_RE_KEYWORDS : re_keywords;

    if (start == NULL)
    {
        return NULL;
    }

    cleri_grammar_t * grammar =
            (cleri_grammar_t *) malloc(sizeof(cleri_grammar_t));
    if (grammar == NULL)
    {
        return NULL;
    }

    const char * pcre_error_str;
    int pcre_error_offset;
    grammar->re_keywords = pcre_compile(
            re_kw,
            0,
            &pcre_error_str,
            &pcre_error_offset,
            NULL);
    if(grammar->re_keywords == NULL)
    {
        /* this is critical and unexpected, memory is not cleaned */
        fprintf(stderr,
                "error: cannot compile '%s' (%s)\n",
                re_kw,
                pcre_error_str);
        free(grammar);
        return NULL;
    }
    grammar->re_kw_extra =
            pcre_study(grammar->re_keywords, 0, &pcre_error_str);

    /* pcre_study() returns NULL for both errors and when it can not
     * optimize the regex.  The last argument is how one checks for
     * errors (it is NULL if everything works, and points to an error
     * string otherwise. */
    if(pcre_error_str != NULL)
    {
        fprintf(stderr,
                "error: cannot compile '%s' (%s)\n",
                re_kw,
                pcre_error_str);
        free(grammar->re_keywords);
        free(grammar);
        return NULL;
    }

    /* bind root element and increment the reference counter */
    grammar->start = start;
    cleri_object_incref(start);

    return grammar;
}

void cleri_grammar_free(cleri_grammar_t * grammar)
{
    free(grammar->re_keywords);

    if (grammar->re_kw_extra != NULL)
    {
        free(grammar->re_kw_extra);
    }

    cleri_object_decref(grammar->start);
    free(grammar);
}
