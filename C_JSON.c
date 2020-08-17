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
                    printf("%d\n", *(int*)kv->value);
                if (kv->v_type == CHAR_V)
                    printf("%s\n", (char *)kv->value);
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
