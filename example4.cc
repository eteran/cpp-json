
#include "cpp-json/json.h"
#include <iostream>

//------------------------------------------------------------------------------
// Name: main
//------------------------------------------------------------------------------
int main() {

	// construct programmatically using object literal syntax in C++11
#if __cplusplus >= 201103L	
	json::array arr = {
		1,
		2,
		3,
		4,
		"Testing 1 2 3", json::object{
			{ "hello", 1234 },
			{ "world", 5678 }
		}
	};
	std::cout << stringify(arr) << std::endl;
#endif
}
