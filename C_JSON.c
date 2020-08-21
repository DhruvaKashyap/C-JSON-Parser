#include "C_JSON.h"
#include <stdio.h>
#include <stdlib.h>

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
    for (int i = 0; key[i] != '\0'; ++i)
        kv->key[i] = key[i];
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

void display_JSON(JSON_t *j)
{
    if (j != NULL)
    {
        if (j->head != NULL)
        {
            KV_t *kv = j->head;
            printf("{\n");
            while (kv != NULL)
            {
                printf("%s:", kv->key);
                if (kv->v_type == INT_V)
                    printf("%d\n", (int)kv->value);
                if (kv->v_type == CHAR_V)
                    printf("%s\n", (char *)kv->value);
                if (kv->v_type == LIST_V)
                {
                    printf("\n\n");
                    print_list((LIST_t *)kv->value);
                    printf("\n\n");
                }
                if (kv->v_type == JSON_V)
                {
                    printf("\n\n");
                    display_JSON((JSON_t *)kv->value);
                    printf("\n");
                }
                kv = kv->next;
            }
            printf("}\n");
        }
    }
}

void insert(JSON_t *j, char *key, void *val, int val_type)
{
    if (j != NULL)
    {
        if (j->head == NULL)
        {
            j->head = init_KV(key, val, val_type);
        }
        else
        {
            KV_t *n = j->head;
            j->head = init_KV(key, val, val_type);
            j->head->next = n;
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

void print_list(LIST_t *l)
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
                    printf("\n\n");
                    print_list((LIST_t *)n->d);
                    printf("\n\n");
                }
                if (l->ltype == JSON_V)
                {
                    printf("\n\n");
                    display_JSON((JSON_t *)n->d);
                    printf("\n");
                }
                printf(",");
                n = n->next;
            }
            printf("]");
        }
    }
}
void linsert(LIST_t *l, void *v)
{
    if (l != NULL)
    {
        if (l->head == NULL)
        {
            l->head = cnode(v);
        }
        else
        {
            node_t *n = l->head;
            l->head = cnode(v);
            l->head->next = n;
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