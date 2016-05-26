#ifndef TOKEN_H
#define TOKEN_H

#include <string>

#include "mytype.h"

class Token
{
  public:
    Token(const std::string str, ASTType type=INVALID);
    Token(){type_ = INVALID;}
    bool valid() const
    {
      if (type_ == INVALID)
        return false;
      else
        return true;
    }
    const std::string& str() const
    {
      return str_;
    }
    ASTType type() const
    {
      return type_;
    }

    std::string str_;
    ASTType type_;
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
#endif
