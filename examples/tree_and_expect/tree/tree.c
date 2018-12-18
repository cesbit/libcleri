#include <stdio.h>
#include <cleri/cleri.h>
#include "tree.h"

/* Prints a JSON string including spaces, tabs and newlines  */
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
                if ((i-1) > 0)
                {
                    if (string[i-1] == '[')
                    printf("]");
                    else
                    printf("\n%*.s]", indent*JTAB, "");
                }
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
    /* Prints the last character, because the "for" loop will stop before. */
    printf("\n%c\n", string[i]);
    return 0;
}

/* Returns the object type as a string */
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

/* Recursive function. Gets children of the parse tree and concatenates them
by using the buffer_printf(). The end result is a JSON string without spaces,
tabs and newlines.*/
int get_children(cleri_children_t * child, const char * orig, buffer_t * buf)
{
    int count = 0;
    int rc = 0;
    while (child != NULL)
    {
        cleri_node_t * node = child->node;
        if (!count++)
            rc += buffer_printf(buf, "\"children\":[");

        rc += buffer_printf(
            buf,
            "{\"%s\":\"%s\",\"%s\":\"%.*s\",\"%s\":%ld,",
            "type",
            strtp(node->cl_obj->tp),
            "string",
            (int)node->len,
            node->str,
            "position",
            node->str - orig);

        cleri_children_t * grandchild = node->children;
        rc += get_children(grandchild, orig, buf);
        rc += buffer_printf(buf, "}");
        child = child->next;
        if (child != NULL)
            rc += buffer_printf(buf, ",");
    }

    if (!count)
        rc += buffer_printf(buf, "%s:[]", "\"children\"");
    else
        rc += buffer_printf(buf, "]");

    /* When error occurs in the buffer_printf(), it returns -1. */
    return rc;
}

/* Prints the parse tree in JSON format. Note that the cl_obj
is NULL for the root node and the first can be found in its children. */
void prt_tree(cleri_grammar_t * grammar, const char * str)
{
    cleri_parse_t * pr = cleri_parse(grammar, str);
    if (pr == NULL)
        abort();

    printf("Test string '%s': %s\n", str, pr->is_valid ? "true" : "false");
    cleri_node_t * tree = pr->tree;

    /* Creates a dynamic buffer that will store the string to be parsed.
    If error occurs, NULL is returned and redirected to goto. */
    buffer_t * buf = buffer_create();
    if (buf == NULL)
        goto prt_tree;

    /* Check if the tree node (root node) has children and if true
    get_children() will be called. */
    if (cleri_node_has_children(tree))
    {
        cleri_children_t * child = tree->children;
        int rc = get_children(child, pr->str, buf);

        /* Prints the end result collected in buf->buf in JSON format including
        space, tabs and newlines */
        if (!rc)
        {
            prt_JSON(buf->buf);
        }
    }
    buffer_destroy(buf);

prt_tree:
    cleri_parse_free(pr);
}


