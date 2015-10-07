CXX = g++
DIR1D = ./field1D
BUILD = ./build
CXXFLAGS = -std=c++11 -O2 -I $(DIR1D) -c


all: emag

emag: $(BUILD)/field2.o $(BUILD)/main.o $(BUILD)/matrix.o $(BUILD)/pml.o $(BUILD)/field.o $(BUILD)/tfsf.o
	$(CXX) -std=c++11 -O2 $(BUILD)/field2.o $(BUILD)/main.o $(BUILD)/matrix.o $(BUILD)/pml.o $(BUILD)/tfsf.o $(BUILD)/field.o -o emag
	mv *.o $(BUILD)

$(BUILD)/matrix.o: matrix.cc matrix.h
	$(CXX) $(CXXFLAGS) matrix.cc
	mv matrix.o $(BUILD)

$(BUILD)/field2.o: field2.cc field2.h 
	$(CXX) $(CXXFLAGS) field2.cc 
	mv field2.o $(BUILD)

$(BUILD)/field.o: field1D/field.cc field1D/field.h
	$(CXX) $(CXXFLAGS) field1D/field.cc
	mv field.o $(BUILD)

$(BUILD)/main.o: main.cc field2.o
	$(CXX) $(CXXFLAGS) main.cc
	mv main.o $(BUILD)

$(BUILD)/pml.o: pml.cc field2.h
	$(CXX) $(CXXFLAGS) pml.cc
	mv pml.o $(BUILD)

$(BUILD)/tfsf.o: tfsf.cc field1D/field.h
	$(CXX) $(CXXFLAGS) tfsf.cc
	mv tfsf.o $(BUILD)

clean:
	rm $(BUILD)/*.o
	rm emag
