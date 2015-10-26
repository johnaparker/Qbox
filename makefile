CXX = h5c++
DIR1D = ./field1D
BUILD = ./build
CXXFLAGS = -std=c++11 -O2 -I $(DIR1D)  -c


all: emag

emag: $(BUILD)/field2.o $(BUILD)/main.o $(BUILD)/pml.o $(BUILD)/field.o $(BUILD)/tfsf.o $(BUILD)/object.o $(BUILD)/source.o $(BUILD)/h5out.o
	$(CXX) -std=c++11 -O2 $(BUILD)/field2.o $(BUILD)/main.o $(BUILD)/pml.o $(BUILD)/tfsf.o $(BUILD)/field.o $(BUILD)/object.o $(BUILD)/source.o $(BUILD)/h5out.o -o emag
	mv *.o $(BUILD)

$(BUILD)/field2.o: field2.cc field2.h 
	$(CXX) $(CXXFLAGS) field2.cc 
	mv field2.o $(BUILD)

$(BUILD)/object.o: object.cc object.h
	$(CXX) $(CXXFLAGS) object.cc 
	mv object.o $(BUILD)

$(BUILD)/source.o: source.cc source.h
	$(CXX) $(CXXFLAGS) source.cc 
	mv source.o $(BUILD)

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

$(BUILD)/h5out.o: h5out.cc
	$(CXX) $(CXXFLAGS) h5out.cc
	mv h5out.o $(BUILD)

clean:
	rm $(BUILD)/*.o
	rm emag
