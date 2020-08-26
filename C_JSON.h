#ifndef C_JSON
#define C_JSON
#include <stdio.h>

// Enum for types of values
typedef enum
{
    NULL_V,
    INT_V,
    CHAR_V,
    LIST_V,
    JSON_V
} types;

//element in a list
struct node
{
    void *d;
    struct node *next;
};

//key value pairs
struct kv
{
    char *key;
    void *value;
    int v_type;
    struct kv *next;
};

//list structure
struct list
{
    struct node *head;
    struct node *tail;
    int ltype;
};

//JSON structure
struct JSON
{
    struct kv *head;
    struct kv *tail;
};

//Interface
typedef struct node node_t;
typedef struct list LIST_t;
typedef struct kv KV_t;
typedef struct JSON JSON_t;

node_t *cnode(void *val);                        //creates a node with a value
void print_node(const node_t *n, const types a); //prints node
void free_node(node_t *n, const types t);        //frees node

KV_t *init_KV(const char *key, void *value, const types type); //Initialize key value pair
void print_KV(const KV_t *kv);                                 //print key value pairs
void free_KV(KV_t *kv);                                        //free key value pairs

LIST_t *init_list(const types val_type);   //initialize the list
void print_list(const LIST_t *l);          //prints the list
void insert_to_list(LIST_t *l, void *val); //inserts to a list
void free_list(LIST_t *l);                 //frees the list

JSON_t *init_json();                                                              //initialize JSON object
void display_json(const JSON_t *j);                                               //display json object
void insert_to_JSON(JSON_t *j, const char *key, void *val, const types val_type); //insert value of type to JSON
KV_t *get(const JSON_t *j, const char *key);                                      //return value for a given key in a JSON
void free_json(JSON_t *j);                                                        //frees the JSON

int *int_parse(FILE *fp);     //Parser for integer
char *str_parse(FILE *fp);    //Parser for a string
LIST_t *list_parse(FILE *fp); //Parser for a list
JSON_t *json_parse(FILE *fp); //Parser for a JSON

#endif
