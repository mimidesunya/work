#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_SIZE 250

void bubble_sort(char *list, const int size) {
	char tmp[NAME_SIZE];
	for (int i = 0; i < size - 1; ++i) {
		char *a = list + i * NAME_SIZE;
		char *b = list + (i + 1) * NAME_SIZE;
		if (strcmp(a, b) > 0) {
			strcpy(tmp, a);
			strcpy(a, b);
			strcpy(b, tmp);
			i = 0;
			continue;
		}
	}
}

void main(int argc, const char *argv[]) {
	const char *file = argv[1];
	const char *index = argv[2];
	char *list = (char*)malloc(NAME_SIZE * 30000000L);
	char line[1000];
	FILE *fp = fopen(file, "r");
	char *i = list;
	while (fgets(line, sizeof line, fp)) {
		char *comma = strchr(line, ',') + 1;
		char *comma2 = strchr(comma, ',');
		*comma2 = 0;
		strcpy(i, comma);
		i += NAME_SIZE;
		if ((i - list) / NAME_SIZE >= 1000) {
			break;
		}
	}
	fclose(fp);
	
	printf("Sorting ... \n");
	const int size = (i - list) / NAME_SIZE;
	bubble_sort(list, size);

	FILE *ip = fopen(index, "w");
	i = list;
	for (int j = 0; j < size; ++j) {
		fprintf(ip, "%s\n", i);
		i += NAME_SIZE;
	}
	fclose(ip);

	free(list);
}
