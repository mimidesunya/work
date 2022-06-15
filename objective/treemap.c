#include "treemap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct node node_t;

typedef enum label {
	L, E, R
} label_t;

struct node {
	char* value;
	char* key;
	node_t* parent;
	node_t* left;
	node_t* right;
	label_t label;
};

typedef struct {
	node_t *root;
} tree_data;

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
	int a = strcmp(root->key, node->key);
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

void tree_insert(void *data, char* key, char* value) {
	tree_data *tree = data;
	node_t *node = malloc(sizeof(node_t));
	node->key = key;
	node->value = value;
	tree->root = insert_internal(tree->root, tree->root, node);
}

node_t* next_node(node_t* node) {
	if (node->right != NULL) {
		node = node->right;
		while(node->left != NULL) {
				node = node->left;
		}
	}
	else {
		while(node->parent != NULL) {
			if (node == node->parent->left) {
				node = node->parent;
				break;
			}
			node = node->parent;
		}
	}
	return node;
}

node_t* equal_or_next(node_t *node, char* key) {
	if (node == NULL) {
		return NULL;
	}
	int a = strcmp(node->key, key);
	if (a > 0) {
		node = equal_or_next(node->left, key);
	}
	else if (a < 0) {
		if (node->right != NULL) {
			node = equal_or_next(node->right, key);
		}
		else {
			node = next_node(node);
		}
	}
	return node;
}

char* tree_search(void* data, char* key) {
	tree_data *tree = data;
	node_t* node = equal_or_next(tree->root, key);
	return (node == NULL || strcmp(node->key, key) != 0) ? NULL : node->value;
}

node_t* remove_balance(node_t *theroot, node_t *root, label_t label) {
	bool up = true;
	switch (root->label) {
		case E:
		root->label = label;
		break;
		
		case L:
		if (label == R) {
			root->label = E;
			if (root->parent) {
				return remove_balance(theroot, root->parent, root->parent->left == root ? R : L);
			}
			break;
		}
		node_t* left = root->left;
		if (left == NULL) {
			printf("Illegal state.\n");
			exit(1);
		}
		if (left->label == R) {
			if (left->right == NULL) {
				exit(1);
			}
			switch(left->right->label) {
				case L:
					root->label = R;
					left->label = E;
				break;
				case E:
					root->label = E;
					left->label = E;
				break;
				case R:
					root->label = E;
					left->label = L;
				break;
				default:
					printf("Illegal state.\n");
					exit(1);
				break;
			}
			left->right->label = E;
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
			switch(left->label) {
				case E:
					root->label = L;
					left->label = R;
					up = false;
				break;
				case L:
					root->label = E;
					left->label = E;
				break;
				default:
					printf("Illegal state.\n");
					exit(1);
				break;
			}
		}
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
		if (up && left->parent) {
			return remove_balance(theroot, left->parent, left->parent->left == left ? R : L);
		}
		break;
		
		case R:
		if (label == L) {
			root->label = E;
			if (root->parent) {
				return remove_balance(theroot, root->parent, root->parent->left == root ? R : L);
			}
			break;
		}
		node_t* right = root->right;
		if (right == NULL) {
			printf("Illegal state.\n");
			exit(1);
		}
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
				case E:
					root->label = E;
					right->label = E;
				break;
				case L:
					root->label = E;
					right->label = R;
				break;
				default:
					printf("Illegal state.\n");
					exit(1);
				break;
			}
			right->left->label = E;
			right->left->parent = root;
			root->right = right->left;
			if (root->right->right) {
				root->right->right->parent = right;
				if (right->right == NULL) {
					printf("Illegal state.\n");
					exit(1);
				}
			}
			right->left = root->right->right;
			right->parent = root->right;
			root->right->right = right;
			right = right->parent;
		}
		else {
			switch(right->label) {
				case E:
					root->label = R;
					right->label = L;
					up = false;
				break;
				case R:
					root->label = E;
					right->label = E;
				break;
				default:
					printf("Illegal state.\n");
					exit(1);
				break;
			}
		}
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
		if (up && right->parent) {
			return remove_balance(theroot, right->parent, right->parent->left == right ? R : L);
		}
		break;
		
		default:
		exit(1);
		break;
	}
	return theroot;
}

