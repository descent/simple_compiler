#include "astnode.h"

#include <cstdlib>
#include <map>
using namespace std;

#define PREORDER

//#define DEBUG_PRINTF_STRING

// #define PRINT_AST_TYPT_STR


#define PRINT_TREE_STRING

//extern map<string, ASTNode*> func_map;

// for environment or symbol table
ASTNode *gen_ast_node(const ASTNode *var_name)
{
  return new ASTNode(var_name);
}

ASTNode *get_true_node()
{
  static ASTNode true_node(true_token);
  return &true_node;
}

int add_to_env(Environment *env, ASTNode *i, bool is_global)
{
  env->add(i->str(), gen_ast_node(i));
  return 0;
}

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

string tree_string(const string &str)
{
    string print_str{str};
    string s1("%");
    string s2("(");
    string s3(")");
    char s4='\n';

    auto pos = print_str.find(s1);
    //cout << endl << "XXX:" << print_str << "YYY" << endl;
    while ((pos=print_str.find(s1, pos)) != string::npos)
    {
      print_str.replace(pos, s1.length(), R"(\%)");
      pos+=2;
      //cout << endl << "BBB:" << print_str << endl;
    }

    pos = print_str.find(s2);
    while ((pos=print_str.find(s2, pos)) != string::npos)
    {
      //cout << endl << "CCC:" << print_str << endl;
      print_str.replace(pos, s2.length(), R"(\()");
      pos+=2;
    }

    pos = print_str.find(s3);
    while ((pos=print_str.find(s3, pos)) != string::npos)
    {
      print_str.replace(pos, s3.length(), R"(\))");
      pos+=2;
    }

    pos = print_str.find(s4);
    while ((pos=print_str.find(s4, pos)) != string::npos)
    {
      print_str.replace(pos, 1, R"(\\n)");
      pos+=2;
      //cout << endl << "AAA:" << print_str << endl;
    }
  return print_str;
}

