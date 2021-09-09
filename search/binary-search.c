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

void binary_search() {
	FILE *index = fopen("2007.index", "r");
	char num[13];
	char pos[11];
	const size_t LINE_SIZE = 12+10+2;
	size_t top, bottom, middle;

	top = 0;
	fseek(index, 0, SEEK_END);
	bottom = ftell(index) / LINE_SIZE;

	while(top != bottom) {
		middle = (top + bottom) / 2;
		fseek(index, middle * LINE_SIZE, SEEK_SET);
		fscanf(index, "%s %s\n", num, pos);
		int ret = strcmp(num, "0857-53-1777");
		if (ret == 0) {
			print_by_position(atoi(pos));
			break;
		}
		if (ret < 0) {
			top = middle;
			continue;
		}
		bottom = middle;
	}
	fclose(index);
}

void main() {
	binary_search();
}
