#Makefile
#by Christian Dorfer, christian.dorfer@mail.ru

# Suppress display of executed commands.
$(VERBOSE).SILENT:

#Paths for files
SRC_DIR = source/src
INCLUDE_DIR = source/include
DRIVER_DIR = source/drivers
INCLUDES = -I $(INCLUDE_DIR) -I /usr/include -I source/drivers
TEMP = source/temp

# Compiler
CC = gcc
CXX = g++

#Compiler Flags
CXX_FLAGS = -std=c++0x

# ROOT flags and libs (Scientific Linux)
ifdef ROOTSYS
ROOTCFLAGS := $(shell  $(ROOTSYS)/bin/root-config --cflags)
ROOTCFLAGS += -DHAVE_ROOT -DUSE_ROOT
ROOTLIBS   := $(shell  $(ROOTSYS)/bin/root-config --libs) -Wl,-rpath,$(ROOTSYS)/lib
ROOTLIBS   += -lThread
endif

#names of the targets
OBJS = $(TEMP)/DataAcquisition.o $(TEMP)/HistoManager.o $(TEMP)/ModuleManager.o $(TEMP)/UiManager.o $(TEMP)/main.o
OBJS += $(TEMP)/ModuleManager.o $(TEMP)/ConfigFileManager.o $(TEMP)/DataBlock.o $(TEMP)/v2718.o $(TEMP)/v1731.o  
OBJS += $(TEMP)/v1290N.o $(TEMP)/v1785.o

LIBS = -lm -lz -lutil -lnsl -pthread -lrt -lCAENVME -lCAENComm -lCAENDigitizer -lconfig++


all: main

main: $(OBJS)
	$(CXX) -o $@ $^ $(INCLUDES) $(ROOTCFLAGS) $(ROOTLIBS) $(LIBS) $(CXX_FLAGS)
	@echo Linking...
	@echo DONE!
	@echo

$(TEMP)/main.o: main.cc
	$(CXX) -c $< -o $@ $(INCLUDES) $(LIBS) $(CXX_FLAGS)
	@echo Compiling $<...

$(TEMP)/ConfigFileManager.o: $(SRC_DIR)/ConfigFileManager.cc
	$(CXX) -c $< -o $@ $(INCLUDES) $(CXX_FLAGS)
	@echo Compiling $<...

$(TEMP)/DataAcquisition.o: $(SRC_DIR)/DataAcquisition.cc
	$(CXX) -c $< -o $@ $(INCLUDES) $(CXX_FLAGS)
	@echo Compiling $<...

$(TEMP)/HistoManager.o: $(SRC_DIR)/HistoManager.cc
	$(CXX) -c $< -o $@ $(INCLUDES) $(ROOTCFLAGS) $(CXX_FLAGS)
	@echo Compiling $<...

$(TEMP)/ModuleManager.o: $(SRC_DIR)/ModuleManager.cc
	$(CXX) -c $< -o $@ $(INCLUDES) $(CXX_FLAGS)
	@echo Compiling $<...

$(TEMP)/RootDict.cxx: $(INCLUDE_DIR)/LinkDef.hh
	rootcint -f $@ -c $^
	sed -i '1i#include <map>' $(subst .cxx,.h,$@)

$(TEMP)/UiManager.o: $(SRC_DIR)/UiManager.cc $(TEMP)/RootDict.cxx
	$(CXX) -c $^ -o $@ $(INCLUDES) -I $(TEMP) $(CXX_FLAGS) $(ROOTCFLAGS)
	@echo Compiling $^...

$(TEMP)/DataBlock.o: $(SRC_DIR)/DataBlock.cc
	$(CXX) -c $< -o $@ $(INCLUDES) $(CXX_FLAGS)
	@echo Compiling $<...


#Driver files for each card
$(TEMP)/v2718.o: $(DRIVER_DIR)/v2718.cc
	$(CXX) -c $< -o $@ $(INCLUDES) $(LIBS) $(CXX_FLAGS)
	@echo Compiling $<...

$(TEMP)/v1731.o: $(DRIVER_DIR)/v1731.cc
	$(CXX) -c $< -o $@ $(INCLUDES) $(LIBS) $(CXX_FLAGS)
	@echo Compiling $<...

$(TEMP)/v1785.o: $(DRIVER_DIR)/v1785.cc
	$(CXX) -c $< -o $@ $(INCLUDES) $(LIBS) $(CXX_FLAGS)
	@echo Compiling $<...

$(TEMP)/v1290N.o: $(DRIVER_DIR)/v1290N.cc
	$(CXX) -c $< -o $@ $(INCLUDES) $(LIBS) $(CXX_FLAGS)
	@echo Compiling $<...

.PHONY: clean
clean:
	rm -f *.o *~ \*
	rm -r source/temp/*.o source/temp/RootDict.*
	rm -f main
