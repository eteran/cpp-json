
#include "cpp-json/json.h"
#include <iostream>
#include <fstream>

//------------------------------------------------------------------------------
// Name: main
//------------------------------------------------------------------------------
int main() {

	// construct from a file
	std::ifstream file("example1.json");
	if(file) {
		json::value v1 = json::parse(file);
		std::cout << stringify(v1, json::PRETTY_PRINT | json::ESCAPE_UNICODE) << std::endl;
	}
}
