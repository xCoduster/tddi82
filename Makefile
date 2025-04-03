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
	./build/lab1/prog

build/lab1: | build
	mkdir build/lab1

build/lab1/%.o: lab1/%.cpp | build/lab1
	$(CXX) $(CXXFLAGS) -o $@ -c $<
