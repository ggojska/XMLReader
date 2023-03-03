#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "rapidxml-1.13/rapidxml.hpp"

using namespace rapidxml;

std::string createFileName(std::string name)
{
	unsigned first_delim_pos = name.find("");
	unsigned end_pos_of_first_delim = first_delim_pos;
	unsigned last_delim_pos = name.find(".xml");

	return name.substr(end_pos_of_first_delim, last_delim_pos - end_pos_of_first_delim) + "_result.xml";
}


xml_document<> doc;
xml_node<>* root_node = NULL;

void calculateValues(std::vector<int>& v, std::vector<int>& final, std::string type)
{
	if (type == "addition")
	{
		int tmpVal = 0;
		for (auto a : v)
		{
			tmpVal += a;
		}
		final.push_back(tmpVal);
	}
	else if (type == "multiplication")
	{
		int tmpVal = 1;
		for (auto m : v)
		{
			tmpVal *= m;
		}
		final.push_back(tmpVal);
	}
	else if (type == "division")
	{
		final.push_back(v[0] / v[1]);
	}
	else if (type == "subtraction")
	{
		final.push_back(v[0] - v[1]);
	}
	else
	{
		std::cout << "ERROR: Unknow currentOperation type";
	}
}

void writeToResultFile(std::vector<int>& values, std::vector<std::string> & idStrings, std::string fileName)
{

	if (idStrings.size() != values.size())
	{
		std::cout << "ERROR - idStrings size not equal to values size." << "( " << idStrings.size() << " != " << values.size() << " )" << std::endl;
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
	for (auto i = 0; i <  idStrings.size(); i++)
	{
		resultsFile << "	<result id=\"" <<  idStrings[i] << "\">" <<  values[i] << "</result>";
		resultsFile << '\n';
	}
	resultsFile << "</expressions>";

	resultsFile.close();
}

int main(void)
{
	std::vector<int> finalValues;
	std::vector<std::string> idValues;
    // Read the sample.xml file
	std::string fileName = "test2.xml";
	std::ifstream theFile(fileName);
    std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
    buffer.push_back('\0');

    // Parse the buffer
    doc.parse<0>(&buffer[0]);

    // Find out the root node
    root_node = doc.first_node("expressions");
    // Iterate over the nodes
    for (xml_node<>* operation_node = root_node->first_node();
        operation_node; operation_node = operation_node->next_sibling())
    {
		std::cout << "Operation type = " << operation_node->name();
		std::cout << ", id =   " << operation_node->first_attribute()->value() << std::endl;

		idValues.push_back(operation_node->first_attribute()->value());

        std::vector<int> values;

        // Interate over the operation arguments
        for (xml_node<>* operation_argument_node = operation_node->first_node();
            operation_argument_node; operation_argument_node = operation_argument_node->next_sibling())
        {
			std::cout << "	" << operation_argument_node->name() << std::endl;
			std::cout << "	" << operation_argument_node->first_node()->name() << std::endl;
			if ((std::string)operation_argument_node->first_node()->name() != "addition" && (std::string)operation_argument_node->first_node()->name() != "subtraction" &&
				(std::string)operation_argument_node->first_node()->name() != "multiplication" && (std::string)operation_argument_node->first_node()->name() != "division")
			{
				std::cout << "		Value =   " << operation_argument_node->value() << std::endl;
				values.push_back(std::stoi(operation_argument_node->value()));
			}
			else
			{
				std::vector<int> nestedValues;
				for (xml_node<>* nested_operation_argument_node = operation_argument_node->first_node()->first_node();
					nested_operation_argument_node; nested_operation_argument_node = nested_operation_argument_node->next_sibling())
				{
					std::cout << "			Value x=   " << nested_operation_argument_node->value() << std::endl;
					nestedValues.push_back(std::stoi(nested_operation_argument_node->value()));
				}
				calculateValues(nestedValues, values, operation_argument_node->first_node()->name());
			}
        }
        calculateValues(values, finalValues, operation_node->name());
		std::cout << std::endl;
    }
	writeToResultFile(finalValues, idValues, fileName);
    return 0;
}