#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>

const std::string fileName = "test2.xml";
const char spaceChar = ' ';

std::string getStrBetweenTwoStr(const std::string& s, const std::string& start_delim, const std::string& stop_delim)
{
	unsigned first_delim_pos = s.find(start_delim);
	unsigned end_pos_of_first_delim = first_delim_pos + start_delim.length();
	unsigned last_delim_pos = s.find(stop_delim);

	return s.substr(end_pos_of_first_delim, last_delim_pos - end_pos_of_first_delim);
}


std::string createFileName(std::string name)
{
	std::string s = getStrBetweenTwoStr(name, "", ".xml");
	return s + "_result.xml";
}

void writeToResultFile(std::vector<std::string> &idStrings, std::vector<int> &values)
{

	if (idStrings.size() != values.size())
	{
		std::cout << "ERROR - idStrings size not equal to values size." << "( " << idStrings.size() << ", " << values.size() << " )" << std::endl;
		for (auto v : values)
		{
			std::cout << v << ", ";
		}
		return;
	}

	std::ofstream resultsFile;
	resultsFile.open(createFileName(fileName), std::ofstream::out | std::ofstream::trunc);

	resultsFile << "<expressions>";
	resultsFile << '\n';
	for (auto i = 0; i < idStrings.size(); i++)
	{
		resultsFile << "	<result id=\"" << idStrings[i] << "\">" << values[i] << "</result>";
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
}

int main()
{
	std::ifstream inputFile;

	inputFile.open(fileName, std::ios::in);

	std::vector<std::string> idStrings;
	std::vector<int> values;
	std::vector<std::vector<int>> tmpValues;

	std::vector<std::string> currentOperation;

	bool isComplex = false;
	std::cout << "BEGIN" << std::endl;
	if (inputFile)
	{
		std::string line;
		while (std::getline(inputFile, line))
		{
			if (line.find(" id") != std::string::npos) 
			{
				std::string c = getStrBetweenTwoStr(line, "id=\"", "\" ");
				c = getStrBetweenTwoStr(c, "", "\"");
				idStrings.push_back(c);
				std::cout << c << std::endl;
				currentOperation.push_back(getStrBetweenTwoStr(line, "<", " id"));
				std::cout << std::string(currentOperation.size() * 6, spaceChar) << "CURRENT OPERATION: " << currentOperation.back() << std::endl;
				continue;
			}

			if (line.find("<addition>") != std::string::npos || line.find("<multiplication>") != std::string::npos || 
				line.find("<division>") != std::string::npos || line.find("<subtraction>") != std::string::npos)
			{
				currentOperation.push_back(getStrBetweenTwoStr(line, "<", ">"));
				std::cout << std::string(currentOperation.size() * 6, spaceChar) << "CURRENT NESTED OPERATION: " << currentOperation.back() << std::endl;
				continue;
			}

			if (currentOperation.size() > 0 && line.find("</" + currentOperation.back() + ">") != std::string::npos)
			{
				std::cout << std::string(currentOperation.size() * 6, spaceChar) << "END OF OPERATION: " << currentOperation.back() << std::endl;
				if(tmpValues.size() > 1)
					calculateValue(tmpValues[currentOperation.size() - 1], tmpValues[currentOperation.size() - 2], currentOperation.back());
				else
					calculateValue(tmpValues[currentOperation.size() - 1], values, currentOperation.back());
				currentOperation.pop_back();
				tmpValues.pop_back();
				continue;
			}

			if (currentOperation.size() > 0)
			{
				std::string c = getStrBetweenTwoStr(line, ">", "</");
				if (c != "")
				{
					std::cout << std::string((currentOperation.size() + 1) * 6, spaceChar) << "FOUND VALUE: " << c << std::endl;
					if (tmpValues.size() < currentOperation.size())
					{
						while(tmpValues.size() < currentOperation.size())
							tmpValues.push_back(std::vector<int>());
					}
					tmpValues[currentOperation.size() - 1].push_back(std::stol(c));
				}	
			}
		}
	}
	writeToResultFile(idStrings, values);
	inputFile.close();
	std::cout << "END" << std::endl;
	return 0;
}