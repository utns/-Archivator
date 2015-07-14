#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <algorithm>
#include "tree_struct.h"
#include "differeent func.h"
using namespace std;

vector <vector <int >> c_branch;
//vector <char> symbols;

class letter {
public:
	int code_length;
	char character;
	letter(int Code_length, char Character) {
		code_length = Code_length;
		character = Character;
	}
};

bool compare(letter i, letter j) {
	if (i.code_length == j.code_length) {
		return (i.character < j.character);
	}
	return (i.code_length < j.code_length);
}

Treeptr print_letter(int current_bit, Treeptr buf_node, Treeptr root, FILE* out) {
	if (current_bit == 0)//TODO: new function for out 
	{
		if (buf_node->left != NULL)
		{
			buf_node = buf_node->left;
		}
		else {
			//Error! Work On!
		}
	}
	else
		if (current_bit == 1)
		{
			if (buf_node->right != NULL)
			{
				buf_node = buf_node->right;
			}
			else {
				//Error! Work On!
			}
		}
	if (buf_node->sheet)
	{
		fprintf(out, "%c", buf_node->key);
		buf_node = root;
	}
	return buf_node;
}

int unpacke(string input_file, string output_file) {
	vector <char> all_code_table(256);
	unsigned char c_byte, c_letter;	
	//input_file = "output.txt"; output_file = "out.txt";
	FILE *in = fopen(file_name(input_file), "rb");
	FILE* out = fopen(file_name(output_file), "wb");
	if (in == NULL) {
		return -1;
	}
	for (int i = 0; i < 256; i++) {
		fscanf(in, "%c", &c_byte);
		if (&c_byte != 0) {
			c_letter = i;
			all_code_table[i] = c_byte;
		}
	}
	vector <int> code_table;
	vector <letter> TEST_symbols;	
	for (int l = 0; l < all_code_table.size(); l++) {
		if (all_code_table[l] > 0) {			
			char index = l;
			TEST_symbols.push_back(letter(all_code_table[l], index));
		}
	}
	sort(TEST_symbols.begin(), TEST_symbols.end(), compare);
	code_table.resize(TEST_symbols.size());
	code_table[0] = 0;
	int last_length = TEST_symbols[0].code_length, last_code = 0;
	for (int l = 1; l < TEST_symbols.size(); l++) {		
		if (TEST_symbols[l].code_length == last_length) {
			code_table[l] = last_code + 1;
		}
		else {
			code_table[l] = (last_code + 1) << 1;
		}
		last_code = code_table[l];
		last_length = TEST_symbols[l].code_length;
	}
	c_branch.resize(code_table.size());
	for (int l = 0; l < code_table.size(); l++) {
		int current_byte = code_table[l];
		int bit_length = TEST_symbols[l].code_length;
		int current_bit;
		int buf;
		for (int k = 0; k < bit_length; k++) {
			buf = current_byte;
			buf = buf >> k;
			current_bit = buf & 1;
			c_branch[l].push_back(current_bit);
		}
	}
	int size_bufer;
	for (int l = 0; l < c_branch.size(); l++) {
		size_bufer = c_branch[l].size();
		for (int k = 0; k < size_bufer / 2; k++) {
			swap(c_branch[l][k], c_branch[l][size_bufer - k - 1]);
		}
	}	
	Treeptr root = new_node(0, false);
	Treeptr buf_node = root;
	for (size_t i = 0; i < c_branch.size(); i++) {	
		for (size_t j = 0; j < c_branch[i].size(); j++) {
			if (c_branch[i][j] == 0) {
				if (buf_node->left == NULL){
					if (j == c_branch[i].size() - 1) {
						buf_node->left = new_node(TEST_symbols[i].character, true);
						buf_node = root;
					}
					else {
						buf_node->left = new_node(0, false);
						buf_node = buf_node->left;
					}
				}
				else {
					buf_node = buf_node->left;
				}
			}
			if (c_branch[i][j] == 1) {
				if (buf_node->right == NULL){
					if (j == c_branch[i].size() - 1) {
						buf_node->right = new_node(TEST_symbols[i].character, true);
						buf_node = root;
					}
					else{
						buf_node->right = new_node(0, false);
						buf_node = buf_node->right;
					}
				}
				else {
					buf_node = buf_node->right;
				}
			}
		}
	}
	/*Here I already got number of bytes for decode data, from FAT ENTRY*/
	int data_counter = 5;
	buf_node = root;
	for (int i = 0; i < data_counter; i++) {
		fscanf(in, "%c", &c_byte);
		unsigned char data_buf;
		for (int j = 0; j < 8; j++) {
			data_buf = c_byte;
			data_buf = data_buf >> j;
			int current_data_bit = data_buf & 1;
			buf_node = print_letter(current_data_bit, buf_node, root, out);
		}
	}
	return 0;	
}