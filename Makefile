.PHONY: all clean compile_commands test

all: compile_commands

main: main.cc
	clang++ -o main main.cc -std=c++20 $(shell pkg-config --cflags --libs fmt)

test_dual: test_dual.cc dual.h
	clang++ -o test_dual test_dual.cc -std=c++20 $(shell pkg-config --cflags --libs catch2-with-main)

test_vector: test_vector.cc vector.h
	clang++ -o test_vector test_vector.cc -std=c++20 $(shell pkg-config --cflags --libs catch2-with-main)

test_gradient: test_gradient.cc gradient.h dual.h vector.h
	clang++ -o test_gradient test_gradient.cc -std=c++20 $(shell pkg-config --cflags --libs catch2-with-main)

test_optimiser: test_optimiser.cc optimiser.h gradient.h dual.h vector.h
	clang++ -o test_optimiser test_optimiser.cc -std=c++20 $(shell pkg-config --cflags --libs catch2-with-main)

test: test_dual test_vector test_gradient test_optimiser
	./test_dual
	./test_vector
	./test_gradient
	./test_optimiser

compile_commands:
	@bear -- $(MAKE) main test_dual test_vector test_gradient test_optimiser

clean:
	rm -f main test_dual test_vector test_gradient test_optimiser compile_commands.json
