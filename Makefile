
SHELL=/bin/sh

.SUFFIXES:

CXXFLAGS  := -std=c++11 -pedantic -Wextra -Wall -Iinclude -O2 -g3

TARGETS := example1 example2 example3 example4
TESTS   := example1.test example2.test example3.test example4.test

.PHONY:     all clean $(TESTS)

all: $(TARGETS)

example1: example1.cc include/cpp-json/json.h \
 include/cpp-json/exception.h include/cpp-json/value.h \
 include/cpp-json/object.h include/cpp-json/array.h \
 include/cpp-json/parser.h include/cpp-json/json.tcc \
 include/cpp-json/object.tcc include/cpp-json/array.tcc \
 include/cpp-json/value.tcc include/cpp-json/parser.tcc
	$(CXX) $(CXXFLAGS) $< -o $@
	
example2: example2.cc include/cpp-json/json.h \
 include/cpp-json/exception.h include/cpp-json/value.h \
 include/cpp-json/object.h include/cpp-json/array.h \
 include/cpp-json/parser.h include/cpp-json/json.tcc \
 include/cpp-json/object.tcc include/cpp-json/array.tcc \
 include/cpp-json/value.tcc include/cpp-json/parser.tcc
	$(CXX) $(CXXFLAGS) $< -o $@

example3: example3.cc include/cpp-json/json.h \
 include/cpp-json/exception.h include/cpp-json/value.h \
 include/cpp-json/object.h include/cpp-json/array.h \
 include/cpp-json/parser.h include/cpp-json/json.tcc \
 include/cpp-json/object.tcc include/cpp-json/array.tcc \
 include/cpp-json/value.tcc include/cpp-json/parser.tcc
	$(CXX) $(CXXFLAGS) $< -o $@

example4: example4.cc include/cpp-json/json.h \
 include/cpp-json/exception.h include/cpp-json/value.h \
 include/cpp-json/object.h include/cpp-json/array.h \
 include/cpp-json/parser.h include/cpp-json/json.tcc \
 include/cpp-json/object.tcc include/cpp-json/array.tcc \
 include/cpp-json/value.tcc include/cpp-json/parser.tcc
	$(CXX) $(CXXFLAGS) $< -o $@
	
test: $(TESTS)
	@echo "All Passed"

example1.test: example1
	@./example1 | diff example_output/example1.txt -

example2.test: example2
	@./example2 | diff example_output/example2.txt -
	
example3.test: example3
	@./example3 | diff example_output/example3.txt -
	
example4.test: example4
	@./example4 | diff example_output/example4.txt -     

clean:
	$(RM) $(TARGETS)


