all: vype

parser.tab.c parser.tab.h: parser.y table_symbols.h
	bison -d --debug parser.y

parser.tab.o: parser.tab.c table_symbols.h
	g++ -c parser.tab.c

scanner.cpp: scanner.l parser.tab.h	
	flex -o scanner.cpp scanner.l

scanner.o: scanner.cpp parser.tab.h table_symbols.h
	g++ -c scanner.cpp

semantic.o: semantic.cpp semantic.h table_symbols.h
	g++ -c semantic.cpp

error.o: error.cpp error.h
	g++ -c error.cpp

main.o: main.cpp parser.tab.h table_symbols.h error.h
	c++ -c main.cpp

vype: scanner.o parser.tab.o semantic.o error.o main.o
	g++ parser.tab.o scanner.o semantic.o error.o main.o -o vype

clean:
	rm vype parser.tab.c parser.tab.h scanner.cpp
	rm parser.tab.o scanner.o semantic.o error.o main.o
	rm out.asm