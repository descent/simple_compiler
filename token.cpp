#include "token.h"

Token::Token(const std::string str, ASTType ast_type) : str_(str), ast_type_(ast_type)
{
}

Token invalid_token("invalid_token");
Token then_block("then_block", THEN_BLOCK);
Token else_block("else_block", ELSE_BLOCK);
Token g_var_token("g_var", GLOBAL_VAR);
Token var_token("var", VAR);
Token para_token("para", VAR);
Token func_token("func", FUNC_NAME);
Token func_body_token("func_body", FUNC_BODY);
Token func_call_token("func call", FUNC_CALL);
Token prog_token("prog", PROG);
Token true_token("t", TRUE);
Token false_token("f", FALSE);
Token while_token("while_block", WHILE_BLOCK);
Token return_token("return", RETURN);
