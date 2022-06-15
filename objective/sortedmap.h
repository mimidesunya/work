#include "map.h"
#include <stdbool.h>

typedef struct {
	bool (*has_next)(void* data);
	void (*next)(void* data);
	char* (*key)(void* data);
	char* (*value)(void* data);
	void (*dispose)(void* data);
	void* data;
} iterator;

typedef struct {
	void (*insert)(void* data, char* key, char* value);
	char* (*search)(void* data, char* key);
	char* (*remove)(void* data, char* key);
	void (*dispose)(void* data);
	void* data;
	iterator* (*iterator)(void* data, char* key1, char* key2);
} sortedmap;

iterator* sortedmap_iterator(sortedmap* map, char* key1, char* key2);

bool itr_has_next(iterator* itr);

void itr_next(iterator* itr);

char* itr_key(iterator* itr);

char* itr_value(iterator* itr);

void itr_dispose(iterator* itr);
