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

/*typedef struct tnode *Treeptr;
typedef struct tnode
{
	int key, count;
	struct tnode *left;
	struct tnode *right;
} Treenode;

Treeptr new_node(int key)
{
	Treeptr node = (Treeptr)malloc(sizeof(Treenode));
	node->key = key;
	node -> count = 0;
	node -> left = NULL;
	node -> right = NULL;
	return node;
}*/

class symbol
{
	public:
		unsigned char code;
		int len;
};

int ans[8];
Treeptr nodes[256];
symbol symbols[256];

const unsigned char b[8] = {0x80, 0x40, 0x20, 0x10, 0x8, 0x4, 0x2, 0x1};

int treewalk(Treeptr t, int d, FILE *output)
{
	if (t)
	{
		if (t->key != -1)
		{
			fprintf(output, "%c", t->key);
			unsigned char a = 0;
			for (int i = 0; i < d; ++i)
			{
				if (ans[i])
				{
					a = a | b[i];
				}
			}
			fprintf(output, "%c%c", a, d);
			symbols[t->key].code = a;
			symbols[t->key].len = d;
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
		return (a->key < b->key);
	}
	else
	{
		return (a->count > b->count);
	}
}


int archive(string input_file, string output_file)
{
	FILE *input = fopen(file_name(input_file), "rb");
	FILE *output = fopen(file_name(output_file), "wb");
	fseek(input, 0, SEEK_END);
	int count = ftell(input);
	fseek(input, 0, SEEK_SET);

	for (int i = 0; i < 256; ++i)
	{
		nodes[i] = new_node(i);
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
	while (pq.size() > 1)
	{
		Treeptr n1 = pq.top();
		pq.pop();
		Treeptr n2 = pq.top();
		pq.pop();
		Treeptr t = new_node(-1);
		t->left = n1;
		t->right = n2;
		t->count = n1->count + n2->count;
		pq.push(t);
	}
	Treeptr root = pq.top();
	fprintf(output, "-Arh");
	fprintf(output, "%c", count_nodes - 1);
	treewalk(root, 0, output);
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
	for (int i = 0; i < count; ++i)
	{
		if (i < count)
		{
			fscanf(input, "%c", &c_in);
		}
		c_out = c_out | (symbols[c_in].code >> c_len);
		c_len += symbols[c_in].len;
		if (c_len >= 8)
		{
			fprintf(output, "%c", c_out);
			c_out = 0;
			if (c_len > 8)
			{
				c_out = c_out | (symbols[c_in].code << (symbols[c_in].len - (c_len - 8)));
				c_len -= 8;
			}
			if (c_len == 8)
			{
				c_len = 0;
			}
		}
	}
	if (c_len != 0)
	{
		fprintf(output, "%c", c_out);
	}
	fprintf(output, "%c", c_len);
	return 0;
}


