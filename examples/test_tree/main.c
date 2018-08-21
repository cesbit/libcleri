#include <stdio.h>
#include <cleri/cleri.h>
#include "buffer.h"

#define JTAB 4

const char * strtp(cleri_tp tp)
{
    switch (tp)
    {
    case CLERI_TP_SEQUENCE: return "sequence";
    case CLERI_TP_OPTIONAL: return "optional";
    case CLERI_TP_CHOICE: return "choice";
    case CLERI_TP_LIST: return "list";
    case CLERI_TP_REPEAT: return "repeat";
    case CLERI_TP_PRIO: return "prio";
    case CLERI_TP_RULE: return "rule";
    case CLERI_TP_THIS: return " this";
    case CLERI_TP_KEYWORD: return "keyword";
    case CLERI_TP_TOKEN: return "token";
    case CLERI_TP_TOKENS: return "tokens";
    case CLERI_TP_REGEX: return "regex";
    case CLERI_TP_REF: return "ref";
    case CLERI_TP_END_OF_STATEMENT: return "end of statement";
    default: return "unknown type";
    }
}

int prt_JSON(char * string)
{
    int indent = 0;
    int i;

    for (i=0; string[i+1] != '\0'; i++) {
        switch(string[i])
        {
            case '[':
                ++indent;
                if (string[i+1] == ']')
                    printf("[");
                else
                    printf("[\n%*.s", indent*JTAB, "");
                break;
            case '{':
                ++indent;
                if (string[i+1] == '}')
                    printf("{");
                else
                    printf("{\n%*.s", indent*JTAB, "");
                break;
            case '}':
                --indent;
                printf("\n%*.s}", indent*JTAB, "");
                break;
            case ']':
                --indent;
                if (string[i-1] == '[')
                    printf("]");
                else
                    printf("\n%*.s]", indent*JTAB, "");
                break;
            case ',':
                printf(",\n%*.s", indent*JTAB, "");
                break;
            case ':':
                printf(": ");
                break;
            default:
                printf("%c", string[i]);
                break;
        }
    }
    printf("\n%c\n", string[i]); // causes the last character to be printed as the "for" loop will stop before.
    return 0;
}


int view_children(cleri_children_t * child, const char * orig, buffer_t * buf)
{
    int count = 0;
    int rc = 0;
    while (child != NULL)
    {
        cleri_node_t * node = child->node;
        if (!count++)
        {
            rc += buffer_printf(buf, "children:[");
        }
        rc += buffer_printf(buf, "{%s:%s,%s:%.*s,%s:%ld,",
            "type",
            strtp(node->cl_obj->tp),
            "string",
            (int)node->len,
            node->str,
            "position",
            node->str - orig);

        cleri_children_t * grandchild = node->children;
        rc += view_children(grandchild, orig, buf);
        rc += buffer_printf(buf, "}");
        child = child->next;
        if (child != NULL)
            rc += buffer_printf(buf, ",");
    }
    if (!count)
    {
        rc += buffer_printf(buf, "%s:[]", "children");
    }
    else
    {
        rc += buffer_printf(buf, "]");
    }
    return rc;
}

void test_str(cleri_grammar_t * grammar, const char * str)
{
    cleri_parse_t * pr = cleri_parse(grammar, str);
    printf("Test string '%s': %s\n", str, pr->is_valid ? "true" : "false");
    cleri_node_t * tree = pr->tree;
    buffer_t * buf = buffer_create();
    //printf("\n\n\nhi there I reached line 189\n\n\n");
    if (cleri_node_has_children(tree))
    {
        cleri_children_t * child = tree->children;
        int rc = view_children(child, pr->str, buf);
        printf("\n\n\n rc: %d \n\n\n", rc);
        prt_JSON(buf->buf);
        printf("JSON: %s", buf->buf);

    }
    buffer_destroy(buf);
    cleri_parse_free(pr);
}


int main(void)
{
    /* define grammar */
    cleri_t * k_hi = cleri_keyword(0, "hi", 0);
    cleri_t * k_bye = cleri_keyword(0, "bye", 0);
    cleri_t * r_name = cleri_regex(0, "^(?:\"(?:[^\"]*)\")+");
    cleri_t * start = cleri_repeat(0,
                                   cleri_sequence(0,
                                                  2,
                                                  cleri_choice(0,
                                                               0,
                                                               2,
                                                               k_hi,
                                                               k_bye),
                                                  r_name),
                                   0,
                                   0);

    /* compile grammar */
    cleri_grammar_t * my_grammar = cleri_grammar(start, NULL);

    /* test some strings */
    test_str(my_grammar, "hi \"Iris\" bye \"libcleri\"");  // true

    /* cleanup grammar */
    cleri_grammar_free(my_grammar);

    return 0;
}





