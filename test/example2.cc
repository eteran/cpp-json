
#include "cpp-json/json.h"
#include <iostream>

//------------------------------------------------------------------------------
// Name: main
//------------------------------------------------------------------------------
int main() {

	// construct programatically (legacy C++ API)
	// See example4.cc for recommended API
	json::object obj1;
	obj1.insert("test1", "hello world")
		.insert("test2", 10)
		.insert("test3", json::object().insert("x", nullptr))
		.insert("test4", json::array().append(1, 2, 3, 4, "hello", true, 1.3));

	std::cout << stringify(obj1, json::PRETTY_PRINT) << std::endl;
}
