#include "token.h"

Token::Token(const std::string str, ASTType type) : str_(str), type_(type)
{
}

Token invalid_token("invalid_token");
