
#include "cpp-json/json.h"
#include <iostream>

//------------------------------------------------------------------------------
// Name: main
//------------------------------------------------------------------------------
int main() {

	// construct from string (C++11 raw string literals work nicely here!)
	auto v = json::parse(R"(
	{
		"test3" : {
			"x" : 123.456
		},
		"test4" : [
			1,
			2,
			3,
			{
				"z" : 12345
			}
		],
		"test1" : "hello world",
		"test2" : "BLAH\uD840\uDC8ABLAH"
	}
	)");

	std::cout << stringify(v, json::PRETTY_PRINT) << std::endl;
	std::cout << "----------" << std::endl;

	// get a specific value
	json::value z = v["test4"][3]["z"];
	std::cout << to_number(z) << std::endl;

	std::cout << stringify(v, json::ESCAPE_UNICODE) << std::endl;
}
