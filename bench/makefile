CPP = g++
CPPFLAGS = -O3 -std=c++11

IN=
OUT=$(IN:.cpp=)

all: $(OUT).sim

$(OUT).o: $(IN)
	$(CPP) $(CPPFLAGS) -c $(IN)

$(OUT).sim: $(OUT).o 
	$(CPP) $(CPPFLAGS) -o $(OUT).sim `pkg-config --cflags meep_mpi` $(OUT).o `pkg-config --libs meep_mpi` -lh5cpp

clean:
	rm -f $(OUT).o
	rm $(OUT).sim
