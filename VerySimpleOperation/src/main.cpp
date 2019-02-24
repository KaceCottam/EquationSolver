#include "Parser.hpp"
#include <iostream>
#include "Matrix.h"

using std::string;

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
			return c >= '0' && c <= '9' || c == '.';
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
	IntParser parser;
	std::string input = "5*4-(30/2)^2+1992/7";
	int output;
	do
	{
		std::cout << input << std::endl;
	} while (parser.Parse(input, output, IntParser::End) == IntParser::Working);

	//string test = "[{1,2,3},{3,2,1},{5,6,7}]";
	//Matrix m;

	//m.setData(test);
	////m.displayData();

	////cout << m.getNumRows() << "x" << m.getNumCols();

	//Matrix n(m);
	////n.displayData();

	//m.add(n);
	//m.displayData();

	//Matrix o(scalarMult(2, m));
	//o.displayData();

	/*string test1 = "[{1,2},{3,4},{5,6}]";
	string test2 = "[{1,2,3,7},{4,5,6,8}]";*/

	string test1 = "[{0,-1,2},{4,11,2}]";
	string test2 = "[{3,-1},{1,2},{6,1}]";
	string test3 = "[{1,3,5,9},{1,3,1,7},{4,3,9,7},{5,2,0,9}]";

	Matrix mat1;
	mat1.setData(test3);

	cout << "determinant: " << calculateDeterminant(4, mat1);

	/*Matrix mat1;
	mat1.setData(test1);

	Matrix mat2;
	mat2.setData(test2);

	Matrix mat3(crossProduct(mat1, mat2));

	mat1.displayData();
	cout << " * ";
	mat2.displayData();
	cout << " = ";
	mat3.displayData();*/

	//string str;
	//if (argc > 1)
	//{
	//	str = argv[2];
	//}
	//else
	//{
	//	str = "1*4+(1*(2-62)+100)^2-30/(2^3+2*5)+100/20-30+60*2-5*100-(50-34*23)+70";
	//}
	//// BUG: parenthesis at beginning of string causes error
	//// BUG: minus sign at beginning of string causes error
	//int output;
	//std::cout << str << std::endl;
	//while (Parse(str, output, 1) == 0)
	//{
	//	std::cout << str << std::endl;
	//}

	return 0;

}
