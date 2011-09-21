
#include "json.h"
#include <fstream>
#include <iostream>

int main(int argc, char *argv[]) {
	for(int i = 1; i < argc; ++i) {
		std::ifstream file(argv[i]);
		json::value json = json::parse(file);
	}
}
