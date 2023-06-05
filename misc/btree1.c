#include <stdio.h>
#include <stdlib.h>

typedef struct _node {
	int val;
	struct _node *left;
	struct _node *right;
} node;


node *btree_insert(int v, node *b) {
	node *tmp;


	if (b == NULL) {
		tmp = (node *)malloc(sizeof(node));
		if (tmp == NULL)
			return b;
		tmp->val = v;
		tmp->left = NULL;
		tmp->right = NULL;
		return tmp;
	}

	if (v > b->val)
		b->right = btree_insert(v, b->right);
	else
		b->left = btree_insert(v, b->left);

	return b;
}

node *btree_delete(int v, node *b) {
	/*
	 * If it's leaf node or has only one child then it's easy.
	 * Otherwise, the node which is to be deleted, is replaced with its in-order successor or predecessor 
	 * recursively until the node value (to be deleted) is placed on the leaf of the tree. 
	 * After the procedure, replace the node with NULL and free the allocated space.
	 */
}

node *btree_reverse(node *b) {

	node *tmp;

	if (b == NULL)
		return b;

	tmp = b->left;
	b->left = b->right;
	b->right = tmp;

	b->left = btree_reverse(b->left);
	b->right = btree_reverse(b->right);

	return b;
}

void btree_show(node *b) {

	if (b == NULL)
		return;

	btree_show(b->left);
	printf("%d ", b->val);
	btree_show(b->right);
}

void main() {
	node *btree = NULL;

	int n1 = 101, n2 = 50, num;
	for (int i=0;i<100;i++) {
		if (i%2 == 0)
		{
			num = n2;
			n2++;
		} else {
			num = n1;
			n1++;
		}
		btree = btree_insert(num, btree);
	}

	printf("\nThe binary tree is:");
	btree_show(btree);
	printf("\n");
	btree = btree_reverse(btree);
	printf("\nThe binary tree after reversal is:");
	btree_show(btree);
	printf("\n");

}
