#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define LEFT(i) ((i + 1) * 2 - 1)
#define RIGHT(i) ((i + 1) * 2)
#define LAST_INTERNAL(size) ((size + 1) / 2 - 1)

void heapify(entry_t *list, int size, int i) {
	int min = i;
	if (strcmp(list[LEFT(i)].str, list[min].str) > 0) {
	        min = LEFT(i);
	}
	if (RIGHT(i) < size &&
	                strcmp(list[RIGHT(i)].str, list[min].str) > 0) {
	        min = RIGHT(i);
	}
	if (min != i) {
	        entry_t a = list[min];
	        list[min] = list[i];
	        list[i] = a;
		if (min <= LAST_INTERNAL(size)) { 
			heapify(list, size, min);
		}
	}
}

void build_heap(entry_t *list, int size) {
	for (int i = LAST_INTERNAL(size); i >= 0; --i) {
		heapify(list, size, i);
	}
}

entry_t dequeue(entry_t *list, int size) {
	entry_t top = list[0];
	list[0] = list[size - 1];
	heapify(list, size - 1, 0);
	return top;
}

void heap_sort(entry_t *list, int size) {
	build_heap(list, size);
	for (int i = size; i >= 1; --i) {
		list[i - 1] = dequeue(list, i);
	}
}

void main(int argc, const char *argv[]) {
	const char *file = argv[1];
	const char *index = argv[2];
	make_index(file, index, heap_sort, 30000000);
}
