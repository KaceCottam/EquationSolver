//#comment file
#pragma once
#include <string>
#include <unordered_map>
#include <functional>
#include <sstream>

//#comment begin
template<class T>
class Parser
//#comment end
{
public:
	Parser() = default;
	virtual ~Parser() = default;
protected:
	//#comment
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

	//#comment
	virtual std::pair<std::pair<T, T>, std::pair<int, int>> DiscoverInput(int index, std::string& in, std::string& operation) = 0;
	//#comment
	virtual int CheckFunction(std::string& in) = 0;
	//#comment
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
	//#comment
	int RegisterUnaryOperator(std::string const& inputs, std::function<T(T&)> const function)
	{
		UnaryOperators[std::string(inputs)] = function;
		return 1;
	}
	//#comment
	int RegisterBinaryOperator(std::string const& inputs, std::function<T(T&, T&)> const function)
	{
		BinaryOperators[std::string(inputs)] = function;
		return 1;
	}
	//#comment
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
	//#comment begin
	virtual bool Parse(std::string& inputString, T& output, int const steps = Infinite)
	{
		return DoParse(inputString, output, steps);
	}
	//#comment end
};