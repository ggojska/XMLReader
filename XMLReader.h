#pragma once
#include <string>
#include <fstream>
#include <vector>

class XMLReader
{
private:
	std::string fileName;
	std::ifstream inputFile;
	std::vector<std::string> idStrings;
	std::vector<int> values;
	bool debugMode;

	void writeToResultFile();
	std::string createFileName(std::string name);
	std::string getStrBetweenTwoStr(const std::string& s, const std::string& start_delim, const std::string& stop_delim);
	void calculateValue(std::vector<int>& tmp, std::vector<int> & final, std::string currentOperation);
	void calculateValue(std::vector<int>& tmp, std::string currentOperation);

public:
	XMLReader(std::string s);
	XMLReader(std::string s, bool d);
	~XMLReader();

	void parseXML();
};

