
#include "cpp-json/json.h"
#include <iostream>

/**
 * @brief main
 * @return
 */
int main() {

	try {
		// construct from string (C++11 raw string literals work nicely here!)
		auto v = json::parse(R"({
            "foo": ["bar", "baz"],
            "": 0,
            "a/b": 1,
            "c%d": 2,
            "e^f": 3,
            "g|h": 4,
            "i\\j": 5,
            "k\"l": 6,
            " ": 7,
            "m~n": 8
        })");

		std::cout << json::stringify(v[json::ptr("")]) << std::endl;
		std::cout << json::stringify(v[json::ptr("/foo")]) << std::endl;
		std::cout << json::stringify(v[json::ptr("/foo/0")]) << std::endl;
		std::cout << json::stringify(v[json::ptr("/")]) << std::endl;
		std::cout << json::stringify(v[json::ptr("/a~1b")]) << std::endl;
		std::cout << json::stringify(v[json::ptr("/c%d")]) << std::endl;
		std::cout << json::stringify(v[json::ptr("/e^f")]) << std::endl;
		std::cout << json::stringify(v[json::ptr("/g|h")]) << std::endl;
		std::cout << json::stringify(v[json::ptr("/i\\j")]) << std::endl;
		std::cout << json::stringify(v[json::ptr("/k\"l")]) << std::endl;
		std::cout << json::stringify(v[json::ptr("/ ")]) << std::endl;
		std::cout << json::stringify(v[json::ptr("/m~0n")]) << std::endl;

		std::cout << "-----------------" << std::endl;

		std::cout << json::stringify(v[json::ptr("#")]) << std::endl;
		std::cout << json::stringify(v[json::ptr("#/foo")]) << std::endl;
		std::cout << json::stringify(v[json::ptr("#/foo/0")]) << std::endl;
		std::cout << json::stringify(v[json::ptr("#/")]) << std::endl;
		std::cout << json::stringify(v[json::ptr("#/a~1b")]) << std::endl;
		std::cout << json::stringify(v[json::ptr("#/c%25d")]) << std::endl;
		std::cout << json::stringify(v[json::ptr("#/e%5Ef")]) << std::endl;
		std::cout << json::stringify(v[json::ptr("#/g%7ch")]) << std::endl;
		std::cout << json::stringify(v[json::ptr("#/i%5Cj")]) << std::endl;
		std::cout << json::stringify(v[json::ptr("#/k%22l")]) << std::endl;
		std::cout << json::stringify(v[json::ptr("#/%20")]) << std::endl;
		std::cout << json::stringify(v[json::ptr("#/m~0n")]) << std::endl;

	} catch (const json::lexing_error &e) {
		std::cerr << "Error @ " << e.index() << std::endl;
		throw;
	}
}
