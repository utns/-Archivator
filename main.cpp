#include <iostream>
#include <fstream>
#include <string>
#include <assert.h>

using namespace std;
int main(int argc, char *argv[])  {	
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
		input_file = argv[2];
		command = argv[1];
		output_file = argv[3];
	}
	if (command == "-a") {
		cout << "archive" << endl;
		cout << "input file '" << input_file << "'" << endl;
		cout << "output file '" << output_file << "'" << endl;
		//archive(file's in);
	} else 
	if (command == "-x") {
		cout << "unarchive" << endl;
		cout << "input file '" << input_file << "'" << endl;
		cout << "output file '" << output_file << "'" << endl;
		//inarchive(file's in);		
	}
	else {
		cout << "'" << command << "'" << " unknown comand";
	}
	//TODO checking for file opening
	return 0;
}