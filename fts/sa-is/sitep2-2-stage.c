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
	
	// バケットごとに計数
	uint32_t buckets[255];
	memset(buckets, 0, sizeof(buckets));
	// S/Lに分類
	// Lを計数
	sa[len - 1].pos = len - 1;
	sa[len - 1].type = 'S';
	int lcount = 0;
	buckets[STR[len - 1]]++;
	for (int i = len - 2; i >= 0; --i) {
		sa[i].pos = i;
		if (STR[i] == STR[i + 1]) {
			sa[i].type = sa[i + 1].type;
		}
		else {
			sa[i].type = STR[i] > STR[i + 1] ? 'L' : 'S';
		}
		buckets[STR[i]]++;
		if (sa[i].type == 'L') {
			lcount++;
		}
	}

	// バケットごとの上辺と底辺を求める
	uint32_t bucketBottoms[255];
	{
		int j = 0;
		for (int i = 0; i < 256; ++i) {
			int a = j;
			j += buckets[i];
			buckets[i] = a;
			bucketBottoms[i] = j - 1;
		}
	}
	
	// Lだけを整列
	suffix_array_t* lsa = malloc(sizeof(suffix_array_t) * lcount);
	{
		int j = 0;
		for (int i = 0; i < len; ++i) {
			if (sa[i].type == 'L') {
				lsa[j++] = sa[i];
			}
		}
		qsort(lsa, lcount, sizeof(suffix_array_t), compare_sa);
	}
	
	// バケットごとに分類
	suffix_array_t* orgsa = sa;
	sa = malloc(sizeof(suffix_array_t) * len);
	memset(sa, 0, sizeof(suffix_array_t) * len);
	// Lだけを埋める
	for (int i = 0; i < lcount; ++i) {
		sa[buckets[STR[lsa[i].pos]]++] = lsa[i];
	}
	
	// 表示
	printf("Lだけを整列\n");
	for (int i = 0; i < len; ++i) {
		if (sa[i].type == 0) {
			printf("\n");
		}
		else {
			printf("%2d (%c) %s\n", sa[i].pos, sa[i].type, STR + sa[i].pos);
		}
	}
	
	// Sを埋める
	for (int i = len - 1; i >= 0; --i) {
		if (sa[i].type != 0 && sa[i].pos > 0 && orgsa[sa[i].pos - 1].type == 'S') {
			sa[bucketBottoms[STR[sa[i].pos - 1]]--] = orgsa[sa[i].pos - 1];
		}
	}
	sa[0] = orgsa[len - 1];
	
	// 表示
	printf("整列済み\n");
	for (int i = 0; i < len; ++i) {
		printf("%2d (%c) %s\n", sa[i].pos, sa[i].type, STR + sa[i].pos);
	}
}
