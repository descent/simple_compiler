#include "token.h"

Token::Token(const std::string str, ASTType type) : str_(str), type_(type)
{
}

Token invalid_token("invalid_token");
Token then_block("then_block", THEN_BLOCK);
Token else_block("else_block", ELSE_BLOCK);
Token var_token("var", VAR);
Token prog_token("prog", VAR);
