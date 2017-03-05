#ifndef ASTNODE_H
#define ASTNODE_H

#include <vector>
#include <iostream>

using namespace std;

#include "symbol_table.h"
#include "mytype.h"
#include "token.h"
#include "env.h"

typedef int (*GenGasFunc)();

class GenLabel
{
  public:
    GenLabel(const string &prefix, u32 idx=0): prefix_(prefix)
    {
      index_ = idx;
    }
    string operator() ()
    {
      ++index_;
      return prefix_+std::to_string(index_);
    }
  private:
    string prefix_;
    u32 index_;
};

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
    bool is_int() const
    {
      if (int_)
        return true;
      else
        return false;
    }
    bool is_pointer() const
    {
      if (pointer_ == true)
        return true;
      else
        return false;
    }
    bool is_char() const
    {
      if (char_ == true)
        return true;
      else
        return false;
    }

    bool is_string() const
    {
      if (pointer_ == true && char_ == true)
        return true;
      else
        return false;
    }
    bool set_func_para(bool fp) // fp: function parameter
    {
      func_para_ = fp;
    }
    bool is_func_para() // function parameter
    {
      return func_para_;
    }

    bool pointer_;
    bool char_;
    bool int_;
    bool func_;
    bool array_;
    bool global_;
    u32 pointer_number_;
    bool func_para_; // function parameter
};

typedef void (ASTNode::*GenGasOp)(const string &reg);

class ASTNode
{
  public:
    //ASTNode(const ASTNode* l, const ASTNode* op, const ASTNode* r);
    ASTNode()
    {
      init();
    }
    ASTNode(const Token &token):token_(token)
    {
      init();
    }
    ~ASTNode()
    {
      static u32 i=0;
      //cout << "~ASTNode: " << id_ << ": " << str() << endl;
      ++i;
      free_children();
      // delete eval_result_; // need not delete eval_result_;
    }
    bool is_op()
    {
      if (is_mul_div() == true || is_add_sub() == true || is_relational_op() == true)
        return true;
      else
        return false;
    }

    bool is_relational_op() const
    {
      if (LESS == ast_type() || GREAT == ast_type() || EQUAL == ast_type())
      {
        return true;
      }
      else
      {
        return false;
      }
    }

    bool is_mul_div()
    {
      if (MUL == ast_type() || DIV == ast_type())
        return true;
      else
        return false;
    }
    bool is_add_sub()
    {
      if (ADD == ast_type() || SUB == ast_type())
        return true;
      else
        return false;
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
    //void code_gen();
    void gen_gas_syntax();
    void gen_gas_add_sub(const string &reg);
    void gen_gas_mul_div(const string &reg);
    void gen_gas_relation(const string &reg);
    void gen_gas_op(GenGasFunc gen_gas_func);
    ASTNode* eval(Environment *env);
    void set_ast_type(ASTType ast_type)
    {
      token_.ast_type_ = ast_type;
    }
    ASTType ast_type() const
    {
      return token_.ast_type();
    }
    const char* type_str() const
    {
      return token_.ast_type_str();
    #if 0
      const char *type_str[]=
      {
        "INVALID", "ROOT", "PROG", "COMMENT", 
        "TRUE", "FALSE", 
        "ENUM", "INT", "CHAR", 
        "ADDR_OF", "DEREF", // &, *
        "IF", "WHILE", "RETURN", "RETURN_VAL", 
        "THEN_BLOCK", "ELSE_BLOCK", "SEP", "NEG", "ASSIGN", "EQUAL", 
        "NAME", "GLOBAL_VAR", "VAR", 
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
    #endif
    }

    const std::vector<ASTNode*> &children() const
    {
      return children_;
    }

    // for tool tree to print AST
    void print_tree();
    void print();
    const ObjType & obj_type() const
    {
      return obj_type_;
    }
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

    ASTNode *left_child() // don't do check, before call it, need call is_right_op() to check
    {
      return children_[0];
    }
    ASTNode *right_child() // don't do check
    {
      return children_[1];
    }

    bool is_valid_op() const // has 2 children is valid
    {
      if (2 == children().size())
        return true;
      else
        return false;
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
    u16 occupy_size()
    {
      if (obj_type_.is_int()) return 4;
      if (obj_type_.is_pointer()) return 4;
      if (obj_type_.is_char()) return 1;

      return 0; // the node is not variable
    }
    string string_label_;
    string local_var_addr_; // ex: -4(%epb)
  private:
    void init();
    vector<ASTNode*> children_;
    ASTNode* eval_result_;
    Token token_;
    ObjType obj_type_;
    u32 id_;
    static u32 no_;
    CodeGenState code_gen_state_;
    int offset_;
    int var_num_;
    map <ASTType, GenGasOp> gen_gas_op_;
};

ASTNode *get_root();
void print_ast();

#endif
