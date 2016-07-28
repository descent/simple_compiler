#ifndef TOKEN_H
#define TOKEN_H

#include <string>

#include "mytype.h"

class Token
{
  public:
    Token(const std::string str, ASTType ast_type=INVALID);
    Token(){ast_type_ = INVALID;}
    bool valid() const
    {
      if (ast_type_ == INVALID)
        return false;
      else
        return true;
    }
    const std::string& str() const
    {
      return str_;
    }
    ASTType ast_type() const
    {
      return ast_type_;
    }

    std::string str_;
    ASTType ast_type_;
  private:
};

extern Token invalid_token;
extern Token then_block;
extern Token else_block;
extern Token var_token;
extern Token para_token;
extern Token func_token;
extern Token func_call_token;
extern Token func_body_token;
extern Token prog_token;
extern Token true_token;
extern Token false_token;
extern Token while_token;
extern Token return_token;

#endif
