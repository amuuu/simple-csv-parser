
#include "csvparser.h"

#include <iostream>
using namespace CSVParser;

int main()
{
	Parser parser{};
	
	/*{
		ParserSettings settings{};
		settings.ignoredFirstRowsCount = 2;
		//settings.SetConvertorMethodForType<int>([](const std::string& token) { return 12; });
		parser.Parse<std::string, int>("example-csvs/simple.csv", settings);
	}*/

	{
		ParserSettings settings{};
		settings.ignoredFirstRowsCount = 1;
		using str = std::string; using f = float;
		parser.Parse<str,f,str,f,f,f,str,f,f>("example-csvs/actions.csv", settings);
	}
}
