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

    const char* ast_type_str() const
    {
      const char *type_str[]=
      {
        "INVALID", "ROOT", "PROG", "COMMENT",
        "TRUE", "FALSE",
        "ENUM", "INT", "CHAR", "S8",
        "ADDR_OF", "DEREF", // &, *
        "IF", "WHILE", "WHILE_BLOCK", "RETURN", "RETURN_VAL",
        "THEN_BLOCK", "ELSE_BLOCK", "SEP", "NEG", "ASSIGN", "EQUAL",
        "NAME", "GLOBAL_VAR", "VAR", "PARA"
        "FUNC_CALL", "FUNC_BODY", "FUNC_NAME", "FUNC_PARA",
        "ADD", "SUB", "MUL", "DIV", "GREAT", "LESS",
        "NUMBER", "STRING", "EOL"
      };

      if (INVALID <= ast_type() && ast_type() < LAST)
      {
        return type_str[ast_type()];
      }
      else
      {
        return "unknown";
      }
    }
    std::string str_;
    ASTType ast_type_;
  private:
};

extern Token invalid_token;
extern Token then_block;
extern Token else_block;
extern Token g_var_token;
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
