
#include <string>
#include <iostream>
#include "parse.hpp"
using std::string;



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
	while (parse::Parse(str, output, 1) == 0)
	{
		std::cout << str << std::endl;
	}

	return 0;

}