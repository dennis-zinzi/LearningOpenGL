#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include "common.h"

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::ifstream;
using std::stringstream;

class FileIO{
	public:
		static vector<string> ReadShaderFile(const string &file);
		static string vectorToString(const vector<string> &list, const char &seperator);
};