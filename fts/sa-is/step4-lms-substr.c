#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

const char* STRING = "TENTENTEMMARITENTEMARITENTENTEMARINOTEGASORETE";

typedef struct suffix_array {
	uint32_t pos;
	char type;
} suffix_array_t;

// 誘導整列
void induced_sort(
	const uint32_t* str,
	suffix_array_t* sa, const uint32_t len,
	const suffix_array_t* lmsa, const uint32_t mcount,
	const uint32_t* buckets, const uint32_t blen) {
	// バケットごとの底辺を求める
	uint32_t* bucketBottoms = malloc(sizeof(uint32_t) * blen);
	{
		uint32_t j = 0;
		for (uint32_t i = 0; i < blen; ++i) {
			j += buckets[i];
			bucketBottoms[i] = j - 1;
		}
	}

	// バケットごとに分類
	suffix_array_t* orgsa = sa;
	sa = malloc(sizeof(suffix_array_t) * len);
	memset(sa, 0, sizeof(suffix_array_t) * len);
	// LMSだけを埋める
	for (uint32_t i = mcount - 1; i != -1; --i) {
		sa[bucketBottoms[str[lmsa[i].pos]]--] = lmsa[i];
	}
	// バケットごとの上辺と底辺を求める
	uint32_t* bucketTops = malloc(sizeof(uint32_t) * blen);
	{
		uint32_t j = 0;
		for (uint32_t i = 0; i < blen; ++i) {
			bucketTops[i] = j;
			j += buckets[i];
			bucketBottoms[i] = j - 1;
		}
	}
	
	// Lを埋める
	for (uint32_t i = 0; i < len; ++i) {
		if (sa[i].type != 0 && sa[i].pos > 0 && orgsa[sa[i].pos - 1].type == 'L') {
			sa[bucketTops[str[sa[i].pos - 1]]++] = orgsa[sa[i].pos - 1];
		}
	}
	// Mを消す
	for (uint32_t i = 0; i < len; ++i) {
		if (sa[i].type == 'M') {
			sa[i].type = 0;
		}
	}
	// Sを埋める
	for (uint32_t i = len - 1; i != -1; --i) {
		if (sa[i].type != 0 && sa[i].pos > 0 && (orgsa[sa[i].pos - 1].type == 'S' || orgsa[sa[i].pos - 1].type == 'M')) {
			sa[bucketBottoms[str[sa[i].pos - 1]]--] = orgsa[sa[i].pos - 1];
		}
	}
	sa[0] = orgsa[len - 1];
	memcpy(orgsa, sa, sizeof(suffix_array_t) * len);
	free(sa);
	free(bucketBottoms);
	free(bucketTops);
}

