#include <stddef.h>

typedef struct {
	char* str;
	size_t pos;
} entry_t;

void make_index(const char *file, const char *index, void (*sort)(entry_t *list, int size), int limit);
