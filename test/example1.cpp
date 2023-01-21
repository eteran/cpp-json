
#include "cpp-json/json.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>

/**
 * @brief main
 * @return
 */
int main() {

	try {
		// construct from a file
		std::ifstream file("example1.json");
		assert(file);

		std::string s{std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};

		auto v1 = json::parse(s);
		std::cout << stringify(v1, json::PrettyPrint | json::EscapeUnicode) << '\n';

		assert(json::to_number<int>(v1["test1"]) == 1);
		assert(json::to_bool(v1["b1"]) == true);
		assert(json::to_bool(v1["b2"]) == false);
		assert(json::is_null(v1["n1"]));
		assert(json::to_number<int>(v1["n2"]) == -1234);

	} catch (const json::exception &e) {
		std::cerr << "Error on line: " << e.line << ", column: " << e.column << std::endl;
		throw;
	}
}
