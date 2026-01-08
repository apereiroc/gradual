.PHONY: all clean compile_commands test

all: compile_commands

main: main.cc
	g++ -o main main.cc -std=c++17 $(shell pkg-config --cflags --libs fmt)

test_dual: test_dual.cc dual.h
	g++ -o test_dual test_dual.cc -std=c++17 $(shell pkg-config --cflags --libs catch2-with-main)

test: test_dual
	./test_dual

compile_commands:
	@bear -- $(MAKE) main test_dual

clean:
	rm -f main test_dual compile_commands.json
