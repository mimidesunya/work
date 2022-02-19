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

node_t* balance(node_t *theroot, node_t *root, label_t label) {
	switch (root->label) {
		case E:
		root->label = label;
		if (root->parent) {
			return balance(theroot, root->parent, root->parent->left == root ? L : R);
		}
		break;
		
		case L:
		if (label == R) {
			root->label = E;
			break;
		}
		node_t* left = root->left;
		if (left->label == R) {
			if (left->right == NULL) {
				exit(1);
			}
			switch(left->right->label) {
				case L:
					root->label = R;
					left->label = E;
				break;
				case R:
					root->label = E;
					left->label = L;
				break;
				default:
					root->label = left->label = E;
				break;
			}
			left->right->parent = root;
			root->left = left->right;
			if (root->left->left) {
				root->left->left->parent = left;
				if (left->left == NULL) {
					printf("Illegal state.\n");
					exit(1);
				}
			}
			left->right = root->left->left;
			left->parent = root->left;
			root->left->left = left;
			left = left->parent;
		}
		else {
			root->label = E;
		}
		left->label = E;
		if (left->right) {
			left->right->parent = root;
		}
		root->left = left->right;
		left->parent = root->parent;
		if (left->parent) {
			if (left->parent->left == root) {
				left->parent->left = left;
			}
			else {
				if (left->parent->right != root) {
					printf("Illegal state.\n");
					exit(1);
				}
				left->parent->right = left;
			}
		}
		else {
			theroot = left;
		}
		root->parent = left;
		left->right = root;
		break;
		
		case R:
		if (label == L) {
			root->label = E;
			break;
		}
		node_t* right = root->right;
		if (right->label == L) {
			if (right->left == NULL) {
				printf("Illegal state.\n");
				exit(1);
			}
			switch(right->left->label) {
				case R:
					root->label = L;
					right->label = E;
				break;
				case L:
					root->label = E;
					right->label = R;
				break;
				default:
					root->label = right->label = E;
				break;
			}
			right->left->parent = root;
			root->right = right->left;
			if (root->right->right) {
				root->right->right->parent = right;
			}
			right->left = root->right->right;
			right->parent = root->right;
			root->right->right = right;
			right = right->parent;
		}
		else {
			root->label = E;
		}
		right->label = E;
		if (right->left) {
			right->left->parent = root;
		}
		root->right = right->left;
		right->parent = root->parent;
		if (right->parent) {
			if (right->parent->left == root) {
				right->parent->left = right;
			}
			else {
				if (right->parent->right != root) {
					printf("Illegal state.\n");
					exit(1);
				}
				right->parent->right = right;
			}
		}
		else {
			theroot = right;
		}
		root->parent = right;
		right->left = root;
		break;
		
		default:
		exit(1);
		break;
	}
	return theroot;
}

int depth(node_t *root) {
	if (!root) {
		return 0;
	}
	int l = depth(root->left);
	int r = depth(root->right);
	if (l >= r + 2 || r >= l + 2) {
		printf("Illegal state: %d %d\n", l, r);
		exit(1);
	}
	if (l > r) {
		return l + 1;
	}
	return r + 1;
}

node_t* insert_internal(node_t *theroot, node_t *root, node_t *node) {
	int a = strcmp(root->name, node->name);
	if (a > 0) {
		if (root->left) {
			return insert_internal(theroot, root->left, node);
		}
		else {
			root->left = node;
			node->parent = root;
			node->left = node->right = NULL;
			node->label = E;
			return balance(theroot, node->parent, L);
		}
	}
	else if (a < 0) {
		if (root->right) {
			return insert_internal(theroot, root->right, node);
		}
		else {
			root->right = node;
			node->parent = root;
			node->left = node->right = NULL;
			node->label = E;
			return balance(theroot, node->parent, R);
		}
	}
	return theroot;
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
	
	printf("depth %d\n", depth(root));
}
