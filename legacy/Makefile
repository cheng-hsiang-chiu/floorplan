inputname = circuit1

inputfile := ./circuits/$(inputname).txt

outputfile := ./$(inputname)_sol.txt

outputfilejson := ./$(inputname)_sol.json

all: floorplan execution

floorplan: floorplan.cpp
	g++ -std=c++17 floorplan.cpp -O3 -o floorplan

# third argument is iteration
# forth argument is temperature
# fifth argument is FROZEN
execution: floorplan
	./floorplan $(inputfile) $(outputfile) 1000 100.0 0.01
	#cat $(outputfilejson)

clean:
	rm -f floorplan
