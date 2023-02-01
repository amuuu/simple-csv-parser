
#include "csvparser.h"

#include <iostream>
using namespace CSVParser;

int main()
{
	
	///*
	{
		ParserSettings settings{};
		settings.ignoredFirstRowsCount = 2;
		settings.SetConvertorMethod<int>([](const std::string& token) { return std::stoi(token); });
		settings.SetConvertorMethod<float>([](const std::string& token) { return std::stof(token); });
		settings.SetConvertorMethod<std::string>([](const std::string& token) { return token; });

		Parser<std::string, int> parser{ "example-csvs/simple.csv", settings };
		parser.Parse();
	}
	//*/

	/*
	{
		Parser<std::string, float, std::string, float, float, float, std::string, float, float> parser{};
		
		ParserSettings settings{};
		settings.ignoredFirstRowsCount = 1;
		
		parser.Parse("example-csvs/actions.csv", settings);
	}
	*/
}
