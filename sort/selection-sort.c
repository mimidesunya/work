#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
	char* str;
	size_t pos;
} entry_t;

void selection_sort(entry_t *list, int size) {
	for (int i = 0; i < size - 1; ++i) {
		int min = i;
		for (int j = i + 1; j < size; ++j) {
			if (strcmp(list[min].str, list[j].str) > 0) {
				min = j;
			}
		}
		if (min != i) {
                	entry_t tmp = list[min];
                	list[min] = list[i];
                	list[i] = tmp;
		}
	}
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
	selection_sort(list, i);
	printf("Output ... \n");

	FILE *ip = fopen(index, "w");
	for (int j = 0; j < i; ++j) {
		fprintf(ip, "%010ld %-200s\n", list[j].pos, list[j].str);
	}
	fclose(ip);

	free(list);
	free(data);
}
