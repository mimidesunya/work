#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct node node_t;

typedef enum label {
	L, E, R
} label_t;

struct node {
	char* name;
	node_t* parent;
	node_t* left;
	node_t* right;
	label_t label;
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

node_t* insert_balance(node_t *theroot, node_t *root, label_t label) {
	switch (root->label) {
		case E:
		root->label = label;
		if (root->parent) {
			return insert_balance(theroot, root->parent, root->parent->left == root ? L : R);
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
			if (right->parent->right == root) {
				right->parent->right = right;
			}
			else {
				if (right->parent->left != root) {
					printf("Illegal state.\n");
					exit(1);
				}
				right->parent->left = right;
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

node_t* insert_internal(node_t *theroot, node_t *root, node_t *node) {
	node->left = node->right = NULL;
	node->label = E;
	if (theroot == NULL) {
		return node;
	}
	int a = strcmp(root->name, node->name);
	if (a > 0) {
		if (root->left) {
			return insert_internal(theroot, root->left, node);
		}
		else {
			root->left = node;
			node->parent = root;
			return insert_balance(theroot, node->parent, L);
		}
	}
	else if (a < 0) {
		if (root->right) {
			return insert_internal(theroot, root->right, node);
		}
		else {
			root->right = node;
			node->parent = root;
			return insert_balance(theroot, node->parent, R);
		}
	}
	return theroot;
}

node_t* insert(node_t *root, node_t *node) {
	return insert_internal(root, root, node);
}

void walk(node_t *node) {
	OUTER: while (node != NULL) {
		printf("%s", node->name);
		if (node->right != NULL) {
			node = node->right;
			while(node->left != NULL) {
					node = node->left;
			}
			continue;
		}
		while(node->parent != NULL) {
			if (node == node->parent->left) {
				node = node->parent;
				goto OUTER;
			}
			node = node->parent;
		}
		break;
	}
}

void main(int argc, const char *argv[]) {
	node_t *list = (node_t*)malloc(sizeof(node_t) * 48);
	if (!list) {
		return;
	}
	
	char* data = "???\0???\0???\0???\0???\0???\0???\0???\0???\0???\0???\0???\0???\0???\0???\0???\0???\0???\0???\0???\0???\0???\0???\0???\0???\0???\0???\0???\0???\0???\0???\0???\0???\0???\0???\0???\0???\0???\0???\0???\0???\0???\0???\0???\0???\0???\0???\0???\0";
	node_t *root = NULL;
	node_t *node = list;
	
	for (int i = 0; i < 48; ++i) {
		node->name = data + i * 4;
		root = insert(root, node++);
	}
	
	printf("search\n");
	node_t* hit = search(root, "???");
	walk(hit);
}
