#include <iostream>
#include <fstream>
#include <string>
#include <vector>
//#include <assert.h>
#include <cstdlib>
#include "unpack and archive.h"
#include "differeent func.h"
//vector <string> input_file_names;
//string output_file_name;
//#include <cstdio>
vector <string> input_file_names;
string output_file("");

using namespace std;
bool lz_al;

int read_head(string input_file) {
	bool huffman_flag = false;
	bool lz_algo = false;
	FILE *in = fopen(file_name(input_file), "rb");	
	if (in == NULL) {
		cout << "File not found" << endl;
		return -1;
	}
	char sign[3];
	fread(sign, sizeof(char), 3, in);
	/*if (strcmp(sign, "UPA") == 0) {
	cout << "Incorrect input file format" << endl;
	return -1;
	}*/
	if (!(sign[0] == 'U' && sign[1] == 'P' && sign[2] == 'A')) {
		cout << "Incorrect input file format" << endl;
		return -1;
	}
	char method_id[5];
	fread(method_id, 1, 4, in);
	method_id[4] = '\0';
	if (strcmp(method_id, "HUFF") == 0) {
	//if (method_id[0] == 'H' && method_id[1] == 'U' && method_id[2] == 'F' && method_id[3] == 'F') {
		huffman_flag = true;
	}
	else if (strcmp(method_id, "LZAL") == 0) {
		lz_algo = true;
	} else {
		cout << "I can't work with this algo, it is temporarily" << endl;
		return -1;
	}
	bool solid_stream_flag;
	fread(&solid_stream_flag, sizeof(solid_stream_flag), 1, in);
	if (solid_stream_flag) {
		cout << "I cant  work with solid stream data, it is temporarily" << endl;
		return -1;
	}
	unsigned short int files_count;
	fread(&files_count, sizeof(files_count), 1, in);
	vector <string> files_name(files_count, "");
	vector <unsigned long long int> files_size(files_count, 0);
	vector <unsigned long long int> pack_files_size(files_count, 0);
	unsigned char fn_len;
	for (int i = 0; i < files_count; i++) {
		fread(&fn_len, sizeof(fn_len), 1, in);
		fn_len++;
		char* file_name = (char*)malloc(fn_len * sizeof(char));
		fread(file_name, sizeof(char), fn_len, in);
		for (int j = 0; j < fn_len; j++) {
			files_name[i].push_back(file_name[j]);
		}
		unsigned long long int pack_size;
		fread(&pack_size, sizeof(pack_size), 1, in);
		unsigned long long int original_size;
		fread(&original_size, sizeof(original_size), 1, in);
		files_size[i] = original_size;
		pack_files_size[i] = pack_size;
		/*unsigned char file_attr;
		fread(&file_attr, sizeof(file_attr), 1, in);
		if (file_attr != 0) {
		return -1;
		}
		int file_date;
		fread(&file_date, sizeof(file_date), 1, in);*/
	}
	if (lz_algo) {
		for (int i = 0; i < files_count; i++) {
			lz_unpacke(in, files_name[i], pack_files_size[i]);
		}
	}
	else {
		for (int i = 0; i < files_count; i++) {
			unpacke(in, files_name[i], files_size[i]);
		}
	}
	
	return 0;
}


int archive_all_files()
{
	vector <long int> FAT_offset;
	FILE *output = fopen(file_name(output_file), "wb");
	fprintf(output, "UPA");
	if (lz_al) {
		fprintf(output, "LZAL");
	}
	else {
		fprintf(output, "HUFF");
	}	
	bool is_solid = false;
	fwrite(&is_solid, sizeof(is_solid), 1, output);
	unsigned short int input_files_count = input_file_names.size();
	fwrite(&input_files_count, sizeof(input_files_count), 1, output);
	for (size_t i = 0; i < input_file_names.size(); ++i)
	{
		unsigned char fn_len = input_file_names[i].size() - 1;
		fwrite(&fn_len, sizeof(fn_len), 1, output);
		for (size_t j = 0; j < input_file_names[i].size(); ++j)
		{
			fprintf(output, "%c", input_file_names[i][j]);
		}
		FAT_offset.push_back(ftell(output));
		fseek(output, 2 * sizeof(unsigned long long int), SEEK_CUR);
	}
	for (size_t i = 0; i < input_file_names.size(); ++i)
	{
		unsigned long long int packsize, origsize;
		if (lz_al) {
			LZ_archive(input_file_names[i], output, &packsize, &origsize);
		} else 
		if (archive(input_file_names[i], output, &packsize, &origsize) == -1)
		{
			break;
		}
		long int cur_offset = ftell(output);
		fseek(output, FAT_offset[i], SEEK_SET);
		fwrite(&packsize, sizeof(packsize), 1, output);
		fwrite(&origsize, sizeof(origsize), 1, output);
		fseek(output, cur_offset, SEEK_SET);
	}
	fclose(output);
	return 0;
}

int main(int argc, char *argv[])  {		
	string input_file(""), command(""), key("");
	lz_al = false;
	if (argc == 1) {
		cout << "   -Archivator" << endl;
		cout << "   Utin Nikita, Andrey Machlyarchuk" << endl << "Launch options: " << endl;
		cout << "     '-a input_file_name output_file_name' - archive" << endl;
		cout << "     '-x input_*.upa_file_name' - unarchive" << endl;		
		return 0;
	} else
	if (argc >= 4) {
		command = argv[1];
		key = argv[2];
		if (key == "-lz") {
			lz_al = true;
		}
		if (command == "-a") {
			int i;
			if (lz_al) {
				cout << "LZ algo key used" << endl;
				for (i = 3; i < argc - 1; i++) {
					input_file_names.push_back(argv[i]);
				}
			}
			else {
				for (i = 2; i < argc - 1; i++) {
					input_file_names.push_back(argv[i]);
				}
			}			
			output_file = argv[i++];
			cout << "Input files:" << endl;
			for (i = 0; i < input_file_names.size(); i++) {
				cout << input_file_names[i] << endl;
			}
			cout << "Output file:" << endl;
			cout << output_file << endl;			
			//return 0; 
			archive_all_files();
			/*Func called*/
			return 0;
		}		
		else {
			cout << "'" << command << "'" << " unknown command";
			return 0;
		}
	} else
	if (argc == 3) {
		input_file = argv[2];
		command = argv[1];
		if (command == "-x") {
			FILE* input = fopen(file_name(input_file), "rb");
			//lz_unpacke(input, "out.txt", 0);
			read_head(input_file);
		}
		else if (command == "-a") {
			cout << " incorrect command format" << " for command " << "'" << command << "'" << endl;
		}
		else {
			cout << "'" << command << "'" << " unknown command";
			return 0;
		}		
		return 0;
	}
	else {
		command = argv[1];
		cout << " incorrect command format" << "for command" << "'" << command << "'" << endl;
	}
	return 0;	
}
