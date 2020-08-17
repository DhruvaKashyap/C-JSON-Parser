#include "C_JSON.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
	JSON_t *json;
	json = init_json();
	int a=1;
	int b=2;
	insert(json, "HI", &a, 0);
	// display_JSON(json);
	insert(json, "BYE", &b, 0);
	// display_JSON(json);
	insert(json, "HI", "ok", 1);
	display_JSON(json);
	free_json(json);
	return 0;
}
