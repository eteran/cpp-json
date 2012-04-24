
#include "json.h"
#include <fstream>
#include <iostream>

int main(int argc, char *argv[]) {
	
	json::object obj1;
	obj1.append("test1", "hello world")
		.append("test2", 10)
		.append("test3", json::object().append("x", 123.456))
		.append("test4", json::array().append(1).append(2).append(3).append(4));

	std::cout << json::pretty_print(obj1) << std::endl;
	
	
	json::value v = json::from_string("{\"test3\" : {\"x\" : 123.456},\"test4\" : [1,2,3,{\"z\":12345}],\"test1\" : \"hello world\",\"test2\" : 10}"	);
	std::cout << json::pretty_print(v) << std::endl;
	
	json::value z = v["test4"][3]["z"];
	
	std::cout << json::to_number(z) << std::endl;
	

	

}
