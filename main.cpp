#include <iostream>
#include <fstream>
#include <string>
//#include <assert.h>
#include <cstdlib>
#include "unpack and archive.h"

//#include <cstdio>

using namespace std;

int main(int argc, char *argv[])  {	
	/*TEST CODE BEGIN*/
	//unpacke("", "");
	//archive("", "");
	//return 0;
	/*TEST CODE END*/
	string input_file(""), command(""), output_file("");	
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
		//cout << argv[2] << endl;
		//cout << argv[3] << endl;
		input_file = argv[2];
		command = argv[1];
		output_file = argv[3];		
	}
	if (command == "-a") {
		int result = archive(input_file, output_file);
		if (result == BAD_FILE_NAME) {
			//cout << "Input file not found";    arch.exe  -x <input.txt
			return 0;
		}
		cout << "archive" << endl;
		cout << "input file '" << input_file << "'" << endl;
		cout << "output file '" << output_file << "'" << endl;		
	} else 
	if (command == "-x") {
		int result = unpacke(input_file, output_file);
		if (result == BAD_FILE_NAME) {
			cout << "Input file not found";
			return 0;
		}
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