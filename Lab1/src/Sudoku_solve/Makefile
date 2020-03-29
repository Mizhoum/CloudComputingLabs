CXXFLAGS+=-O2 -ggdb -DDEBUG
CXXFLAGS+=-Wall -Wextra

all: sudoku_solve

sudoku_solve: main.cpp sudoku_dancing_links.cpp
	g++ -std=c++11 -O2 -o $@ $^ -lpthread
