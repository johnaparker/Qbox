CXX = g++
CXXFLAGS = -std=c++11 -O2 -c

all: emag

emag: field2.o main.o matrix.o pml.o field.o tfsf.o
	$(CXX) -std=c++11 -O2 field2.o main.o matrix.o pml.o tfsf.o field.o -o emag

matrix.o: matrix.cc matrix.h
	$(CXX) $(CXXFLAGS) matrix.cc

field2.o: field2.cc field2.h 
	$(CXX) $(CXXFLAGS) -I/field1D field2.cc 

field.o: field1D/field.cc field1D/field.h
	$(CXX) $(CXXFLAGS) -I/field1D field1D/field.cc

main.o: main.cc field2.o
	$(CXX) $(CXXFLAGS) main.cc

pml.o: pml.cc field2.h
	$(CXX) $(CXXFLAGS) pml.cc

tfsf.o: tfsf.cc field1D/field.h
	$(CXX) $(CXXFLAGS) -I/field1D tfsf.cc

clean:
	rm *.o
	rm emag
