#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node node_t;
struct node {
	char* number;
	char* name;
	node_t* parent;
	node_t* left;
	node_t* right;
};

node_t* search(node_t *root, const char* name) {
	int a = strcmp(root->name, name);
	if (a == 0) {
		return root;
	}
	if (a > 0) {
		if (root->left) {
			return search(root->left, name);
		}
	}
	else if (a < 0) {
		if (root->right) {
			return search(root->right, name);
		}
	}
	return root;
}

node_t* insert(node_t *root, node_t *node) {
	int a = strcmp(root->name,node->name);
	if (a > 0) {
		if (root->left) {
			insert(root->left, node);
		}
		else {
			root->left = node;
			node->parent = root;
			node->left = node->right = NULL;
		}
	}
	else if (a < 0) {
		if (root->right) {
			insert(root->right, node);
		}
		else {
			root->right = node;
			node->parent = root;
			node->left = node->right = NULL;
		}
	}
	return root;
}

void main(int argc, const char *argv[]) {
	const char *file = argv[1];
	size_t limit = 30000000L;
	
	char *data = (char*)malloc(100L * 30000000L);
	if (!data) {
		return;
	}
	node_t *list = (node_t*)malloc(sizeof(node_t) * 30000000L);
	if (!list) {
		return;
	}
	
	char line[700];
	FILE *fp = fopen(file, "r");
	char *pdata = data;
	size_t i = 0;
	node_t *root = list;
	root->parent = root->left = root->right = NULL;
	while (fgets(line, sizeof line, fp)) {
		node_t *node = list + i;
		char *comma = strchr(line, ',');
		char *comma2 = strchr(comma + 1, ',');
		*comma = *comma2 = 0;
		size_t length = comma2 - line + 1;
		memcpy(pdata, line, length);
		
		node->number = pdata;
		node->name = pdata + (comma - line) + 1;
		
		root = insert(root, node);
		
		i++;
		if (i % 10000 == 0) {
			printf("build tree %ld\n", i);
		}
		if (i > limit) {
			break;
		}
		pdata += length;
	}
	fclose(fp);
	
	printf("search\n");
	node_t* hit = search(root, "花うさぎ");
	printf("%s,%s\n", hit->number, hit->name);
}
