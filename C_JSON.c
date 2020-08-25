#include "C_JSON.h"
#include <stdlib.h>
#include <string.h>
#define STR_MAX 1000
JSON_t *init_json()
{
    JSON_t *j = (JSON_t *)malloc(sizeof(JSON_t));
    j->head = NULL;
    return j;
}

void free_json(JSON_t *j)
{
    if (j != NULL)
    {
        KV_t *n = j->head;
        KV_t *t;
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

KV_t *init_KV(char *key, void *value, int type)
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
    {
        free(kv->key);
    }
    free(kv);
    kv = NULL;
}

void _display_JSON(JSON_t *j)
{
    printf("{");
    if (j != NULL)
    {
        if (j->head != NULL)
        {
            KV_t *kv = j->head;

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

void insert(JSON_t *j, char *key, void *val, int val_type)
{
    if (j != NULL)
    {
        if (j->head == NULL)
        {
            j->head = j->tail = init_KV(key, val, val_type);
        }
        else
        {
            KV_t *n = j->tail;
            j->tail = init_KV(key, val, val_type);
            n->next = j->tail;
        }
    }
}

node_t *cnode(void *val)
{
    node_t *n = malloc(sizeof(node_t));
    n->d = val;
    n->next = NULL;
    return n;
}

void print_node(node_t *n, types a)
{
    if (a == NULL_V)
        printf("null");
    if (a == INT_V)
        printf("%d", (int)n->d);
    if (a == CHAR_V)
        printf("\"%s\"", (char *)n->d);
    if (a == LIST_V)
    {
        _print_list((LIST_t *)n->d);
    }
    if (a == JSON_V)
    {
        _display_JSON((JSON_t *)n->d);
    }
    if (n->next != NULL)
        printf(",");
}
LIST_t *init_list(int val_type)
{
    LIST_t *l = malloc(sizeof(LIST_t));
    l->head = NULL;
    l->ltype = val_type;
    return l;
}

void _print_list(LIST_t *l)
{
    printf("[");
    if (l != NULL)
    {
        if (l->head != NULL)
        {
            node_t *n = l->head;

            while (n != NULL)
            {
                print_node(n, l->ltype);
                n = n->next;
            }
        }
    }
    printf("]");
}

void print_list(LIST_t *l)
{
    _print_list(l);
    printf("\n");
}

void linsert(LIST_t *l, void *v)
{
    if (l != NULL)
    {
        if (l->head == NULL)
        {
            l->head = l->tail = cnode(v);
        }
        else
        {
            node_t *n = l->tail;
            l->tail = cnode(v);
            n->next = l->tail;
        }
    }
}
void freelist(LIST_t *l)
{
    if (l != NULL)
    {
        node_t *n = l->head;
        node_t *t;
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

void display(JSON_t *j)
{
    _display_JSON(j);
    printf("\n");
}

KV_t *get(JSON_t *j, char *key)
{
    KV_t *n = j->head;
    while (strcmp(n->key, key))
    {
        n = n->next;
    }
    if (n != NULL)
        return n;
    return NULL;
}

void print_KV(KV_t *kv)
{
    if (kv != NULL)
    {
        printf("\"%s\":", kv->key);
        if (kv->v_type == NULL_V)
            printf("null");
        if (kv->v_type == INT_V)
            printf("%d", (int)kv->value);
        if (kv->v_type == CHAR_V)
            printf("\"%s\"", (char *)kv->value);
        if (kv->v_type == LIST_V)
        {
            _print_list((LIST_t *)kv->value);
        }
        if (kv->v_type == JSON_V)
        {
            _display_JSON((JSON_t *)kv->value);
        }
    }
}

LIST_t *list_parse(FILE *fp)
{
    char c;
    int typ;
    c = fgetc(fp);
    if (c == '[')
    {
        c = fgetc(fp);
        while (c == ' ' || c == '\n' || c == '\t')
            c = fgetc(fp);
        if (c == ']')
        {
            return NULL;
        }
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
        LIST_t *l = init_list(typ);
        void *val = NULL;
        while (c != EOF && c != ']')
        {
            c = fgetc(fp);
            while (c == ' ' || c == '\n' || c == '\t')
                c = fgetc(fp);
            if (c == ',')
                c = fgetc(fp);
            if (c == ']')
            {
                break;
            }
            if (typ == JSON_V)
            {
                val = (JSON_t *)json_parse(fp);
            }
            else if (typ == LIST_V)
            {
                ungetc(c, fp);
                val = list_parse(fp);
                c = fgetc(fp);
            }
            else if (typ == CHAR_V)
            {
                val = (char *)malloc(sizeof(char) * STR_MAX);
                c = fgetc(fp);
                int i = 0;
                while (c != '"')
                {
                    ((char *)val)[i++] = c;
                    c = fgetc(fp);
                }
                c = fgetc(fp);
                // printf(":%s:\n",(char*)val);
            }
            else if (typ == NULL_V)
                val = NULL;
            while (c == ' ' || c == '\n' || c == '\t')
                c = fgetc(fp);
            linsert(l, val);
        }
        return l;
    }
    return NULL;
}

JSON_t *json_parse(FILE *fp)
{
    char c;
    int instr = 0;
    JSON_t *j = init_json();
    while ((c = fgetc(fp)) != EOF)
    {
        // printf("%c\n", c);
        if (c == '\n' || c == '\t' || c == ' ')
        {
            ;
        }
        else if (c == '"')
        {
            instr = !instr;
        }
        else if (c == '{' || c == ',')
        {
            ;
        }
        else if (c == '}')
        {
            break;
        }
        else if (instr)
        {
            char key[STR_MAX];
            void *val;
            int i = 0;
            int typ;
            char tp;
            while (c != '"')
            {
                key[i++] = c;
                c = fgetc(fp);
                // printf("%c",c);
            }
            key[i] = '\0';
            while (c != ':' && c != EOF)
                c = fgetc(fp);
            // printf("%c",c);
            if (c == ':')
            {
                c = fgetc(fp);
                while (c == ' ' || c == '\n' || c == '\t')
                    c = fgetc(fp);
                tp = c;
                // printf("typ=%c\n", tp);
                if (tp == ',')
                {
                    typ = NULL_V;
                    val = NULL;
                }
                else if (tp <= '9' && tp >= '0')
                {
                    typ = INT_V;
                    int num = c - '0';
                    c = fgetc(fp);
                    while (c != ',' && c != '}' && c != '\n')
                    {
                        // printf("%c",c);
                        if (c > '9' || c < '0')
                        {
                            printf(":%c:,1.PARSE ERROR!!\n", c);
                            free_json(j);
                            return NULL;
                        }
                        num = num * 10 + (c - '0');
                        c = fgetc(fp);
                    }
                    val = (void *)num;
                }
                else if (tp == '"')
                {
                    typ = CHAR_V;
                    val = (char *)malloc(sizeof(char) * STR_MAX);
                    c = fgetc(fp);
                    i = 0;
                    while (c != '"')
                    {
                        ((char *)val)[i++] = c;
                        c = fgetc(fp);
                    }
                }
                else if (tp == '[')
                {
                    typ = LIST_V;
                    ungetc('[', fp);
                    val = list_parse(fp);
                    c = fgetc(fp);
                }
                else if (tp == '{')
                {
                    typ = JSON_V;
                    ungetc('{', fp);
                    val = json_parse(fp);
                    c = fgetc(fp);
                }
                else
                {
                    if (c == 'n' && ((c = fgetc(fp)) == 'u') && ((c = fgetc(fp)) == 'l') && ((c = fgetc(fp)) == 'l'))
                    {
                        typ = NULL_V;
                        val = NULL;
                    }
                    else
                    {
                        printf("\n:%c: 2.PARSE ERROR!!\n", tp);
                        free_json(j);
                        return NULL;
                    }
                }
                insert(j, key, val, typ);
            }
            instr = 0;
        }
        else
        {
            printf("\n:%c: 3.PARSE ERROR!!\n", c);
            free_json(j);
            return NULL;
        }
    }
    return j;
}

typedef enum{
    MARKDOWN,
    CODE,
}cell_types;

void ipynbtopy(JSON_t *j)
{
    KV_t *cells = get(j, "cells");
    // print_KV(n);
    // printf("\n");
    LIST_t *l = cells->value;
    // print_list(l);
    // printf("\n");
    node_t *n = l->head;
    KV_t *ctype;
    KV_t *source;
    while (n != NULL)
    {
        // print_node(n,l->ltype);
        // printf("\n\n\n\n%d\n",l->ltype);
        // display((JSON_t*)n->d);
        ctype = get((JSON_t*)n->d, "cell_type");
        // print_KV(ctype);
        // printf("%s\n",(char*)ctype->value);
        if(strcmp((char*)ctype->value,"markdown")==0)
            printf("#\n");
        source=get((JSON_t*)n->d, "source");
        LIST_t *ln=(LIST_t*)source->value;
        print_list(ln);
        n = n->next;
    }
}