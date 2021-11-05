#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void merge(entry_t *list, entry_t *work, int top, int middle, int bottom) {
	int i = top;
	int j = middle;
	int k = 0;
	while(i < middle || j < bottom) {
		if (j >= bottom || (i < middle && strcmp(list[i].str, list[j].str) <= 0)) {
			work[k++] = list[i++];
		}
		else {
			work[k++] = list[j++];
		}
	}
	memcpy(&list[top], work, sizeof(entry_t) * k);
}

void sort(entry_t *list, entry_t *work, int top, int bottom) {
	if (top >= bottom - 1) {
		return;
	}
	int middle = (bottom + top) / 2;
	sort(list, work, top, middle);
	sort(list, work, middle, bottom);
	merge(list, work, top, middle, bottom);
}

void merge_sort(entry_t *list, int size) {
	entry_t *work = (entry_t*)malloc(sizeof(entry_t) * size);
	sort(list, work, 0, size);
	free(work);
}

void main(int argc, const char *argv[]) {
	const char *file = argv[1];
	const char *index = argv[2];
	make_index(file, index, merge_sort, 30000000);
}
