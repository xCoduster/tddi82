CC := g++ -std=c++17 -I. -O3
LIBS := -lsfml-graphics -lsfml-window -lsfml-system -lpthread

HEADERS=$(shell find -name "*.h")
SOURCES=$(shell find -name "node.cpp")

all: build build/math.o build/gfx.o build/diagram.o build/window.o build/main.o $(SOURCES)
	$(CC) build/math.o build/gfx.o build/diagram.o build/window.o build/main.o $(SOURCES) -o build/graph $(LIBS)

show:
	@echo $(HEADERS)
	@echo $(SOURCES)

build/main.o: $(HEADERS) lib/window.h lib/main.cc
	$(CC) lib/main.cc -c -o build/main.o

build/math.o: $(HEADERS) lib/math.h lib/math.cc
	$(CC) lib/math.cc -c -o build/math.o

build/gfx.o: $(HEADERS) lib/math.h lib/gfx.h lib/gfx.cc
	$(CC) lib/gfx.cc -c -o build/gfx.o

build/diagram.o: $(HEADERS) lib/deduce.h lib/math.h lib/gfx.h lib/diagram.h lib/diagram.cc
	$(CC) lib/diagram.cc -c -o build/diagram.o

build/window.o: $(HEADERS) lib/deduce.h lib/math.h lib/gfx.h lib/window.h lib/window.cc
	$(CC) lib/window.cc -c -o build/window.o

build:
	@mkdir -p build

.PHONY: graph clean reset


graph: all
	./build/graph

clean:
	rm build/*.o

reset:
	rm -rf build
