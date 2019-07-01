
#include "cpp-json/json.h"
#include <iostream>

/**
 * @brief main
 * @return
 */
int main() {

	// construct programmatically using object literal syntax in C++11	
	auto arr = json::array {
		1,
		2,
		3,
		4,
		"Testing 1 2 3",
		json::object{
			{ "hello", 1234 },
			{ "world", 5678 }
		}
	};
	
	std::cout << stringify(arr) << '\n';
}
