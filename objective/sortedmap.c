#include "sortedmap.h"
#include <stdlib.h>

extern iterator* sortedmap_iterator(sortedmap* map, char* key1, char* key2) {
	return map->iterator(map->data, key1, key2);
}

extern bool itr_has_next(iterator* itr) {
	return itr->has_next(itr->data);
}

extern void itr_next(iterator* itr) {
	itr->next(itr->data);
}

extern char* itr_key(iterator* itr) {
	return itr->key(itr->data);
}

extern char* itr_value(iterator* itr) {
	return itr->value(itr->data);
}

extern void itr_dispose(iterator* itr) {
	itr->dispose(itr->data);
	free(itr);
}