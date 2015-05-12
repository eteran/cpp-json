
SHELL=/bin/sh

.SUFFIXES:

CXXFLAGS  := -std=c++11 -pedantic -Wextra -Wall -Iinclude -O2 -g3

.PHONY:     all clean

TARGETS := example1 example2 example3 example4

all: $(TARGETS)

example1: example1.cc 
	$(CXX) $(CXXFLAGS) $^ -o $@
	
example2: example2.cc 
	$(CXX) $(CXXFLAGS) $^ -o $@

example3: example3.cc 
	$(CXX) $(CXXFLAGS) $^ -o $@

example4: example4.cc 
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	$(RM) $(TARGETS)


