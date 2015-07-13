#include <cstdlib>
#include "differeent func.h"


Treeptr new_node(char key) {
	Treeptr node = (Treeptr)malloc(sizeof(Treenode));
	node->key = key;
	node->count = 0;
	node->left = NULL;
	node->right = NULL;
	return node;
}
char* file_name(string name) {
	char *S;
	S = (char*)malloc(sizeof(*S) * name.size());
	size_t i;
	for (i = 0; i < name.size(); i++) {
		S[i] = name[i];
	}
	S[i++] = '\0';
	return S;
}