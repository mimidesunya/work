#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct suffix_array {
	uint32_t pos;
	char type;
} suffix_array_t;

// 誘導整列
void induced_sort(
	FILE* str,
	const suffix_array_t* orgsa, suffix_array_t* sa, const int len,
	const suffix_array_t* lmsa, const int mcount,
	const uint32_t* buckets, const int blen) {
	// バケットごとの底辺を求める
	uint32_t* bucketBottoms = malloc(sizeof(uint32_t) * blen);
	{
		int j = 0;
		for (int i = 0; i < blen; ++i) {
			j += buckets[i];
			bucketBottoms[i] = j - 1;
		}
	}
	
	uint32_t a;
	// バケットごとに分類
	memset(sa, 0, sizeof(suffix_array_t) * len);
	// LMSだけを埋める
	for (int i = mcount - 1; i >= 0; --i) {
		fseek(str, sizeof(uint32_t) * (lmsa[i].pos), SEEK_SET);
		fread(&a, sizeof(uint32_t), 1, str);
		sa[bucketBottoms[a]--] = lmsa[i];
	}
	// バケットごとの上辺と底辺を求める
	uint32_t* bucketTops = malloc(sizeof(uint32_t) * blen);
	{
		int j = 0;
		for (int i = 0; i < blen; ++i) {
			bucketTops[i] = j;
			j += buckets[i];
			bucketBottoms[i] = j - 1;
		}
	}
	
	// Lを埋める
	for (int i = 0; i < len; ++i) {
		if (sa[i].type != 0 && sa[i].pos > 0 && orgsa[sa[i].pos - 1].type == 'L') {
			fseek(str, sizeof(uint32_t) * (sa[i].pos - 1), SEEK_SET);
			fread(&a, sizeof(uint32_t), 1, str);
			sa[bucketTops[a]++] = orgsa[sa[i].pos - 1];
		}
	}
	// Mを消す
	for (int i = 0; i < len; ++i) {
		if (sa[i].type == 'M') {
			sa[i].type = 0;
		}
	}
	// Sを埋める
	for (int i = len - 1; i >= 0; --i) {
		if (sa[i].type != 0 && sa[i].pos > 0 && (orgsa[sa[i].pos - 1].type == 'S' || orgsa[sa[i].pos - 1].type == 'M')) {
			fseek(str, sizeof(uint32_t) * (sa[i].pos - 1), SEEK_SET);
			fread(&a, sizeof(uint32_t), 1, str);
			sa[bucketBottoms[a]--] = orgsa[sa[i].pos - 1];
		}
	}
	sa[0] = orgsa[len - 1];
	free(bucketBottoms);
	free(bucketTops);
}

