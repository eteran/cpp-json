There are a few different JSON parsing libraries out there. But cpp-json aims to be the simplest to use while still being efficient by using modern c++ techniques. Additionally, this library is header only making it trivial to include in existing projects.

Currently, the only active item on the TODO list is to better support Unicode. The parser can be given iterators referring to wide characters, but there is no runtime detection or endian-ness detection (yet)

However, Unicode is generally **well supported** in the form of `\uXXXX` encoding, including code points which require surrogate pairs. The resulting `json::value` object will contain the string, encoded as UTF-8 since it is stored in a `std::string`.

Of course special consideration is needed when displaying these strings if they do in fact contain non ASCII characters.

So, for example,

    {"test1" : "\uD840\uDC8A"}
   
will correctly parse and the object's "test1" member will have the byte sequence: `0xF0 0xA0 0x82 0x8A`

Here is a simple example of the usage of this library:

	#include "cpp-json/json.h"
	#include <fstream>
	#include <iostream>

	int main() {
        	// open a file
        	std::ifstream file("test.json");

        	// json::parse can take two iterators or a std::istream
        	json::value json = json::parse(file);

        	// you can access objects like associative array's easily
        	// the result is a json::value
        	// ... though in real code you may want to check the type first ;-)
        	auto servlets = json["web-app"]["servlet"];

        	// when dealing with arrays, you can just use iterators, 
        	// or feel free to use C++11 ranged-for
        	const json::array &a = as_array(servlets);
        	for(auto it = a.begin(); it != a.end(); ++it) {
                	const json::value &v = *it;
                	// all basic types (numbers, strings, booleans) can be converted 
                	// to a string
                	std::cout << to_string(v["servlet-name"]) << std::endl;
        	}
	}
	
You can also programmatically create `json::value` objects like this:

	int main(int argc, char *argv[]) {
        	json::object obj;
        	// construct programatically
        	obj1.insert("test1", "hello world")
                	.insert("test2", 10)
                	.insert("test3", json::object().insert("x", 123.456))
                	.insert("test4", json::array().append(1).append(2).append(3).append(4).append("hello"));

        	std::cout << stringify(obj1) << std::endl;
	}
	
When compiling with C++11, you can benefit from variadic templates and have more concise syntax too

	int main(int argc, char *argv[]) {
        	json::object obj;
        	// construct programmatically
        	obj1.insert("test1", "hello world")
                	.insert("test2", 10)
                	.insert("test3", json::object().insert("x", 123.456))
                	.insert("test4", json::array().append(1, 2, 3, 4, "hello"));

        	std::cout << stringify(obj1) << std::endl;
	}

Both of which, of course results in an object representing the following JSON:

	{
	  "test3" : {
    	"x" : 123.456
	  },
	  "test4" : [
    	1,
    	2,
    	3,
    	4,
    	"hello"
	  ],
	  "test1" : "hello world",
	  "test2" : 10
	}
	
Finally, this library is very fast, when processing a 190 MB JSON file I randomly selected, parsing took no more than 18 seconds on my machine. For a Qt4 JSON parsing library, you can also checkout my other project: [QJson4](https://github.com/eteran/qjson4)
