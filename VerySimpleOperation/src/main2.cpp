#include "Parser.hpp"
#include <iostream>

class IntParser final : public Parser<int>
{
public:
	IntParser()
	{
		RegisterUnaryOperator("sqrt", [](int const& a) {return sqrt(a); });
		RegisterBinaryOperator("^", [](int const& a, int const& b) {return pow(a, b); });
		RegisterBinaryOperator("mod", [](int const& a, int const& b) {return a%b; });
		RegisterBinaryOperator("%", [](int const& a, int const& b) {return a%b; });
		RegisterBinaryOperator("*", [](int const& a, int const& b) {return a*b; });
		RegisterBinaryOperator("/", [](int const& a, int const& b) {return a / b; });
		RegisterBinaryOperator("+", [](int const& a, int const& b) {return a + b; });
		RegisterWrappingOperator("(", ")", [&](std::string& inputString, int& output)
		{
			return Parse(inputString, output, Infinite);
		});
	}
protected:
	std::pair<std::pair<int, int>, std::pair<int, int>> DiscoverInput(int index, std::string& in, std::string& operation) override
	{
		//TODO: ADD HANDLING FOR MULTI_CHAR OPERATORS
		int leftIndex = index - 1;
		int rightIndex = index + operation.length();
		int right;
		std::istringstream(in.substr(index + operation.length())) >> right;
		const auto isNumber = [](char c)
		{
			return c >= '0' && c <= '9' || c == '-';
		};
		while (rightIndex < in.length() - 1 && isNumber(in[rightIndex])) { rightIndex++; }
		while (leftIndex > 0 && isNumber(in[leftIndex])) { leftIndex--; }
		if (leftIndex < 0 || !isNumber(in[leftIndex])) ++leftIndex;
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
class DoubleParser final : public Parser<double>
{
public:
	DoubleParser()
	{
		RegisterUnaryOperator("round", [](double const& a) {return round(a); });
		RegisterUnaryOperator("ceil", [](double const& a) {return ceil(a); });
		RegisterUnaryOperator("floor", [](double const& a) {return floor(a); });
		RegisterUnaryOperator("csch", [](double const& a) {return 1 / sinh(a); });
		RegisterUnaryOperator("sech", [](double const& a) {return 1 / cosh(a); });
		RegisterUnaryOperator("coth", [](double const& a) {return 1 / tanh(a); });
		RegisterUnaryOperator("sinh", [](double const& a) {return sinh(a); });
		RegisterUnaryOperator("cosh", [](double const& a) {return cosh(a); });
		RegisterUnaryOperator("tanh", [](double const& a) {return tanh(a); });
		RegisterUnaryOperator("acsc", [](double const& a) {return 1 / asin(a); });
		RegisterUnaryOperator("asec", [](double const& a) {return 1 / acos(a); });
		RegisterUnaryOperator("acot", [](double const& a) {return 1 / atan(a); });
		RegisterUnaryOperator("csc", [](double const& a) {return 1 / sin(a); });
		RegisterUnaryOperator("sec", [](double const& a) {return 1 / cos(a); });
		RegisterUnaryOperator("cot", [](double const& a) {return 1 / tan(a); });
		RegisterUnaryOperator("asin", [](double const& a) {return asin(a); });
		RegisterUnaryOperator("acos", [](double const& a) {return acos(a); });
		RegisterUnaryOperator("atan", [](double const& a) {return atan(a); });
		RegisterUnaryOperator("sin", [](double const& a) {return sin(a); });
		RegisterUnaryOperator("cos", [](double const& a) {return cos(a); });
		RegisterUnaryOperator("tan", [](double const& a) {return tan(a); });
		RegisterUnaryOperator("sqrt", [](double const& a) {return sqrt(a); });
		RegisterBinaryOperator("^", [](double const& a, double const& b) {return pow(a, b); });
		RegisterBinaryOperator("*", [](double const& a, double const& b) {return a*b; });
		RegisterBinaryOperator("/", [](double const& a, double const& b) {return a / b; });
		RegisterBinaryOperator("+", [](double const& a, double const& b) {return a + b; });
		RegisterWrappingOperator("(", ")", [&](std::string& inputString, double& output)
		{
			return Parse(inputString, output, Infinite);
		});
	}
protected:
	std::pair<std::pair<double, double>, std::pair<int, int>> DiscoverInput(int index, std::string& in, std::string& operation) override
	{
		//TODO: ADD HANDLING FOR MULTI_CHAR OPERATORS
		int leftIndex = index - 1;
		int rightIndex = index + operation.length();
		double right;
		std::istringstream(in.substr(index + operation.length())) >> right;
		const auto isNumber = [](char c)
		{
			return c >= '0' && c <= '9' || c == '-';
		};
		while (rightIndex < in.length() - 1 && isNumber(in[rightIndex])) { rightIndex++; }
		while (leftIndex > 0 && isNumber(in[leftIndex])) { leftIndex--; }
		if (leftIndex < 0 || !isNumber(in[leftIndex])) ++leftIndex;
		if (!isNumber(in[rightIndex])) --rightIndex;
		double left;
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
	bool Parse(std::string& inputString, double& output, int const steps = Infinite) override
	{
		auto result = DoParse(inputString, output, steps);
		CheckFunction(inputString);
		return result;
	}
	~DoubleParser() = default;
};
int main()
{
	DoubleParser parser;
	std::string input = "sqrt(100^2+3)";
	double output;
	do
	{
		std::cout << input << std::endl;
	} while (parser.Parse(input, output, IntParser::End) == IntParser::Working);
}