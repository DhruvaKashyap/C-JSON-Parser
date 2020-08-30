#include "C_JSON.h"
#include <stdlib.h>
#include <string.h>
//Maximum legth of a string
#define STR_MAX 1000

//creates a node with a value
node_t *cnode(void *val)
{
    node_t *n;
    n = (node_t *)malloc(sizeof(node_t));
    n->d = val;
    n->next = NULL;
    return n;
}

//prints node
void print_node(const node_t *n, const types a)
{
    if (n != NULL)
    {
        switch (a)
        {
        case NULL_V:
            printf("null");
            break;
        case INT_V:
            printf("%d", *(int *)n->d);
            break;
        case CHAR_V:
            printf("\"%s\"", (char *)n->d);
            break;
        case LIST_V:
            print_list((LIST_t *)n->d);
            break;
        case JSON_V:
            display_json((JSON_t *)n->d);
            break;
        default:
            break;
        }
        if (n->next != NULL)
            printf(",");
    }
}

//frees node
void free_node(node_t *n, types t)
{
    if (n != NULL)
    {
        switch (t)
        {
        case CHAR_V:
        case INT_V:
            free(n->d);
            break;
        case LIST_V:
            free_list(n->d);
            break;
        case JSON_V:
            free_json(n->d);
            break;
        default:
            break;
        }
    }
    free(n);
    n = NULL;
}

//Initialize key value pair
KV_t *init_KV(const char *key, void *value, const types type)
{
    KV_t *kv;
    kv = (KV_t *)malloc(sizeof(KV_t));
    kv->key = (char *)malloc(sizeof(char) * (strlen(key) + 1));
    strcpy(kv->key, key);
    kv->value = value;
    kv->v_type = type;
    kv->next = NULL;
    return kv;
}

//print key value pairs
void print_KV(const KV_t *kv)
{
    if (kv != NULL)
    {
        printf("\"%s\":", kv->key);
        switch (kv->v_type)
        {
        case NULL_V:
            printf("null");
            break;
        case INT_V:
            printf("%d", *(int *)kv->value);
            break;
        case CHAR_V:
            printf("\"%s\"", (char *)kv->value);
            break;
        case LIST_V:
            print_list((LIST_t *)kv->value);
            break;
        case JSON_V:
            display_json((JSON_t *)kv->value);
            break;
        default:
            break;
        }
    }
}

//free key value pairs
void free_KV(KV_t *kv)
{
    if (kv != NULL)
    {
        free(kv->key);
        switch (kv->v_type)
        {
        case CHAR_V:
        case INT_V:
            free(kv->value);
            break;
        case LIST_V:
            free_list(kv->value);
            break;
        case JSON_V:
            free_json(kv->value);
            break;
        default:
            break;
        }
    }
    free(kv);
    kv = NULL;
}

//initialize the list
LIST_t *init_list(types val_type)
{
    LIST_t *l;
    l = (LIST_t *)malloc(sizeof(LIST_t));
    l->head = NULL;
    l->ltype = val_type;
    return l;
}

//prints the list
void print_list(const LIST_t *l)
{
    node_t *n;
    printf("[");
    if (l != NULL)
    {
        if (l->head != NULL)
        {
            n = l->head;
            while (n != NULL)
            {
                print_node(n, l->ltype);
                n = n->next;
            }
        }
    }
    printf("]");
}

//inserts to a list
void insert_to_list(LIST_t *l, void *v)
{
    //insert at end
    node_t *n;
    if (l != NULL)
    {
        if (l->head == NULL)
            l->head = l->tail = cnode(v);
        else
        {
            n = l->tail;
            l->tail = cnode(v);
            n->next = l->tail;
        }
    }
}

//frees the list
void free_list(LIST_t *l)
{
    node_t *n;
    node_t *t;
    if (l != NULL)
    {
        n = l->head;
        while (n != NULL)
        {
            t = n->next;
            free_node(n, l->ltype);
            n = t;
        }
    }
    free(l);
    l = NULL;
}

//initialize JSON object
JSON_t *init_json()
{
    JSON_t *j;
    j = (JSON_t *)malloc(sizeof(JSON_t));
    j->head = NULL;
    return j;
}

//display json object
void display_json(const JSON_t *j)
{
    KV_t *kv;
    printf("{");
    if (j != NULL)
    {
        if (j->head != NULL)
        {
            kv = j->head;
            while (kv != NULL)
            {
                print_KV(kv);
                if (kv->next != NULL)
                    printf(",");
                kv = kv->next;
            }
        }
    }
    printf("}");
}

//insert value of type to JSON
void insert_to_JSON(JSON_t *j, const char *key, void *val, const types val_type)
{
    //inserts at the end
    KV_t *n;
    if (j != NULL)
    {
        if (j->head == NULL)
            j->head = j->tail = init_KV(key, val, val_type);
        else
        {
            n = j->tail;
            j->tail = init_KV(key, val, val_type);
            n->next = j->tail;
        }
    }
}

