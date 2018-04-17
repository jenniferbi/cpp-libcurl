CXX = g++
CXXFLAGS = -g -Wall -O2 -std=c++14

simple: http.o
simple.o: handle.h urldata.h 
http.o: urldata.h

