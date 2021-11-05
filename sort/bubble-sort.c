#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
	char* str;
	size_t pos;
} entry_t;

void bubble_sort(entry_t *list, int size) {
	bool swapped;
	do {
		swapped = false;
		for (int i = 1; i < size; ++i) { 
			if (strcmp(list[i - 1].str, list[i].str) > 0) {
				entry_t tmp = list[i - 1];
				list[i - 1] = list[i];
				list[i] = tmp;
				swapped = true;
			}
		}
		--size;
	} while(swapped);
}

void main(int argc, const char *argv[]) {
	const char *file = argv[1];
	const char *index = argv[2];
	char *data = (char*)malloc(100L * 30000000L);
	if (!data) {
		return;
	}
	entry_t *list = (entry_t*)malloc(sizeof(entry_t) * 30000000L);
	if (!list) {
		return;
	}
	char line[700];
	FILE *fp = fopen(file, "r");
	char *pdata = data;
	size_t i = 0;
	size_t pos = 0;
	while (fgets(line, sizeof line, fp)) {
		char *comma = strchr(line, ',') + 1;
		char *comma2 = strchr(comma, ',');
		*comma2 = 0;
		strcpy(pdata, comma);
		list[i].str = pdata;
		list[i].pos = pos;
		pdata += comma2 - comma + 1;
		i++;
		pos = ftell(fp);
		if (i > 100000) {
			break;
		}
	}
	fclose(fp);
	
	printf("Sorting ... \n");
	bubble_sort(list, i);
	printf("Output ... \n");

	FILE *ip = fopen(index, "w");
	for (int j = 0; j < i; ++j) {
		fprintf(ip, "%010ld %-200s\n", list[j].pos, list[j].str);
	}
	fclose(ip);

	free(list);
	free(data);
}
