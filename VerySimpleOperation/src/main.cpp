#include <string>
#include <iostream>
#include <map>
#include <functional>
#include <vector>
using std::string;

template<typename T>
T OperationAdd(T const a, T const b) { return a + b; }
template<typename T>
T OperationSubtract(T const a, T const b) { return a - b; }
template<typename T>
T OperationMultiply(T const a, T const b) { return a * b; }
template<typename T>
T OperationDivide(T const a, T const b) { return a / b; }
template<typename T>
T OperationMod(T const a, T const b) { return (int)a%(int)b; }
template<typename T>
T OperationPow(T const a, T const b) { return pow(a, b); }

template<typename T>
static std::map<char, std::function<T(T, T)>> OperatorMap()
{
	std::map<char, std::function<T(T, T)>> operatorMap;
	operatorMap['*'] = OperationMultiply<T>;
	operatorMap['/'] = OperationDivide<T>;
	operatorMap['+'] = OperationAdd<T>;
	operatorMap['-'] = OperationSubtract<T>;
	operatorMap['%'] = OperationMod<T>;
	operatorMap['^'] = OperationPow<T>;
	return operatorMap;
}

enum class Precedence : int
{
	Power,
	Multiply,
	Divide,
	Mod,
	Add,
	Subtract,
	Num = 6
};
enum class PrecedenceCharacter : char
{
	Multiply = '*',
	Divide = '/',
	Add = '+',
	Subtract = '-',
	Mod = '%',
	Power = '^',
	Num = 6
};

static std::map<PrecedenceCharacter, Precedence> PrecedenceMap()
{
	std::map<PrecedenceCharacter, Precedence> precedenceMap;
	precedenceMap[PrecedenceCharacter::Multiply] = Precedence::Multiply;
	precedenceMap[PrecedenceCharacter::Divide] = Precedence::Divide;
	precedenceMap[PrecedenceCharacter::Add] = Precedence::Add;
	precedenceMap[PrecedenceCharacter::Subtract] = Precedence::Subtract;
	precedenceMap[PrecedenceCharacter::Power] = Precedence::Power;
	precedenceMap[PrecedenceCharacter::Mod] = Precedence::Mod;
	return precedenceMap;
}

//returns 0 if working, 1 if done
// TODO: precond: string is perfect
template<typename T>
T Parse(string& eqn, T& output, int steps = -1)
{
	/*
	 * Looping here
	 */
	if (steps == -1)
	{
		while (Parse(eqn, output, 1) == 0) {}
	}
	else if (steps != 0)
	{
		for (auto i = 0; i < steps; i++)
		{
			if (!Parse(eqn, output, steps - 1)) return 0;
		}
		output = atoi(eqn.c_str());
	}
	else
	{
		string curr(eqn);

		//find parenthesis
		auto findParenthesis = eqn.find_last_of('(');
		if (findParenthesis != string::npos)
		{
			string subEqnWithParen = &eqn[findParenthesis];
			string subEqn = &subEqnWithParen[1];
			subEqnWithParen[subEqnWithParen.find_first_of(')') + 1] = '\0';
			subEqn[subEqn.find_first_of(')')] = '\0';
			subEqnWithParen = subEqnWithParen.c_str();
			subEqn = subEqn.c_str();
			if (!Parse(subEqn, output, 1))
			{
				subEqn.append(")");
				subEqn.insert(0, "(");
				eqn.replace(findParenthesis, subEqnWithParen.length(), subEqn);
			}
			else
			{
				if (output < 0)
				{
					if (eqn[findParenthesis - 1] == '-')
					{
						eqn.replace(findParenthesis - 1, subEqnWithParen.length() + 1, subEqn);
						eqn[findParenthesis - 1] = '+';
					}
					else if (eqn[findParenthesis - 1] == '+')
					{
						eqn.replace(findParenthesis - 1, subEqnWithParen.length() + 1, subEqn);
					}
					else
					{
						eqn.replace(findParenthesis, subEqnWithParen.length(), subEqn);
					}
				}
				else
				{
					if (eqn[findParenthesis - 1] == '-')
					{
						eqn.replace(findParenthesis - 1, subEqnWithParen.length() + 1, subEqn);
					}
					else
					{
						eqn.replace(findParenthesis, subEqnWithParen.length(), subEqn);
					}
				}
			}
			//eqn.replace(findParenthesis, subEqnWithParen.length(), subEqn);


			return 0;
		}
		char operation;
		bool operatorFound = false;
		int operatorFoundIndex = 0;
		for (auto i = 0; i < (int)Precedence::Num; i++)
		{
			// read through and if found do the operation, then break out of this function.
			int times = 0;
			for (auto& j : curr)
			{
				auto precedenceMap = PrecedenceMap();
				auto findCharacter = precedenceMap.find((PrecedenceCharacter)j);
				if (findCharacter != precedenceMap.end())
					if (findCharacter->second == (Precedence)i)
					{
						operatorFound = true;
						operatorFoundIndex = times;
						operation = j;
						break;
					}
				times++;
			}
			if (operatorFound)
			{
				break;
			}
		}

		if (operatorFound)
		{
			//DO OPERATION
			std::function<T(T, T)> func = OperatorMap<T>().find(operation)->second;
			T right = atoi(curr.c_str() + operatorFoundIndex + 1);
			int rightIndex = operatorFoundIndex + 1;
			int leftIndex = operatorFoundIndex - 1;
			auto IsNumber = [](char c)
			{
				return c >= '0' && c <= '9' || c == '-' || c == '.';
			};
			if (leftIndex < 0)
			{
				return 1;
			}
			while (IsNumber(curr[leftIndex]) && leftIndex > 0)
			{
				--leftIndex;
				if (curr[leftIndex] == '-')
					break;
			}
			while (IsNumber(curr[rightIndex]) && rightIndex < (int)curr.length())
			{
				if (rightIndex+1 != (int)curr.length() && curr[rightIndex + 1] == '-')
					break;
				++rightIndex;
			}

			if (!IsNumber(curr[leftIndex])) if (curr[leftIndex] != '-') ++leftIndex;
			if (!IsNumber(curr[rightIndex])) --rightIndex;

			T left = atoi(curr.c_str() + leftIndex);

			T result = func(left, right);

			curr[rightIndex] = '\0';

			if (leftIndex != 0 && curr[leftIndex] == '-')
			{
				if (result < 0)
					eqn.replace(leftIndex, rightIndex - leftIndex + 1, std::to_string(result).c_str());
				else
				{
					eqn.replace(leftIndex + 1, rightIndex - leftIndex, std::to_string(result).c_str());

					eqn[leftIndex] = '+';
				}
			}
			else
			{
				eqn.replace(leftIndex, rightIndex - leftIndex + 1, std::to_string(result).c_str());

				if (result < 0)
				{
					eqn[leftIndex] = '-';
				}
			}
			return 0;
		}
	}
	return 1;
}

int main(int argc, char *argv[])
{

	string str;
	if (argc > 1)
	{
		str = argv[2];
	}
	else
	{
		str = "1*4+(1*(2-62)+100)^2-30/(2^3+2*5)+100/20-30+60*2-5*100-(50-34*23)+70";
	}
	// BUG: parenthesis at beginning of string causes error
	// BUG: minus sign at beginning of string causes error
	int output;
	std::cout << str << std::endl;
	while (Parse(str, output, 1) == 0)
	{
		std::cout << str << std::endl;
	}

	return 0;

}