
#include "json.h"
#include <fstream>
#include <iostream>

int main(int argc, char *argv[]) {

#if 1
	for(int i = 1; i < argc; ++i) {
		std::ifstream file(argv[i]);
		const json::value json = json::parse(file);

#if 1
		// test outputing the object as a JSON string
		std::cout << "--------" << std::endl;
		std::cout << json::pretty_print(json) << std::endl;
#endif
	}
#else

	json::array obj3;
	obj3.append(json::value(1))
		.append(json::value(2))
		.append(json::value(3))
		.append(json::value(4));
	
	json::object obj;
	obj1.append("test1", json::value("hello world"))
		.append("test2", json::value(10))
		.append("test3", json::value(json::object().append("x", json::value(123.456))))
		.append("test4", json::value(obj3));

	std::cout << json::pretty_print(json::value(obj1)) << std::endl;
	
#endif
}
