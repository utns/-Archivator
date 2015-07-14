#pragma once;
#include <string>
#include "tree_struct.h"
using namespace std;
char* file_name(string name);
Treeptr new_node(char key, bool sheet, int tree_count);
Treeptr new_node(char key, bool sheet);
