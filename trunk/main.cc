
#include "json.h"
#include <fstream>
#include <iostream>

int main(int argc, char *argv[]) {
	for(int i = 1; i < argc; ++i) {
		std::ifstream file(argv[i]);
		json::value json = json::parse(file);
		
		boost::unordered_set<std::string> keys = json::keys(json);
		for(boost::unordered_set<std::string>::iterator it = keys.begin(); it != keys.end(); ++it) {
			std::cout << *it << std::endl;
		}
	}
}
