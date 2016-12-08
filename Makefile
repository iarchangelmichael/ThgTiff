CC=gcc
CXX=g++
RM=rm -f
SOURCE=thgtiff.cpp
CXXFLAGS= ${HLFLAGS} -std=c++11

all: $(SOURCE)
	$(CXX) $(SOURCE) $(CXXFLAGS) -o thgtiff

clean:
	rm ./thgtiff