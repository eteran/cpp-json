
#include "cpp-json/json.h"
#include <iostream>

//------------------------------------------------------------------------------
// Name: main
//------------------------------------------------------------------------------
int main() {

	// construct programatically (legacy C++03 API)
	// See example4.cc for recommended API
	json::object obj1;
	obj1.insert("test1", "hello world")
		.insert("test2", 10)
		.insert("test3", json::object().insert("x", nullptr))

		// if we are using C++11, we have a MUCH more elegant array initializer option
	#if __cplusplus >= 201103L
		.insert("test4", json::array().append(1, 2, 3, 4, "hello", true, 1.3));
	#else
		.insert("test4", json::array().append(1).append(2).append(3).append(4).append("hello").append(true).append(1.3));
	#endif

	std::cout << stringify(obj1, json::PRETTY_PRINT) << std::endl;
}
