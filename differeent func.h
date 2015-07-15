#pragma once;
#include <string>
#include "tree_struct.h"
using namespace std;
class letter {
public:
	int code_length;
	unsigned char character;
	letter(int Code_length, char Character) {
		code_length = Code_length;
		character = Character;
	}
};
char* file_name(string name);
Treeptr new_node(char key, bool sheet, int tree_count);
Treeptr new_node(char key, bool sheet);
bool letter_compare(letter i, letter j);
