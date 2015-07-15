#pragma once;
#include <string>
#define BAD_FILE_NAME -1
#define SIGNATURE -Arh

using namespace std;

int unpacke(string input_file, string output_file);
int archive(string input_file, string output_file);
