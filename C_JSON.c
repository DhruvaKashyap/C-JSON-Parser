#include "C_JSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    if (j != NULL)
    {
        if (j->head != NULL)
        {
            KV_t *kv = j->head;
            printf("{");
            while (kv != NULL)
            {
                print_KV(kv);
                if (kv->next != NULL)
                    printf(",");
                kv = kv->next;
            }
            printf("}");
        }
    }
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

LIST_t *init_list(int val_type)
{
    LIST_t *l = malloc(sizeof(LIST_t));
    l->head = NULL;
    l->ltype = val_type;
    return l;
}

void _print_list(LIST_t *l)
{
    if (l != NULL)
    {
        if (l->head != NULL)
        {
            node_t *n = l->head;
            printf("[");
            while (n != NULL)
            {
                if (l->ltype == INT_V)
                    printf("%d", (int)n->d);
                if (l->ltype == CHAR_V)
                    printf("%s", (char *)n->d);
                if (l->ltype == LIST_V)
                {
                    print_list((LIST_t *)n->d);
                }
                if (l->ltype == JSON_V)
                {
                    _display_JSON((JSON_t *)n->d);
                }
                if (n->next != NULL)
                    printf(",");
                n = n->next;
            }
            printf("]");
        }
    }
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
        printf("%s:", kv->key);
        if (kv->v_type == NULL_V)
            printf("NULL");
        if (kv->v_type == INT_V)
            printf("%d", (int)kv->value);
        if (kv->v_type == CHAR_V)
            printf("%s", (char *)kv->value);
        if (kv->v_type == LIST_V)
        {
            print_list((LIST_t *)kv->value);
        }
        if (kv->v_type == JSON_V)
        {
            _display_JSON((JSON_t *)kv->value);
        }
    }
}

JSON_t *parse()
{
    char c;
    int instr = 0;
    JSON_t *j = init_json();
    while ((c = getchar()) != '\n')
    {
        if (c == '\n' || c == '\t' || (instr == 0 && c == ' '))
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
        else
        {
            char key[100];
            void *val;
            int i = 0;
            int typ;
            while (c != '"')
            {
                key[i++] = c;
                c = getchar();
            }
            key[i] = '\0';
            c = getchar(); //:
            if (c == ':')
            {
                c = getchar(); //signifies type
                if (c == ',')
                {
                    typ = NULL_V;
                    val = NULL;
                }
                else if (c <= '9' && c >= '0')
                {
                    typ = INT_V;
                    int num = c - '0';
                    c = getchar();
                    while (c != ',' && c != '}')
                    {
                        num = num * 10 + (c - '0');
                        c = getchar();
                    }
                    val = (void *)num;
                }
                else if (c == '"')
                {
                    typ = CHAR_V;
                    val = (char *)malloc(sizeof(char) * 100);
                    c = getchar();
                    i = 0;
                    while (c != '"')
                    {
                        ((char *)val)[i++] = c;
                        c = getchar();
                    }
                }
                else if (c == '[')
                {
                    typ = LIST_V;
                }
                else if (c == '{')
                {
                    typ = JSON_V;
                    val = parse();
                }
                else
                {
                    break;
                }
                insert(j, key, val, typ);
            }
        }
    }
    return j;
}