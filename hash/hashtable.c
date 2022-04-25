#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define DELETED (entry*)-1L

typedef struct {
	char* name;
	char* number;
} entry;

size_t hash(const char* key, const size_t tableSize) {
	size_t h = 0;
	size_t len = strlen(key);
	for(size_t i = 0; i < len; ++i) {
		h = 31 * h + key[i];
	}
	return h % tableSize;
}

void insert(entry **table, const size_t tableSize, entry *e) {
	size_t i = hash(e->name, tableSize);
	while (table[i] != NULL && table[i] != DELETED) {
		i = (i + 1) % tableSize;
	}
	table[i] = e;
}

entry** rehash(entry **table, const size_t tableSize, const size_t newSize) {
	entry **newTable = (entry**)malloc(sizeof(entry*) * newSize);
	for (size_t i = 0; i < newSize; ++i) {
		newTable[i] = NULL;
	}
	for (size_t i = 0; i < tableSize; ++i) {
		if (table[i] != NULL && table[i] != DELETED) {
			insert(newTable, newSize, table[i]);
		}
	}
	free(table);
	return newTable;
}

entry* search(entry **table, const size_t tableSize, const char* name) {
	size_t i = hash(name, tableSize);
	while (table[i] != NULL) {
		if (table[i] != DELETED && strcmp(table[i]->name, name) == 0) {
			return table[i];
		}
		i = (i + 1) % tableSize;
	}
	return NULL;
}

entry* erase(entry **table, const size_t tableSize, const char* name) {
	size_t i = hash(name, tableSize);
	size_t h = i;
	while (table[i] != NULL) {
		if (table[i] != DELETED && strcmp(table[i]->name, name) == 0) {
			entry* result = table[i];
			table[i] = DELETED;
			return result;
		}
		i = (i + 1) % tableSize;
	}
	return NULL;
}

void main(int argc, const char *argv[]) {
	const char *file = argv[1];
	const size_t size = 30000000L;
	const size_t limit = size;
	
	char *data = (char*)malloc(100L * size);
	if (!data) {
		return;
	}
	entry *list = (entry*)malloc(sizeof(entry) * size);
	if (!list) {
		return;
	}

	size_t tableSize = limit * 2;
	OUTER: for (;;) {
		const size_t max = sqrt(tableSize);
		for (size_t i = 2; i <= max; ++i) {
			if (tableSize % i == 0) {
				tableSize++;
				goto OUTER;
			}
		}
		break;
	}
	
	entry **table = (entry**)malloc(sizeof(entry*) * tableSize);
	if (!table) {
		return;
	}
	for (size_t i = 0; i < tableSize; ++i) {
		table[i] = NULL;
	}
	
	{
		char line[700];
		FILE *fp = fopen(file, "r");
		char *pdata = data;
		size_t i = 0;
		while (fgets(line, sizeof line, fp)) {
			entry *entry = list + i;
			char *comma = strchr(line, ',');
			char *comma2 = strchr(comma + 1, ',');
			*comma = *comma2 = 0;
			size_t length = comma2 - line + 1;
			memcpy(pdata, line, length);
			
			entry->number = pdata;
			entry->name = pdata + (comma - line) + 1;
			
			insert(table, tableSize, entry);
			
			i++;
			if (i % 10000 == 0) {
				printf("build hashtable %ld\n", i);
			}
			if (i > limit) {
				break;
			}
			pdata += length;
		}
		fclose(fp);
	}
	
	printf("search\n");
	{
		entry* hit = search(table, tableSize, "花うさぎ");
		printf("%s,%s\n", hit->number, hit->name);
	}
	
	size_t newSize = tableSize * 1.5;
	printf("rehash %ld\n", newSize);
	table = rehash(table, tableSize, newSize);
	tableSize = newSize;
	
	printf("search\n");
	{
		entry* hit = search(table, tableSize, "花うさぎ");
		printf("%s,%s\n", hit->number, hit->name);
	}
	
	{
		char line[700];
		FILE *fp = fopen(file, "r");
		char *pdata = data;
		size_t i = 0;
		entry *result;
		while (fgets(line, sizeof line, fp)) {
			entry *entry = list + i;
			char *comma = strchr(line, ',');
			char *comma2 = strchr(comma + 1, ',');
			*comma = *comma2 = 0;
			size_t length = comma2 - line + 1;
			memcpy(pdata, line, length);
			
			entry->number = pdata;
			char *name = pdata + (comma - line) + 1;
			
			result = erase(table, tableSize, name);
			if (result == NULL) {
				printf("error!\n");
				exit(1);
			}
			
			i++;
			if (i % 10000 == 0) {
				printf("erase %ld\n", i);
			}
			if (i > limit) {
				break;
			}
			pdata += length;
		}
		fclose(fp);
	}

	for (size_t i = 0; i < tableSize; ++i) {
		if (table[i] != NULL && table[i] != DELETED) {
			printf("error!\n");
			exit(1);
		}
	}

}
