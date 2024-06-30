CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra

all: mync ttt

mync: mync.cpp
	$(CXX) $(CXXFLAGS) -g -o $@ $^

ttt: tic_tac_toe.cpp
	$(CXX) $(CXXFLAGS) -g -o $@ $^
	
.PHONY: clean

clean:
	rm -f mync ttt

# ./mync -b TCPS4050
# ./mync -b TCPClocalhost,4050