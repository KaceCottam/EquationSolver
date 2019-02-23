#include <string>
using std::string;
template<typename T>
class Parse final
{
public:
	static T Evaluate()
	{
		if (!std::is_integral<T>::value)
		{
			throw "Bad type";
		}



		return T();
	}

};
