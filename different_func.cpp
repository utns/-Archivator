#include <cstdlib>
#include "differeent func.h"


Treeptr new_node(char key, bool sheet, int tree_count) {
	Treeptr node = (Treeptr)malloc(sizeof(Treenode));
	node->sheet = sheet;
	node->key = key;
	node->count = 0;
	node->tree_count = tree_count;
	node->left = NULL;
	node->right = NULL;
	return node;
}

Treeptr new_node(char key, bool sheet) {
	Treeptr node = (Treeptr)malloc(sizeof(Treenode));
	node->sheet = sheet;
	node->key = key;
	node->count = 0;
	node->tree_count = 0;
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

bool letter_compare(letter i, letter j) {
   if (i.code_length == j.code_length) {
	   return (i.character < j.character);
   }
   return (i.code_length < j.code_length);
}
