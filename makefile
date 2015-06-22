emag: field2.o main.o
	g++ -std=c++11 -Wall field2.o main.o -o emag

field.o: field2.cc field2.h
	g++ -std=c++11 -Wall -c field2.cc

main.o: main.cc field2.o
	g++ -std=c++11 -Wall -c main.cc
