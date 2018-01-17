#include "FileIO.h"

vector<string> FileIO::ReadShaderFile(const string &file){
	ifstream fileStream(file);

	//Check file exists
	if(!fileStream.good()){
		cout << "Error: file " << file << " not found, check name and/or path" << endl;
		return vector<string>();
	}

	vector<string> fileData;

	for(string line; getline(fileStream, line);){
		//Store line containing data in vector
		fileData.push_back(line);
	}

	return fileData;
}

string FileIO::vectorToString(const vector<string> &list, const char &seperator){
	stringstream ss;

	for(size_t i = 0; i < list.size(); i++){
		ss << list[i];

		if(i != list.size() - 1){
			ss << seperator;
		}
	}

	string s = ss.str();

	return s;
}
