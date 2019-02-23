#pragma once
#include <string>
using std::string;

template<typename T>
class StringEquation final
{
	string Data;
public:
	StringEquation() = default;
	StringEquation(string const& data) : Data(data){}
	StringEquation(StringEquation const& other) =	default;
	StringEquation(StringEquation&& other) = default;
	StringEquation& operator=(StringEquation const& other) = default;

	StringEquation& operator=(StringEquation&& other) noexcept
	{
		Data = other.Data;
		other.Data = nullptr;
		return *this;
	}
	string operator*() const
	{
		return Data;
	}

	int Read(string const& data)
	{
		Data = data;
		if (!Data.empty())
			return 0;
		return 1;
	}
};