
#include "json.h"
#include <iostream>
#include <fstream>

int main(int argc, char *argv[]) {

	for(int i = 1; i < argc; ++i) {
		std::ifstream file(argv[i]);
		if(file) {
			json::value v1 = json::parse(file);
			std::cout << json::pretty_print(v1) << std::endl;
			std::cout << "----------" << std::endl;
		}
	}
	
#if 0
	// construct from a file
	std::ifstream file("test.json");
	json::value v1 = json::parse(file);
	std::cout << json::pretty_print(v1) << std::endl;
	std::cout << "----------" << std::endl;

	// construct programatically
	json::object obj1;
	obj1.append("test1", "hello world")
		.append("test2", 10)
		.append("test3", json::object().append("x", 123.456))
		.append("test4", json::array().append(1).append(2).append(3).append(4));

	std::cout << json::pretty_print(obj1) << std::endl;
	std::cout << "----------" << std::endl;
	
	// construct from string
	json::value v = json::decode("{\"test3\" : {\"x\" : 123.456},\"test4\" : [1,2,3,{\"z\":12345}],\"test1\" : \"hello world\",\"test2\" : \"\\uD840\\uDC8A\"}");
	std::cout << json::pretty_print(v) << std::endl;
	std::cout << "----------" << std::endl;
	
	// get a specific value
	json::value z = v["test4"][3]["z"];
	std::cout << json::to_number(z) << std::endl;
	
	std::cout << json::encode(v) << std::endl;
#endif
}
