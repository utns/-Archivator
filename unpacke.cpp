#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <cstring>
#include "tree_struct.h"
#include "differeent func.h"
using namespace std;

vector <vector <int>> c_branch;
 
unsigned long long int  byte_counter = 0;
//vector <char> symbols;

class letter {
public:
	int code_length;
	unsigned char character;
	letter(int Code_length,unsigned char Character) {
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
		byte_counter++;
		buf_node = root;
	}
	return buf_node;
}

int unpacke(FILE* in, string output_file, unsigned long long int origin_size) {
	c_branch.clear();
	byte_counter = 0;
	unsigned char all_code_table[256];
	unsigned char c_byte, c_letter;			
	//FILE *in = fopen(file_name(input_file), "rb");
	FILE* out = fopen(file_name(output_file), "wb");
	if (in == NULL) {
		return -1;
	}	
	fread(all_code_table, 1, 256, in);	
	vector <int> code_table;
	vector <letter> TEST_symbols;	
	for (int l = 0; l < 256; l++) {
		if (all_code_table[l] > 0) {			
			unsigned char index = l;
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
			code_table[l] = (last_code + 1) << (TEST_symbols[l].code_length - last_length);
		}
		last_code = code_table[l];
		last_length = TEST_symbols[l].code_length;
		printf("%c: %d %d\n", TEST_symbols[l].character, last_length, last_code);
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
	//int data_counter = 26;
	buf_node = root;
	unsigned long long int size_none_pack = origin_size;
	while (byte_counter < size_none_pack) {
		fscanf(in, "%c", &c_byte);
		unsigned char data_buf;
		for (int j = 0; j < 8 && byte_counter < size_none_pack; j++) {
			data_buf = c_byte;
			data_buf = data_buf >> j;
			int current_data_bit = data_buf & 1;
			buf_node = print_letter(current_data_bit, buf_node, root, out);
		}
	}
	return 0;	
}