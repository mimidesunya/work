#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void bubble_sort(char **list, const int size) {
	char *tmp;
	for (int i = 0; i < size - 1; ++i) {
		if (strcmp(list[i], list[i + 1]) > 0) {
			tmp = list[i];
			list[i] = list[i + 1];
			list[i + 1] = tmp;
			i = 0;
			continue;
		}
	}
}

void main(int argc, const char *argv[]) {
	const char *file = argv[1];
	const char *index = argv[2];
	char *data = (char*)malloc(100L * 30000000L);
	char **list = (char**)malloc(30000000L);
	char line[1000];
	FILE *fp = fopen(file, "r");
	char *pdata = data;
	size_t i = 0;
	while (fgets(line, sizeof line, fp)) {
		char *comma = strchr(line, ',') + 1;
		char *comma2 = strchr(comma, ',');
		*comma2 = 0;
		strcpy(pdata, comma);
		list[i] = pdata;
		pdata += strlen(comma) + 1;
		i++;
		if (i >= 3000) {
			break;
		}
	}
	fclose(fp);
	
	printf("Sorting ... \n");
	bubble_sort(list, i);

	FILE *ip = fopen(index, "w");
	for (int j = 0; j < i; ++j) {
		fprintf(ip, "%s\n", list[j]);
	}
	fclose(ip);

	free(list);
	free(data);
}
