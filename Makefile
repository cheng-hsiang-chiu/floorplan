inputname = circuit3

inputfile := ./circuits/$(inputname).txt

outputfile := ./$(inputname)_sol.txt

all: floorplan execution

floorplan: floorplan.cpp
	g++ -std=c++17 floorplan.cpp -O3 -o floorplan

execution: floorplan
	./floorplan $(inputfile) $(outputfile)

clean:
	rm -f floorplan
