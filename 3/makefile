CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra

all: ttt mync

ttt: tic_tac_toe.cpp
	$(CXX) $(CXXFLAGS) -o ttt tic_tac_toe.cpp

mync: mync.cpp
	$(CXX) $(CXXFLAGS) -o mync -g mync.cpp

.PHONY: clean

clean:
	rm -f ttt mync

# ./mync -e "ttt 123456789" -i TCPS4061
# nc localhost 4061

# ./mync -e "ttt 123456789" -b TCPS4062
# nc localhost 4062


# nc -l 4455
# ./mync -e "ttt 123456789" -i TCPS4050 -o TCPClocalhost,4455
# nc localhost 4050