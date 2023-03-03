#include "XMLReader.h"
#include <string>
#include <fstream>
#include <vector>
#include <iostream>

XMLReader::XMLReader(std::string s) : fileName{s}, debugMode{ false }
{
	this->inputFile.open(fileName, std::ios::in);
}

XMLReader::XMLReader(std::string s, bool d) : fileName{ s }, debugMode{ d }
{
	this->inputFile.open(fileName, std::ios::in);
}

XMLReader::~XMLReader()
{
	this->inputFile.close();
}

std::string XMLReader::getStrBetweenTwoStr(const std::string& s, const std::string& start_delim, const std::string& stop_delim)
{
	unsigned first_delim_pos = s.find(start_delim);
	unsigned end_pos_of_first_delim = first_delim_pos + start_delim.length();
	unsigned last_delim_pos = s.find(stop_delim);

	return s.substr(end_pos_of_first_delim, last_delim_pos - end_pos_of_first_delim);
}

std::string XMLReader::createFileName(std::string name)
{
	std::string s = getStrBetweenTwoStr(name, "", ".xml");
	return s + "_result.xml";
}

void XMLReader::writeToResultFile()
{

	if (this->idStrings.size() != this->values.size())
	{
		if(this->debugMode) std::cout << "ERROR - idStrings size not equal to values size." << "( " << this->idStrings.size() << " != " << this->values.size() << " )" << std::endl;
		for (auto v : this->values)
		{
			if(this->debugMode) std::cout << v << ", ";
		}
		return;
	}

	std::ofstream resultsFile;
	resultsFile.open(createFileName(this->fileName), std::ofstream::out | std::ofstream::trunc);

	resultsFile << "<expressions>";
	resultsFile << '\n';
	for (auto i = 0; i < this->idStrings.size(); i++)
	{
		resultsFile << "	<result id=\"" << this->idStrings[i] << "\">" << this->values[i] << "</result>";
		resultsFile << '\n';
	}
	resultsFile << "</expressions>";

	resultsFile.close();
}

void XMLReader::calculateValue(std::vector<int>& tmp, std::vector<int>& final, std::string currentOperation)
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
		if(this->debugMode) std::cout << "ERROR: Unknow currentOperation type";
	}
}

void XMLReader::calculateValue(std::vector<int>& tmp, std::string currentOperation)
{
	if (currentOperation == "addition")
	{
		int tmpVal = 0;
		for (auto a : tmp)
		{
			tmpVal += a;
		}
		this->values.push_back(tmpVal);
	}
	else if (currentOperation == "multiplication")
	{
		int tmpVal = 1;
		for (auto m : tmp)
		{
			tmpVal *= m;
		}
		this->values.push_back(tmpVal);
	}
	else if (currentOperation == "division")
	{
		this->values.push_back(tmp[0] / tmp[1]);
	}
	else if (currentOperation == "subtraction")
	{
		this->values.push_back(tmp[0] - tmp[1]);
	}
	else
	{
		if(this->debugMode) std::cout << "ERROR: Unknow currentOperation type";
	}
}

void XMLReader::parseXML()
{
	std::vector<std::vector<int>> tmpValues;
	std::vector<std::string> currentOperation;

	if (inputFile)
	{
		if(this->debugMode) std::cout << "BEGIN" << std::endl;

		std::string line;
		while (std::getline(inputFile, line))
		{
			if (line.find(" id") != std::string::npos)
			{
				std::string c = getStrBetweenTwoStr(line, "id=\"", "\" ");
				c = getStrBetweenTwoStr(c, "", "\"");
				this->idStrings.push_back(c);
				if(this->debugMode) std::cout << "Operation ID: " << c << std::endl;
				currentOperation.push_back(getStrBetweenTwoStr(line, "<", " id"));
				if(this->debugMode) std::cout << std::string(currentOperation.size() * 6, ' ') << "CURRENT OPERATION: " << currentOperation.back() << std::endl;
				continue;
			}

			if (line.find("<addition>") != std::string::npos || line.find("<multiplication>") != std::string::npos ||
				line.find("<division>") != std::string::npos || line.find("<subtraction>") != std::string::npos)
			{
				currentOperation.push_back(getStrBetweenTwoStr(line, "<", ">"));
				if(this->debugMode) std::cout << std::string(currentOperation.size() * 6, ' ') << "CURRENT NESTED OPERATION: " << currentOperation.back() << std::endl;
				continue;
			}

			if (currentOperation.size() > 0 && line.find("</" + currentOperation.back() + ">") != std::string::npos)
			{
				if(this->debugMode) std::cout << std::string(currentOperation.size() * 6, ' ') << "END OF OPERATION: " << currentOperation.back() << std::endl;
				if (tmpValues.size() > 1)
					this->calculateValue(tmpValues[currentOperation.size() - 1], tmpValues[currentOperation.size() - 2], currentOperation.back());
				else
					this->calculateValue(tmpValues[currentOperation.size() - 1], currentOperation.back());
				currentOperation.pop_back();
				tmpValues.pop_back();
				continue;
			}

			if (currentOperation.size() > 0)
			{
				std::string c = getStrBetweenTwoStr(line, ">", "</");
				if (c != "")
				{
					if(this->debugMode) std::cout << std::string((currentOperation.size() + 1) * 6, ' ') << "FOUND VALUE: " << c << std::endl;
					if (tmpValues.size() < currentOperation.size())
					{
						while (tmpValues.size() < currentOperation.size())
							tmpValues.push_back(std::vector<int>());
					}
					tmpValues[currentOperation.size() - 1].push_back(std::stol(c));
				}
			}
		}
		if(this->debugMode) std::cout << "END" << std::endl << std::endl << std::endl;
	}
	this->writeToResultFile();
}
