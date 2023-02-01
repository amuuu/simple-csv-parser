#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <queue>
#include <typeinfo>
#include <functional>
#include <map>
#include <iostream>
#include <any>
#include <utility>
#include <type_traits>
#include <cassert>

// todo: handle special cases containing ' and " ; i don't need them at the moment so it's not handled yet
// todo: support utf-8


namespace CSVParser
{

	static std::string EMPTY_STRING{ "" };

	struct ParserSettings
	{
	public:

		unsigned int numColumns{ 0 };
		unsigned int ignoredFirstRowsCount{ 0 };
		
		template<typename TargetType>
		void SetConvertorMethod(std::function<TargetType(const std::string&)> convertor)
		{
			convertors.push_back(convertor);
		}

		template<typename TargetType>
		TargetType CallConvertorMethod(const std::string& token)
		{
			using CType = std::function<TargetType(const std::string&)>;

			for (const auto& convertor : convertors)
			{
				if (convertor.type() == typeid(CType))
				{
					return std::any_cast<CType>(convertor)(token);
				}
			}

			return ParserSettings::CallDefaultConvertorMethod<TargetType>(token);
		}

	private:

		template<typename TargetType>
		static inline TargetType CallDefaultConvertorMethod(const std::string& token)
		{
			if constexpr (std::is_same_v<TargetType, int>)
			{
				return std::stoi(token);
			}
			else if constexpr (std::is_same_v<TargetType, float>)
			{
				return std::stof(token);
			}
			else if constexpr (std::is_same_v<TargetType, std::string>)
			{
				return token;
			}
			else
			{
				std::cout << "No default convertor for the type of " << typeid(TargetType).name << std::endl;
				exit(-1);
			}
		}

		std::vector<std::any> convertors{};
	};

	template<typename T, typename...TRest>
	class Parser
	{
	public:

		// tuples need a bit of work as a solution in this case i think
		// using Data = std::tuple<T, TRest...>;
		// template <std::size_t N>
		// using DataIndex = typename std::tuple_element<N, Data>::type;
		using RowData = std::map<unsigned int, std::any>;

		Parser(const std::string& dir, const ParserSettings& settings)
		{
			this->dir = dir;
			this->settings = settings;
		}

		void Parse()
		{
			std::ifstream fstream{};
			fstream.open(dir);

			std::string line{};
			unsigned int rowCounter{ 0 };

			while (std::getline(fstream, line))
			{
				if (settings.ignoredFirstRowsCount > rowCounter++)
					continue;

				std::cout << "line -> " << line << std::endl;

				auto tokens = AnalyzeRow(line, settings.numColumns);
				
				RowData thisRowData{};
				ConvertTypes<T, TRest...>(tokens, thisRowData, 0);
				datas.push_back(thisRowData);
			}

			fstream.close();
		}

		template<typename TargetType>
		TargetType GetRowData(int row, int col)
		{
			if (datas[row].find(col) != datas[row].end())
			{
				return std::any_cast<TargetType>(datas[row][col]);
			}
			else
			{
				std::cout << std::endl << "ERROR: row and col with the following type doesn't exist:\n" << typeid(TargetType).name() << std::endl;
				exit(-1);
			}
		}


	private:

		ParserSettings settings{};
		std::vector<RowData> datas{};
		std::string dir{};

		std::queue<std::string> AnalyzeRow(std::string& l, const unsigned int& cols)
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
		void ConvertTypes(std::queue<std::string>& tokens, RowData& data, unsigned int currentCol = 0)
		{
			auto& target = tokens.front();

			if (target != EMPTY_STRING)
			{
				data[currentCol] = settings.CallConvertorMethod<_T>(target);
			}

			if constexpr (sizeof...(_TRest) > 0)
			{
				tokens.pop();
				
				if (!tokens.empty())
					ConvertTypes<_TRest...>(tokens, data, currentCol + 1);
			}
		}

	};
}