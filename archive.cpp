#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <iterator>
#include <queue>
#include <cstdlib>
#include <functional>
#include <cstring>
#include "tree_struct.h"
#include "differeent func.h"

using namespace std;

class symbol
{
	public:
		unsigned int code;
		int len;
		unsigned char cur_bytes[4];
		int len_byte(int i)
		{
			if (len - 8 *(i + 1) > 0)
			{
				return 8;
			}
			else
			{
				return 8 + (len - 8 *(i + 1));
			}
		}
};

Treeptr nodes[256];
symbol a_symbols[256];
vector <int> code_table;
vector <letter> b_symbols;

int treewalk(Treeptr t, int d, FILE *output)
{
	if (t)
	{
		if (t->sheet)
		{
			b_symbols.push_back(letter(d, t->key));
			if (d == 0)
			{
				a_symbols[t->key].len = 1;
			}
			else
			{
				a_symbols[t->key].len = d;
			}
			return 0;
		}
		treewalk(t->left, ++d, output);
		--d;
		treewalk(t->right, ++d, output);
		--d;
	}
	return 0;
}

typedef bool (*comp)(Treeptr, Treeptr);
bool compare(Treeptr a, Treeptr b)
{
	if (a->count == b->count)
	{
		if (a->sheet && b->sheet)
		{
			return (a->key > b->key);
		}
		if (!a->sheet && b->sheet)
		{
			return (true);
		}
		if (!b->sheet && a->sheet)
		{
			return (false);
		}
		if (!b->sheet && !a->sheet)
		{
			return (a->tree_count > b->tree_count);
		}
	}
	else
	{
		return (a->count > b->count);//<
	}
}


unsigned char Invert(unsigned char x)
{
	int base = 256;
	unsigned char res = 0;
	while (x != 0)
	{
		base >>= 1;
		res += (x & 1) * base;
		x >>= 1;
	}
	return res;
}

