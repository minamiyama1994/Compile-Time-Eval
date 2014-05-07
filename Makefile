CXXFLAGS=-I./include -I../FTMP/include -I../Sprout
all :
	$(CXX) $(CXXFLAGS) -std=c++1y -o test test.cpp