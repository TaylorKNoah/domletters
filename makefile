CFLAGS = -g -Wall -Werror
CXX = g++

all: domlets

domlets: domlets.o
	$(CXX) $(CFLAGS) domlets.o -o domlets 

domlets.o: domlets.cpp
	$(CXX) -c $(CFLAGS) domlets.cpp -o domlets.o


clean:
	rm domlets domlets.o
