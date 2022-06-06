FLAGS= -Wall -pedantic -std=c++17 -iquote inc

output: obj/main.o obj/macierz_graf.o obj/lista_graf.o obj/Bellman.o
	g++ obj/main.o obj/macierz_graf.o obj/lista_graf.o obj/Bellman.o -o output
	

obj:
	mkdir obj
	
obj/main.o: src/Main.cpp
	g++ -c ${FLAGS} -o obj/main.o src/Main.cpp	
	
obj/macierz_graf.o: src/macierz_graf.cpp inc/Macierz_graf.hpp
	g++ -c ${FLAGS} -o obj/macierz_graf.o src/macierz_graf.cpp

obj/lista_graf.o: src/lista_graf.cpp inc/Lista_Graf.hpp
	g++ -c ${FLAGS} -o obj/lista_graf.o src/lista_graf.cpp
	
obj/Bellman.o: src/Bellman.cpp inc/Bellman.hpp
	g++ -c ${FLAGS} -o obj/Bellman.o src/Bellman.cpp
					
clean:
	rm obj/*.o output

