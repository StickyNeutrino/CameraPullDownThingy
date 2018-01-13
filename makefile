#define compiler
CC = g++

#flags
CFLAGS = -Wall

default : ImagePullDownThingy

ImagePullDownThingy : CaptureThread.o main.o DeliveryThread.o
	$(CC) $(CFLAGS) -lcurl -o ImagePullDownThingy main.o CaptureThread.o DeliveryThread.o

main.o : main.cpp main.hpp
	$(CC) $(CFLAGS) -c main.cpp

CaptureThread.o : CaptureThread.cpp CaptureThread.hpp
	$(CC) $(CFLAGS) -c CaptureThread.cpp

DeliveryThread.o : DeliveryThread.cpp DeliveryThread.hpp
	$(CC) $(CFLAGS) -c DeliveryThread.cpp

clean :
	rm ImagePullDownThingy *.o
