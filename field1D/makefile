emag: field.o main.o
	g++ -std=c++11 -O3 field.o main.o -o emag

field.o: field.cc field.h
	g++ -std=c++11 -O3 -c field.cc

main.o: main.cc field.o
	g++ -std=c++11 -O3 -c main.cc
