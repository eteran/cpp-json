
#include "cpp-json/json.h"
#include <iostream>
#include <fstream>

/**
 * @brief main
 * @return
 */
int main() {

	// construct from a file
	std::ifstream file("example1.json");
	if(file) {
		auto v1 = json::parse(file);
		std::cout << stringify(v1, json::PrettyPrint | json::EscapeUnicode) << '\n';
	}
}
