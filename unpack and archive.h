#pragma once;
#include <string>
#define BAD_FILE_NAME -1
#define SIGNATURE -Arh

using namespace std;

int lz_unpacke(FILE* in, string output_file, unsigned long long int pack_size);
int unpacke(FILE* in, string output_file, unsigned long long int packed_size);
int archive(string input_file, FILE *output, unsigned long long int *packsize, unsigned long long int *origsize);
int LZ_archive(string input_file, FILE* output, unsigned long long int *packsize, unsigned long long int *origsize);