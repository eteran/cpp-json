
#include "cpp-json/json.h"
#include <iostream>

/**
 * @brief main
 * @return
 */
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
				"z" : 12345.6
			}
		],
		"test1" : "hello world",
		"test2" : "BLAH\uD840\uDC8ABLAH"
	}
	)");

	std::cout << stringify(v, json::PrettyPrint) << '\n';
	std::cout << "----------" << std::endl;

	// get a specific value
	json::value z = v["test4"][3]["z"];
	std::cout << json::to_number<float>(z) << std::endl;
	std::cout << json::to_number<int>(z) << std::endl;

	std::cout << stringify(v, json::EscapeUnicode) << '\n';
}
