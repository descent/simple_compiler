#include "astnode.h"

#include <iostream>
#include <map>

#define PREORDER

using std::cout;
using std::endl;

#define PRINT_TREE_STRING

std::map<std::string, ASTNode *> env;

void print_ast()
{
  ASTNode *root = get_root();
  cout << "===========================" << endl;
#ifdef PRINT_TREE_STRING
  cout << "\\tree";
  root->print_tree();
  cout << endl;
#else
  cout << "ast node type: " << root->type_str() << endl;
  root->print();
  cout << endl;
#endif
  cout << "---------------------------" << endl;
}

std::string ObjType::str()
{
  std::string s;
  if (pointer_)
    s += " |ptr<" + std::to_string(pointer_number_) + ">";
  if(char_)
    s += " |char";
  if(int_)
    s += " |int";
  if(func_)
    s += " |func";
  if(array_)
    s += " |array";
  if(global_)
    s += " |global";
  return s;
}

void ASTNode::print_tree()
{
  if (children_.size() == 0) // leaf node
  {
    cout << "(" << token_.str_;
    if (ast_type() == NAME)
      cout << obj_type_.str();

    cout << ")";
  }
  else
  {
    cout << "( " << token_.str_;
    if (ast_type() == NAME)
      cout << obj_type_.str();
    //for (std::vector<ASTNode*>::size_type i=0 ; i < children_.size() ; ++i)
    for (auto i : children_)
    {
      i->print_tree();
    }
    cout << ")";
  }
}

void ASTNode::print()
{
  if (children_.size() == 0) // leaf node
  {
    cout << token_.str_ << " ";
  }
  else
  {
    cout << "( ";
#ifdef PREORDER
    cout << token_.str_ << " "; // preorder
#endif
    for (std::vector<ASTNode*>::size_type i=0 ; i < children_.size() ; ++i)
    {
      children_[i]->print();
#ifdef INORDER
      if (i == 0)
        cout << token_.str_ << " "; // inorder
#endif
    }
#ifdef POSTORDER
    cout << token_.str_ << " "; // postorder 
#endif
    cout << ")";
  #if 0
    switch (type())
    {
      case ADD:
      {
        cout << "(";
        children_[0]->print();
        cout << " + ";
        children_[1]->print();
        cout << ")" << endl;
      }
      case MUL:
      {
        cout << "(";
        children_[0]->print();
        cout << " * ";
        children_[1]->print();
        cout << ")" << endl;
      }
    }
  #endif
  }
}

ASTNode* ASTNode::eval()
{
  if (children_.size() == 0) // leaf node
  {
    if (ast_type() == NAME)
    {
      //env.find(str());
      if (env.count(str()))
      {
        ASTNode *n = env[str()];
        cout << "n: " << n->str() << endl;
        return env[str()];
      }
      else
        return this;
    }
    else
      return this;
  }
  else
  {
    for (const auto &i : children_)
    {
      bool leaf = i->is_leaf();
      i->eval();
      if (leaf == false)
        print_ast();

    }
    if (token_.str_ == "+" || token_.str_ == "-" || token_.str_ == "*")
    {
      if (children_.size() == 2)
      {
        ASTNode *c1 = children_[0]->eval();
        ASTNode *c2 = children_[1]->eval();

        int n1 = stoi(c1->str());
        int n2 = stoi(c2->str());
        int ret = 0;
        if (token_.str_ == "+")
          ret = n1 + n2;
        if (token_.str_ == "-")
          ret = n1 - n2;
        if (token_.str_ == "*")
          ret = n1 * n2;

        printf("ret: %d, n1: %d, n2: %d\n", ret, n1, n2);
        set_str(std::to_string(ret));
        free_children();

#if 0
        Token t;
        t.str_ = std::to_string(ret);
        ASTNode *tmp = new ASTNode(t);
        *this = *tmp;
        //return new ASTNode(t);
#endif
        return this;
      }
      else
      {
        printf("children_.size() != 2, %ld\n", children_.size());
      }
    }
    else if (str() == "=")
         {
           cout << "op is =" << endl;
           // add var/val to env
           ASTNode *c1 = children_[0]->eval();
           ASTNode *c2 = children_[1]->eval();
           env.insert({c1->str(), c2});
           //free_children();
           //delete this;
           // how to free myself
         }
  }
}

#if 0
ASTNode::ASTNode(const ASTNode* l, const ASTNode* op, const ASTNode* r)
{
    vector<ASTNode*> children_;
}
#endif
