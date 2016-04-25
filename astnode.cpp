#include "astnode.h"

#include <iostream>

#define PREORDER

using std::cout;
using std::endl;

ASTNode::ASTNode(ASTType asttype)
{
}

void ASTNode::print_tree()
{
  if (children_.size() == 0) // leaf node
  {
    cout << "(" << token_.str_ << ")";
  }
  else
  {
    cout << "( " << token_.str_;
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

#if 0
ASTNode::ASTNode(const ASTNode* l, const ASTNode* op, const ASTNode* r)
{
    vector<ASTNode*> children_;
}
#endif
