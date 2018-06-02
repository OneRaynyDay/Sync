CC = g++
BOOST_LINK_FLAGS = -lboost_system -lboost_filesystem -I /usr/local/Cellar/boost/1.65.0
DEBUG_FLAGS = -Wall -g -Werror
CPP_VER = -std=c++11
FLAGS = $(DEBUG_FLAGS) $(CPP_VER)

run: build
	build/main

build: diff.o main.o
	$(CC) $(FLAGS) $(BOOST_LINK_FLAGS) -o build/main build/main.o build/diff.o

test: diff-test
	build/diff-test

# ~~ INDIVIDUAL TESTS ~~
# Include main-test.o in all *-test because we need catch.hpp's main to run.
main-test.o: test/main-test.cpp
	$(CC) $(FLAGS) -c test/main-test.cpp -o build/main-test.o

diff-test: main-test.o diff.o test/diff-test.cpp 
	$(CC) $(FLAGS) $(BOOST_LINK_FLAGS) build/main-test.o build/diff.o test/diff-test.cpp -o build/diff-test

# ~~ .O FILE GENERATION ~~
# MAIN ROUTINE
main.o: src/main.cpp
	$(CC) $(FLAGS) -c src/main.cpp -o build/main.o

diff.o: src/diff.cpp
	$(CC) $(FLAGS) -c src/diff.cpp -o build/diff.o

