#define compiler
CC = g++

#flags
CFLAGS = -Wall

default : ImagePullDownThingy

ImagePullDownThingy : CaptureThread.o main.o
	$(CC) $(CFLAGS) -o ImagePullDownThingy main.o CaptureThread.o

main.o : main.cpp main.hpp
	$(CC) $(CFLAGS) -c main.cpp

CaptureThread.o : CaptureThread.cpp CaptureThread.hpp
	$(CC) $(CFLAGS) -c CaptureThread.cpp

clean :
	rm ImagePullDownThingy *.o
