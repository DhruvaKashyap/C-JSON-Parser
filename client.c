#include "C_JSON.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
	JSON_t *json;
	json=parse();
	display(json);
	// JSON_t *json2;
	// json = init_json();
	// json2 = init_json();
	// int a = 1;
	// int b = 2;
	// insert(json, "int1", a, INT_V);
	// // display_JSON(json);
	// insert(json, "int2", b, INT_V);
	// // display_JSON(json);
	// insert(json, "char1", "ok", CHAR_V);
	// insert(json2, "null1", NULL, INT_V);
	// insert(json2, "json1", json, JSON_V);
	// LIST_t *l;
	// l = init_list(JSON_V);
	// linsert(l, json);
	// insert(json2, "list", l, LIST_V);
	// // display(json2);
	// KV_t *n=get(json,"a");
	// print_KV(n);
	// print_list(n->value);
	// free_json(json);
	// freelist(l);
	// free_json(json2);

	return 0;
}
