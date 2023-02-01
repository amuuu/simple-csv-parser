
#include "csvparser.h"

#include <iostream>
using namespace CSVParser;

int main()
{
	Parser parser{};
	ParserSettings settings{};
	
	settings.ignoredFirstRowsCount = 2;
	settings.ignoredFirstRowsCount = 2;

	parser.Parse<std::string, int>("example-csvs/simple.csv", settings);
}
