
#include "json.h"
#include <fstream>
#include <iostream>

int main(int argc, char *argv[]) {
	std::ifstream file(argv[1]);
	json::value json = json::parse((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	
	const json::value servlets = json["web-app"]["servlet"];
		
	for(size_t i = 0; i < json::size(servlets); ++i) {
		std::cout << json::to_string(servlets[i]["servlet-name"]) << std::endl;
	}
}
