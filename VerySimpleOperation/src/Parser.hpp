/*
 * Author: Kace Cottam, Maia Whitley
 * Section: CPTS 122, CCHackathon
 * Date: Sunday, February 24, 2019
 * Description:
 * 	This file contains a base class called "Parser" which handles parsing a string, 
 * 	given the programmer overrides the pure virtual functions within.
 */

#pragma once
#include <string>
#include <unordered_map>
#include <functional>
#include <sstream>

 /*
  * The template is of any type. When inheriting this class, use `class NAME : public Parser<TYPE>`.
  * Integers, for example, would be something like `class IntParser : public Parser<int>`
  */
template<class T>
class Parser

{
public:
	Parser() = default;
	virtual ~Parser() = default;
protected:
	/*
	 * This is used purely for unordered_map for pairs.
	 */
	struct pair_hash {
		template <class T1, class T2>
		std::size_t operator () (const std::pair<T1, T2> &p) const {
			auto h1 = std::hash<T1>{}(p.first);
			auto h2 = std::hash<T2>{}(p.second);

			return h1 ^ h2;
		}
	};

	std::unordered_map<std::pair<std::string, std::string>, std::function<bool(std::string&, T&)>, pair_hash> WrappingOperators;
	std::unordered_map<std::string, std::function<T(T&)>> UnaryOperators;
	std::unordered_map<std::string, std::function<T(T&, T&)>> BinaryOperators;

