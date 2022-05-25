#include "hashmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define DELETED (char*)-1L

typedef struct {
	char* key;
	char* value;
} entry;

typedef struct {
	entry* table;
	size_t table_size;
} hash_data;

size_t next_prime(size_t a) {
	OUTER: for (;;) {
		const size_t max = sqrt(a);
		for (size_t i = 2; i <= max; ++i) {
			if (a % i == 0) {
				a++;
				goto OUTER;
			}
		}
		break;
	}
	return a;
}

size_t hash(const char* key, const size_t table_size) {
	size_t h = 0;
	size_t len = strlen(key);
	for(size_t i = 0; i < len; ++i) {
		h = 31 * h + key[i];
	}
	return h % table_size;
}

void hash_insert(void* data, char* key, char* value) {
	hash_data *hashdata = data;
	size_t i = hash(key, hashdata->table_size);
	while (hashdata->table[i].key != NULL && hashdata->table[i].key != DELETED) {
		i = (i + 1) % hashdata->table_size;
	}
	hashdata->table[i].key = key;
	hashdata->table[i].value = value;
}

char* hash_search(void* data, char* key) {
	hash_data *hashdata = data;
	size_t i = hash(key, hashdata->table_size);
	while (hashdata->table[i].key != NULL) {
		if (hashdata->table[i].key != DELETED && strcmp(hashdata->table[i].key, key) == 0) {
			return hashdata->table[i].value;
		}
		i = (i + 1) % hashdata->table_size;
	}
	return NULL;
}

char* hash_remove(void* data, char* key) {
	hash_data *hashdata = data;
	size_t i = hash(key, hashdata->table_size);
	size_t h = i;
	while (hashdata->table[i].key != NULL) {
		if (hashdata->table[i].key != DELETED && strcmp(hashdata->table[i].key, key) == 0) {
			hashdata->table[i].key = DELETED;
			return hashdata->table[i].value;
		}
		i = (i + 1) % hashdata->table_size;
	}
	return NULL;
}

void hash_dispose(void* data) {
	hash_data *hashdata = data;
	free(hashdata->table);
	free(hashdata);
}

extern map* create_hashmap(size_t size) {
	map* hashmap = malloc(sizeof(map));
	hashmap->insert = hash_insert;
	hashmap->search = hash_search;
	hashmap->remove = hash_remove;
	hashmap->dispose = hash_dispose;
	hash_data* data = hashmap->data = malloc(sizeof(hash_data));
	data->table_size = next_prime(size * 2);
	data->table = malloc(sizeof(entry) * data->table_size);
	for (size_t i = 0; i < data->table_size; ++i) {
		data->table[i].key = NULL;
	}
	return hashmap;
}
