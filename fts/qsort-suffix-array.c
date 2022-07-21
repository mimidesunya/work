#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

char* text;

int compare_sa(const void *a, const void *b) {
    return strcmp(text + *((uint32_t*)a), text + *((uint32_t*)b));
}

void main(int argc, const char *argv[]) {
	const char* file = argv[1];
	const size_t len = 10000;
	text = malloc(len);
	text[len - 1] = 0;
	
	printf("読み込み len=%ld\n", len);
	{
		FILE* fp = fopen(file, "rb");
		fread(text, 1, len - 1, fp);
		fclose(fp);
	}

	uint32_t* sa = malloc(sizeof(uint32_t) * len);
	for (size_t i = 0; i < len; ++i) {
		sa[i] = (uint32_t)i;
	}
	
	printf("整列\n");
	qsort(sa, len, sizeof(uint32_t), compare_sa);
	
	printf("出力\n");
	{
		FILE *fp = fopen("sa", "wb");
		fwrite(sa, sizeof(uint32_t), len, fp);
		fclose(fp);
	}
	free(sa);
}

