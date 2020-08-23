#ifndef C_JSON
#define C_JSON
#define NULL_V 0
#define INT_V 1
#define CHAR_V 2
#define LIST_V 3
#define JSON_V 4

#include <stdio.h>

struct node
{
    void *d;
    struct node *next;
};

struct list
{
    struct node *head;
    struct node *tail;
    int ltype;
};

union vals
{
    int num;
    char *str;
    struct list *lhead;
    struct JSON *jhead;
};

struct kv
{
    char *key;
    void *value;
    int v_type;
    struct kv *next;
};

struct JSON
{
    struct kv *head;
    struct kv *tail;
};

typedef struct node node_t;
typedef struct list LIST_t;
typedef union vals VAL_t;
typedef struct kv KV_t;
typedef struct JSON JSON_t;

node_t *cnode(void *val);

KV_t *init_KV(char *key, void *value, int type);
void print_KV(KV_t *kv);
void free_KV(KV_t *kv);

JSON_t *init_json();
void _display_JSON(JSON_t *j);
void display(JSON_t *j);
void insert(JSON_t *j, char *key, void *val, int val_type);
KV_t *get(JSON_t *j, char *key);
JSON_t *json_parse(FILE *fp);
void free_json(JSON_t *j);

LIST_t *init_list(int val_type);
void print_list(LIST_t *l);
void _print_list(LIST_t *l);
void linsert(LIST_t *l, void *val);
LIST_t *list_parse(FILE *fp);
void freelist(LIST_t *l);

#endif
