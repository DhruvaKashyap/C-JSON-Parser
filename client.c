#include "C_JSON.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
	JSON_t *json;
	FILE *fp = fopen("b.json", "r");
	json = json_parse(fp);
	display_json(json);
	printf("\n");
	free_json(json);
	fclose(fp);
	return 0;
}
