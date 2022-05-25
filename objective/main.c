#include "hashmap.h"
#include "treemap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main(int argc, const char *argv[]) {
	const size_t size = 30000000L;
	const size_t limit = 10000;
	map* map = create_treemap(size);

	char *data = (char*)malloc(100L * size);
	if (!data) {
		return;
	}
	const char *file = argv[1];
	{
		char line[700];
		FILE *fp = fopen(file, "r");
		char *pdata = data;
		size_t i = 0;
		while (fgets(line, sizeof line, fp)) {
			char *comma = strchr(line, ',');
			char *comma2 = strchr(comma + 1, ',');
			*comma = *comma2 = 0;
			size_t length = comma2 - line + 1;
			memcpy(pdata, line, length);
			
			char* value = pdata;
			char* key = pdata + (comma - line) + 1;
			
			map_insert(map, key, value);
			
			i++;
			if (i % 10000 == 0) {
				printf("build map %ld\n", i);
			}
			if (i > limit) {
				break;
			}
			pdata += length;
		}
		fclose(fp);
	}
	
	printf("search\n");
	{
		char* hit = map_search(map, "花うさぎ");
		printf("%s\n", hit);
	}
	
	{
		char line[700];
		FILE *fp = fopen(file, "r");
		char *pdata = data;
		size_t i = 0;
		while (fgets(line, sizeof line, fp)) {
			char *comma = strchr(line, ',');
			char *comma2 = strchr(comma + 1, ',');
			*comma = *comma2 = 0;
			size_t length = comma2 - line + 1;
			memcpy(pdata, line, length);
			
			char *key = pdata + (comma - line) + 1;
			
			char* value = map_remove(map, key);
			
			i++;
			if (i % 10000 == 0) {
				printf("remove %ld\n", i);
			}
			if (i > limit) {
				break;
			}
			pdata += length;
		}
		fclose(fp);
	}

	map_dispose(map);
}
