#include <stdio.h>
#include <string.h>

void search(const char *file, const char *number) {
	FILE* fp = fopen(file, "r");
	char line[1000];

	while(fgets(line, sizeof line, fp)) {
		if (strstr(line, number)) {
			printf("%s", line);
			break;
		}
	}
	fclose(fp);
}

void main(const char *argv[], int argc) {
	search(argv[1], argv[2]);
}
