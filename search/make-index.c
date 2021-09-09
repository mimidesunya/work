#include <stdio.h>
#include <string.h>

void make_index() {
	FILE *fp = fopen("2007.csv", "r");
	FILE *index = fopen("2007.index", "w");
	char line[1000];

	for(;;) {
		size_t pos = ftell(fp);
		char *ret = fgets(line, sizeof line, fp);
		if (!ret) {
			break;
		}
		char *comma = strchr(line, ',');
		*comma = 0;
		fprintf(index, "%12s %010ld\n", line, pos);
	}
	fclose(index);
	fclose(fp);
}

void main() {
	make_index();
}
