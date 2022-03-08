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
	char* number;
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


node_t* delete_balance(node_t *theroot, node_t *root, label_t label) {
	bool up = true;
	switch (root->label) {
		case E:
		root->label = label;
		break;
		
		case L:
		if (label == R) {
			root->label = E;
			if (root->parent) {
				return delete_balance(theroot, root->parent, root->parent->left == root ? R : L);
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
			return delete_balance(theroot, left->parent, left->parent->left == left ? R : L);
		}
		break;
		
		case R:
		if (label == L) {
			root->label = E;
			if (root->parent) {
				return delete_balance(theroot, root->parent, root->parent->left == root ? R : L);
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
			return delete_balance(theroot, right->parent, right->parent->left == right ? R : L);
		}
		break;
		
		default:
		exit(1);
		break;
	}
	return theroot;
}

node_t* delete(node_t *root, node_t *node) {
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
		root = delete_balance(root, parent, label);
		return root;
	}
	else if (node->left != NULL) {
		node->left->parent = node->parent;
		if (node->parent == NULL) {
			root = node->left;
		}
		else if (node->parent->left == node) {
			node->parent->left = node->left;
			root = delete_balance(root, node->parent, R);
		}
		else {
			node->parent->right = node->left;
			root = delete_balance(root, node->parent, L);
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
			root = delete_balance(root, node->parent, R);
		}
		else {
			node->parent->right = node->right;
			root = delete_balance(root, node->parent, L);
		}
		return root;
	}
	else {
		if (node->parent == NULL) {
			root = NULL;
		}
		else if (node->parent->left == node) {
			node->parent->left = NULL;
			root = delete_balance(root, node->parent, R);
		}
		else {
			node->parent->right = NULL;
			root = delete_balance(root, node->parent, L);
		}
		return root;
	}
}

int depth(node_t *root) {
	if (root == NULL) {
		return 0;
	}
	int l = depth(root->left);
	int r = depth(root->right);
	if (l >= r + 2 || r >= l + 2) {
		printf("Illegal state: %d %d\n", l, r);
		exit(1);
	}
	switch(root->label) {
		case L:
		if (l != r + 1) {
			printf("Illegal state: L bat %d %d\n", l, r);
			exit(1);
		}
		break;
		case E:
		if (l != r) {
			printf("Illegal state: E %d %d\n", l, r);
			exit(1);
		}
		break;
		case R:
		if (l != r - 1) {
			printf("Illegal state: R %d %d\n", l, r);
			exit(1);
		}
		break;
	}
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
	FILE *fp = fopen(file, "r");
	char *pdata = data;
	size_t i = 0;
	node_t *root = NULL;
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
	printf("depth %d\n", depth(root));
	
	printf("%s,%s\n", hit->number, hit->name);
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
