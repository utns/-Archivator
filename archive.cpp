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
		int len_byte(int i)
		{
			if (len - 8 *(i + 1) > 0)
			{
				//cout << i << "=8 ";
				return 8;
			}
			else
			{
				//cout << i << "=" <<  8 + (len - 8 *(i + 1)) << " ";
				return 8 + (len - 8 *(i + 1));
			}
		}
};

class letter {
public:
	unsigned int code_length;
	unsigned char character;
	letter(int Code_length, char Character) {
		code_length = Code_length;
		character = Character;
	}
};

bool l_compare(letter i, letter j) {
	if (i.code_length == j.code_length) {
		return (i.character < j.character);
	}
	return (i.code_length < j.code_length);
}

Treeptr nodes[256];
symbol a_symbols[256];
vector <int> code_table;
vector <letter> b_symbols;

const unsigned char b[8] = {0x80, 0x40, 0x20, 0x10, 0x8, 0x4, 0x2, 0x1};

int treewalk(Treeptr t, int d, FILE *output)
{
	if (t)
	{
		if (t->sheet)
		{
			b_symbols.push_back(letter(d, t->key));
			a_symbols[t->key].len = d;
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

	sort(b_symbols.begin(), b_symbols.end(), l_compare);
	a_symbols[b_symbols[0].character].code = 0;
	int last_length = b_symbols[0].code_length, last_code = 0;
	for (int l = 1; l < b_symbols.size(); l++) {
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
		unsigned char cur_bytes[4];
		memcpy(cur_bytes, &a_symbols[c_in].code, sizeof(int));
		for (int i = 0; i < byte_count; ++i)
		{
			c_out = c_out | (cur_bytes[3 - i] >> c_len);
			c_len += a_symbols[c_in].len_byte(i);
			if (c_len >= 8)
			{
				fprintf(output, "%c", Invert(c_out));
				(*packsize)++;
				c_out = 0;
				if (c_len > 8)
				{
					c_out = c_out | (cur_bytes[3 - i] << (a_symbols[c_in].len_byte(i) - (c_len - 8)));
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


