CC=gcc
CXX=g++
RM=rm -f
SOURCE=thgtiff.cpp
CXXFLAGS= ${HLFLAGS} -std=c++11 -ltiff -I"C:/Program Files (x86)/GnuWin32/include" -L"C:/Program Files (x86)/GnuWin32/lib"

all: $(SOURCE)
	$(CXX) $(SOURCE) $(CXXFLAGS) -o thgtiff

clean:
	rm ./thgtiff