	/*
	 * This is a function that needs to be overridden. It is the algorithm your parser will use to find the correct data.
	 * OUTPUT: {{data left of operator, data right of operator},{int leftIndex of whole discovery,int rightIndex of whole discovery}}
	 * INPUTS:
	 * 	index: index of the operator
	 * 	in: the inputString
	 * 	operation: a string showing what operation was found. It is recommended that you use length of this string to offset the start of the rightIndex.
	 */
	virtual std::pair<std::pair<T, T>, std::pair<int, int>> DiscoverInput(int index, std::string& in, std::string& operation) = 0;
	/*
	 * This is a function that is called to check and reformat the inputString, in.
	 */
	virtual int CheckFunction(std::string& in) = 0;
	/*
	 * This function does the heavy lifting. It evaluates a string.
	 * To do this, it searches for operators that are registered.
	 * It goes by this order: WrappingOperator, UnaryOperator, BinaryOperator.
	 *
	 * INPUTS:
	 * 	inputString: The string you want to input. This cannot be a string literal as it is modified.
	 * 	output: This is the final number. If there are wrapping operators, this will be set by their functions. Do not trust it!
	 * 	steps: This is the number of steps the equation will do. Inside the class, use the integer enum to set this, or any number above 0.
	 */
	bool DoParse(std::string& inputString, T& output, int const steps = Infinite)
	{
		switch (steps)
		{
		case Infinite:
		{
			while (DoParse(inputString, output, End) == Working) {}
			std::istringstream(inputString) >> output;
			return Finished;
		}
		default: // fixed number of steps
		{
			for (auto i = 0; i < steps && DoParse(inputString, output, End) == Working; i++) {}
			std::istringstream(inputString) >> output;
			return Finished;
		}
		case End:
		{
			for (auto& i : WrappingOperators)
			{
				auto key = i.first;
				auto evaluate = i.second;
				//Find Wrapping Operators
				int positionOfWrappingOperatorBegin = inputString.find_last_of(key.first);
				if (positionOfWrappingOperatorBegin != std::string::npos)
				{
					auto trimString = [](std::string& input)
					{
						input = input.c_str();
						return input;
					};
					auto min = [](T const& num, T const& min)
					{
						return (int)(num < min ? min : num);
					};
					auto max = [](T const&num, T const& max)
					{
						return (int)(num > max ? max : num);
					};

					auto inputCopy = std::string(&(inputString[positionOfWrappingOperatorBegin + 1]));
					int positionofWrappingOperatorEnd = inputCopy.find_first_of(key.second);
					if (positionofWrappingOperatorEnd == std::string::npos)
					{
						throw "Broke!";
					}
					inputCopy[positionofWrappingOperatorEnd] = '\0';
					trimString(inputCopy);
					int lengthOfReplacementString = max(positionofWrappingOperatorEnd + 1, inputString.length());
					std::string beforeEvaluate = inputString.substr(
						min(positionOfWrappingOperatorBegin, 0),
						lengthOfReplacementString + 1
					);
					auto evaluationResult = evaluate(inputCopy, output);

					auto findResult = inputString.find(beforeEvaluate);

					if (findResult == std::string::npos)
					{
						throw std::out_of_range("Wrapping broke!");
					}

					if (evaluationResult == Finished)
					{
						inputString.replace(positionOfWrappingOperatorBegin, lengthOfReplacementString + 1, inputCopy, 0);
					}
					else
					{
						inputString.replace(positionOfWrappingOperatorBegin, lengthOfReplacementString + 1, inputCopy, 0);
					}

					return Working;
				}
			}

			//if none, find operators (unary, then binary)
			auto discover = [](std::string& in, std::string& srch)
			{
				auto const result = in.find(srch);
				if (result != std::string::npos)
					return result;
				return std::string::npos;
			};

			enum DiscoveryStatus : int
			{
				Undiscovered,
				Unary,
				Binary
			};
			using DataPair = std::pair<T, T>;
			using DataCoords = std::pair<int, int>;
			using DataDetails = std::pair<DataPair, DataCoords>;
			using DiscoveryDetails = std::pair<DiscoveryStatus, std::string>;
			std::pair<DataDetails, DiscoveryDetails> discovery;
			for (auto& i : UnaryOperators)
			{
				auto operatorString = i.first;
				auto operatorFunction = i.second;
				auto discoverIndex = discover(inputString, operatorString);
				if (discoverIndex != std::string::npos)
				{
					discovery.second.first = Unary;
					discovery.second.second = operatorString;
					discovery.first = DiscoverInput(discoverIndex, inputString, operatorString);
				}
			}

			for (auto& i : BinaryOperators)
			{
				auto operatorString = i.first;
				auto operatorFunction = i.second;
				auto discoverIndex = discover(inputString, operatorString);
				if (discoverIndex != std::string::npos)
				{
					discovery.second.first = Binary;
					discovery.second.second = operatorString;
					discovery.first = DiscoverInput(discoverIndex, inputString, operatorString);
					break;
				}
			}

			//if operator found, run discovery function left and right of operator
			if (discovery.second.first != Undiscovered)
			{
				DataPair& data = discovery.first.first;
				DataCoords& coords = discovery.first.second;

				T result;
				switch (discovery.second.first)
				{
				case Unary:
					result = UnaryOperators[discovery.second.second](data.second);
					break;
				case Binary:
					result = BinaryOperators[discovery.second.second](data.first, data.second);
					break;
				}
				std::ostringstream outputString;
				outputString << result;
				inputString.replace(coords.first, coords.second - coords.first + 1, outputString.str(), 0);

				return Working;
			}
			else
			{
				return Finished;
			}
		}
		}
	}
public:
	/*
	 * This registers a unary operator.
	 * inputs is the string that denotes it, the function is the operation that is done.
	 * The function must return T, and it must take in a T reference.
	 * This function is the unary operation you wish to perform. This can be inputted with lambda functions.
	 */
	int RegisterUnaryOperator(std::string const& inputs, std::function<T(T&)> const function)
	{
		UnaryOperators[std::string(inputs)] = function;
		return 1;
	}
	/*
	 * This registers a binary operator. inputs is the string that denotes it, the function is the operation that is done.
	 * The function must return T, and it must take in two T references.
	 * The first reference is the left side of the operator, while the other one is the right.
	 * This function is the binary operation you wish to perform. This can be inputted with lambda functions.
	 */
	int RegisterBinaryOperator(std::string const& inputs, std::function<T(T&, T&)> const function)
	{
		BinaryOperators[std::string(inputs)] = function;
		return 1;
	}
	/*
	 * This registers a wrapping operator.
	 * left is the left side, right is the right side, and the function is what is done.
	 * With parenthesis, you would want them to evaluate first.
	 * Therefore, the function is called with the string inside of parenthesis, with the output as the other parameter.
	 */
	int RegisterWrappingOperator(std::string const& left, std::string const& right, std::function<bool(std::string&, T&)> const function)
	{
		WrappingOperators[{left, right}] = function;
		return 1;
	}

	enum : int
	{
		Infinite = -1,
		End
	};
	enum : bool
	{
		Working = false,
		Finished = true
	};
	/*
	 * This function is overrideable. It is what the programmer calls outside of the class.
	 */
	virtual bool Parse(std::string& inputString, T& output, int const steps = Infinite)
	{
		return DoParse(inputString, output, steps);
	}

};
