#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// gcc fts/search-suffix-array.c ; ./a.out 2007.csv ./sa '‰Ô'
void main(int argc, const char *argv[]) {
	const char *file = argv[1];
	const char*index = argv[2];
	const char *w = argv[3];
	
	FILE *fp = fopen(file, "rb");
	FILE *ip = fopen(index, "rb");
	uint32_t pos;
	const int len = strlen(w);
	char str[len + 1];
	str[len] = 0;
	
	printf("w=%s, len=%d\n", w, len);
	
	const size_t POS_SIZE = sizeof(uint32_t);
	size_t top, bottom, middle;

	top = 0;
	fseek(ip, 0, SEEK_END);
	bottom = ftell(ip) / POS_SIZE;

	while(top < bottom) {
		middle = (top + bottom) / 2;
		fseek(ip, middle * POS_SIZE, SEEK_SET);
		fread(&pos, POS_SIZE, 1, ip);
		fseek(fp, pos, SEEK_SET);
		fread(str, 1, len, fp);
		int ret = strcmp(str, w);
		if (ret == 0) {
			fseek(fp, pos, SEEK_SET);
			char s[100];
			fread(s, 1, sizeof(s), fp);
			printf("Hit: pos=%d data=%s\n", pos, s);
			break;
		}
		if (ret < 0) {
			top = middle + 1;
			continue;
		}
		bottom = middle;
	}
	fclose(ip);
	fclose(fp);
}
