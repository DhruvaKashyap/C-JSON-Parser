#ifndef C_JSON
#define C_JSON
#define INT_V 0
#define CHAR_V 1
#define LIST_V 2
#define JSON_V 3

struct node
{
    int d;
    struct node *next;
};

struct list
{
    struct node *head;
};

union vals {
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
};

typedef struct node node_t;
typedef struct list LIST_t;
typedef union vals VAL_t;
typedef struct kv KV_t;
typedef struct JSON JSON_t;

void display_JSON(JSON_t *j);
void insert(JSON_t *j, char *key, void *val);
#endif
