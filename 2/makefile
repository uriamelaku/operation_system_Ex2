CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra

all: ttt mync

ttt: tic_tac_toe.cpp
	$(CXX) -fprofile-arcs -ftest-coverage $(CXXFLAGS) -o ttt tic_tac_toe.cpp

mync: mync.cpp
	$(CXX) -fprofile-arcs -ftest-coverage $(CXXFLAGS) -o mync -g mync.cpp

.PHONY: clean

clean:
	rm -f ttt mync

# ./mync -e "ttt 123456789"

# gcov mync.cpp
# cat mync.cpp.gcov