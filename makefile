CXXFLAGS=-g -std=c++11
ifdef DLEXER
CXXFLAGS+= -DDEBUG_LEXER
endif

ifdef DPARSER
CXXFLAGS+= -DDEBUG_PARSER
endif


parser: parser.o astnode.o token.o lexer.o
	$(CXX) $(CXXFLAGS) -o $@ $^
parser.o: parser.cpp astnode.h mytype.h token.h parser.h
	$(CXX) $(CXXFLAGS) -c $<
astnode.o: astnode.cpp astnode.h mytype.h token.h
	$(CXX) $(CXXFLAGS) -c $<

lexer: lexer.o token.o
	$(CXX) $(CXXFLAGS) -o $@ $<
lexer.o: lexer.cpp mytype.h token.h
	$(CXX) $(CXXFLAGS) -c $<
token.o: token.cpp token.h mytype.h
	$(CXX) $(CXXFLAGS) -c $<
clean:
	rm parser lexer *.o
