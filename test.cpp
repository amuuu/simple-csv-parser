
#include "csvparser.h"

#include <iostream>
using namespace CSVParser;

int main()
{
	
	/*{
		ParserSettings settings{};
		settings.ignoredFirstRowsCount = 2;
		//settings.SetConvertorMethodForType<int>([](const std::string& token) { return 12; });
		parser.Parse<std::string, int>("example-csvs/simple.csv", settings);
	}*/

	{
		Parser<std::string, float, std::string, float, float, float, std::string, float, float> parser{};
		
		ParserSettings settings{};
		settings.ignoredFirstRowsCount = 1;
		
		parser.Parse("example-csvs/actions.csv", settings);
	}
}
