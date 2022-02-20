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
	if (root == NULL) {
		return NULL;
	}
	int a = strcmp(root->name, name);
	if (a > 0) {
		return search(root->left, name);
	}
	else if (a < 0) {
		return search(root->right, name);
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

node_t* delete(node_t *root, node_t *node) {
	if (node->left != NULL && node->right != NULL) {
		node_t *leftmax = node->left;
		while (leftmax->right != NULL) {
			leftmax = leftmax->right;
		}
		if (leftmax == node->left) {
			leftmax->right = node->right;
			if (node->right != NULL) {
				node->right->parent = leftmax;
			}
		}
		else {
			leftmax->parent->right = leftmax->left;
			if (leftmax->left != NULL) {
				leftmax->left->parent = leftmax->parent;
			}
			leftmax->left = node->left;
			node->left->parent = leftmax;
			leftmax->right = node->right;
			node->right->parent = leftmax;
		}
		leftmax->parent = node->parent;
		if (node->parent == NULL) {
			root = leftmax;
		}
		else if (node->parent->left == node) {
			node->parent->left = leftmax;
		}
		else {
			node->parent->right = leftmax;
		}
		return root;
	}
	else if (node->left != NULL) {
		node->left->parent = node->parent;
		if (node->parent == NULL) {
			root = node->left;
		}
		else if (node->parent->left == node) {
			node->parent->left = node->left;
		}
		else {
			node->parent->right = node->left;
		}
		return root;
	}
	else if (node->right != NULL) {
		node->right->parent = node->parent;
		if (node->parent == NULL) {
			root = node->right;
		}
		else if (node->parent->left == node) {
			node->parent->left = node->right;
		}
		else {
			node->parent->right = node->right;
		}
		return root;
	}
	else {
		if (node->parent == NULL) {
			root = NULL;
		}
		else if (node->parent->left == node) {
			node->parent->left = NULL;
		}
		else {
			node->parent->right = NULL;
		}
		return root;
	}
}

void walk(node_t *node) {
	if (node->left != NULL) {
		walk(node->left);
	}
	printf("%s\n", node->name);
	if (node->right != NULL) {
		walk(node->right);
	}
}

int depth(node_t *root) {
	if (root == NULL) {
		return 0;
	}
	int l = depth(root->left);
	int r = depth(root->right);
	if (l > r) {
		return l + 1;
	}
	return r + 1;
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
	char *pdata = data;
	node_t *root = list;
	root->parent = root->left = root->right = NULL;
	{
		size_t i = 0;
		FILE *fp = fopen(file, "r");
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
	}
	
	printf("search\n");
	{
		node_t* hit = search(root, "花うさぎ");
		printf("%s,%s\n", hit->number, hit->name);
	}
	printf("depth %d\n", depth(root));
	
	walk(root);
	
	{
		size_t i = 0;
		FILE *fp = fopen(file, "r");
		while (fgets(line, sizeof line, fp)) {
			node_t *node = list + i;
			char *comma = strchr(line, ',');
			char *comma2 = strchr(comma + 1, ',');
			*comma = *comma2 = 0;
			char *name = comma + 1;
			node_t* hit = search(root, name);
			if (hit != NULL) {
				root = delete(root, hit);
			}
			
			i++;
			if (i % 10000 == 0) {
				printf("delete %ld\n", i);
			}
			if (i > limit) {
				break;
			}
		}
	}
	printf("depth %d\n", depth(root));
}