node_t* internal_remove(node_t *root, node_t *node) {
	if (node->left != NULL && node->right != NULL) {
		node_t *leftmax = node->left;
		while (leftmax->right != NULL) {
			leftmax = leftmax->right;
		}
		label_t label;
		node_t * parent;
		if (leftmax->parent == node) {
			leftmax->right = node->right;
			if (node->right != NULL) {
				node->right->parent = leftmax;
			}
			label = R;
			parent = leftmax;
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
			label = L;
			parent = leftmax->parent;
		}
		leftmax->parent = node->parent;
		leftmax->label = node->label;
		if (node->parent == NULL) {
			root = leftmax;
		}
		else if (node->parent->left == node) {
			node->parent->left = leftmax;
		}
		else {
			node->parent->right = leftmax;
		}
		root = remove_balance(root, parent, label);
		return root;
	}
	else if (node->left != NULL) {
		node->left->parent = node->parent;
		if (node->parent == NULL) {
			root = node->left;
		}
		else if (node->parent->left == node) {
			node->parent->left = node->left;
			root = remove_balance(root, node->parent, R);
		}
		else {
			node->parent->right = node->left;
			root = remove_balance(root, node->parent, L);
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
			root = remove_balance(root, node->parent, R);
		}
		else {
			node->parent->right = node->right;
			root = remove_balance(root, node->parent, L);
		}
		return root;
	}
	else {
		if (node->parent == NULL) {
			root = NULL;
		}
		else if (node->parent->left == node) {
			node->parent->left = NULL;
			root = remove_balance(root, node->parent, R);
		}
		else {
			node->parent->right = NULL;
			root = remove_balance(root, node->parent, L);
		}
		return root;
	}
}

char* tree_remove(void* data, char* key) {
	tree_data *tree = data;
	node_t* node = equal_or_next(tree->root, key);
	if (node == NULL || strcmp(node->key, key) != 0) {
		return NULL;
	}
	char* value = node->value;
	tree->root = internal_remove(tree->root, node);
	free(node);
	return value;
}

void node_dispose(node_t* node) {
	if (node == NULL) {
		return;
	}
	node_dispose(node->left);
	node_dispose(node->right);
	free(node);
}

void tree_dispose(void* data) {
	tree_data *tree = data;
	node_dispose(tree->root);
	free(tree);
}

typedef struct {
	node_t *current;
	node_t *next;
	char* key2;
} tree_itr_data;

bool tree_itr_has_next(void* data) {
	tree_itr_data* itr_data = (tree_itr_data*)data;
	return itr_data->next != NULL;
}

void tree_itr_next(void* data) {
	tree_itr_data* itr_data = (tree_itr_data*)data;
	if (itr_data->next == NULL) {
		return;
	}
	itr_data->current = itr_data->next;
	itr_data->next = next_node(itr_data->next);
	if (itr_data->next != NULL) {
		if (strcmp(itr_data->next->key, itr_data->key2) > 0) {
			itr_data->next = NULL;
		}
	}
}

char* tree_itr_key(void* data) {
	tree_itr_data* itr_data = (tree_itr_data*)data;
	return itr_data->current->key;
}

char* tree_itr_value(void* data) {
	tree_itr_data* itr_data = (tree_itr_data*)data;
	return itr_data->current->value;
}

void tree_itr_dispose(void* data) {
	tree_itr_data* itr_data = (tree_itr_data*)data;
	free(itr_data->key2);
	free(itr_data);
}

iterator* tree_iterator(void* data, char* key1, char* key2)	{
	tree_data *tree = data;
	iterator* itr = malloc(sizeof(iterator));
	itr->has_next = tree_itr_has_next;
	itr->next = tree_itr_next;
	itr->key = tree_itr_key;
	itr->value = tree_itr_value;
	itr->dispose = tree_itr_dispose;
	tree_itr_data* itr_data = itr->data =  malloc(sizeof(tree_itr_data));
	itr_data->current = NULL;
	itr_data->next = equal_or_next(tree->root, key1);
	itr_data->key2 = malloc(strlen(key2) + 1);
	strcpy(itr_data->key2, key2);
	return itr;
}


extern sortedmap* create_treemap(size_t size) {
	sortedmap* treemap = malloc(sizeof(sortedmap));
	treemap->insert = tree_insert;
	treemap->search = tree_search;
	treemap->remove = tree_remove;
	treemap->dispose = tree_dispose;
	treemap->iterator = tree_iterator;
	tree_data* data = treemap->data = malloc(sizeof(tree_data));
	data->root = NULL;
	return treemap;
}