void ASTNode::print_tree()
{
  if (children_.size() == 0) // leaf node
  {
    string print_str = tree_string(token_.str_);
    cout << "(" << print_str;
    if (ast_type() == NAME || ast_type() == FUNC_NAME)
    {
      cout << obj_type_.str();
#ifdef PRINT_AST_TYPT_STR
      cout << "| " << type_str();
#endif
    }

    cout << ")";
  }
  else
  {
  #if 0
    string str("one three two four");
    string str2("three");
    str.replace(str.find(str2),str2.length(),"five");
  #endif

    string print_str = tree_string(token_.str_);
    //cout << "( " << token_.str_;
    cout << "( " << print_str;
    if (ast_type() == NAME || ast_type() == FUNC_NAME)
    {
      cout << obj_type_.str();
#ifdef PRINT_AST_TYPT_STR
      cout << "| " << type_str();
#endif
    }
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

ASTNode* ASTNode::eval(Environment *env)
{
#if 1

#if 0
  if(obj_type().is_pointer())
  {
    ASTNode *f = env->lookup(str());
    if (f)
      return f;
    else
    {
      cout << "can not found value!" << endl;
      return this;
    }
  }
#endif
  if (ast_type() == DEREF) // *
  {
    ASTNode *ref_node = env->lookup(children_[0]->str());
    ASTNode *deref_node = env->lookup(ref_node->str());
    return deref_node;

#if 0
    ASTNode *c1 = children_[0]->eval(env);

    uintptr_t addr_val = stoul(c1->str());
    //cout << "DEREF: " << hex << addr_val << dec << endl;
    string str = *((string*)addr_val);
    //cout << "xx str: " << str << endl;

    Token t(str, NUMBER);
    return new ASTNode(t);

    ASTNode *f = env->lookup(c1->str());
    if (f)
    {
      return f;
    }
    else
    {
      cout << "can not found value!" << endl;
      return this;
    }
#endif
  }

  if (ast_type() == ADDR_OF) // &
  {
    if (children_.size() == 1)
    {
      return children_[0];
    #if 0
      ASTNode *f = env->lookup(children_[0]->str());
      if (f)
      {
        //return f;
      }
      else
      {
        cout << "can not found value!" << endl;
        return this;
      }

#if 1
      void *addr = &f->token_.str_;
      uintptr_t addr_val = (uintptr_t)addr;
      #if 0
      cout << "yy addr: " << addr << endl;
      cout << "yy addr_val: " << hex << addr_val << dec << endl;
      #endif
      Token t(std::to_string(addr_val), NUMBER);
      return new ASTNode(t);
#endif
      //return children_[0];
      #endif
    }
    else
    {
      cout << "ADDR_OF op should has 1 child." << endl;
      return this;
    }
  }

  if (ast_type() == NEG)
  {
    if (children_.size() == 1)
    {
      ASTNode *c1 = children_[0]->eval(env);
      int n1 = -(stoi(c1->str()) );
      Token t(std::to_string(n1), NUMBER);
      eval_result_ = new ASTNode(t);
      return eval_result_;
    }
    else
    {
      cout << "NEG node should has 1 child" << endl;
      exit(NEG_ERR);
      return this;
    }
  }

  if (ast_type() == GLOBAL_VAR)
  {
    for (auto &i : children())
    {
      add_to_env(env, i, true);
    }
    
    return this;
  }
  if (ast_type() == FUNC_NAME)
  {
    if (env->lookup(str()))
    {
      cout << "redefinition of function: " << str() << endl;
      exit(FUNC_REDEFINITION);
    }
    else
    {
      env->add(str(), this);
    }
    return this;
  }
  if (ast_type() == RETURN)
  {
    ASTNode *r;
    for (auto &i : children())
    {
      r = i->eval(env);
    }
    r->set_ast_type(RETURN_VAL);
    return r;
  }
  if (ast_type() == VAR) // var declare
  {
    //cout << "Xx env str:" << env->name() << endl;

    for (auto &i : children())
    {
      //cout << "add: " << i->str() << " to env: " << env->name() << endl;
      add_to_env(env, i, false);
    }
    return this;
  }
#endif
  if (ast_type() == FUNC_CALL) 
  {
    if (str()=="printf")
    {
      string fmt = children()[0]->str();
      string s1{"%p"};

      
      auto pos = fmt.find(s1);
      while ((pos = fmt.find(s1, pos)) != string::npos)
      {
        if (0 == pos)
        {
          fmt.replace(pos, s1.length(), R"(%x)");
        }
        else
        {
          // handle %%p don;t replace to %%x
          if ('%' != fmt[pos-1])
          {
            cout << "%" << endl;
            fmt.replace(pos, s1.length(), R"(%x)");
          }
        }
        ++pos;
      }

      string cmd{R"(printf ")"};
      cmd += fmt;
      cmd += "\" ";


      auto arg_num =  children().size();

      for (decltype(arg_num) i=1 ; i < arg_num; ++i)
      {
        ASTNode *eval_node = children()[i]->eval(env);

        cmd += "\"";
        cmd += eval_node->str();
        cmd += "\" ";
      }

      #ifdef DEBUG_PRINTF_STRING
      cout << "=== printf cmd: " << cmd << "---" << endl;
      #endif

      system(cmd.c_str());

      #if 0
      cout << "print arg: " << endl;
      for (auto &i : children())
      {
        cout << i->str() << endl;
      }
      switch (children().size())
      {
        case 0:
        {
          cout << "printf has no arguments" << endl;
          break;
        }
        case 1:
        {
          printf(children()[0]->str().c_str());
          break;
        }
        case 2:
        {
          //printf(children()[0]->str().c_str(), stoi(children()[1]->eval(env)->str()));
          if (children()[1]->eval(env)->ast_type() == STRING)
          {
            printf(children()[0]->str().c_str(), children()[1]->eval(env)->str().c_str());
          }
          else if (children()[1]->eval(env)->ast_type() == NUMBER)
               {
                 printf(children()[0]->str().c_str(), stoi(children()[1]->eval(env)->str()));
               }
               else
               {
                 cout << "only handle STRING/NUMBER" << endl;
               }
          break;
        }
        case 3:
        {
          ASTNode *arg1 = children()[1]->eval(env);
          ASTNode *arg2 = children()[2]->eval(env);

                  (arg1->ast_type() == STRING) ? arg1->str().c_str() : stoi(arg1->str().c_str());

#if 0
          printf(children()[0]->str().c_str(), 
                  (arg1->ast_type() == STRING) ? arg1->str().c_str() : stoi(arg1->str()), 
                  (arg2->ast_type() == STRING) ? arg2->str().c_str() : stoi(arg2->str())
                )
    #endif             
          break;
        }
        case 4:
        {
          printf(children()[0]->str().c_str(), stoi(children()[1]->eval(env)->str()), stoi(children()[2]->eval(env)->str()), stoi(children()[3]->eval(env)->str()));
          break;
        }
        case 5:
        {
          printf(children()[0]->str().c_str(), stoi(children()[1]->eval(env)->str()), stoi(children()[2]->eval(env)->str()), stoi(children()[3]->eval(env)->str()), stoi(children()[4]->eval(env)->str()));
          break;
        }
        case 6:
        {
          printf(children()[0]->str().c_str(), stoi(children()[1]->eval(env)->str()), stoi(children()[2]->eval(env)->str()), stoi(children()[3]->eval(env)->str()), stoi(children()[4]->eval(env)->str()), stoi(children()[5]->eval(env)->str()));
          break;
        }

      }
      #endif
      return get_true_node();
    }
    if (children().size() > 0) // has arguments
    {
      // new env
      //Environment *env = new Environment();
    }

    
    ASTNode *f_node = env->lookup(str());
    if (f_node == 0)
    {
      cout << "can not find function deifition: " << str() << endl;
      exit(FUNC_NOT_DEFINE);
    }
    ASTNode *f_para = 0;
    ASTNode *f_body = 0;

    if (f_node)
    {

    if (f_node->children().size() == 1)
    {

      f_body = f_node->children()[0];


      //return f_body->eval();
      //cout << "xxx " << endl;
      //exit(0);
    }
    else if (f_node->children().size() == 2)
         {
           f_para = f_node->children()[0];
           f_body = f_node->children()[1];
         }
         else
         {
           // some error
           cout << "eval func error!!" << endl;
           exit(5);
         }
    }


      Environment *func_env = new Environment(env, str() + "_env");
      #if 0
      cout << "func_env->name(): ";
      cout << func_env->name() << endl;
      cout << "f_para" << endl;
      #endif
      if (f_para)
      {
        //if (children().size() > 0) // has arguments
        //for (auto &i : f_para->children())

        auto var_size = f_para->children().size();
        auto val_size = children().size();

        if (var_size == val_size)
        {
          for (decltype(var_size) i=0 ; i < var_size ; ++i)
          {
            ASTNode *arg_node = children()[i];
            #if 0
            ASTNode *n = func_env->lookup(f_para->children()[i]->str());
            if (n)
            {
              func_env->edit(f_para->children()[i]->str(), arg_node->eval(func_env));
            }
            else
            #endif
            func_env->add(f_para->children()[i]->str(), arg_node->eval(func_env));
          }
        }
        else
        {
          cout << "ASTNode: " << str()<< ", var_size: " << var_size << ", val_size: " << val_size << " are not the same." << endl;
          exit(FUNC_PARA_ARGU_NOT_MATCH);
        }

        //f_para->print();
      }

      ASTNode *ret=0;
#if 0
      cout << "2222 " << endl;
      f_body->print();
      cout << "3333 " << endl;
#endif
      for (auto &i : f_body->children())
      {
        ret = i->eval(func_env);

        if (ret->ast_type() == RETURN_VAL) // the eval is a return statement, so break the loop.
          break;
      }
      return ret;
  }

  if (children_.size() == 0) // leaf node
  {
    if (ast_type() == NAME)
    {
      //cout << "lookup: " << str() << ", in env name: " << env->name() << endl;
      ASTNode *n = env->lookup(str());
      if (n)
      {
        //cout << "find" << endl;
        return n;
      }
      else
      {
        //cout << "not found" << endl;
        return this;
      }

    #if 0
      //if (env.count(str()))
      {
        //ASTNode *n = env[str()];
        //cout << "eval name '" << str() << "' : " << n->str() << endl;
        //return env[str()];
      }
      else
      #endif
        //return this;
    }
    else
      return this;
  }
  else
  {
    //cout << "cur node: " << str() << endl;

    //for (const auto &i : children_)
    {

    if (token_.str_ == "+" || token_.str_ == "-" || token_.str_ == "*")
    {
      if (children_.size() == 2)
      {
        ASTNode *c1 = children_[0]->eval(env);
        #ifdef DEBUG_MSG
        cout << "xx cur: " << str() << endl;
        cout << "c1: " << c1->str() << endl;
        #endif
        ASTNode *c2 = children_[1]->eval(env);
        #ifdef DEBUG_MSG
        cout << "c2: " << c2->str() << endl;
        #endif

#if 0
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
#endif

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
        #if 0
        cout << "str: " << str() << endl;
        cout << "c1: " << c1->str() << endl;
        cout << "c2:" << c2->str() << endl;
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
        #ifdef DEBUG_MSG
        cout << "eval: " << n1 << " " << str() << " " << n2 << " = " << ret << endl;
        #endif

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
           ASTNode *c1 = children_[1]->eval(env);
           ASTNode *c0 = children_[0];
           if ("*" == c0->str())
           {
             ASTNode *ref_node = env->lookup(c0->children_[0]->str());
             env->edit(ref_node->str(), c1);
           }
           else
           {

           if (c0->ast_type() != NAME && c0->str() != "*") // deref
           {
             cout << "= error: left side should be variable name or deref (*)" << endl;
             return this;
           }

           #ifdef DEBUG_MSG
           string s{c0->str()};

           cout << "s: " << s << endl;
           #endif
           ASTNode *f = env->lookup(c0->str());

           if (f->obj_type().is_pointer())
           {
             cout << "assign pointer" << endl;
           }
           else
           {
        #ifdef DEBUG_MSG
           cout << "op is =" << endl;
        #endif
           // add var/val to env



        #if 0
           cout << "c0 str: " << c0->str() << ", c1: " << c1->str() << endl;
        #endif
           //env.insert({c0->str(), c1});
           #if 0
           env[c0->str()] = c1;
           if (env.count(c0->str()))
           {
             ASTNode *f = env[c0->str()];
             cout << "f str: " << f->str() << endl;
           }
           #endif
           if (f)
           {
             env->edit(c0->str(), c1);
           }
           else
           {
             cout << "cannot look up: " << c0->str() << endl;
             exit(VAR_NOT_DEFINE);
           }



#if 0
           if (children_[0] != c1)
           {
             delete children_[0];
             children_[0] = c1;
             print_ast();
           }
#endif

#if 0
           if (children_[1] != c1)
           {
             delete children_[1];
             children_[1] = c1;
             print_ast();
           }
#endif
           c1->set_ast_type(ASSIGN);
           return c1;
           }
           }

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
                  n = i->eval(env);
                  #if 0
                  cout << "xx n: " << n->str() << endl;
                  //delete i;
                  //  i = n;
                  #endif
                }

                //cout << " yy op: " << str() << endl;
                //children_ = new_children;
                //print_ast();
                return this;

              }
              else if ((str() == "prog"))
                   {
                     ASTNode *n=0;
                     
                     for (auto &i : children_)
                     {
                       n = i->eval(env);
                       //cout << " op: " << str() << ", op child: " << i->str() << ", eval op child: " << n->str() << endl;
                       #if 0
                       if (n != i)
                       {
                         delete i; // need free ASTNode
                         i = n;
                       }
                       #endif
                       //print_ast();
                       //new_children.push_back(n);
                     }
                     //children_ = new_children;
                     return this;
                   }
                   else if ((str() == "if"))
                        {
                          ASTNode *then_node = this; // return nullptr, avoid Segmentation fault
                          ASTNode *else_node = this; // return nullptr, avoid Segmentation fault
                          auto children_size = children().size();

                          if (children_size >= 2)
                          {
                            ASTNode *c0 = children_[0]->eval(env);
                            if (c0->ast_type() == TRUE)
                            {
                              then_node = children_[1]->eval(env);
                              return then_node;
                            }
                            else
                            {
                              if (children_size >= 3)
                              {
                                else_node = children_[2]->eval(env);
                                return else_node;
                              }
                            }


                          }
                          else
                          {
                            cout << "if node error" << endl;
                            return this;
                          }
                          return this; // return nullptr, avoid Segmentation fault
                        }
                        else if ((str() == ">") || (str() == "<") || (str() == "==") || (str() == "!="))
                             {
                               if (children().size() == 2)
                               {
                                 ASTNode *c0 = children_[0]->eval(env);
                                 ASTNode *c1 = children_[1]->eval(env);
                                 int n0 = stoi(c0->str());
                                 int n1 = stoi(c1->str());
                                 #ifdef DEBUG_MSG
                                 cout << endl << "n0: " << n0 << " " << str() << " n1: " << n1 << endl;
                                 #endif
                                 if (str() == ">")
                                 {
                                   if (n0 > n1)
                                   {
                                     //cout << "true node" << endl;
                                     return new ASTNode(true_token);
                                   }
                                   else
                                   {
                                     //cout << "false node" << endl;
                                     return new ASTNode(false_token);
                                   }
                                 }
                                 if (str() == "<")
                                 {
                                   if (n0 < n1)
                                   {
                                     //cout << "true node" << endl;
                                     return new ASTNode(true_token);
                                   }
                                   else
                                   {
                                     //cout << "false node" << endl;
                                     return new ASTNode(false_token);
                                   }
                                 }
                                 if (str() == "==")
                                 {
                                   if (n0 == n1)
                                   {
                                     //cout << "true node" << endl;
                                     return new ASTNode(true_token);
                                   }
                                   else
                                   {
                                     //cout << "false node" << endl;
                                     return new ASTNode(false_token);
                                   }
                                 }
                                 if (str() == "!=")
                                 {
                                   if (n0 == n1)
                                   {
                                     //cout << "true node" << endl;
                                     return new ASTNode(true_token);
                                   }
                                   else
                                   {
                                     //cout << "false node" << endl;
                                     return new ASTNode(false_token);
                                   }
                                 }


                               }
                               else
                               {
                                 cout << "> node error: " << children().size() << endl;
                                 return this;
                               }
                             }
                             else if ((str() == "then_block") || (str() == "while_block"))
                                  {

                                    ASTNode *n=0;
                                     
                                    for (auto &i : children_)
                                    {
                                      n = i->eval(env);
                                      #ifdef DEBUG_MSG
                                      cout << " op: " << str() << ", op child: " << i->str() << ", eval op child: " << n->str() << endl;
                                      #endif
                                      #if 0
                                      if (n != i)
                                      {
                                        delete i; // need free ASTNode
                                        i = n;
                                      }
                                      #endif
                                      //print_ast();
                                    }
                                    return n;
                                  }
                                  else if ((str() == "else_block"))
                                       {
                                        ASTNode *n=0;
                                       
                                        for (auto &i : children_)
                                        {
                                          n = i->eval(env);

                                          #ifdef DEBUG_MSG
                                          cout << " op: " << str() << ", op child: " << i->str() << ", eval op child: " << n->str() << endl;
                                          #endif
                                          #if 0
                                          if (n != i)
                                          {
                                            delete i; // need free ASTNode
                                            i = n;
                                          }
                                          #endif
                                          //print_ast();
                                        }
                                        return n;
                                       }
                                       else if ((str() == "while"))
                                            {
                                              //cout << "begin eval while" << endl;
                                              if (children().size() == 2)
                                              {
                                                ASTNode *c0 = children_[0];
                                                ASTNode *eval_while = this; // return nullptr, avoid Segmentation fault
                                                while(c0->eval(env)->ast_type() == TRUE)
                                                {
                                                  eval_while = children_[1]->eval(env);
                                                  //cout << "eval while eval_while:" << eval_while->str() << endl;
                                                }
                                                return eval_while;
                                              }
                                              else
                                              {
                                                cout << "while node fail" << endl;
                                              }
                                              return this;
                                            }

                                       else
                                       {
                                         cout << "can not handle op: " << str() << ", ast type: " << type_str() << endl;
                                         return this;
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
