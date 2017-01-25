# test parser
# make clean; make DPARSER=1 c_parser
# make clean; make DPARSER=1 
# make clean; make DPARSER=1 parser_4op

# test lexer
# make clean; make DLEXER=1 lexer

CXXFLAGS=-g -std=c++14 -Wall -m32
ifdef DLEXER
CXXFLAGS+= -DDEBUG_LEXER
endif

ifdef DPARSER
CXXFLAGS+= -DDEBUG_PARSER
endif

all: 
	make DPARSER=1 c_parser


c_parser: c_parser.o astnode.o token.o lexer.o op.o env.o symbol_table.o
	$(CXX) $(CXXFLAGS) -o $@ $^

parser: parser.o astnode.o token.o lexer.o op.o
	$(CXX) $(CXXFLAGS) -o $@ $^

parser.o: parser.cpp astnode.h mytype.h token.h parser.h op.h lexer.h
	$(CXX) $(CXXFLAGS) -c $<

c_parser.o: c_parser.cpp astnode.h mytype.h token.h parser.h op.h lexer.h
	$(CXX) $(CXXFLAGS) -c $<

parser_4op: parser_4op.o astnode.o token.o lexer.o op.o
	$(CXX) $(CXXFLAGS) -o $@ $^
parser_4op.o: parser_4op.cpp astnode.h mytype.h token.h parser.h op.h
	$(CXX) $(CXXFLAGS) -c $<
astnode.o: astnode.cpp astnode.h mytype.h token.h
	$(CXX) $(CXXFLAGS) -c $<

env.o: env.cpp env.h
	$(CXX) $(CXXFLAGS) -c $<

env: env.cpp 
	$(CXX) -DTEST_DEBUG $(CXXFLAGS) -o $@ $^

op.o: op.cpp 
	$(CXX) $(CXXFLAGS) -c $<

lexer: lexer.o token.o
	$(CXX) $(CXXFLAGS) -o $@ $<
lexer.o: lexer.cpp mytype.h token.h lexer.h
	$(CXX) $(CXXFLAGS) -c $<
token.o: token.cpp token.h mytype.h
	$(CXX) $(CXXFLAGS) -c $<

symbol_table.o: symbol_table.cpp symbol_table.h
	$(CXX) $(CXXFLAGS) -c $<

doc_html:
	make -C doc
clean:
	rm parser lexer *.o ; make -C doc clean
