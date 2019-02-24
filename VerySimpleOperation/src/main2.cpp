#include "Parser.hpp"
#include <iostream>

class IntParser final : public Parser<int>
{
public:
	IntParser()
	{
		RegisterBinaryOperator("^", [](int const& a, int const& b) {return pow(a, b); });
		RegisterBinaryOperator("%", [](int const& a, int const& b) {return a%b; });
		RegisterBinaryOperator("*", [](int const& a, int const& b) {return a*b; });
		RegisterBinaryOperator("/", [](int const& a, int const& b) {return a / b; });
		RegisterBinaryOperator("+", [](int const& a, int const& b) {return a + b; });
		//RegisterBinaryOperator("-", [](int const& a, int const& b) {return a - b; });
		RegisterWrappingOperator("(", ")", [&](std::string& inputString, int& output)
		{
			return Parse(inputString, output, Infinite);
		});
	}
protected:
	std::pair<std::pair<int, int>, std::pair<int, int>> DiscoverInput(int index, std::string& in) override
	{
		//TODO: ADD HANDLING FOR MULTI_CHAR OPERATORS
		int leftIndex = index - 1;
		int rightIndex = index + 1;
		int right;
		std::istringstream(in.substr(index + 1)) >> right;
		const auto isNumber = [](char c)
		{
			return c >= '0' && c <= '9' || c == '-';
		};
		while (rightIndex < in.length() - 1 && isNumber(in[rightIndex])) { rightIndex++; }
		while (leftIndex > 0 && isNumber(in[leftIndex])) { leftIndex--; }
		if (!isNumber(in[leftIndex])) ++leftIndex;
		if (!isNumber(in[rightIndex])) --rightIndex;
		int left;
		std::istringstream(in.substr(leftIndex)) >> left;
		return std::make_pair(std::make_pair(left, right), std::make_pair(leftIndex, rightIndex));
	}

	int CheckFunction(std::string& in) override
	{
		const auto isNumber = [](char c)
		{
			return c >= '0' && c <= '9';
		};
		for (auto i = 0; i < in.length(); i++)
		{
			if (in[i] == '-')
			{
				if (i - 1 > 0)
					if (isNumber(in[i - 1]))
					{
						in.insert(i++, std::string("+"), 0);
					}
			}
		}

		return Working;
	}

public:
	bool Parse(std::string& inputString, int& output, int const steps = Infinite) override
	{
		auto result = DoParse(inputString, output, steps);
		CheckFunction(inputString);
		return result;
	}
	~IntParser() = default;
};

int main()
{
	IntParser parser;
	std::string input = "5*((35+40)/5)^2-(30/5+90*12/6)*20%3";
	int output;
	do
	{
		std::cout << input << std::endl;
	} while (parser.Parse(input, output, IntParser::End) == IntParser::Working);
}