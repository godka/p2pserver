CXX=g++
CC=gcc
CPPFLAGS=-I. -I/usr/include/
CPPSOURCES=$(wildcard *.cpp)
CPPOBJECTS=$(CPPSOURCES:.cpp=.o)
CSOURCES=$(wildcard *.c)
COBJECTS=$(CSOURCES:.c=.o)
TARGET=mythP2PServer

.PHONY: all clean

all: .dcpp .dc $(CPPSOURCES) $(CSOURCES) $(TARGET)

.dcpp: $(CPPSOURCES)
	$(CXX) $(CPPFLAGS) -MM $(CPPSOURCES) >.dcpp
.dc: $(CSOURCES)
	$(CC) $(CFLAGS) -MM $(CSOURCES) >.dc
-include .dcpp
-include .dc
$(TARGET): $(CPPOBJECTS) $(COBJECTS)
	$(CXX) $(CPPOBJECTS) $(COBJECTS) -o $@
clean:
	rm $(CPPOBJECTS) $(COBJECTS) .dcpp .dc
install:
	cp $(TARGET) /usr/local/bin
