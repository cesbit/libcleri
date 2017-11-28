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
#define PCRE2_CODE_UNIT_WIDTH 8

#include <cleri/grammar.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcre2.h>
#include <assert.h>

/*
 * Returns a grammar object or NULL in case of an error.
 *
 * Warning: this function could write to stderr in case the re_keywords could
 * not be compiled.
 */
cleri_grammar_t * cleri_grammar(cleri_t * start, const char * re_keywords)
{
    const char * re_kw = (re_keywords == NULL) ?
            CLERI_DEFAULT_RE_KEYWORDS : re_keywords;

    assert (re_kw[0] == '^');

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

    int pcre_error_num;
    PCRE2_SIZE pcre_error_offset;

    grammar->re_keywords = pcre2_compile(
            (PCRE2_SPTR8) re_kw,
            PCRE2_ZERO_TERMINATED,
            0,
            &pcre_error_num,
            &pcre_error_offset,
            NULL);
    if(grammar->re_keywords == NULL)
    {

        PCRE2_UCHAR buffer[256];
        pcre2_get_error_message(pcre_error_num, buffer, sizeof(buffer));
        /* this is critical and unexpected, memory is not cleaned */
        fprintf(stderr,
                "error: cannot compile '%s' (%s)\n",
                re_kw,
                buffer);
        free(grammar);
        return NULL;
    }

    grammar->match_data = \
        pcre2_match_data_create_from_pattern(grammar->re_keywords, NULL);

    if (grammar->match_data == NULL)
    {
        pcre2_code_free(grammar->re_keywords);
        fprintf(stderr, "error: cannot create matsch data\n");
        free(grammar);
        return NULL;
    }

    /* bind root element and increment the reference counter */
    grammar->start = start;
    cleri_incref(start);

    return grammar;
}

void cleri_grammar_free(cleri_grammar_t * grammar)
{
    pcre2_match_data_free(grammar->match_data);
    pcre2_code_free(grammar->re_keywords);
    cleri_free(grammar->start);
    free(grammar);
}
