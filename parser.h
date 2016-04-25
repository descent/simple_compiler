#ifndef PARSER_H
#define PARSER_H

#include "astnode.h"
#include "op.h"

ASTNode* expression();
ASTNode* term();
ASTNode* factor();
ASTNode* expr();
Precedence* next_op();
ASTNode* do_shift(ASTNode* l, int prec);
ASTNode* statement();

#endif