// 接尾辞配列誘導整列
void sa_is(const uint32_t* str, suffix_array_t* sa,
	const uint32_t len, const uint32_t blen) {
	// バケットごとに計数
	uint32_t* buckets = malloc(sizeof(uint32_t) * blen);
	memset(buckets, 0, sizeof(uint32_t) * blen);
	// S/L/LMS(M)に分類
	// LSMを計数
	sa[len - 1].pos = len - 1;
	sa[len - 1].type = 'S';
	int mcount = 0;
	buckets[str[len - 1]]++;
	for (uint32_t i = len - 2; i != -1; --i) {
		sa[i].pos = i;
		if (str[i] == str[i + 1]) {
			sa[i].type = sa[i + 1].type;
		}
		else {
			sa[i].type = str[i] > str[i + 1] ? 'L' : 'S';
		}
		if (sa[i].type== 'L' && sa[i + 1].type == 'S') {
			sa[i + 1].type = 'M';
			mcount++;
		}
		buckets[str[i]]++;
	}
	
	// 表示
	printf("最初の状態\n");
	for (int i = 0; i < len; ++i) {
		char* type = "   ";
		switch(sa[i].type) {
			case 0:
			printf("\n");
			continue;
			
			case 'L':
			type = "(L)     ";
			break;
			
			case 'S':
			type = "(S)     ";
			break;
			
			case 'M':
			type = "(S) LMS ";
			break;
		}
		printf("%2d %s", sa[i].pos, type);
		for (int j = i; j < len; ++j) {
			uint32_t c = str[sa[j].pos];
			if (c < 20) {
				printf("[%2d]", c);
			}
			else {
				printf("%c", c);
			}
		}
		printf("\n");
	}

	// LMSを抽出
	suffix_array_t* lmsa = malloc(sizeof(suffix_array_t) * mcount);
	uint32_t* lmspos = malloc(sizeof(uint32_t) * len);
	{
		uint32_t j = 0;
		for (uint32_t i = 0; i < len; ++i) {
			if (sa[i].type == 'M') {
				lmspos[sa[i].pos] = j;
				lmsa[j++] = sa[i];
			}
		}
	}
	
	// 誘導整列
	suffix_array_t* orgsa = malloc(sizeof(suffix_array_t) * len);
	memcpy(orgsa, sa, sizeof(suffix_array_t) * len);
	induced_sort(str, sa, len, lmsa, mcount, buckets, blen);
	
	// LMSの順序
	uint32_t* lmsstr = malloc(sizeof(uint32_t) * mcount + 1);
	printf("整列後LMS部分文字列\n");
	uint32_t msort = 0;
	{
		uint32_t ppos = 0, plen = 0;
		for (uint32_t i = 0; i < len; ++i) {
			if (sa[i].type == 'M') {
				uint32_t lpos = lmspos[sa[i].pos];
				uint32_t llen = (lpos == mcount - 1) ? 1 : lmsa[lpos + 1].pos - lmsa[lpos].pos + 1;
				if (llen != plen ||
					memcmp(str + lmsa[lpos].pos, str + lmsa[ppos].pos, sizeof(uint32_t) * llen) != 0) {
					++msort;
				}
				
				lmsstr[lpos] = msort;
				ppos = lpos;
				plen = llen;
				
				printf("%2d ", msort);
				for (uint32_t j = lmsa[lpos].pos; j < lmsa[lpos].pos + llen; ++j) {
					if(str[j] < 0x20) {
						printf("[%2d]", str[j]);
					}
					else {
						printf("%c", str[j]);
					}
				}
				printf("\n");
			}
		}
	}
	free(lmspos);
	
	// LMS部分文字列がユニークであることが終了条件
	if (msort != mcount) {
		// lmsstrに対して再帰処理
		printf("再帰 mcount=%d msort=%d\n", mcount, msort);
		uint32_t rlen = mcount + 1;
		suffix_array_t* rsa = malloc(sizeof(suffix_array_t) * rlen);
		sa_is(lmsstr, rsa, rlen, msort + 1);
		
		// 再整列したLMS
		suffix_array_t* rlmsa = malloc(sizeof(suffix_array_t) * mcount);
		for (uint32_t i = 1; i < rlen; ++i) {
			rlmsa[i - 1] = lmsa[rsa[i].pos];
		}
		free(lmsa);
		printf("整列済みLMS\n");
		for (uint32_t i = 0; i < mcount; ++i) {
			printf("%2d %s\n", rlmsa[i].pos, STRING + rlmsa[i].pos);
		}
		lmsa = rlmsa;
		
		// 再度誘導整列
		memcpy(sa, orgsa, sizeof(suffix_array_t) * len);
		induced_sort(str, sa, len, lmsa, mcount, buckets, blen);
	}
	
	free(buckets);
	free(lmsa);
	free(lmsstr);
	free(orgsa);
}

void main(int argc, const char *argv[]) {
	const uint32_t len = strlen(STRING) + 1;
	uint32_t* str = malloc(sizeof(uint32_t) * len);
	for (uint32_t i = 0; i < len; ++i) {
		str[i] = (unsigned char)STRING[i];
	}
	suffix_array_t* sa = malloc(sizeof(suffix_array_t) * len);
	sa_is(str, sa, len, 256);
	
	// 表示
	printf("整列済み接尾辞配列\n");
	for (uint32_t i = 0; i < len; ++i) {
		char* type = "   ";
		switch(sa[i].type) {
			case 0:
			printf("\n");
			continue;
			
			case 'L':
			type = "(L)     ";
			break;
			
			case 'S':
			type = "(S)     ";
			break;
			
			case 'M':
			type = "(S) LMS ";
			break;
		}
		printf("%2d %s %s\n", sa[i].pos, type, STRING + sa[i].pos);
	}
	
	free(sa);
}
