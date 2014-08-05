CC = g++
CFLAGS = -I../../source/include $(shell root-config --cflags) -g
LDFLAGS = $(shell root-config --libs)

all: server client

server: server.cc ../../source/src/UiManager.cc ../../source/src/HistoManager.cc
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) -lrt

client: client.cc
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f *.o *~ server client
