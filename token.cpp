#include "token.h"

Token::Token(const std::string str, ASTType ast_type) : str_(str), ast_type_(ast_type)
{
}

Token invalid_token("invalid_token");
Token then_block("then_block", THEN_BLOCK);
Token else_block("else_block", ELSE_BLOCK);
Token var_token("var", VAR);
Token para_token("para", VAR);
Token func_token("func", NAME);
Token func_body_token("func_body", VAR);
Token func_call_token("func call", FUNC_NAME);
Token prog_token("prog", VAR);
