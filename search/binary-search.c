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

void binary_search(const char *file, const char*index, const char *number) {
	FILE *ip = fopen(index, "r");
	char num[13];
	char pos[11];
	const size_t LINE_SIZE = 12+10+2;
	size_t top, bottom, middle;

	top = 0;
	fseek(ip, 0, SEEK_END);
	bottom = ftell(ip) / LINE_SIZE;

	while(top < bottom) {
		middle = (top + bottom) / 2;
		fseek(ip, middle * LINE_SIZE, SEEK_SET);
		fscanf(ip, "%s %s\n", num, pos);
		int ret = strcmp(num, number);
		if (ret == 0) {
			print_by_position(file, atol(pos));
			break;
		}
		if (ret < 0) {
			top = middle + 1;
			continue;
		}
		bottom = middle;
	}
	fclose(ip);
}

void main(int argc, const char *argv[]) {
	binary_search(argv[1], argv[2], argv[3]);
}
