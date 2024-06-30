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

# ./mync -e "ttt 123456789" -i UDPS4050 -t 10
# ./mync -o UDPClocalhost,4050


# ./mync -o UDPClocalhost,4058
# ./mync -i TCPS4459
# ./mync -e "ttt 123456789" -i UDPS4058 -o TCPClocalhost,4459