all:
	bison -d -o minilisp.tab.cc minilisp.yy
	g++ -c -g -I.. minilisp.tab.cc
	flex -o lex.yy.cc minilisp.l
	g++ -c -g -I.. lex.yy.cc
	g++ -o minilisp minilisp.tab.o lex.yy.o -ll