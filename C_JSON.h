#ifndef C_JSON
#define C_JSON
#define INT_V 0
#define CHAR_V 1
#define LIST_V 2
#define JSON_V 3

struct node
{
    void* d;
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

JSON_t *init_json();
void free_json(JSON_t *j);
KV_t *init_KV(char *key, void *value, int type);
void free_KV(KV_t *kv);
void _display_JSON(JSON_t *j);
void display(JSON_t *j);
void insert(JSON_t *j, char *key, void *val, int val_type);
node_t *cnode(void *val);
LIST_t* init_list(int val_type);
void print_list(LIST_t *l);
void _print_list(LIST_t *l);
void linsert(LIST_t *l, void *val);
void freelist(LIST_t *l);
KV_t* get(JSON_t* j,char* key);
#endif