// 接尾辞配列誘導整列
void sa_is(suffix_array_t* sa, FILE* str, const int len, const int blen) {
	// バケットごとに計数
	uint32_t* buckets = malloc(sizeof(uint32_t) * blen);
	memset(buckets, 0, sizeof(uint32_t) * blen);
	// S/L/LMS(M)に分類
	// LSMを計数
	printf("分類\n");
	sa[len - 1].pos = len - 1;
	sa[len - 1].type = 'S';
	int mcount = 0;
	uint32_t a, b;
	fseek(str, sizeof(uint32_t) * (len - 1), SEEK_SET);
	fread(&a, sizeof(uint32_t), 1, str);
	buckets[a]++;
	for (int i = len - 2; i >= 0; --i) {
		sa[i].pos = i;
		fseek(str, sizeof(uint32_t) * i, SEEK_SET);
		fread(&a, sizeof(uint32_t), 1, str);
		fread(&b, sizeof(uint32_t), 1, str);
		if (a == b) {
			sa[i].type = sa[i + 1].type;
		}
		else {
			sa[i].type = a > b ? 'L' : 'S';
		}
		if (sa[i].type == 'L' && sa[i + 1].type == 'S') {
			sa[i + 1].type = 'M';
			mcount++;
		}
		buckets[a]++;
	}

	// LMSを抽出
	printf("LMSを抽出\n");
	suffix_array_t* lmsa = malloc(sizeof(suffix_array_t) * mcount);
	uint32_t* lmspos = malloc(sizeof(uint32_t) * len);
	{
		int j = 0;
		for (int i = 0; i < len; ++i) {
			if (sa[i].type == 'M') {
				lmspos[sa[i].pos] = j;
				lmsa[j++] = sa[i];
			}
		}
	}
	
	// 誘導整列
	printf("誘導整列\n");
	suffix_array_t* orgsa = malloc(sizeof(suffix_array_t) * len);
	memcpy(orgsa, sa, sizeof(suffix_array_t) * len);
	induced_sort(str, orgsa, sa, len, lmsa, mcount, buckets, blen);
	
	// LMSの順序
	printf("LMS部分文字列を抽出\n");
	uint32_t* lmsstr = malloc(sizeof(uint32_t) * mcount + 1);
	int msort = 0;
	{
		int ppos = 0, plen = 0;
		for (int i = 0; i < len; ++i) {
			if (sa[i].type == 'M') {
				int lpos = lmspos[sa[i].pos];
				uint32_t llen = (lpos == mcount - 1) ? 1 : lmsa[lpos + 1].pos - lmsa[lpos].pos + 1;
				uint32_t aa[llen];
				uint32_t bb[llen];
				fseek(str, sizeof(uint32_t) * lmsa[lpos].pos, SEEK_SET);
				fread(aa, sizeof(uint32_t), llen, str);
				fseek(str, sizeof(uint32_t) * lmsa[ppos].pos, SEEK_SET);
				fread(bb, sizeof(uint32_t), llen, str);
				if (llen != plen ||
				memcmp(aa, bb, sizeof(uint32_t) * llen) != 0) {
					++msort;
				}
				
				lmsstr[lpos] = msort;
				ppos = lpos;
				plen = llen;
			}
		}
	}
	free(lmspos);
	
	// LMS部分文字列がユニークであることが終了条件
	if (msort != mcount) {
		// lmsstrに対して再帰処理
		printf("再帰 mcount=%d msort=%d\n", mcount, msort);
		int rlen = mcount + 1;
		suffix_array_t* rsa = malloc(sizeof(suffix_array_t) * rlen);
		lmsstr[mcount] = 0;
		
		char rstrf[100];
		sprintf(rstrf, "str%d", rlen);
		FILE* rstr = fopen(rstrf, "wb");
		fwrite(lmsstr, sizeof(uint32_t), rlen, rstr);
		fclose(rstr);
		rstr = fopen(rstrf, "rb");
		sa_is(rsa, rstr, rlen, msort + 1);
		
		// 再整列したLMS
		suffix_array_t* rlmsa = malloc(sizeof(suffix_array_t) * mcount);
		for (int i = 1; i < rlen; ++i) {
			rlmsa[i - 1] = lmsa[rsa[i].pos];
		}
		free(lmsa);
		lmsa = rlmsa;
		
		// 再度誘導整列
		printf("再整列\n");
		memcpy(sa, orgsa, sizeof(suffix_array_t) * len);
		induced_sort(str, orgsa, sa, len, lmsa, mcount, buckets, blen);
	}
	
	free(buckets);
	free(lmsa);
	free(lmsstr);
	free(orgsa);
}

void main(int argc, const char *argv[]) {
	//const size_t len = 10000L;
	const size_t len = 2442952959L - 8L;
	printf("読み込み len=%ld\n", len);
	char* text;
	{
		FILE* fp = fopen(argv[1], "rb");
		fseek(fp, 0, SEEK_SET);
		text = malloc(len);
		fread(text, 1, len - 1, fp);
		text[len - 1] = 0;
		fclose(fp);
	}

	const char* strf = "/tmp/str";
	FILE* str = fopen(strf, "wb");
	for (int i = 0; i < len; ++i) {
		uint32_t a = (unsigned char)text[i];
		fwrite(&a, sizeof(uint32_t), 1, str);
	}
	fclose(str);
	free(text);
	str = fopen(strf, "rb");
	
	printf("整列\n");
	suffix_array_t* sa = malloc(sizeof(suffix_array_t) * len);
	sa_is(sa, str, len, 256);
	fclose(str);
	
	printf("出力\n");
	{
		FILE *fp = fopen("sa", "wb");
		for (int i = 0; i < len; ++i) {
			fwrite(&sa[i].pos, sizeof(uint32_t), 1, fp);
		}
		fclose(fp);
	}
	free(sa);
}
