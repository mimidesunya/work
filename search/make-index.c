#include <stdio.h>
#include <string.h>

void make_index(const char *file, const char *index) {
	FILE *fp = fopen(file, "r");
	FILE *ip = fopen(index, "w");
	char line[1000];

	for(;;) {
		size_t pos = ftell(fp);
		char *ret = fgets(line, sizeof line, fp);
		if (!ret) {
			break;
		}
		char *comma = strchr(line, ',');
		*comma = 0;
		fprintf(ip, "%12s %010ld\n", line, pos);
	}
	fclose(ip);
	fclose(fp);
}

void main(int argc, const char *argv[]) {
	make_index(argv[1], argv[2]);
}
