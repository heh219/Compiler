OPTS = -c -g -Wall -Werror

microc:microc.o parser.o lex.o token.o hashtable.o stack.o symboltable.o hash.o link.o
	g++ -o microc microc.o parser.o lex.o token.o hashtable.o stack.o symboltable.o hash.o link.o
token.o:token.h token.cc
	g++ $(OPTS) token.cc
lex.o:lex.h lex.cc token.h
	g++ $(OPTS) lex.cc
parser.o:parser.h parser.cc lex.h token.h symboltable.h
	g++ $(OPTS) parser.cc
microc.o:microc.cc token.h lex.h parser.h
	g++ $(OPTS) microc.cc
hash.o: hash.cc hash.h
	g++ $(OPTS) hash.cc
link.o: link.cc link.h
	g++ $(OPTS) link.cc
hashtable.o: hashtable.h hashtable.cc hash.h link.h
	g++ $(OPTS) hashtable.cc
stack.o:stack.h stack.cc
	g++ $(OPTS) stack.cc
symboltable.o:symboltable.h symboltable.cc hashtable.h stack.cc
	g++ $(OPTS) symboltable.cc
clean:
	rm microc *.o