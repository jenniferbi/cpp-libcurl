CC = g++
CXX = g++
CXXFLAGS = $(INCLUDES) -g -O2 -ftemplate-depth-256 -std=c++14 -DASIO_STANDALONE
LDFLAGS = -g -lssl -lcrypto -lboost_system -lboost_thread-mt -lboost_chrono-mt
INCLUDES =

simple: http.o setopt.o
simple.o: handle.h urldata.h
http:
	g++ -g -O2 -ftemplate-depth-256   -o http http.o -lboost_system -lboost_thread-mt -lboost_chrono-mt
http.o:

setopt:

setopt.o: urldata.h

.PHONY: clean
clean:
	rm *.o simple
