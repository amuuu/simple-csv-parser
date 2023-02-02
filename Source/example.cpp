
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

		Parser<std::string, int> parser{ "Source/simple.csv", settings };
		
		parser.Parse();

		std::string a{};
		int b{};
		
		for (int r = 0; r < parser.GetRowCount(); r++)
		{
			parser.GetRowData<std::string, int>(r, a, b);
			std::cout << "a: " << a << " b: " << b << std::endl;
		}
	}

	
	{
		ParserSettings settings{};
		settings.ignoredFirstRowsCount = 1;
		
		Parser<std::string, float, std::string, float, float, float, std::string, float, float> parser{ "Source/actions.csv", settings };
		
		parser.Parse();

		std::string a{}; float b{}; std::string c{}; float d{}; float e{}; float f{}; std::string g{}; float h{}; float i{};
		for (int r = 0; r < parser.GetRowCount(); r++)
		{
			parser.GetRowData<std::string, float, std::string, float, float, float, std::string, float, float>(r, a, b, c, d, e, f, g, h, i);
			std::cout << "a: " << a << " b: " << b << " c: " << c << " d: " << d << " e: " << e << " f: " << f << " g: " << g << " h: " << h << " i: " << i << std::endl;
		}
	}
	
}
