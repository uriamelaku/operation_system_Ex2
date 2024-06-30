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

## UDSSD:
# ./mync -e "ttt 123456789" -i UDSSDuria
# nc -u -U uria

# UDSCD:
# socat - UNIX-RECVFROM:blue,fork
# ./mync -e "ttt 123456789" -o UDSCDblue

## UDSSS:
# ./mync -e "ttt 123456789" -i UDSSSomer
# socat - UNIX-CONNECT:omer

## UDSCS:
# nc -l -U green
# ./mync -e "ttt 123456789" -o UDSCSgreen