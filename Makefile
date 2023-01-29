all: compile run

compile:
	rm -f *.o
	rm -f bin/main
	rm -f src/tempCodeRunnerFile.cpp
	g++ -c src/*.cpp include/lib/jsoncpp.cpp -I"include" -I"include/lib" -I"include/pch" 
	g++ *.o -o bin/main -lsfml-graphics -lsfml-window -lsfml-system 
	rm -f *.o 

static:
	if exist *.o (del *.o)
	if exist bin/main (del bin/main)
	if exist include/pch/pch.hpp.gch (del include/pch/pch.hpp.gch)
	g++ -c src/*.cpp -I"C:/2.Perso/Programation/C++/.Libraries/SFML-2.5.1/include" -I"include" -I"include/pch" -DSFML_STATIC --static
	g++ *.o -o bin/release/main -L"C:/2.Perso/Programation/C++/.Libraries/SFML-2.5.1/lib" -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lfreetype -lwinmm -lgdi32 -mwindows -static-libgcc -static-libstdc++ --static
	del *.o 

pch:
	g++ include/pch/pch.hpp -I"include/lib" -I"C:/2.Perso/Programation/C++/.Libraries/SFML-2.5.1/include" 

class:
	python creat_class.py

del_pch:
	del include/pch/pch.hpp.gch

del_obj:
	if exist *.o (del *.o)

del_exe:
	if exist bin/main (del bin/main)

run:
	bin/main
