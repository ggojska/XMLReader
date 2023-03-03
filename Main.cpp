#include "XMLReader.h"

int main()
{
	XMLReader reader("test.xml", false);
	XMLReader reader2("test2.xml", true);
	reader.parseXML();
	reader2.parseXML();
	return 0;
}
