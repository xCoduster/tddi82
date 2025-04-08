CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -pedantic -Weffc++ -Wold-style-cast -Woverloaded-virtual -fmax-errors=3 -O0 -g
CXXFLAGS_LIB ?= -std=c++17 -w -O0
objs = $(foreach dir,$(1),$(patsubst $(dir)/%.cpp,build/$(dir)/%.o,$(wildcard $(dir)/*.cpp)))

.PHONY: default
default:
	@echo "error: no default target"

.PHONY: clean
clean:
	rm -rf build

build:
	mkdir build

.PHONY: lab1
lab1: $(call objs,lab1) | build/lab1
	$(CXX) $(CXXFLAGS) $(call objs,lab1) -o build/lab1/prog
	./build/lab1/prog lab1/short.txt --remove=the --print

build/lab1: | build
	mkdir build/lab1

build/lab1/%.o: lab1/%.cpp | build/lab1
	$(CXX) $(CXXFLAGS) -o $@ -c $<

.PHONY: lab2
lab2: $(call objs,lab2) | build/lab2
	$(CXX) $(CXXFLAGS) $(call objs,lab2) -o build/lab2/prog
	./build/lab2/prog

.PHONY: valgrind
valgrind: $(call objs,lab2) | build/lab2
	$(CXX) $(CXXFLAGS) $(call objs,lab2) -o build/lab2/prog
	valgrind ./build/lab2/prog

build/lab2: | build
	mkdir build/lab2

build/lab2/%.o: lab2/%.cpp | build/lab2
	$(CXX) $(CXXFLAGS) -o $@ -c $<

build/lab2/uppgift2.o: lab2/uppgift2.cpp lab2/counted_ptr.tcc lab2/counted_ptr.h | build/lab2
	$(CXX) $(CXXFLAGS) -o build/lab2/uppgift2.o -c lab2/uppgift2.cpp
