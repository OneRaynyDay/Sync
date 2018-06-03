CC = g++
BOOST_LINK_FLAGS = -lboost_system -lboost_filesystem -I /usr/local/Cellar/boost/1.65.0
DEBUG_FLAGS = -Wall -g -Werror
CPP_VER = -std=c++11
FLAGS = $(DEBUG_FLAGS) $(CPP_VER)

run: build
	build/main

build: build/walk.o build/diff.o build/main.o
	$(CC) $(FLAGS) $(BOOST_LINK_FLAGS) -o build/main $^ 

test: diff-test walk-test

# ~~ INDIVIDUAL TESTS ~~
# Include main-test.o in all *-test because we need catch.hpp's main to run.
main-test.o: test/main-test.cpp
	$(CC) $(FLAGS) -c test/main-test.cpp -o build/main-test.o

%-test: build/main-test.o build/%.o test/%-test.cpp
	$(CC) $(FLAGS) $(BOOST_LINK_FLAGS) $^ -o build/$*-test
	build/$*-test

# ~~ .O FILE GENERATION ~~
# MAIN ROUTINE
build/%.o: src/%.cpp
	$(CC) $(FLAGS) -c $^ -o $<
