#include "astnode.h"

#include <iostream>

using std::cout;
using std::endl;

ASTNode::ASTNode(ASTType asttype)
{
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
    cout << token_.str_ << " "; // preorder
    for (int i=0 ; i < children_.size() ; ++i)
    {
      children_[i]->print();
    }
    // cout << token_.str_ << " "; // postorder 
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
