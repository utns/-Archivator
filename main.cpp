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

int archive_all_files()
{
//	vector <string> input_file_names;
	vector <long int> FAT_offset;
//	input_files.push_back("asfdasf");
	FILE *output = fopen(file_name(output_file), "wb");
	//FILE *output = fopen(file_name(output_file), "wb");
	fprintf(output, "UPA");
	fprintf(output, "HUFF");
	bool is_solid = false;
	fwrite(&is_solid, sizeof(is_solid), 1, output);
	unsigned short int input_files_count = input_file_names.size();
	fwrite(&input_files_count, sizeof(input_files_count), 1, output);
	//fprintf(output, "%c", 255);
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
//		char *input_file_name = file_name(input_files[i]);
//		fwrite(&input_file_name, sizeof(input_files[i]), 1, output);
	}
	for (size_t i = 0; i < input_file_names.size(); ++i)
	{
		unsigned long long int packsize, origsize;
		archive(input_file_names[i], output, &packsize, &origsize);
		long int cur_offset = ftell(output);
		fseek(output, FAT_offset[i], SEEK_SET);
		fwrite(&packsize, sizeof(packsize), 1, output);
		fwrite(&origsize, sizeof(origsize), 1, output);
		fseek(output, cur_offset, SEEK_SET);
//		char *input_file_name = file_name(input_files[i]);
//		fwrite(&input_file_name, sizeof(input_files[i]), 1, output);
	}
	fclose(output);
	return 0;
}

int main(int argc, char *argv[])  {	
	/*TEST CODE BEGIN*/
	//unpacke("", "");
	//archive("", "");
	//return 0;
	/*TEST CODE END*/
	string input_file(""), command("");
	if (argc == 1) {
		cout << "   -Archivator" << endl;
		cout << "   Utin Nikita, Andrey Machlyarchuk" << endl << "Launch options: " << endl;
		cout << "     '-a input_file_name output_file_name' - archive" << endl;
		cout << "     '-x input_file output_file' - unarchive" << endl;
		//cout << "	  " << endl;
		return 0;
	}
	if (argc > 4) {
		//cout << argv[1];
		command = argv[1];
		if (command == "-a") {
			int i;
			for (i = 2; i < argc - 1; i++) {
				input_file_names.push_back(argv[i]);
			}
			output_file = argv[i++];
			cout << "Input files:" << endl;
			for (i = 0; i < input_file_names.size(); i++) {
				cout << input_file_names[i] << endl;
			}
			cout << "Output file:" << endl;
			cout << output_file << endl;
			archive_all_files();
			/*Func called*/
			return 0;
		}		
		else {
			cout << "'" << command << "'" << " unknown command";
			return 0;
		}
	}
	/*if (argc == 2) {
		command = argv[2];
		if (command == "-m") {
			vector <string> file_names;
			string current_name;			
			cout << "Please write name of input file and press ENTER, to archive wrirte '/x' and press ENTER" << endl;
			while (current_name != "/x") {
				cin >> current_name;
				if (current_name == "/status") {
					cout << "Files for archive" << endl;
					for (size_t i = 0; i < file_names.size(); i++) {
						cout << file_names[i];
					}
				}
			file_names.push_back(current_name);
			}			
		}
	}*/
	/*if (argc != 4) {
		cout << "incorrect input";
		return 0;
	}*/
	if (argc == 4) {
		//cout << argv[2] << endl;
		//cout << argv[3] << endl;
		input_file = argv[2];
		command = argv[1];
		output_file = argv[3];		
	}
	/*if (command == "-a") {
		int result = archive(input_file, output_file);
		if (result == BAD_FILE_NAME) {
			//cout << "Input file not found";    
			return 0;
		}
		cout << "archive" << endl;
		cout << "input file '" << input_file << "'" << endl;
		cout << "output file '" << output_file << "'" << endl;		
	} else*/
	if (command == "-x") {
		int result = unpacke(input_file, output_file);
		if (result == BAD_FILE_NAME) {
			cout << "Input file not found";
			return 0;
		}
		cout << "unpack" << endl;
		cout << "input file '" << input_file << "'" << endl;
		cout << "output file '" << output_file << "'" << endl;		
		//unpacke(input_file, output_file);
		//inarchive(file's in);		
	}
	else {
		cout << "'" << command << "'" << " unknown comand";
	}
	//TODO checking for file opening	
	return 0;	
}
