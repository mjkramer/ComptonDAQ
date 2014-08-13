CC = g++

INCLUDES = -I include -I ../../source/include

all: daqctl

RootDict.cxx: ../../source/include/LinkDef.hh
	rootcint -f $@ -c $^
	sed -i '1i#include <map>' RootDict.h

daqctl: main.cc src/Client.cc src/ArgParser.cc RootDict.cxx
	$(CC) -std=c++0x $^ -o $@ $(INCLUDES) `root-config --libs --cflags`

.PHONY: clean
clean:
	@rm -f daqctl RootDict.* *~
