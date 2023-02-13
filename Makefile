all: compile run

compile:
	rm -f *.o
	rm -f bin/main
	g++ -c src/*.cpp include/lib/jsoncpp.cpp -I"include" -I"include/lib" -I"include/pch" 
	g++ *.o -o bin/main -lsfml-graphics -lsfml-window -lsfml-system 
	rm -f *.o 

pch:
	g++ include/pch/pch.hpp -I"include/lib" 

class:
	python3 creat_class.py

rm_pch:
	rm -f include/pch/pch.hpp.gch

rm_obj:
	rm -f *.o

rm_exe:
	rm -f bin/main

run:
	bin/main
