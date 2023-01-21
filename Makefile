all: compile run

compile:
	if exist *.o (del *.o)
	if exist bin\main.exe (del bin\main.exe)
	if exist src\tempCodeRunnerFile.cpp (del src\tempCodeRunnerFile.cpp)
	g++ -c src/*.cpp -I"include" -I"include\pch" -I"C:\2.Perso\Programation\C++\.Libraries\SFML-2.5.1\include"
	g++ *.o -o bin\main -L"C:\2.Perso\Programation\C++\.Libraries\SFML-2.5.1\lib" -lsfml-graphics -lsfml-window -lsfml-system 
	del *.o 

static:
	if exist *.o (del *.o)
	if exist bin\main.exe (del bin\main.exe)
	if exist include\pch\pch.hpp.gch (del include\pch\pch.hpp.gch)
	g++ -c src/*.cpp -I"C:\2.Perso\Programation\C++\.Libraries\SFML-2.5.1\include" -I"include" -I"include\pch" -DSFML_STATIC --static
	g++ *.o -o bin\release\main -L"C:\2.Perso\Programation\C++\.Libraries\SFML-2.5.1\lib" -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lfreetype -lwinmm -lgdi32 -mwindows -static-libgcc -static-libstdc++ --static
	del *.o 

pch:
	g++ include\pch\pch.hpp -I"C:\2.Perso\Programation\C++\.Libraries\SFML-2.5.1\include" 

class:
	python creat_class.py

del_pch:
	del include\pch\pch.hpp.gch

del_obj:
	if exist *.o (del *.o)

del_exe:
	if exist bin\main.exe (del bin\main.exe)

run:
	bin\main.exe