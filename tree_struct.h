#pragma once;

typedef struct tnode *Treeptr;

typedef struct tnode {
	unsigned char key;
	bool sheet;
	int	count, tree_count;
	struct tnode *left;
	struct tnode *right;
} Treenode;

