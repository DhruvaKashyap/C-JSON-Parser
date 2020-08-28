#include "C_JSON.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
	JSON_t *json;
	FILE *fp;
	fp = fopen("a.json", "r");
	if (fp)
	{
		json = json_parse(fp);
		display_json(json);
		printf("\n");
		free_json(json);
		fclose(fp);
	}
	else
	{
		printf("Unable to open FILE\n");
		return 1;
	}
	return 0;
}
