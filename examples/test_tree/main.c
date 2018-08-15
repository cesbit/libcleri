#include <stdio.h>
#include <cleri/cleri.h>
#include <string.h>
#include <ctype.h>
//int view_children(cleri_children_t * child, const char * orig, int n);
//void test_str(cleri_grammar_t * grammar, const char * str);
#define JTAB 8

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
    }
}

int view_children(cleri_children_t * child, const char * orig, int n)
{
    int count = 0;
    while (child != NULL)
    {
        cleri_node_t * node = child->node;
        if (count++ == 0) printf("%*.s%s: [\n", n, "", "children");
        printf("%*.s{\n\t%*.s%s: %s,\n\t%*.s%s: %.*s,\n\t%*.s%s: %ld,\n",
                n,"",
                n,"", "type",
                strtp(node->cl_obj->tp),
                n,"", "string",
                (int)node->len,
                node->str,
                n,"", "position",
                node->str - orig);
        cleri_children_t * grandchild = node->children;
        view_children(grandchild, orig, n+=JTAB);

        printf("%*s}", (n-=JTAB), "");
        child = child->next;
        if (child != NULL) printf(",\n");
        else printf("\n");


    }
    if (count == 0) printf("%*.s%s: []\n", n, "", "children");
    else printf("%*s]\n", (n-=JTAB), "");;
    return 0;
}

void test_str(cleri_grammar_t * grammar, const char * str)
{
    cleri_parse_t * pr = cleri_parse(grammar, str);
    printf("Test string '%s': %s\n", str, pr->is_valid ? "true" : "false");
    cleri_node_t * tree = pr->tree;
    int n;

    if (cleri_node_has_children(tree))
    {
        cleri_children_t * child = tree->children;
        view_children(child, pr->str, n=0);

    }
    cleri_parse_free(pr);
}

// char * prt_JSON(char * string)
// {
//     int count = 0;
//     int bracket_in = 0;
//     int brace_in = 0;

//     while(*string != '\0') {

//         printf("%c", *string);
//         switch(*string)
//         {
//             case '[':
//                 printf("\n");
//                 ++bracket_in;
//                 printf("%*.s", bracket_in*JTAB, "");
//                 break;

//             case '{':
//                 printf("\n");
//                 ++brace_in;
//                 printf("%*.s", brace_in*JTAB, "");
//                 break;

//             case '}':
//                 printf("\n");
//                 --brace_in;
//                 printf("%*.s", brace_in*JTAB, "");
//                 break;

//             case ']':
//                 printf("\n");
//                 --bracket_in;
//                 printf("%*.s", bracket_in*JTAB, "");
//                 break;

//             case ',':
//                 printf("\n");
//                 printf("%*.s", bracket_in*JTAB, "");
//                 break;

//             case ':':
//                 printf(" ");
//                 break;

//             default:
//                 break;

//         }
//         // move the ptr pointer to the next memory location


//         string++;
//         count++;
//     }
//     printf("count: %d  %d   %d", count, brace_in, bracket_in);
//     return 0;
// }


// char * view_children(cleri_children_t * child, const char * orig, char string[600])
// {
//     int count = 0;
//     while (child != NULL)
//     {
//         cleri_node_t * node = child->node;
//         if (count++ == 0) sprintf(string == "0\0" ? string : string + strlen(string), "%s:[", "children");
//         sprintf(string + strlen(string), "{%s:%s,%s:%.*s,%s:%ld,",
//                 "type",
//                 strtp(node->cl_obj->tp),
//                 "string",
//                 (int)node->len,
//                 node->str,
//                 "position",
//                 node->str - orig);

//         cleri_children_t * grandchild = node->children;
//         view_children(grandchild, orig, string);
//         sprintf(string + strlen(string), "}");
//         child = child->next;
//         if (child != NULL) sprintf(string + strlen(string), ",");
//     }
//     if (child == NULL && count ==0) sprintf(string + strlen(string), "%s:[]", "children");
//     else sprintf(string + strlen(string), "]");

//     return string;
// }

// void test_str(cleri_grammar_t * grammar, const char * str)
// {
//     cleri_parse_t * pr = cleri_parse(grammar, str);
//     printf("Test string '%s': %s\n", str, pr->is_valid ? "true" : "false");
//     cleri_node_t * tree = pr->tree;
//     int n;
//     char string[600];
//     sprintf(string, "0");

//     if (cleri_node_has_children(tree))
//     {
//         cleri_children_t * child = tree->children;
//         char * s = view_children(child, pr->str, string);
//         prt_JSON(s);
//         printf("zooooooo: %s", s);

//     }
//     cleri_parse_free(pr);
// }


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


// int view_children(cleri_children_t * child, const char * orig, int n)
// {
//     int count = 0;
//     while (child != NULL)
//     {
//         cleri_node_t * node = child->node;
//         if (count++ == 0) printf("%*.s%s: [\n", n, "", "children");
//         printf("%*.s{\n\t%*.s%s: %s,\n\t%*.s%s: %.*s,\n\t%*.s%s: %ld,\n",
//                 n,"",
//                 n,"", "type",
//                 strtp(node->cl_obj->tp),
//                 n,"", "string",
//                 (int)node->len,
//                 node->str,
//                 n,"", "position",
//                 node->str - orig);
//         cleri_children_t * grandchild = node->children;
//         view_children(grandchild, orig, n+=JTAB);

//         printf("%*s}", (n-=JTAB), "");
//         child = child->next;
//         if (child != NULL) printf(",\n");
//         else printf("\n");


//     }
//     if (count ==0) printf("%*.s%s: []\n", n, "", "children");
//     else printf("%*s]\n", (n-=JTAB), "");;
//     return 0;
// }


// void test_str(cleri_grammar_t * grammar, const char * str)
// {
//     cleri_parse_t * pr = cleri_parse(grammar, str);
//     printf("Test string '%s': %s\n", str, pr->is_valid ? "true" : "false");
//     cleri_node_t * tree = pr->tree;
//     int n;

//     if (cleri_node_has_children(tree))
//     {
//         cleri_children_t * child = tree->children;
//         view_children(child, pr->str, n=0);

//     }
//     cleri_parse_free(pr);
// }



