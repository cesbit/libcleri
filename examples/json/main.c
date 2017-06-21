#include <stdio.h>
#include <cleri/object.h>
#include "json.h"

const char * TestJSON = "{\"Name\": \"Iris\", \"Age\": 4}";

int main(void)
{
    cleri_grammar_t * json_grammar = compile_grammar();
    cleri_parse_t * pr = cleri_parse(json_grammar, TestJSON);

    printf("Test '%s': %s\n", TestJSON, pr->is_valid ? "true" : "false");

    /* cleanup */
    cleri_parse_free(pr);
    cleri_grammar_free(json_grammar);

    return 0;
}
