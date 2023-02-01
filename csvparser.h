#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <array>
#include <sstream>

#include <iostream>
#include <stack>


// todo: handle special cases containing ' and " ; i don't need them at the moment so it's not handled yet


namespace CSVParser
{
	struct ParserSettings
	{
		unsigned int numColumns;
		unsigned int ignoredFirstRowsCount{ 0 };
	};

	class Parser
	{
	public:

		/*template<typename T, typename...TRest>*/
		void Parse(const std::string& dir, const ParserSettings& settings = {})
		{
			std::ifstream fstream{};
			fstream.open(dir);

			std::string line{};
			unsigned int rowCounter{ 0 };

			while (std::getline(fstream, line))
			{
				if (settings.ignoredFirstRowsCount > rowCounter++)
					continue;

				std::cout << std::endl << line << std::endl;

				AnalyzeLine(line, settings.numColumns);
			}

			/*if constexpr (sizeof...(rest) > 0)
			{
				args->next = new Args{};
				::PackArgs(args->next, rest...);
			}
			else
			{
				fstream.close();
			}*/
		}

	private:

		void ExtractTokens(std::string& l, const unsigned int& cols)
		{
			std::vector<std::string> result{};
			std::stringstream ss{ l };
			std::string item{};

			while (std::getline(ss, item, ',')) 
			{
				result.push_back(item);
			}

			for (const auto& t : result)
				std::cout << "t: " << t << std::endl;
		}

		void AnalyzeLine(std::string& l, const unsigned int& cols)
		{
			ExtractTokens(l, cols);
		}

	};
}