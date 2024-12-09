sources := $(shell find . -name '*.cpp')
headers := $(shell find . -name '*.h')

CFLAGS := -std=c++17 -Wall -O2 

ltl-conversion: $(sources) $(headers) Makefile
	g++ -I. $(CFLAGS) -o $@ $(sources)
