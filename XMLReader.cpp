#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>

const std::string fileName = "test2.xml";

std::string getStrBetweenTwoStr(const std::string& s, const std::string& start_delim, const std::string& stop_delim)
{
	unsigned first_delim_pos = s.find(start_delim);
	unsigned end_pos_of_first_delim = first_delim_pos + start_delim.length();
	unsigned last_delim_pos = s.find(stop_delim);

	return s.substr(end_pos_of_first_delim, last_delim_pos - end_pos_of_first_delim);
}

void writeToResultFile(std::vector<std::string> &idStrings, std::vector<int> &values)
{

	if (idStrings.size() != values.size())
	{
		std::cout << "ERROR - idStrings size not equal to values size." << "( " << idStrings.size() << ", " << values.size() << " )";
		for (auto v : values)
		{
			std::cout << v << ", ";
		}
		return;
	}

	std::ofstream resultsFile;
	resultsFile.open("results.xml", std::ofstream::out | std::ofstream::trunc); // TODO: set name

	resultsFile << "<expressions>";
	resultsFile << '\n';
	for (auto i = 0; i < idStrings.size(); i++)
	{
		resultsFile << "	<result id" << idStrings[i] << values[i] << "</result>";
		resultsFile << '\n';
	}
	resultsFile << "</expressions>";

	resultsFile.close();
}

void calculateValue(std::vector<int>& tmp, std::vector<int> & final, std::string currentOperation)
{
	if (currentOperation == "addition")
	{
		int tmpVal = 0;
		for (auto a : tmp)
		{
			tmpVal += a;
		}
		final.push_back(tmpVal);
	}
	else if (currentOperation == "multiplication")
	{
		int tmpVal = 1;
		for (auto m : tmp)
		{
			tmpVal *= m;
		}
		final.push_back(tmpVal);
	}
	else if (currentOperation == "division")
	{
		final.push_back(tmp[0] / tmp[1]);
	}
	else if (currentOperation == "subtraction")
	{
		final.push_back(tmp[0] - tmp[1]);
	}
	else
	{
		std::cout << "ERROR: Unknow currentOperation type";
	}
	tmp.clear();
}

int main()
{
	std::ifstream inputFile;

	inputFile.open(fileName, std::ios::in);

	std::vector<std::string> idStrings;
	std::vector<int> values;
	std::vector<int> tmpValues;

	std::string currentOperation = "";

	if (inputFile)
	{
		std::string line;
		while (std::getline(inputFile, line))
		{
			if (line.find(" id") != std::string::npos) 
			{
				std::string c = line.substr(line.find('='));
				idStrings.push_back(c);
				currentOperation = getStrBetweenTwoStr(line, "<", " id");
				std::cout << "CURRENT OPERATION: " << currentOperation << std::endl;
				continue;
			}
			if (line.find("</" + currentOperation + ">") != std::string::npos)
			{
				std::cout << "END OF OPERATION: " << currentOperation << std::endl;
				calculateValue(tmpValues, values, currentOperation);
				currentOperation = "";
			}
			if (currentOperation != "")
			{
				std::string c = getStrBetweenTwoStr(line, ">", "</");
				std::cout << "FOUND VALUE: " << c << std::endl;
				tmpValues.push_back(std::stoi(c));
			}
		}
	}
	writeToResultFile(idStrings, values);
	inputFile.close();
	return 0;
}