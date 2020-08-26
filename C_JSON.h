#ifndef C_JSON
#define C_JSON
#include <stdio.h>

typedef enum
{
    NULL_V,
    INT_V,
    CHAR_V,
    LIST_V,
    JSON_V
} types;

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
typedef struct kv KV_t;
typedef struct JSON JSON_t;

node_t *cnode(void *val);
void print_node(const node_t *n, const types a);
void free_node(node_t *n,const types t);

KV_t *init_KV(const char *key,void *value,const types type);
void print_KV(const KV_t *kv);
void free_KV(KV_t *kv);

JSON_t *init_json();
void display_json(const JSON_t *j);
void insert(JSON_t *j,const char *key,void *val,const types val_type);
KV_t *get(const JSON_t *j, const char *key);
void free_json(JSON_t *j);

LIST_t *init_list(const types val_type);
void print_list(const LIST_t *l);
void linsert(LIST_t *l, void *val);
void free_list(LIST_t *l);

int int_parse(FILE *fp);
char *str_parse(FILE *fp);
LIST_t *list_parse(FILE *fp);
JSON_t *json_parse(FILE *fp);

#endif
