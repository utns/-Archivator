#pragma once;

typedef struct tnode *Treeptr;

typedef struct tnode {
	char key;
	int	count;
	struct tnode *left;
	struct tnode *right;
} Treenode;