int archive(string input_file, FILE *output, unsigned long long int *packsize, unsigned long long int *origsize)
{
	for (int i = 0; i < 256; ++i)
	{
		a_symbols[i].code = 0;
		a_symbols[i].len = 0;
	}
	code_table.clear();
	b_symbols.clear();
	FILE *input = fopen(file_name(input_file), "rb");
	if (input == NULL)
	{
		cout << "File '" << input_file << "' not found." << endl;
		return -1;
	}
	fseek(input, 0, SEEK_END);
	long int count = ftell(input);
	*origsize = (unsigned long long int)count;
	fseek(input, 0, SEEK_SET);
	*packsize = 0;
	for (int i = 0; i < 256; ++i)
	{
		nodes[i] = new_node(i, true, 0);
	}
	unsigned char c_in;
	for (int i = 0; i < count; ++i)
	{
		fscanf(input, "%c", &c_in);
		++nodes[c_in]->count;
	}

	priority_queue<Treeptr, vector<Treeptr>, comp> pq(compare);
	unsigned char count_nodes = 0;
	for (int i = 0; i < 256; ++i)
	{
		if (nodes[i]->count > 0)
		{
			pq.push(nodes[i]);
			++count_nodes;
		}
	}
	int com_tree_count = 0;
	while (pq.size() > 1)
	{
		Treeptr n1 = pq.top();
		pq.pop();
		Treeptr n2 = pq.top();
		pq.pop();
		Treeptr t = new_node(0, false, ++com_tree_count);
		t->left = n1;
		t->right = n2;
		t->count = n1->count + n2->count;
		pq.push(t);
	}

	Treeptr root = pq.top();
	treewalk(root, 0, output);

	sort(b_symbols.begin(), b_symbols.end(), letter_compare);
	a_symbols[b_symbols[0].character].code = 0;
	int last_length = b_symbols[0].code_length, last_code = 0;
	for (size_t l = 1; l < b_symbols.size(); l++) {
		unsigned int new_code;
		if (b_symbols[l].code_length == last_length) {
			new_code = last_code + 1;
		}
		else {
			new_code = (last_code + 1) << (b_symbols[l].code_length - last_length);
		}
		a_symbols[b_symbols[l].character].code = new_code << (32 - b_symbols[l].code_length);
		last_code = new_code;
		last_length = b_symbols[l].code_length;
	}

//	for (int i = 0; i < 256; ++i)
//	{
//		if (a_symbols[i].len > 0) {
//			printf("%c: %d %ud\n", i, a_symbols[i].len, a_symbols[i].code);
//		}
//	}
	for (int i = 0; i < 256; ++i)
	{
		fprintf(output, "%c", a_symbols[i].len);
		memcpy(a_symbols[i].cur_bytes, &a_symbols[i].code, sizeof(int));
	}
	fseek(input, 0, SEEK_SET);
	unsigned char c_out = 0;
	int c_len = 0;

	for (int i = 0; i < count; ++i)
	{
		if (i < count)
		{
			fscanf(input, "%c", &c_in);
		}
		int byte_count;
		if (a_symbols[c_in].len % 8 == 0)
		{
			byte_count = a_symbols[c_in].len / 8;
		}
		else
		{
			byte_count = a_symbols[c_in].len / 8 + 1;
		}
//		unsigned char cur_bytes[4];
//		memcpy(cur_bytes, &a_symbols[c_in].code, sizeof(int));
		for (int i = 0; i < byte_count; ++i)
		{
			c_out = c_out | (a_symbols[c_in].cur_bytes[3 - i] >> c_len);
			c_len += a_symbols[c_in].len_byte(i);
			if (c_len >= 8)
			{
				fprintf(output, "%c", Invert(c_out));
				(*packsize)++;
				c_out = 0;
				if (c_len > 8)
				{
					c_out = c_out | (a_symbols[c_in].cur_bytes[3 - i] << (a_symbols[c_in].len_byte(i) - (c_len - 8)));
					c_len -= 8;
				}
				if (c_len == 8)
				{
					c_len = 0;
				}
			}
		}
	}
	if (c_len != 0)
	{
		fprintf(output, "%c", Invert(c_out));
		(*packsize)++;
	}
	return 0;
}

vector <string> LZW_code_table;

int find(string str)
{
	for (int i = 0; i < LZW_code_table.size(); ++i)
	{
		if (LZW_code_table[i] == str)
		{
			return i;
			break;
		}
	}
	return 0;
}

typedef struct node node;


int LZ_archive(string input_file, FILE* output, unsigned long long int *packsize, unsigned long long int *origsize)
{
	LZW_code_table.clear();
	LZW_code_table.push_back("");
	FILE *input = fopen(file_name(input_file), "rb");
	fseek(input, 0, SEEK_END);
	long int unpack_size = ftell(input);
	fseek(input, 0, SEEK_SET);
	*origsize = (unsigned long long int)unpack_size;
	char c_in;
	string buf_str;
	unsigned short int prev_index = 0;
	*packsize = 0;
	for (int i = 0; i < unpack_size; ++i)
	{
		fscanf(input, "%c", &c_in);
		unsigned short int cur_index = (unsigned short int)find(buf_str + c_in);
		if (!cur_index)
		{
			LZW_code_table.push_back(buf_str + c_in);
			if (LZW_code_table.size() == 65535)
			{
				prev_index = 0;
				LZW_code_table.clear();
				LZW_code_table.push_back("");
			}
			fwrite(&prev_index, sizeof(prev_index), 1, output);
			fprintf(output, "%c", c_in);
			(*packsize) += 3;
			buf_str.clear();
			prev_index = 0;
		}
		else
		{
			buf_str += c_in;
			prev_index = cur_index;
		}
	}
	unsigned short int cur_index = (unsigned short int)find(buf_str);
	if (buf_str.size() > 0)
	{
		buf_str.erase(buf_str.begin() + buf_str.size() - 1);
		fwrite(&cur_index, sizeof(cur_index), 1, output);
		fprintf(output, "%c", c_in);
		(*packsize) += 3;
	}
	return 0;
}