//return value for a given key in a JSON
KV_t *get(const JSON_t *j, const char *key)
{
    KV_t *n;
    if (j != NULL)
    {
        n = j->head;
        while (strcmp(n->key, key))
            n = n->next;
        return n;
    }
    return NULL;
}

//frees the JSON
void free_json(JSON_t *j)
{
    KV_t *n;
    KV_t *t;
    if (j != NULL)
    {
        n = j->head;
        while (n != NULL)
        {
            t = n->next;
            free_KV(n);
            n = t;
        }
    }
    free(j);
    j = NULL;
}

//Parser for integer
int *int_parse(FILE *fp)
{
    char c;
    c = fgetc(fp);
    int *num;
    num = (int *)malloc(sizeof(int));
    *num = c - '0';
    c = fgetc(fp);
    while (c != EOF && c != ',' && c != '}' && c != ']' && c != '\n' && c != ' ')
    {
        if (c > '9' || c < '0')
        {
            printf(":%c:,7.PARSE ERROR not a number!!\n", c);
            return NULL;
        }
        *num = *num * 10 + (c - '0');
        c = fgetc(fp);
    }
    if (c == EOF)
        return NULL;
    if (c == ',' || c == '}' || c == ']')
        ungetc(c, fp);
    return num;
}

//Parser for a string
char *str_parse(FILE *fp)
{
    char *val;
    char c;
    int i;
    int esc;
    c = fgetc(fp);
    i = 0;
    esc = 0;
    if (c == '"')
        c = fgetc(fp);
    else
        return NULL;
    val = (char *)malloc(sizeof(char) * (STR_MAX + 1));
    while (c != EOF && (esc || c != '"'))
    {
        if (esc == 1)
        {
            if (c == 'n')
                c = '\n';
            else if (c == 't')
                c = '\t';
            else if (c == '"')
                c = '\"';
            esc = 0;
        }
        if (c == '\\')
            esc = 1;
        else if (i < STR_MAX)
            val[i++] = c;
        c = fgetc(fp);
    }
    if (i == STR_MAX)
    {
        printf("WARNING .String size too large. Truncating string to %d characters.\n", STR_MAX);
    }
    val[i] = '\0';
    if (c == EOF)
    {
        free(val);
        return NULL;
    }
    return val;
}

//Parser for a list
LIST_t *list_parse(FILE *fp)
{
    char c;
    types typ;
    c = fgetc(fp);
    LIST_t *l;
    void *val;
    if (c == '[')
    {
        while (((c = fgetc(fp)) == ' ' || c == '\n' || c == '\t') && c != EOF)
            ;
        if (c == EOF)
            return NULL;
        if (c == ']')
            return init_list(NULL_V);
        switch (c)
        {
        case '"':
            typ = CHAR_V;
            break;
        case '{':
            typ = JSON_V;
            break;
        case '[':
            typ = LIST_V;
            break;
        case ',':
            typ = NULL_V;
            break;
        default:
            typ = INT_V;
            break;
        }
        ungetc(c, fp);
        l = init_list(typ);
        val = NULL;
        while (c != EOF && c != ']' && c != '\n')
        {
            if (typ == JSON_V)
                val = (JSON_t *)json_parse(fp);
            else if (typ == LIST_V)
                val = (LIST_t *)list_parse(fp);
            else if (typ == CHAR_V)
                val = (char *)str_parse(fp);
            else if (typ == INT_V)
                val = (int *)int_parse(fp);
            else
                val = NULL;
            while ((c = fgetc(fp)) != EOF && (c == '\n' || c == '\t' || c == ' '))
                ;
            if (c != ',' && c != ']' && c != '\n')
            {
                printf("no comma in list\n");
                free_list(l);
                return NULL;
            }
            insert_to_list(l, val);
            if (c != ']')
            {
                while (c != ']' && (c = fgetc(fp)) != EOF && (c == '\n' || c == '\t' || c == ' '))
                    ;
                ungetc(c, fp);
            }
        }
        return l;
    }
    return NULL;
}

