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
#include "tree_struct.h"
#include "differeent func.h"

using namespace std;

class symbol
{
	public:
		unsigned int code[4];
		int len;
};

int ans[256];
Treeptr nodes[256];
symbol a_symbols[256];

const unsigned char b[8] = {0x80, 0x40, 0x20, 0x10, 0x8, 0x4, 0x2, 0x1};

int treewalk(Treeptr t, int d, FILE *output)
{
	if (t)
	{
		if (t->sheet)
		{
			//fprintf(output, "%c", t->key);
			cout << t->key << " " << nodes[t->key]->count << " " << d << " ";
			unsigned char a[4];
			for (int i = 0; i < 4; ++i)
			{
				a[i] = 0;
			}
			for (int i = 0; i < d; ++i)
			{
				if (ans[i])
				{
					a[i / 8] = a[i / 8] | b[i % 8];
				}
				cout << ans[i];
			}
			//fprintf(output, "%c%", d);
//			a_symbols[t->key].code = a;
			for (int i = 0; i < 4; ++i)
			{
				printf(" %d: %d ", i, a[i]);
				//cout << i << ": " << a[i] << " ";
				a_symbols[t->key].code[i] = a[i];
			}
			cout << " | "<< endl;
			a_symbols[t->key].len = d;
			return 0;
		}
		ans[d] = 0;
		treewalk(t->left, ++d, output);
		--d;
		ans[d] = 1;
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
		if (a->key != -1 && b->key != -1)
		{
			return (a->key > b->key); //>
		}
		if (a->key == -1)
		{
			return (true); //>
		}
		if (b->key == -1)
		{
			return (false); //>
		}
//		else
//		{
//			return (a->key > b->key);
//		}
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
		res += (x & 1) * (base >>= 1);
		x >>= 1;
	}
	return res;
}
int print_byte(unsigned char c, FILE *output)
{

	/*bool b1, b2;
	for (int i = 0; i < 4; ++i)
	{
		b1 = c & b[i];
		b2 = c & b[7 - i];
		if (b1)
		{
			if (!b2)
			{
				c = c + b[7 - i];
			}

		}
		else
		{
			if (b2)
			{
				c = c - b[7 - i];
			}
		}
		if (b2)
		{
			if (!b1)
			{
				c = c + b[i];
			}
		}
		else
		{
			if (b1)
			{
				c = c - b[i];
			}
		}
	}*/
	fprintf(output, "%c", c);
	return 0;
}

int archive(string input_file, string output_file)
{
	FILE *input = fopen(file_name(input_file), "rb");
	FILE *output = fopen(file_name(output_file), "wb");
	//
	//print_byte(188, output);
	fseek(input, 0, SEEK_END);
	int count = ftell(input);
	fseek(input, 0, SEEK_SET);

	for (int i = 0; i < 256; ++i)
	{
		nodes[i] = new_node(i, true);
	}
	unsigned char c_in;
	cout << count << endl;
	for (int i = 0; i < count; ++i)
	{
		fscanf(input, "%c", &c_in);
		++nodes[c_in]->count;
		//printf("|||||%d ||||", c_in);
	}


	priority_queue<Treeptr, vector<Treeptr>, comp> pq(compare);
	priority_queue<Treeptr, vector<Treeptr>, comp> pq1(compare);
	unsigned char count_nodes = 0;
	for (int i = 0; i < 256; ++i)
	{
		if (nodes[i]->count > 0)
		{
			pq.push(nodes[i]);
			++count_nodes;
		}
	}
//	cout << "KKKKK";
//	while (pq.size() > 0)
//	{
//		cout << pq.top()->key;
//		pq.pop();
//	}
//	cout << endl;
	while (pq.size() > 1)
	{
		Treeptr n1 = pq.top();
		pq.pop();
		Treeptr n2 = pq.top();
		pq.pop();
		Treeptr t = new_node(0, false);
		t->left = n1;
		t->right = n2;
		t->count = n1->count + n2->count;
		pq.push(t);
	}

	Treeptr root = pq.top();
	//fprintf(output, "-Arh");
	//fprintf(output, "%c", count_nodes - 1);
	treewalk(root, 0, output);
	for (int i = 0; i < 256; ++i)
	{
		fprintf(output, "%c", nodes[i]->count);
	}

	fprintf(output, "%c%c%c", 255, 255, 255);
	int all_count = 0;
	/*for (int i = 0; i < 256; ++i)
	{
		all_count += (symbols[i].len * nodes[i]->count);
	}
	fprintf(output, "%c", all_count % 8);*/
	fseek(input, 0, SEEK_SET);
	unsigned char c_out = 0;
	int c_len = 0;

	//////
		/*a_symbols[101].code =  32765;
		a_symbols[101].len = 15;
		a_symbols[98].code = 65281;
		a_symbols[98].len = 16;
		count = 2;*/
	//////

	for (int i = 0; i < count; ++i)
	{
		if (i < count)
		{
			fscanf(input, "%c", &c_in);
		}
		/*int len_out =  0;//a_symbols[c_in].len;
		while (len_out != a_symbols[c_in].len)
		{
			c_out = c_out | ((a_symbols[c_in].code << len_out) >> c_len);
			c_len += (a_symbols[c_in].len - len_out - c_len) % 8 + 1;
			len_out += a_symbols[c_in].len - len_out - c_len;
			if (c_len == 8)
			{
				fprintf(output, "%c", c_out);
				c_out = 0;
			}
		}*/
		/*c_out = c_out | (a_symbols[c_in].code >> c_len);
		c_len += a_symbols[c_in].len;
		if (c_len >= 8)
		{
//			print_byte(c_out, output);
			fprintf(output, "%c", Invert(c_out));
			c_out = 0;
			if (c_len > 8)
			{
				c_out = c_out | (a_symbols[c_in].code << (a_symbols[c_in].len - (c_len - 8)));
				c_len -= 8;
			}
			if (c_len == 8)
			{
				c_len = 0;
			}
		}*/
	}
	if (c_len != 0)
	{
//		print_byte(c_out, output);
		fprintf(output, "%c", Invert(c_out));
	}
//	else
//	{
////		print_byte(c_out, output);
//		fprintf(output, "%c", 8);
//	}
//	fprintf(output, "%c", c_len);
	return 0;
}


