#include <stdio.h>
#include <string.h>

void scan(const char *file, const char *w) {
	FILE* fp = fopen(file, "r");

	const int len = strlen(w);
	fseek(fp, 0, SEEK_END);
	const long size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	printf("w=%s, len=%d, size=%ld\n", w, len, size);
	fseek(fp, len, SEEK_SET);
	for(long m = len; m < size; ++m) {
		FOR:
		if (m % 1000000 == 0) {
			printf("%ld\n", m);
		}
		fseek(fp, -len, SEEK_CUR);
		for (int i = 0; i < len; ++i) {
			char c = getc(fp);
			if (c != w[i]) {
				fseek(fp, ++m, SEEK_SET);
				goto FOR;
			}
		}
		m -= len;
		fseek(fp, m, SEEK_SET);
		char s[100];
		fread(s, 1, sizeof(s), fp);
		printf("Hit: %ld:%s\n", m, s);
		break;
	}
	fclose(fp);
}

void main(int argc, const char *argv[]) {
	scan(argv[1], argv[2]);
}
