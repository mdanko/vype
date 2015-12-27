all: vype

parser.tab.c parser.tab.h: parser.y table_symbols.h
	bison -d --debug parser.y

parser.tab.o: parser.tab.c table_symbols.h
	g++ -c $<

scanner.cpp: scanner.l parser.tab.h	
	flex -o scanner.cpp scanner.l

scanner.o: scanner.cpp parser.tab.h table_symbols.h
	g++ -c $<

semantic.o: semantic.h semantic.cpp table_symbols.h
	g++ -c semantic.cpp

vype: scanner.o parser.tab.o semantic.o
	g++ parser.tab.o scanner.o semantic.o -lfl -o vype

clean:
	rm vype parser.tab.c parser.tab.h scanner.cpp
	rm parser.tab.o scanner.o semantic.o
