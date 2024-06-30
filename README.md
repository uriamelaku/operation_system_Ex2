### omer cohen
### omer7530@gmail.com
##
# Tic Tac Toe and mync Utility

## Overview

This project contains a Tic Tac Toe game and a utility named `mync` for various network operations. Below are the instructions for compiling, running, and testing the code, as well as using `gcov` for code coverage analysis.

## Compilation and Running
## Using make
- use command ```make all``` to run all the tasks.


## Commands of running by tasks

### Task 1
```
g++ -o ttt tic_tac_toe.cpp
```
### Task 2
```
./mync -e "ttt 123456789"

// this is for gcov
# gcov mync.cpp
# cat mync.cpp.gcov
```
### Task 3
```
// 1
./mync -e "ttt 123456789" -i TCPS4061
nc localhost 4061

// 2
./mync -e "ttt 123456789" -b TCPS4062
nc localhost 4062

// 3
nc -l 4455
./mync -e "ttt 123456789" -i TCPS4050 -o TCPClocalhost,4455
nc localhost 4050
```
### Task 3.5
```
./mync -b TCPS4050
./mync -b TCPClocalhost,4050
```
### Task 4
```
// 1
./mync -e "ttt 123456789" -i UDPS4050 -t 10
./mync -o UDPClocalhost,4050

// 2
./mync -o UDPClocalhost,4058
./mync -i TCPS4459
./mync -e "ttt 123456789" -i UDPS4058 -o TCPClocalhost,4459
```
### Task 6
```
// UDSSD:
./mync -e "ttt 123456789" -i UDSSDuria
nc -u -U uria

// UDSCD:
socat - UNIX-RECVFROM:blue,fork
./mync -e "ttt 123456789" -o UDSCDblue

// UDSSS:
./mync -e "ttt 123456789" -i UDSSSomer
socat - UNIX-CONNECT:omer

// UDSCS:
nc -l -U green
./mync -e "ttt 123456789" -o UDSCSgreen
```

