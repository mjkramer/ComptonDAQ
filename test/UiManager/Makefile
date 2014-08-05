CC = g++
CFLAGS = -I../../source/include $(shell root-config --cflags) -g
LDFLAGS = $(shell root-config --libs)

all: server client

RootDict.cxx: ../../source/include/LinkDef.hh
	rootcint -f $@ -c $^
	sed -i '1i#include <map>' $(subst .cxx,.h,$@)

server: server.cc ../../source/src/UiManager.cc ../../source/src/HistoManager.cc RootDict.cxx
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) -lrt

client: client.cc RootDict.cxx
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f *.o *.d *.so *~ server client RootDict.*
