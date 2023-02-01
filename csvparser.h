#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <array>
#include <sstream>

#include <iostream>
#include <stack>

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

			fstream.close();
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

		void PreprocessLine(std::string& l, const unsigned int& cols)
		{
			for(int c = 0; c < l.size(); c++)
			{
				if (l[c] == '\'')
				{
					bool nextSQ = ((c + 1 <= l.size() - 1) && l[c + 1] == '\'');
					bool prevSQ = ((c - 1 >= 0) && l[c - 1] == '\'');

					if (!nextSQ && !prevSQ)
					{
						l[c] = '@';
					}
				}
			}
		}

		void AnalyzeLine(std::string& l, const unsigned int& cols)
		{
			PreprocessLine(l, cols);
			ExtractTokens(l, cols);
		}
	};
}