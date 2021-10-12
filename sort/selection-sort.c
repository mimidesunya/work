#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void selection_sort(char **list, int size) {
	for (int i = 0; i < size - 1; ++i) {
		int min = i;
		for (int j = i + 1; j < size; ++j) {
			if (strcmp(list[min], list[j]) > 0) {
				min = j;
			}
		}
		if (min != i) {
                	char *tmp = list[min];
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
	char **list = (char**)malloc(30000000L);
	if (!list) {
		return;
	}
	char line[1000];
	FILE *fp = fopen(file, "r");
	char *pdata = data;
	size_t i = 0;
	while (fgets(line, sizeof line, fp)) {
		int len = strlen(line);
		if (len >= 1000) {
			break;
		}
		char *comma = strchr(line, ',') + 1;
		char *comma2 = strchr(comma, ',');
		*comma2 = 0;
		strcpy(pdata, comma);
		list[i] = pdata;
		pdata += len + 1;
		i++;
		if ((pdata - data) >= 100L * 30000000L) {
			return;
		}
		if (i >= 10000) {
			break;
		}
	}
	fclose(fp);
	
	printf("Sorting ... \n");
	selection_sort(list, i);

	FILE *ip = fopen(index, "w");
	for (int j = 0; j < i; ++j) {
		fprintf(ip, "%s\n", list[j]);
	}
	fclose(ip);

	free(list);
	free(data);
}
