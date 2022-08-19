CFLAG = -g -Wall -w

all: client

client: KVClient.o
	g++ $(CFLAG) -o client $^ -lpthread

KVClient.o: KVClient.cpp
	g++ $(CFLAG) -c $< -lpthread

clean:
	rm *.o client