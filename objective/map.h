#pragma once

typedef struct {
	void (*insert)(void* data, char* key, char* value);
	char* (*search)(void* data, char* key);
	char* (*remove)(void* data, char* key);
	void (*dispose)(void* data);
	void* data;
} map;

void map_insert(map* map, char* key, char* value);

char* map_search(map* map, char* key);

char* map_remove(map* map, char* key);

void map_dispose(map* map);
