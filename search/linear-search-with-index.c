#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_by_position(size_t pos) {
	FILE *fp = fopen("2007.csv", "r");
        fseek(fp, pos, SEEK_SET);
	char line[1000];
	fgets(line, sizeof line, fp);
	fclose(fp);
	printf("%s", line);
}

void linear_search_with_index() {
	FILE *index = fopen("2007.index", "r");
	char num[13];
	char pos[11];

	while(fscanf(index, "%s %s\n", num, pos) != EOF) {
		if (strstr(num, "0857-53-1777")) {
			print_by_position(atoi(pos));
			break;
		}
	}
	fclose(index);
}

void main() {
	linear_search_with_index();
}
