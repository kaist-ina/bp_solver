all:
	g++ -O3 -g solver.cpp Edge.cpp Node.cpp -o solver -lpthread -lrt
clean:
	rm solver
