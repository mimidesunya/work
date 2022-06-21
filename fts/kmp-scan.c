#include <stdio.h>
#include <string.h>

void scan(const char *file, const char *w) {
	FILE* fp = fopen(file, "rb");

	const int len = strlen(w);
	fseek(fp, 0, SEEK_END);
	const long size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	printf("w=%s, len=%d, size=%ld\n", w, len, size);
	
	/* 部分マッチテーブル作成 */
	int T[len];
	{
		int i = 2, j = 0;
		T[0] = -1;
		T[1] = 0;
		while(i < len) {
			if (w[i - 1] == w[j]) {
				T[i] = ++j;
				++i;
			}
			else if (j > 0) {
				j = T[j];
			}
			else {
				T[i] = 0;
				++i;
			}
		}
	}
	
	/* 探索 */
	long m = 0, i = 0;
	OUTER: while(m + i < size) {
		char c = getc(fp);
		if (c == w[i]) {
			++i;
			if (i == len) {
				fseek(fp, m, SEEK_SET);
				char s[100];
				fread(s, 1, sizeof(s), fp);
				printf("Hit: %ld:%s\n", m, s);
				break;
			}
		}
		else {
			m += i - T[i];
			if (i > 0) {
				i = T[i];
			}
			fseek(fp, m + i, SEEK_SET);
		}
		if ((m + i) % 1000000 == 0) {
			printf("%ld\n", m);
		}
	}
	fclose(fp);
}

void main(int argc, const char *argv[]) {
	scan(argv[1], argv[2]);
}
