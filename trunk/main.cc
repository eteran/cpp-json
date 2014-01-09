
#include "json.h"
#include <iostream>
#include <fstream>

//------------------------------------------------------------------------------
// Name: main
//------------------------------------------------------------------------------
int main(int argc, char *argv[]) {

	for(int i = 1; i < argc; ++i) {
		std::ifstream file(argv[i]);
		if(file) {
			json::value v1 = json::parse(file);
			std::cout << json::pretty_print(v1) << std::endl;
			std::cout << "----------" << std::endl;
		}
	}
	
#if 1
	// construct from a file
	std::ifstream file("test.json");
	json::value v1 = json::parse(file);
	std::cout << json::pretty_print(v1, json::ESCAPE_UNICODE) << std::endl;
	std::cout << "----------" << std::endl;

	// construct programatically
	json::object obj1;
	obj1.insert("test1", "hello world")
		.insert("test2", 10)
		.insert("test3", json::object().insert("x", json::null()))
		
		// if we are using C++11, we have a MUCH more elegant array initializer option
	#if __cplusplus >= 201103L 
		.insert("test4", json::array(1, 2, 3, 4, "hello", true, 1.3));
	#else
		.insert("test4", json::array().append(1).append(2).append(3).append(4).append("hello").append(true).append(1.3));
	#endif

	std::cout << json::pretty_print(obj1) << std::endl;
	std::cout << "----------" << std::endl;
	
	// construct from string (C++11 raw string literals work nicely here!)
#if __cplusplus >= 201103L
	json::value v = json::parse(R"({"test3" : {"x" : 123.456},"test4" : [1,2,3,{"z":12345}],"test1" : "hello world","test2" : "BLAH\uD840\uDC8ABLAH"})");
#else
	json::value v = json::parse("{\"test3\" : {\"x\" : 123.456},\"test4\" : [1,2,3,{\"z\":12345}],\"test1\" : \"hello world\",\"test2\" : \"BLAH\\uD840\\uDC8ABLAH\"}");
#endif
	std::cout << json::pretty_print(v) << std::endl;
	std::cout << "----------" << std::endl;
	
	// get a specific value
	json::value z = v["test4"][3]["z"];
	std::cout << json::to_number(z) << std::endl;
	
	std::cout << json::serialize(v, json::ESCAPE_UNICODE) << std::endl;
#endif
}
