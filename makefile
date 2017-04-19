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


CXX=g++

OBJS = c_parser.o astnode.o  env.o  lexer.o  op.o  symbol_table.o  token.o

all: 
	make DPARSER=1 c_parser

parser: parser.o astnode.o token.o lexer.o op.o
	$(CXX) $(CXXFLAGS) -o $@ $^

parser.o: parser.cpp astnode.h mytype.h token.h parser.h op.h lexer.h
	$(CXX) $(CXXFLAGS) -c $<

parser_4op: parser_4op.o astnode.o token.o lexer.o op.o
	$(CXX) $(CXXFLAGS) -o $@ $^
parser_4op.o: parser_4op.cpp astnode.h mytype.h token.h parser.h op.h
	$(CXX) $(CXXFLAGS) -c $<

c_parser: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

dep: $(OBJS:.o=.d)

%.d: %.cpp
	$(CXX) $(CXXFLAGS) -MM -MF $@ $<

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

include $(OBJS:.o=.d)
	

doc_html:
	make -C doc
clean:
	rm parser lexer *.o *.d ; make -C doc clean
