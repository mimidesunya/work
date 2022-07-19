#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

const char* STR = "TENTENTEMMARITENTEMARITENTENTEMARINOTEGASORETE";

typedef struct suffix_array {
	uint32_t pos;
	char type;
} suffix_array_t;

int compare_sa(const void *a, const void *b)
{
    return strcmp(STR + ((suffix_array_t*)a)->pos, STR + ((suffix_array_t*)b)->pos);
}

void main(int argc, const char *argv[]) {
	int len = strlen(STR) + 1;
	suffix_array_t* sa = malloc(sizeof(suffix_array_t) * len);
	
	// S/Lに分類
	sa[len - 1].pos = len - 1;
	sa[len - 1].type = 'S';
	for (int i = len - 2; i >= 0; --i) {
		sa[i].pos = i;
		if (STR[i] == STR[i + 1]) {
			sa[i].type = sa[i + 1].type;
		}
		else {
			sa[i].type = STR[i] > STR[i + 1] ? 'L' : 'S';
		}
	}
	
	// 表示
	printf("最初の状態\n");
	for (int i = 0; i < len; ++i) {
		//printf("%2d (%c) %s\n", sa[i].pos, sa[i].type, STR + sa[i].pos);
		printf("%2d %s\n", sa[i].pos, STR + sa[i].pos);
	}
	
	// 整列後
	printf("整列\n");
	qsort(sa, len, sizeof(suffix_array_t), compare_sa);
	for (int i = 0; i < len; ++i) {
		//printf("%2d (%c) %s\n", sa[i].pos, sa[i].type, STR + sa[i].pos);
		printf("%2d %s\n", sa[i].pos, STR + sa[i].pos);
	}
}
