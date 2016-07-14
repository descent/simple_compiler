#ifndef ASTNODE_H
#define ASTNODE_H

#include <vector>
#include <iostream>

using std::cout;
using std::endl;

#include "mytype.h"
#include "token.h"

//using std::vector;

/**
 * \brief record ASTNode real type. ex: int, int pointer, char, function ...
 */
struct ObjType
{
  public:
    ObjType()
    {
      clear();
    }
    void clear_pointer()
    {
      pointer_number_ = 0;
      pointer_ = false;
    }
    void clear()
    {
      pointer_ = char_ = int_ = func_ = array_ = global_ = false;
      pointer_number_ = 0;
    }
    std::string str();
    void set_int(){int_ = true;}
    void set_char() {char_ = true;}
    void set_func() {func_ = true;}
    void set_global() {global_ = true;}
    void set_pointer(int num=1) 
    {
      pointer_number_ = num;
      pointer_ = true;
    }

    bool pointer_;
    bool char_;
    bool int_;
    bool func_;
    bool array_;
    bool global_;
    u32 pointer_number_;
};

class ASTNode
{
  public:
    //ASTNode(const ASTNode* l, const ASTNode* op, const ASTNode* r);
    ASTNode()
    {
      id_ = no_;
      ++no_;
      eval_result_ = 0;
    }
    ASTNode(const Token &token):token_(token)
    {
      id_ = no_;
      ++no_;
      eval_result_ = 0;
    }
    ~ASTNode()
    {
      static u32 i=0;
      cout << "~ASTNode: " << id_ << ": " << str() << endl;
      ++i;
      free_children();
      // delete eval_result_; // need not delete eval_result_;
    }
    bool add_child(const std::vector<ASTNode*> &children)
    {
      for (auto i : children)
        children_.push_back(i);
      return true;
    }
    void set_token(const Token &t)
    {
      token_ = t;
    }
    bool add_child(ASTNode* l) // for - (NEG), ex: -52
    {
      children_.push_back(l);
      return true;
    }
    bool add_child(ASTNode* l, ASTNode* r)
    {
      children_.push_back(l);
      children_.push_back(r);
      return true;
    }
    ASTNode* eval();
    ASTType ast_type() const
    {
      return token_.ast_type();
    }
    const char* type_str() const
    {
      const char *type_str[]=
      {
        "INVALID", "ROOT", 
        "TRUE", "FALSE", 
        "ENUM", "INT", "CHAR", 
        "IF", "WHILE",
        "THEN_BLOCK", "ELSE_BLOCK", "SEP", "NEG", "ASSIGN", "EQUAL", 
        "NAME", "VAR", "FUNC_NAME", 
        "ADD", "MIN", "MUL", "DIV", "GREAT", "LESS", 
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
    const std::vector<ASTNode*> &children() const
    {
      return children_;
    }

    // for tool tree to print AST
    void print_tree();
    void print();
    void set_obj_type(const ObjType &obj_type)
    {
      obj_type_ = obj_type;
    }
    const std::string str() const
    {
      return token_.str_;
    }
    void set_str(const std::string &s)
    {
      token_.str_ = s;
    }
    bool is_leaf() const
    {
      if (children().size() == 0)
        return true;
      else
        return false;
    }
    void free_children()
    {
    #if 1
      for (auto &i : children_)
        delete i;
    #endif
      children_.resize(0);
    }
    u32 num() const
    {
      return no_;
    }
  private:
    std::vector<ASTNode*> children_;
    ASTNode* eval_result_;
    Token token_;
    ObjType obj_type_;
    u32 id_;
    static u32 no_;
};

ASTNode *get_root();
void print_ast();

#endif