//Parser for a JSON
JSON_t *json_parse(FILE *fp)
{
    char c;
    JSON_t *j;
    int num_lines = 0;
    char key[STR_MAX + 1];
    void *val;
    int i = 0;
    types typ;
    char tp;
    int esc = 0;
    while ((c = fgetc(fp)) != EOF && (c == '\n' || c == '\t' || c == ' '))
        num_lines += c == '\n';
    if (c == EOF)
    {
        printf("0.PARSE ERROR. Blank file\n");
        return NULL;
    }
    j = init_json();
    if (c == '{')
    {
        while (c != EOF && (c == '{' || c == ',' || c == '\n') && c != '}')
        {
            while ((c = fgetc(fp)) != EOF && (c == '\n' || c == '\t' || c == ' '))
                num_lines += c == '\n';
            if (c == '"')
            {
                i = 0;
                c = fgetc(fp);
                while (c != EOF && c != '"')
                {
                    if (esc == 1)
                    {
                        if (c == 'n')
                            c = '\n';
                        else if (c == 't')
                            c = '\t';
                        else if (c == '"')
                            c = '\"';
                        esc = 0;
                    }
                    if (c == '\\')
                        esc = 1;
                    else if (i < STR_MAX)
                        key[i++] = c;
                    c = fgetc(fp);
                }
                if (i == STR_MAX)
                {
                    printf("WARNING .Key size too large on line %d. Truncating string to %d characters.\n", num_lines, STR_MAX);
                    while (c != EOF && (esc || c != '"'))
                    {
                        if (esc == 1)
                        {
                            if (c == 'n')
                                c = '\n';
                            else if (c == 't')
                                c = '\t';
                            else if (c == '"')
                                c = '\"';
                            esc = 0;
                        }
                        if (c == '\\')
                            esc = 1;
                        c = fgetc(fp);
                    }
                }
                key[i] = '\0';
                if (c == EOF)
                {
                    printf("4.PARSE ERROR. Incomplete JSON file key on line:%d\n", num_lines);
                    free_json(j);
                    return NULL;
                }
                while ((c = fgetc(fp)) != EOF && (c == '\n' || c == '\t' || c == ' '))
                    num_lines += c == '\n';
                if (c == EOF)
                {
                    printf("5.PARSE ERROR. Incomplete JSON file:%d\n", num_lines);
                    free_json(j);
                    return NULL;
                }
                if (c == ':')
                {
                    while ((c = fgetc(fp)) != EOF && (c == '\n' || c == '\t' || c == ' '))
                        num_lines += c == '\n';
                    if (c == EOF)
                    {
                        printf("6.PARSE ERROR. Incomplete JSON file(No value for key %s):%d\n", key, num_lines);
                        free_json(j);
                        return NULL;
                    }
                    tp = c;
                    ungetc(tp, fp);
                    if (tp <= '9' && tp >= '0')
                    {
                        typ = INT_V;
                        val = (int *)int_parse(fp);
                    }
                    else if (tp == '"')
                    {
                        typ = CHAR_V;
                        val = (void *)str_parse(fp);
                    }
                    else if (tp == '[')
                    {
                        typ = LIST_V;
                        val = (void *)list_parse(fp);
                    }
                    else if (tp == '{')
                    {
                        typ = JSON_V;
                        val = (void *)json_parse(fp);
                    }
                    else
                    {
                        if (tp == 'f')
                        {
                            c = fgetc(fp); //f
                            c = fgetc(fp); //a
                            c = fgetc(fp); //l
                            c = fgetc(fp); //s
                            c = fgetc(fp); //e
                        }
                        else if (tp == 'n')
                        {
                            c = fgetc(fp); //n
                            c = fgetc(fp); //u
                            c = fgetc(fp); //l
                            c = fgetc(fp); //l
                        }
                        else if (tp == 't')
                        {
                            c = fgetc(fp); //t
                            c = fgetc(fp); //r
                            c = fgetc(fp); //u
                            c = fgetc(fp); //e
                        }
                        typ = NULL_V;
                        val = NULL;
                    }
                    if (((typ != NULL_V && typ != INT_V) && val == NULL))
                    {
                        printf("7.PARSE ERROR. read error for key %s on line:%d\n", key, num_lines);
                        free_json(j);
                        return NULL;
                    }
                    while ((c = fgetc(fp)) != EOF && (c == '\n' || c == '\t' || c == ' '))
                        num_lines += c == '\n';
                    if (c != ',' && c != '}' && c != '\n')
                    {
                        printf("8.PARSE ERROR. Incomplete JSON file(No , for key %s):%d got %c\n", key, num_lines, c);
                        free_json(j);
                        return NULL;
                    }
                    insert_to_JSON(j, key, val, typ);
                }
                else
                {
                    printf("3.PARSE ERROR. No : after key on line:%d got %c\n", num_lines, c);
                    free_json(j);
                    return NULL;
                }
            }
            else if (c == '}')
                return j;
            else
            {
                printf("2.PARSE ERROR. Key not a string on line:%d\n", num_lines);
                free_json(j);
                return NULL;
            }
        }
        if (c == '}')
            return j;
    }
    else
    {
        printf("1.PARSE ERROR. Not a JSON. Line :%d got %c\n", num_lines, c);
        free_json(j);
        return NULL;
    }
    return j;
}