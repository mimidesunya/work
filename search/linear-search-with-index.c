#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_by_position(const char *file, size_t pos) {
	FILE *fp = fopen(file, "r");
        fseek(fp, pos, SEEK_SET);
	char line[1000];
	fgets(line, sizeof line, fp);
	fclose(fp);
	printf("%s", line);
}

void linear_search_with_index(const char *file, const char *index, const char *number) {
	FILE *ip = fopen(index, "r");
	char num[13];
	char pos[11];

	while(fscanf(ip, "%s %s\n", num, pos) != EOF) {
		if (strstr(num, number)) {
			print_by_position(file, atoi(pos));
			break;
		}
	}
	fclose(ip);
}

void main(int argc, const char *argv[]) {
	linear_search_with_index(argv[1], argv[2], argv[3]);
}
