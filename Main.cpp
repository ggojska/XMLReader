#include "XMLReader.h"

int main()
{
	XMLReader reader("test.xml", false);
	XMLReader reader2("test.xml", true);
	reader.parseXML();
	reader2.parseXML();
	return 0;
}
