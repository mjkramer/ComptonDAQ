#Makefile
#by Christian Dorfer, christian.dorfer@mail.ru

# Suppress display of executed commands.
$(VERBOSE).SILENT:

#Paths for files
SRC_DIR = source/src
DRIVER_DIR = source/drivers
INCLUDES = -I source/include
TEMP = source/temp


# Compiler
CC = gcc
CXX = g++

#Compiler Flags
#CXX_FLAGS = -I$(INCLUDE_DIR)

# ROOT flags and libs (OSX)
#ROOTCFLAGS := -I /Applications/root/include
#ROOTLIBS   := -L /Applications/root/lib -lGpad -lHist -lGraf -lGraf3d -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lRIO -lNet -lThread -lCore -lCint -ldl
#end (OSX)

# ROOT flags and libs (Scientific Linux)
ifdef ROOTSYS
ROOTCFLAGS := $(shell  $(ROOTSYS)/bin/root-config --cflags)
ROOTCFLAGS += -DHAVE_ROOT -DUSE_ROOT
ROOTLIBS   := $(shell  $(ROOTSYS)/bin/root-config --libs) -Wl,-rpath,$(ROOTSYS)/lib
ROOTLIBS   += -lThread
endif

#names of the targets
OBJS = $(TEMP)/DataAcquistion.o $(TEMP)/HistoManager.o $(TEMP)/ModuleManager.o $(TEMP)/VisManager.o $(TEMP)/main.o
OBJS += $(TEMP)/ModuleManager.o $(TEMP)/DataBlock.o $(TEMP)/v2718.o $(TEMP)/v1720.o

LIBS = -lm -lz -lutil -lnsl -pthread -lrt -lCAENVME -lCAENComm


all: main

main: $(OBJS)
	$(CXX) -o $@ $^ $(INCLUDES) $(ROOTCFLAGS) $(ROOTLIBS) 
	@echo Linking...
	@echo DONE!
	@echo

$(TEMP)/main.o: main.cc
	$(CXX) -c $< -o $@ $(INCLUDES) $(LIBS)
	@echo Compiling $<...


$(TEMP)/DataAcquistion.o: $(SRC_DIR)/DataAcquistion.cc
	$(CXX) -c $< -o $@ $(INCLUDES)
	@echo Compiling $<...

$(TEMP)/HistoManager.o: $(SRC_DIR)/HistoManager.cc
	$(CXX) -c $< -o $@ $(INCLUDES) $(ROOTCFLAGS)
	@echo Compiling $<...

$(TEMP)/ModuleManager.o: $(SRC_DIR)/ModuleManager.cc
	$(CXX) -c $< -o $@ $(INCLUDES)
	@echo Compiling $<...

$(TEMP)/VisManager.o: $(SRC_DIR)/VisManager.cc
	$(CXX) -c $< -o $@ $(INCLUDES)
	@echo Compiling $<...

$(TEMP)/DataBlock.o: $(SRC_DIR)/DataBlock.cc
	$(CXX) -c $< -o $@ $(INCLUDES)
	@echo Compiling $<...

#Driver files for each card
$(TEMP)/v2718.o: $(DRIVER_DIR)/v2718.cc
	$(CXX) -c $< -o $@ $(INCLUDES) $(LIBS)
	@echo Compiling $<...

$(TEMP)/v1720.o: $(DRIVER_DIR)/v1720.cc
	$(CXX) -c $< -o $@ $(INCLUDES) $(LIBS)
	@echo Compiling $<...

.PHONY: clean
clean:
	rm -f *.o *~ \*
	rm -r source/temp/*.o
	rm -f main
