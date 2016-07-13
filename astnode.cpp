#include "astnode.h"

#include <map>

#define PREORDER


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

u32 ASTNode::no_=0;

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
        cout << "eval name n: " << n->str() << endl;
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
    cout << "cur node: " << str() << endl;

    //for (const auto &i : children_)
    {

    if (token_.str_ == "+" || token_.str_ == "-" || token_.str_ == "*")
    {
      if (children_.size() == 2)
      {
        ASTNode *c1 = children_[0]->eval();
        cout << "xx cur: " << str() << endl;
        cout << "c1: " << c1->str() << endl;
        ASTNode *c2 = children_[1]->eval();
        cout << "c2: " << c2->str() << endl;

        if (children_[0]->eval_result_)
        {
          ASTNode *e = children_[0]->eval_result_;
          delete children_[0];
          children_[0] = e;
          print_ast();
        }

        if (children_[1]->eval_result_)
        {
          ASTNode *e = children_[1]->eval_result_;
          delete children_[1];
          children_[1] = e;
          print_ast();
        }

#if 0
        if (c1_leaf == false)
        { // replace child node by eval node;
          delete children_[0];
          children_[0] = c1;
          print_ast();
        }
        else if(c2_leaf == false)
             { // replace child node by eval node;
               delete children_[1];
               children_[1] = c2;
               print_ast();
             }

        //|| children_[1]->is_leaf() == false)
          //print_ast();
#endif
        int n1 = stoi(c1->str());
        int n2 = stoi(c2->str());
        int ret = 0;
        if (token_.str_ == "+")
          ret = n1 + n2;
        if (token_.str_ == "-")
          ret = n1 - n2;
        if (token_.str_ == "*")
          ret = n1 * n2;

        //printf("ret: %d = n1: %d %s n2: %d\n", ret, n1, str().c_str(), n2);
        cout << "eval: " << n1 << " " << str() << " " << n2 << " = " << ret << endl;

#if 0
        set_str(std::to_string(ret));
        free_children();
        return this;

#else
        Token t(std::to_string(ret), NUMBER);
        //t.str_ = std::to_string(ret);
        eval_result_ = new ASTNode(t);

        //*this = *tmp;
        return eval_result_;
#endif
      }
      else
      {
        cout << "children_.size() != 2, " << children_.size() << endl;
        return nullptr;
      }
    }
    else if (str() == "=")
         {
           cout << "op is =" << endl;
           // add var/val to env
           ASTNode *c1 = children_[0]->eval();
           ASTNode *c2 = children_[1]->eval();


           env.insert({c1->str(), c2});

           if (children_[0] != c1)
           {
             delete children_[0];
             children_[0] = c1;
             print_ast();
           }

           if (children_[1] != c2)
           {
             delete children_[1];
             children_[1] = c2;
             print_ast();
           }

           return this;

           //free_children();
           // delete this;
           // how to free myself
         }
         else if ((str() == "root"))
              {
                ASTNode *n=0;
                cout << " op: root" << endl;
                //cout << " i child: " << i->children()[0]->str() << endl;
                for (auto &i : children_)
                {
                  n = i->eval();
                  #if 1
                  cout << "xx n: " << n->str() << endl;
                  //delete i;
                  //  i = n;
                  #endif
                }

                cout << " yy op: " << str() << endl;
                //children_ = new_children;
                print_ast();
                return this;

              }
              else if ((str() == "prog"))
                   {
                     ASTNode *n=0;
                     
                     for (auto &i : children_)
                     {
                       n = i->eval();
                       cout << " op: " << str() << ", op child: " << i->str() << ", eval op child: " << n->str() << endl;
                       if (n != i)
                       {
                         delete i; // need free ASTNode
                         i = n;
                       }
                       print_ast();
                       //new_children.push_back(n);
                     }
                     //children_ = new_children;
                     return this;
                   }
                   else
                   {
                     cout << " op: " << str() << endl;
                     return nullptr;
                   }
#if 0
      bool leaf = i->is_leaf();
      //i->eval();
      if (leaf == false)
        print_ast();
#endif
    } // end for (const auto &i : children_)
  }
}

#if 0
ASTNode::ASTNode(const ASTNode* l, const ASTNode* op, const ASTNode* r)
{
    vector<ASTNode*> children_;
}
#endif
