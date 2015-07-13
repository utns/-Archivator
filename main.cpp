#include <iostream>
#include <fstream>
#include <string>
//#include <assert.h>
#include <cstdlib>
#include "unpack and archive.h"

//#include <cstdio>

using namespace std;

int main(int argc, char *argv[])  {	
	string input_file(""), command(""), output_file("");
	//input_file = "input.bin";	
	if (argc == 1) {
		cout << "   -Archivator" << endl;
		cout << "   Utin Nikita, Andrey Machlyarchuk" << endl << "Launch options: " << endl;
		cout << "     '-a input_file_name output_file_name' - archive" << endl;
		cout << "     '-x input_file output_file' - unarchive" << endl;
		return 0;
	}
	if (argc != 4) {
		cout << "incorrect input";
		return 0;
	}
	if (argc == 4) {
		input_file = argv[2];
		command = argv[1];
		output_file = argv[3];		
	}
	if (command == "-a") {
		cout << "archive" << endl;
		cout << "input file '" << input_file << "'" << endl;
		cout << "output file '" << output_file << "'" << endl;
		archive(input_file, output_file);
	} else 
	if (command == "-x") {
		cout << "unpack" << endl;
		cout << "input file '" << input_file << "'" << endl;
		cout << "output file '" << output_file << "'" << endl;		
		unpacke(input_file, output_file);
		//inarchive(file's in);		
	}
	else {
		cout << "'" << command << "'" << " unknown comand";
	}
	//TODO checking for file opening
	return 0;	
}