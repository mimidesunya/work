#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node node_t;

typedef enum label {
	L, E, R
} label_t;

struct node {
	char* number;
	char* name;
	node_t* parent;
	node_t* left;
	node_t* right;
	label_t label;
};

node_t* search(node_t *root, const char* name) {
	int a = strcmp(root->name, name);
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

node_t* balance(node_t *root, node_t *node, label_t label) {
	switch (node->label) {
		case E:
		node->label = label;
		if (node->parent) {
			return balance(root, node->parent, node->parent->left == node ? L : R);
		}
		break;
		
		case L:
		if (label == R) {
			node->label = E;
			break;
		}
		node_t* left = node->left;
		if (left->label == R) {
			if (left->right == NULL) {
				exit(1);
			}
			left->label = E;
			left->right->parent = node;
			node->left = left->right;
			if (node->left->left) {
				node->left->left->parent = left;
			}
			left->right = node->left->left;
			left->parent = node->left;
			node->left->left = left;
			left = left->parent;
			node->label = R;
		}
		else {
			node->label = E;
		}
		left->label = E;
		if (left->right) {
			left->right->parent = node;
		}
		node->left = left->right;
		left->parent = node->parent;
		if (left->parent) {
			if (left->parent->left == node) {
				left->parent->left = left;
			}
			else {
				if (left->parent->right != node) {
					exit(1);
				}
				left->parent->right = left;
			}
		}
		else {
			root = left;
		}
		node->parent = left;
		left->right = node;
		break;
		
		case R:
		if (label == L) {
			node->label = E;
			break;
		}
		node_t* right = node->right;
		if (right->label == L) {
			if (right->left == NULL) {
				exit(1);
			}
			right->label = E;
			right->left->parent = node;
			node->right = right->left;
			if (node->right->right) {
				node->right->right->parent = right;
			}
			right->left = node->right->right;
			right->parent = node->right;
			node->right->right = right;
			right = right->parent;
			node->label = L;
		}
		else {
			node->label = E;
		}
		right->label = E;
		if (right->left) {
			right->left->parent = node;
		}
		node->right = right->left;
		right->parent = node->parent;
		if (right->parent) {
			if (right->parent->left == node) {
				right->parent->left = right;
			}
			else {
				if (right->parent->right != node) {
					exit(1);
				}
				right->parent->right = right;
			}
		}
		else {
			root = right;
		}
		node->parent = right;
		right->left = node;
		break;
		
		default:
		exit(1);
		break;
	}
	return root;
}

node_t* insert_internal(node_t *root, node_t *subtree, node_t *node) {
	int a = strcmp(subtree->name,node->name);
	if (a > 0) {
		if (subtree->left) {
			return insert_internal(root, subtree->left, node);
		}
		else {
			subtree->left = node;
			node->parent = subtree;
			node->left = node->right = NULL;
			node->label = E;
			return balance(root, node->parent, L);
		}
	}
	else if (a < 0) {
		if (subtree->right) {
			return insert_internal(root, subtree->right, node);
		}
		else {
			subtree->right = node;
			node->parent = subtree;
			node->left = node->right = NULL;
			node->label = E;
			return balance(root, node->parent, R);
		}
	}
	return root;
}

node_t* insert(node_t *root, node_t *node) {
	return insert_internal(root, root, node);
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
	root->label = E;
	while (fgets(line, sizeof line, fp)) {
		node_t *node = list + i;
		char *comma = strchr(line, ',');
		char *comma2 = strchr(comma + 1, ',');
		*comma = *comma2 = 0;
		size_t length = comma2 - line + 1;
		memcpy(pdata, line, length);
		
		node->name = pdata;
		node->number = pdata + (comma - line) + 1;
		
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
	node_t* hit = search(root, "011-200-1715");
	printf("%s,%s\n", hit->number, hit->name);
}
