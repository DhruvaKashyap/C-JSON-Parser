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

typedef enum
{
    MARKDOWN,
    CODE,
} cell_types;

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
void print_node(node_t *n, types a);

KV_t *init_KV(char *key, void *value, types type);
void print_KV(KV_t *kv);
void free_KV(KV_t *kv);

JSON_t *init_json();
void display_json(JSON_t *j);
void insert(JSON_t *j, char *key, void *val, types val_type);
KV_t *get(JSON_t *j, char *key);
void free_json(JSON_t *j);

LIST_t *init_list(types val_type);
void print_list(LIST_t *l);
void linsert(LIST_t *l, void *val);
void free_list(LIST_t *l);

int int_parse(FILE *fp);
char *str_parse(FILE *fp);
LIST_t *list_parse(FILE *fp);
JSON_t *json_parse(FILE *fp);

void ipynbtopy(JSON_t *j);

#endif
