#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <queue>
#include <typeinfo>
#include <functional>
#include<map>
#include <iostream>
#include <variant>
#include <any>


// todo: handle special cases containing ' and " ; i don't need them at the moment so it's not handled yet
// todo: support utf-8


namespace CSVParser
{

	static std::string EMPTY_STRING{ "" };

	struct ParserSettings
	{
		unsigned int numColumns;
		unsigned int ignoredFirstRowsCount{ 0 };
		
		/*template<typename TargetType>
		void SetConvertorMethodForType(std::function<TargetType(const std::string&)> convertor)
		{
			this->convertor = convertor;
		}

		template<typename TargetType>
		TargetType CallConvertorMethodForType(const std::string& token)
		{
			return convertor();
		}

		std::any convertor;*/
	};

	template<typename T, typename...TRest>
	class Parser
	{
	public:

		struct Data
		{

		};

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

				auto tokens = AnalyzeLine(line, settings.numColumns);
				ConvertTypes<T, TRest...>(tokens);
			}

			fstream.close();
		}

	private:

		std::queue<std::string> AnalyzeLine(std::string& l, const unsigned int& cols)
		{
			std::queue<std::string> result{};
			std::stringstream ss{ l };
			std::string item{};

			while (std::getline(ss, item, ','))
			{
				//std::cout << "t: " << item << std::endl;
				result.push(item);
			}	

			return result;
		}

		template<typename _T, typename..._TRest>
		void ConvertTypes(std::queue<std::string>& tokens)
		{
			auto& target = tokens.front();

			if (target != EMPTY_STRING)
			{
				/*settings.CallConvertorMethodForType<T>(target);*/
				
				// todo: generalize

				if (typeid(_T) == typeid(int))
				{
					std::cout << "int: " << std::stoi(target) << std::endl;
				}
				else if (typeid(_T) == typeid(float))
				{
					std::cout << "float: " << std::stof(target) << std::endl;
				}
				else if (typeid(_T) == typeid(std::string))
				{
					std::cout << "string: " << target << std::endl;
				}
				else
				{
					std::cout << "ERROR" << std::endl;
				}
			}

			if constexpr (sizeof...(_TRest) > 0)
			{
				tokens.pop();
				
				if (!tokens.empty())
					ConvertTypes<_TRest...>(tokens);
			}
		}

	};
}