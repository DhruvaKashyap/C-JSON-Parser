#include "C_JSON.h"
#include <stdlib.h>
#include <string.h>
#define STR_MAX 1000000

JSON_t *init_json()
{
    JSON_t *j;
    j = (JSON_t *)malloc(sizeof(JSON_t));
    j->head = NULL;
    return j;
}

void free_json(JSON_t *j)
{
    KV_t *n;
    KV_t *t;
    if (j != NULL)
    {
        n = j->head;
        t;
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

KV_t *init_KV(char *key, void *value, types type)
{
    KV_t *kv;
    kv = (KV_t *)malloc(sizeof(KV_t));
    kv->key = (char *)malloc(sizeof(key));
    strcpy(kv->key, key);
    kv->value = value;
    kv->v_type = type;
    kv->next = NULL;
    return kv;
}

void free_KV(KV_t *kv)
{
    if (kv != NULL)
        free(kv->key);
    free(kv);
    kv = NULL;
}

void display_json(JSON_t *j)
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

void insert(JSON_t *j, char *key, void *val, types val_type)
{
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

node_t *cnode(void *val)
{
    node_t *n;
    n = (node_t *)malloc(sizeof(node_t));
    n->d = val;
    n->next = NULL;
    return n;
}

void print_node(node_t *n, types a)
{
    if (n != NULL)
    {
        switch (a)
        {
        case NULL_V:
            printf("null");
            break;
        case INT_V:
            printf("%d", (int)n->d);
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
        if(n->next!=NULL)
            printf(",");
    }
}

LIST_t *init_list(types val_type)
{
    LIST_t *l;
    l = (LIST_t *)malloc(sizeof(LIST_t));
    l->head = NULL;
    l->ltype = val_type;
    return l;
}

void print_list(LIST_t *l)
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

void linsert(LIST_t *l, void *v)
{
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
            free(n);
            n = t;
        }
    }
    free(l);
    l = NULL;
}

KV_t *get(JSON_t *j, char *key)
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

void print_KV(KV_t *kv)
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
            printf("%d", (int)kv->value);
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

int int_parse(FILE *fp)
{
    char c;
    c = fgetc(fp);
    int num;
    num = c - '0';
    c = fgetc(fp);
    while (c != EOF && c != ',' && c != '}' && c != '\n' && c != ' ')
    {
        if (c > '9' || c < '0')
        {
            printf(":%c:,7.PARSE ERROR not a number!!\n", c);
            return -1;
        }
        num = num * 10 + (c - '0');
        c = fgetc(fp);
    }
    if (c == EOF)
        return -1;
    if (c == ',' || c == '}')
        ungetc(c, fp);
    return num;
}

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
    val = (char *)malloc(sizeof(char) * STR_MAX);
    while (c != EOF && (c != '"'))
    {
        if (esc == 1)
        {
            if (c == 'n')
                c = '\n';
            else if (c == 't')
                c = '\t';
            esc = 0;
        }
        if (c == '\\')
            esc = 1;
        else
            val[i++] = c;
        c = fgetc(fp);
        if (esc && c == '"')
        {
            val[i++] = '\"';
            c = fgetc(fp);
        }
    }
    val[i] = '\0';
    if (c == EOF)
    {
        free(val);
        return NULL;
    }
    return val;
}

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
                val = (int)int_parse(fp);
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
            linsert(l, val);
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

JSON_t *json_parse(FILE *fp)
{
    char c;
    JSON_t *j = init_json();
    int num_lines = 0;
    char key[STR_MAX];
    void *val;
    int i = 0;
    types typ;
    char tp;
    while ((c = fgetc(fp)) != EOF && (c == '\n' || c == '\t' || c == ' '))
        num_lines += c == '\n';
    if (c == EOF)
    {
        printf("0.PARSE ERROR. Blank file\n");
        free_json(j);
        return NULL;
    }
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
                    key[i++] = c;
                    c = fgetc(fp);
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
                        int num = int_parse(fp);
                        if (num == -1)
                        {
                            printf("7.PARSE ERROR.");
                            printf(" int ");
                            printf("read error for key %s on line:%d\n", key, num_lines);
                            free_json(j);
                            return NULL;
                        }
                        else
                            val = (int)num;
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
                            c = fgetc(fp);      //f
                            c = fgetc(fp);      //a
                            c = fgetc(fp);      //l
                            c = fgetc(fp);      //s
                            c = fgetc(fp);      //e
                        }
                        else if (tp == 'n')
                        {
                            c = fgetc(fp);      //n
                            c = fgetc(fp);      //u
                            c = fgetc(fp);      //l
                            c = fgetc(fp);      //l
                        }
                        else if (tp == 't')
                        {
                            c = fgetc(fp);      //t
                            c = fgetc(fp);      //r
                            c = fgetc(fp);      //u
                            c = fgetc(fp);      //e
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
                    insert(j, key, val, typ);
                }
                else
                {
                    printf("3.PARSE ERROR. No : after key on line:%d got %c\n", num_lines, c);
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
        return NULL;
    }
    return j;
}