CXX = g++
CXXFLAGS = -std=c++11 -O2 -c

all: emag

emag: field2.o main.o matrix.o pml.o
	$(CXX) -std=c++11 -O2 field2.o main.o matrix.o pml.o -o emag

matrix.o: matrix.cc matrix.h
	$(CXX) $(CXXFLAGS) matrix.cc

field.o: field2.cc field2.h 
	$(CXX) $(CXXFLAGS) field2.cc 

main.o: main.cc field2.o
	$(CXX) $(CXXFLAGS) main.cc

pml.o: pml.cc field2.h
	$(CXX) $(CXXFLAGS) pml.cc

clean:
	rm *.o
	rm emag
