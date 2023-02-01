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

			return CallDefaultConvertorMethod<TargetType>(token);
		}

	private:

		template<typename TargetType>
		static inline TargetType CallDefaultConvertorMethod(const std::string& token)
		{
			//const auto targetTypeID = typeid(TargetType);

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
				std::cout << "No default convertor found for type of " << typeid(TargetType).name() << std::endl;
			}
		}

		std::vector<std::any> convertors{};
	};

	template<typename T, typename...TRest>
	class Parser
	{
	public:

		using Data = std::tuple<T, TRest...>;
		
		template <std::size_t N>
		using DataIndex = typename std::tuple_element<N, Data>::type;
		
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

				std::cout << std::endl << line << std::endl;

				auto tokens = AnalyzeLine(line, settings.numColumns);
				
				Data thisLineData{};
				std::size_t index{ 0 };
				ConvertTypes<T, TRest...>(tokens, thisLineData, index); // todo: save converted data somehow using tuple or std::any
				//datas.push_back(thisLineData);
			}

			fstream.close();
		}


	private:

		ParserSettings settings{};
		std::vector<Data> datas{};
		std::string dir{};

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
		void ConvertTypes(std::queue<std::string>& tokens, Data& data, std::size_t index)
		{
			auto& target = tokens.front();

			if (target != EMPTY_STRING)
			{
				std::cout << settings.CallConvertorMethod<_T>(target);
			}

			if constexpr (sizeof...(_TRest) > 0)
			{
				tokens.pop();
				
				if (!tokens.empty())
					ConvertTypes<_TRest...>(tokens, data, index+1);
			}
		}

	};
}