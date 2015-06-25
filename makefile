emag: field2.o main.o matrix.o
	g++ -std=c++11 -O3 field2.o main.o matrix.o -o emag

matrix.o: matrix.cc matrix.h
	g++ -std=c++11 -O3 -c matrix.cc

field.o: field2.cc field2.h 
	g++ -std=c++11 -O3 -c field2.cc 

main.o: main.cc field2.o
	g++ -std=c++11 -Wall -c main.cc
