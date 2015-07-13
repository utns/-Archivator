#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include "tree_struct.h"
#include "differeent func.h"
using namespace std;

vector <vector <int >> c_branch;
vector <char> symbols;

int unpacke(string input_file, string output_file) {
	FILE* in;
	FILE* out;	
	in = fopen(file_name(input_file), "rb");				
	out = fopen(file_name(output_file), "wb");
	fseek(in, 0, SEEK_END);
	int all_file_count = ftell(in);
	fseek(in, 0, SEEK_SET);
	unsigned char fuck;
	for (int k = 0; k < 4; k++) {
		fscanf(in, "%c", &fuck);//TODO: True checking siqnature
	}
	int ending_counter = 0;
	unsigned char current_letter, current_byte, bit_length;	
	unsigned char buf, symbols_count;;
	int count, i;	
	fscanf(in, "%c", &symbols_count);
	count = symbols_count + 1;	
	c_branch.resize(count);
	int current_bit;
	for (i = 0; i < count; i++) {
		fscanf(in, "%c", &current_letter);
		fscanf(in, "%c", &current_byte);
		fscanf(in, "%c", &bit_length);		
		symbols.push_back(current_letter);	
		for (int j = 0; j < bit_length; j++) {
			buf = current_byte;			
			buf = buf >> 8 - (j + 1);
			current_bit = buf & 1;
			c_branch[i].push_back(current_bit);
		}
	}
	unsigned char end_of_table;
	int end_table_counter = 0;
	for (i = 0; i < 3; i++) {
		fscanf(in, "%c", &end_of_table);
		//int buf2 = end_of_table;
		if (end_of_table == 255) {
			end_table_counter++;
		}
	}
	if (end_table_counter != 3) {
		return 0;
	}
	unsigned char last_byte_count;
	//fscanf(in,"%c", &last_byte_count);//
	Treeptr root = new_node(-1);
	Treeptr buf_node = root;
	for (size_t i = 0; i < c_branch.size(); i++) {
		for (size_t j = 0; j < c_branch[i].size(); j++) {
			if (c_branch[i][j] == 0) {
				if (buf_node->left == NULL){
					if (j == c_branch[i].size() - 1) {
						buf_node->left = new_node(symbols[i]);
						buf_node = root;
					}
					else {
						buf_node->left = new_node(-1);
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
						buf_node->right = new_node(symbols[i]);
						buf_node = root;
					}
					else{
						buf_node->right = new_node(-1);
						buf_node = buf_node->right;
					}
				}
				else {
					buf_node = buf_node->right;
				}
			}
		}
	}		
	all_file_count = all_file_count - count * 3 - 5 - 4;
	buf_node = root;
	for (i = 0; i < all_file_count - 1; i++) {		
		fscanf(in, "%c", &current_byte);
		for (int j = 0; j < 8; j++) {
			buf = current_byte;
			buf = buf >> 8 - (j + 1);
			current_bit = buf & 1;
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
			if (buf_node->key != -1)
			{
				fprintf(out, "%c", buf_node->key);
				buf_node = root;
			}			
		}		
	}
	fscanf(in, "%c", &current_byte);
	fscanf(in, "%c", &last_byte_count);
	for (int j = 0; j < last_byte_count; j++) {
		buf = current_byte;
		buf = buf >> 8 - (j + 1);
		current_bit = buf & 1;		
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
		if (buf_node->key != -1)
		{
			fprintf(out, "%c", buf_node->key);
			buf_node = root;
		}
	}
	return 0;
}