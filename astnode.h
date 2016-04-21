#ifndef ASTNODE_H
#define ASTNODE_H

#include <vector>

#include "mytype.h"
#include "token.h"

//using std::vector;


class ASTNode
{
  public:
    ASTNode(ASTType asttype);
    //ASTNode(const ASTNode* l, const ASTNode* op, const ASTNode* r);
    ASTNode(const Token &token):token_(token)
    {
      type_ = token_.type_;
    }
    bool add_child(ASTNode* l, ASTNode* r)
    {
      children_.push_back(l);
      children_.push_back(r);
      return true;
    }
    ASTType type() const
    {
      return type_;
    }
    const char* type_str() const
    {
      const char *type_str[]={"INVALID", "SEP", "NAME", "ADD", "MIN", "MUL", "DIV", "GREAT", "NUMBER"};

      if (INVALID <= type_ && type_ < LAST)
      {
        return type_str[type_];
      }
      else
      {
        return "unknown";
      }
    }
    void print();
  private:
    ASTType type_;
    std::vector<ASTNode*> children_;
    Token token_;
    
};

#endif
