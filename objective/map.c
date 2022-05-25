#include "map.h"
#include <stdlib.h>

extern void map_insert(map* map, char* key, char* value) {
		map->insert(map->data, key, value);
}

extern char* map_search(map* map, char* key) {
		return map->search(map->data, key);
}

extern char* map_remove(map* map, char* key) {
		return map->remove(map->data, key);
}

extern void map_dispose(map* map) {
		map->dispose(map->data);
		free(map);
}
