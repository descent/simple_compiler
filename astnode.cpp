#include "astnode.h"

#include <cstdlib>
#include <map>
#include <sstream>
#include <fstream>

ofstream op_ofs("op.s");
ofstream func_ofs("func.s");
ofstream data_ofs("data.s");
ofstream func_call_ofs("func_call.s");

namespace
{
  u32 cur_need_stack_size;
  u32 max_need_stack_size;
  u32 cur_occupy_size = 0;
}

GenLabel gen_if_then_label{"then_"};
GenLabel gen_if_else_label{"else_"};
GenLabel gen_if_end_label{"end_if_"};

SymbolTable global_symbol_table{"global"};
SymbolTable local_symbol_table{"local"};

void update_stack_usage()
{
  if (cur_need_stack_size > max_need_stack_size)
    max_need_stack_size = cur_need_stack_size;
}

using namespace std;

map<string, int> alloc_stack;

string func_name;

string text_section;
string ro_data_section = "  .section .rodata\n"
                         ".LC0:\n";

#define PREORDER

//#define DEBUG_PRINTF_STRING

// #define PRINT_AST_TYPT_STR


#define PRINT_TREE_STRING

//extern map<string, ASTNode*> func_map;


ASTNode *get_string_ast_node(const string &str="")
{
  Token t(str, STRING);
  return new ASTNode(t);
}

ASTNode *get_number_ast_node(const string &str="0")
{
  Token t(str, NUMBER);
  return new ASTNode(t);
}

ASTNode *get_true_node()
{
  static ASTNode true_node(true_token);
  return &true_node;
}

int add_to_env(Environment *env, ASTNode *i, bool is_global)
{
  if(i->obj_type().is_int())
  {
    //cout << "int xx: " << i->str() << endl;
    env->add(i->str(), get_number_ast_node());
  }
  else if (i->obj_type().is_string())
       {
         //cout << "str xx: " << i->str() << endl;
         env->add(i->str(), get_string_ast_node());
       }
       else
       {
         //cout << "true xx: " << i->str() << endl;
         env->add(i->str(), get_true_node());
       }
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

string replace_backslash(const string &str)
{
  string print_str{str};
  char s4='\n';

  auto pos = print_str.find(s4);

  pos = print_str.find(s4);
  while ((pos=print_str.find(s4, pos)) != string::npos)
  {
    print_str.replace(pos, 1, R"(\n)");
    pos+=2;
    //cout << endl << "AAA:" << print_str << endl;
  }
  return print_str;
}

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




int gen_relation()
{
}

void ASTNode::gen_gas_op(GenGasFunc gen_gas_func)
{
  if (is_valid_op() == false)
  {
    cout << "is_valid_op() fail" << endl;
    return;
  }

  auto l_child = left_child();
  auto r_child = right_child();
  string l_operand_string;
  string r_operand_string;
  string relative_inst = "unknow relative inst";

  if (l_child->is_leaf() && r_child->is_leaf())
  {
    if (NAME == l_child->ast_type())
    {
      // i > j
      auto node = local_symbol_table.lookup(l_child->str());
      l_operand_string = node->local_var_addr_;
    }
    else
    {
      // 1 > 2
      l_operand_string = "$" + l_child->str();
    }

    if (NAME == r_child->ast_type())
    { // i > j
      auto node = local_symbol_table.lookup(r_child->str());

      r_operand_string = node->local_var_addr_;
    }
    else 
    { // 1 > 2
      r_operand_string = "$" + r_child->str();
    }

    // 1 > 2
    // mov $1, %eax
    // cmpl $2, %eax
    //op_ofs << "movl " << l_operand_string << ", %eax" << endl;
    op_ofs << "movl " << l_operand_string << ", %eax" << endl;
    op_ofs << "cmpl " << r_operand_string << ", %eax" << endl;

    switch (ast_type())
    {
      case LESS:
      {
        relative_inst = "setl";
        break;
      }
      case GREAT:
      {
        relative_inst = "setg";
        break;
      }
      case EQUAL:
      {
        relative_inst = "sete";
        break;
      }
      default:
      {
        break;
      }
    }
    op_ofs << relative_inst << " %al" << endl;
    op_ofs << "movzbl %al, %eax" << endl;
    op_ofs << "pushl %eax" << endl;

    cur_need_stack_size += 4;
  }
  else if (l_child->is_leaf() && r_child->is_leaf() != true)
       {
       }
       else if (l_child->is_leaf() != true && r_child->is_leaf())
            {
            }
            else
            {
            }


}

string get_relational_inst(auto ast_type)
{
  string relative_inst;

    switch (ast_type)
    {
      case LESS:
      {
        relative_inst = "setl";
        break;
      }
      case GREAT:
      {
        relative_inst = "setg";
        break;
      }
      case EQUAL:
      {
        relative_inst = "sete";
        break;
      }
      default:
      {
        break;
      }
    }
  return relative_inst;
}

void ASTNode::gen_gas_relation(const string &reg)
{
  if (is_valid_op() == false)
  {
    cout << "is_valid_op() fail" << endl;
    return;
  }

  auto l_child = left_child();
  auto r_child = right_child();
  string l_operand_string;
  string r_operand_string;
  string relative_inst = "unknow relative inst";

  if (l_child->is_leaf() && r_child->is_leaf())
  {
    op_ofs << "# gen code: " << l_child->str() << " " << str() << " " << r_child->str() << endl;

    if (NAME == l_child->ast_type())
    {
      // i > j
      auto node = local_symbol_table.lookup(l_child->str());
      l_operand_string = node->local_var_addr_;
    }
    else
    {
      // 1 > 2
      l_operand_string = "$" + l_child->str();
    }

    if (NAME == r_child->ast_type())
    { // i > j
      auto node = local_symbol_table.lookup(r_child->str());

      r_operand_string = node->local_var_addr_;
    }
    else 
    { // 1 > 2
      r_operand_string = "$" + r_child->str();
    }

    // 1 > 2
    // mov $1, %eax
    // cmpl $2, %eax
    //op_ofs << "movl " << l_operand_string << ", %eax" << endl;
    op_ofs << "movl " << l_operand_string << ", %eax # variable: " << l_child->str() << endl;
    op_ofs << "cmpl " << r_operand_string << ", %eax # variable: " << r_child->str() << endl;

    switch (ast_type())
    {
      case LESS:
      {
        relative_inst = "setl";
        break;
      }
      case GREAT:
      {
        relative_inst = "setg";
        break;
      }
      case EQUAL:
      {
        relative_inst = "sete";
        break;
      }
      default:
      {
        break;
      }
    }
    op_ofs << relative_inst << " %al" << endl;
    op_ofs << "movzbl %al, %eax" << endl;
    op_ofs << "pushl %eax" << endl;

    cur_need_stack_size += 4;
  }
  else if (l_child->is_leaf() && r_child->is_leaf() != true)
       { // 1 < (2 < 3)
         if (r_child->is_add_sub())
         {
           r_child->gen_gas_add_sub("%eax");
         }
         else if (r_child->is_mul_div())
              {
                r_child->gen_gas_mul_div(""); 
              }
              else if (r_child->is_relational_op())
                   {
                     r_child->gen_gas_relation("");
                   }
                   else
                   {
                     cout << "unsupport op" << endl;
                   }

         op_ofs << "# gen code (left child is leaf): " << l_child->str() << " " << str() << " " << r_child->str() << endl;
         if (NAME == l_child->ast_type())
         {
           auto node = local_symbol_table.lookup(l_child->str());
           l_operand_string = node->local_var_addr_;
         }
         else
         {
           l_operand_string = "$" + l_child->str();
         }

         op_ofs << "popl %ebx" << endl;
         update_stack_usage();
         cur_need_stack_size -= 4;

         op_ofs << "movl " << l_operand_string << ", %eax" << endl;
         op_ofs << "cmpl %ebx, %eax" << endl;
     
         relative_inst = get_relational_inst(ast_type());
     
         op_ofs << relative_inst << " %al" << endl;
         op_ofs << "movzbl %al, %eax" << endl;
         op_ofs << "pushl %eax" << endl;
     
         cur_need_stack_size += 4;

       }
       else if (l_child->is_leaf() != true && r_child->is_leaf())
            { // 1 < 2 < 3
              if (l_child->is_add_sub())
              {
                l_child->gen_gas_add_sub("%eax");
              }
              else if (l_child->is_mul_div())
                   {
                     l_child->gen_gas_mul_div(""); 
                   }
                   else if (l_child->is_relational_op())
                        {
                          l_child->gen_gas_relation("");
                        }
                        else
                        {
                          cout << "unsupport op" << endl;
                        }

              op_ofs << "# gen code (right child is leaf): " << l_child->str() << " " << str() << " " << r_child->str() << endl;


              if (NAME == r_child->ast_type())
              {
                auto node = local_symbol_table.lookup(r_child->str());
                r_operand_string = node->local_var_addr_;
              }
              else
              {
                r_operand_string = "$" + r_child->str();
              }
     
              op_ofs << "popl %eax" << endl;
              update_stack_usage();
              cur_need_stack_size -= 4;

              op_ofs << "movl " << r_operand_string << ", %ebx" << endl;
              op_ofs << "cmpl %ebx, %eax" << endl;
     
              relative_inst = get_relational_inst(ast_type());
     
              op_ofs << relative_inst << " %al" << endl;
              op_ofs << "movzbl %al, %eax" << endl;
              op_ofs << "pushl %eax" << endl;
     
              cur_need_stack_size += 4;



            }
            else
            { // (2 < 3) < (6 < 8)
              if (l_child->is_add_sub())
              {
                l_child->gen_gas_add_sub("%eax");
              }
              else if (l_child->is_mul_div())
                   {
                     l_child->gen_gas_mul_div(""); 
                   }
                   else if (l_child->is_relational_op())
                        {
                          l_child->gen_gas_relation("");
                        }
                        else
                        {
                          cout << "left: unsupport op" << endl;
                        }

              if (r_child->is_add_sub())
              {
                r_child->gen_gas_add_sub("%eax");
              }
              else if (r_child->is_mul_div())
                   {
                     r_child->gen_gas_mul_div(""); 
                   }
                   else if (r_child->is_relational_op())
                        {
                          r_child->gen_gas_relation("");
                        }
                        else
                        {
                          cout << "right: unsupport op" << endl;
                        }


              op_ofs << "# gen code (2 children are not leaf): " << l_child->str() << " " << str() << " " << r_child->str() << endl;


              op_ofs << "popl %ebx" << endl; // right child result
              update_stack_usage();
              cur_need_stack_size -= 4;

              op_ofs << "popl %eax" << endl; // left child result
              //update_stack_usage();
              cur_need_stack_size -= 4;

              op_ofs << "cmpl %ebx, %eax" << endl;
              relative_inst = get_relational_inst(ast_type());
              op_ofs << relative_inst << " %al" << endl;
              op_ofs << "movzbl %al, %eax" << endl;
              op_ofs << "pushl %eax" << endl;
              cur_need_stack_size += 4;



            }


}

void ASTNode::gen_gas_mul_div(const string &reg)
{
  if (is_valid_op() == false)
  {
    cout << "is_valid_op() fail" << endl;
    return;
  }
  auto l_child = left_child();
  auto r_child = right_child();
  string l_operand_string;
  string r_operand_string;

  if (l_child->is_leaf() && r_child->is_leaf())
  {
    if (NAME == l_child->ast_type())
    {
      // i*j
      auto node = local_symbol_table.lookup(l_child->str());
      l_operand_string = node->local_var_addr_;
    }
    else
    {
      // 1*2
      l_operand_string = "$" + l_child->str();
    }

    if (NAME == r_child->ast_type())
    { // i*j
      auto node = local_symbol_table.lookup(r_child->str());

      r_operand_string = node->local_var_addr_;
    }
    else 
    { // 1*2
      r_operand_string = "$" + r_child->str();
    }

    cout << "movl " << l_operand_string << ", %eax" << endl;
    cout << "movl " << r_operand_string << ", %ebx" << endl;
    cout << "mul %ebx" << endl;
    cout << "pushl %eax" << endl;

    op_ofs << "movl " << l_operand_string << ", %eax" << endl;
    op_ofs << "movl " << r_operand_string << ", %ebx" << endl;
    op_ofs << "mul %ebx" << endl;
    op_ofs << "pushl %eax" << endl;

    cur_need_stack_size += 4;
  }
  else if (l_child->is_leaf() && r_child->is_leaf() != true)
       {
         if (is_add_sub())
           r_child->gen_gas_add_sub("%eax");
         if (MUL == r_child->ast_type())
           r_child->gen_gas_mul_div("");


         if (NAME == l_child->ast_type())
         {
           // 5*j
           auto node = local_symbol_table.lookup(l_child->str());
           l_operand_string = node->local_var_addr_;
         }
         else
         {
           // 1*2
           l_operand_string = "$" + l_child->str();
         }

         cout << "movl " << l_operand_string << ", %eax" << endl;
         cout << "popl %ebx" << endl;
         cout << "mul %ebx" << endl;
         cout << "pushl %eax" << endl;

         op_ofs << "movl " << l_operand_string << ", %eax" << endl;
         op_ofs << "popl %ebx" << endl;
         update_stack_usage();
         cur_need_stack_size -= 4;

         op_ofs << "mul %ebx" << endl;
         op_ofs << "pushl %eax" << endl;

         cur_need_stack_size += 4;

       }
       else if (l_child->is_leaf() != true && r_child->is_leaf())
            {
              if (is_add_sub())
                l_child->gen_gas_add_sub("%eax");
              if (MUL == l_child->ast_type())
                l_child->gen_gas_mul_div("");

              if (NAME == r_child->ast_type())
              { // i*6
                auto node = local_symbol_table.lookup(r_child->str());

                r_operand_string = node->local_var_addr_;
              }
              else 
              { // 1*2
                r_operand_string = "$" + r_child->str();
              }

              cout << "movl " << r_operand_string << ", %ebx" << endl;
              op_ofs << "movl " << r_operand_string << ", %ebx" << endl;

              cout << "popl %eax" << endl;
              cout << "mul %ebx" << endl;
              cout << "pushl %eax" << endl;

              op_ofs << "popl %eax" << endl;

              update_stack_usage();
              cur_need_stack_size -= 4;

              op_ofs << "mul %ebx" << endl;
              op_ofs << "pushl %eax" << endl;
              cur_need_stack_size += 4;
            }
            else // all are not leaf
            {
              l_child->gen_gas_mul_div("");
              r_child->gen_gas_mul_div("");

              cout << "popl %ebx" << endl;
              cout << "popl %eax" << endl;
              cout << "mul %ebx" << endl;
              cout << "pushl %eax" << endl;


              op_ofs << "popl %ebx" << endl;
              update_stack_usage();
              cur_need_stack_size -= 4;

              op_ofs << "popl %eax" << endl;
              update_stack_usage();
              cur_need_stack_size -= 4;

              op_ofs << "mul %ebx" << endl;
              op_ofs << "pushl %eax" << endl;
              cur_need_stack_size += 4;

            }


}


void ASTNode::gen_gas_add_sub(const string &reg)
{
  if (is_valid_op() == false)
  {
    cout << "is_valid_op() fail" << endl;
    return;
  }
  auto l_child = left_child();
  auto r_child = right_child();
  string reg_str = " %ebx, %eax";
  string l_operand_string;
  string r_operand_string;

  cout << "handle add " << str() << endl;
  if (l_child->is_leaf() && r_child->is_leaf())
  {
    cout << "movl $" << l_child->str() << ", " << reg << endl;
    cout << type_str() << " $" << r_child->str() << ", " << reg << endl;
    cout << "pushl " << reg << endl;

    if (NAME == l_child->ast_type())
    {
      auto node = local_symbol_table.lookup(l_child->str());
      l_operand_string = node->local_var_addr_;
    }
    else
    {
      l_operand_string = "$" + l_child->str();
    }

    if (NAME == r_child->ast_type())
    {
      auto node = local_symbol_table.lookup(r_child->str());

      //cout << "NAME right" << endl;
      //r_child->gen_gas_syntax();
      r_operand_string = node->local_var_addr_;
    }
    else // int number
    {
      r_operand_string = "$" + r_child->str();
    }

    op_ofs << "movl " << l_operand_string << ", " << reg << endl;
    op_ofs << type_str() << " " << r_operand_string << ", " << reg << endl;

    op_ofs << "pushl " << reg << endl;
    cur_need_stack_size += 4;
  #if 0
    cout << "movl $" << child[0]->str() << ", " << reg << endl;
    cout << type_str() << " $" << child[1]->str() << ", " << reg << endl;

    op_ofs << "movl $" << child[0]->str() << ", " << reg << endl;
    op_ofs << type_str() << " $" << child[1]->str() << ", " << reg << endl;
    op_ofs << "pushl " << reg << endl;
  #endif
  }
  else if (l_child->is_leaf() && r_child->is_leaf() != true)
       {
         if (r_child->is_add_sub())
         {
           r_child->gen_gas_add_sub("%ebx");
         }
         if (MUL == r_child->ast_type())
         {
           r_child->gen_gas_mul_div(""); 
         }

         if (NAME == l_child->ast_type())
         {
           auto node = local_symbol_table.lookup(l_child->str());
           l_operand_string = node->local_var_addr_;
         }
         else
         {
           l_operand_string = "$" + l_child->str();
         }

         cout << "11 movl " << l_operand_string << ", %eax" << endl;
         op_ofs << "movl " << l_operand_string << ", %eax" << endl;

         cout << "popl " << " %ebx" << endl;
         op_ofs << "popl " << " %ebx" << endl;
         update_stack_usage();
         cur_need_stack_size -= 4;

         if (reg == "%ebx")
         {
           reg_str = " %eax, %ebx";
         }
         cout << "merge right " << type_str() << reg_str << endl;
         cout << "merge right pushl " << reg << endl;

         op_ofs << type_str() << reg_str << endl;
         op_ofs << "pushl " << reg << endl;
         cur_need_stack_size += 4;


#if 0

         if (reg == "%ebx")
         {
           cout << "merge right " << type_str() << " %eax, %ebx" << endl;
           op_ofs << type_str() << " %eax, %ebx" << endl;
         }
         else
         {
           cout << "merge right " << type_str() << " %ebx, %eax" << endl;
           op_ofs << type_str() << " %ebx, %eax" << endl;
         }

         cout << "merge right pushl " << reg << endl;
         op_ofs << "pushl " << reg << endl;
#endif
       }
       else if (l_child->is_leaf() != true && r_child->is_leaf())
            {
              //cout << "33 reg: " << reg << endl;
              //l_child->gen_gas_add_sub("%eax");

              if (l_child->is_add_sub())
              {
                l_child->gen_gas_add_sub("%eax");
              }
              if (MUL == l_child->ast_type())
              {
                l_child->gen_gas_mul_div(""); 
              }


    if (NAME == r_child->ast_type())
    {
      auto node = local_symbol_table.lookup(r_child->str());

      //cout << "NAME right" << endl;
      //r_child->gen_gas_syntax();
      r_operand_string = node->local_var_addr_;
    }
    else // int number
    {
      r_operand_string = "$" + r_child->str();
    }


              cout << "22 movl " << r_operand_string << ", %ebx" << endl;
              op_ofs << "movl " << r_operand_string << ", %ebx" << endl;

              cout << "popl %eax" << endl;
              op_ofs << "popl %eax" << endl;

              update_stack_usage();
              cur_need_stack_size -= 4;

              if (reg == "%ebx")
              {
                reg_str = " %eax, %ebx";
                //cout << "merge left " << type_str() << " %eax, %ebx" << endl;
                //op_ofs << type_str() << " %eax, %ebx" << endl;
              }
              cout << "merge left " << type_str() << reg_str << endl;
              cout << "merge left pushl " << reg << endl;

              op_ofs << type_str() << reg_str << endl;
              op_ofs << "pushl " << reg << endl;
              cur_need_stack_size += 4;
            }
            else // l_child, r_child are not leaf
            {
              l_child->gen_gas_add_sub("%eax");
              r_child->gen_gas_add_sub("%ebx");

              cout << "popl " << " %ebx" << endl;
              cout << "popl " << " %eax" << endl;
              op_ofs << "popl " << " %ebx" << endl;
              update_stack_usage();
              cur_need_stack_size -= 4;

              op_ofs << "popl " << " %eax" << endl;
              update_stack_usage();
              cur_need_stack_size -= 4;

              if (reg == "%ebx")
              {
                reg_str = " %eax, %ebx";
              }
              cout << "xx left/right " << type_str() << reg_str << endl;
              op_ofs << type_str() << reg_str << endl;

              cout << "pushl " << reg << endl;
              op_ofs << "pushl " << reg << endl;
              cur_need_stack_size += 4;
            }

#if 0
  auto child = children();

  if (child[0]->is_leaf())
  {
    cout << "mov " << str() << ", " << reg << endl;
    //cout << type_str() << " " << child[1]->str() << ", " << reg << endl;
    //cout << "push " << reg << endl;
  }
  else if (is_leaf() != true)
       {
         child[0]->gen_gas_add_sub("%eax");
         child[1]->gen_gas_add_sub("%ebx");
         cout << type_str() << " %ebx, %eax" << endl;
         //cout << type_str() << " " << child[1]->str() << ", " << reg << endl;
       }
#endif
}

string gen_uniq_string_label()
{
  static string uniq_string_label=".LC";
  static u32 index=0;
  ++index;
  return uniq_string_label+std::to_string(index);
}

void ASTNode::gen_gas_syntax()
{
  const u8 var_size = 4;
    
  cout << "cg: " << replace_backslash(str()) << " : " << type_str() << endl;
#if 0
  switch 
  {
    case ADD:
    {
      break;
    }
  }
#endif

  if (ast_type() == STRING) 
  {
    string_label_ = gen_uniq_string_label();
    cout << ".section .rodata" << endl;
    cout << ".LC0:" << endl;
    string new_str = replace_backslash(str());
    cout << "    .string " << "\"" << new_str << "\"" << endl;
    data_ofs << ".section .rodata" << endl;
    data_ofs << string_label_ << ": "<< endl;
    data_ofs << "    .string " << "\"" << new_str << "\"" << endl;
    //cout << "ast type str" << child[0]->type_str() << endl;
  }
  if (ast_type() == FUNC_CALL) 
  {
    if (str() == "main")
    {
      return;
    }

#if 1
    auto child = children();
    auto it = child.rbegin();
    for (; it != child.rend() ; ++it)
    {

      if (NAME == (*it)->ast_type())
      {
        auto node = local_symbol_table.lookup((*it)->str());
        cout << "pushl " << node->local_var_addr_ << endl;
        op_ofs << "pushl " << node->local_var_addr_ << endl;
      }
      else
      {
        (*it)->gen_gas_syntax();
        if (STRING == (*it)->ast_type())
        {
          cout << "pushl $" << (*it)->string_label_ << endl;
          op_ofs << "pushl $" << (*it)->string_label_ << endl;
        }
        else
        {
          cout << "pushl %eax" << endl;
          op_ofs << "pushl %eax" << endl;
        }
      }
    }
#endif
    cout << "xx call " << str() << endl;
    cout << "addl $8, %esp" << endl;

#if 0
    func_call_ofs << "pushl %eax" << endl;
    //func_call_ofs << "pushl $.LC0" << endl;
    func_call_ofs << "pushl $" << child[0]->string_label_ << endl;
    func_call_ofs << "call " << str() << endl;
    func_call_ofs << "addl $16, %esp" << endl;
#endif
    op_ofs << "call " << str() << endl;
    op_ofs << "addl $8, %esp" << endl;
    return;
  }

  if (ast_type() == FUNC_BODY) 
  {
    text_section += ".text\n";
    text_section += ".global " + func_name + "\n";
    text_section += func_name + ":\n";
    text_section += "pushl %ebp\n";
    text_section += "movl %esp, %ebp\n";


  }
  else if (ast_type() == FUNC_NAME) 
       {
         func_name = str();
         cout << "  enter func: " << func_name << endl;
         func_ofs << ".text" << endl;
         func_ofs << ".globl " << func_name << endl;
         func_ofs << ".type " << func_name <<  ", @function" << endl;
         func_ofs << func_name <<  ":" << endl;
         func_ofs << "pushl %ebp" << endl;
         func_ofs << "movl %esp, %ebp" << endl;


         //func_ofs << "subl $128, %esp" << endl; // hard code reserve 128 byte stack
       }
       else if (ast_type() == VAR)
            {
              for (auto &i : children())
              {
                local_symbol_table.add(i->str(), i);

                if (cur_occupy_size == 0)
                  i->local_var_addr_ = "(%ebp)";
                else
                  i->local_var_addr_ = "-" + to_string(cur_occupy_size) + "(%ebp)";

                //cout << "i->local_var_addr_: " << i->local_var_addr_ << endl;
                //cout << "i->occupy_size(): " << i->occupy_size() << endl;
                cur_occupy_size += i->occupy_size();
                //cout << "cur_ccupy_size: " << cur_occupy_size << endl;
              }

              offset_ = -4;
              var_num_ = 1;
              alloc_stack.clear();

              text_section += "subl $" + to_string(children().size() * var_size) + " , %esp\n";

              cout << "enter var" << endl;
              code_gen_state_ = DECLARE_VAR;
            #if 0

              auto var_num = i->children().size();

              for (auto &j : i->children())
              {
                alloc_stack.insert({j->str(), offset});
                offset -= 4;
              }
            #endif
              return;
            }
            else if (ast_type() == NAME)
                 {
                   auto node = local_symbol_table.lookup(str());

                   //text_section += "pushl " + to_string(it->second) + "(%ebp)\n";
                   alloc_stack.insert({str(), offset_});
                   offset_ -= 4;
                 }
                 else if (ast_type() == ASSIGN)
                      {
                        auto l_child = left_child();
                        auto r_child = right_child();

                        if (l_child->ast_type() != NAME)
                        {
                          // error
                          cout << "l_child should be NAME" << endl;
                          exit(0);
                        }


                        // if op
                        if (r_child->is_op())
                        {
                          if (r_child->is_add_sub() || r_child->is_mul_div())
                          {
                            r_child->gen_gas_add_sub("%eax");
                          }
                          if (r_child->is_mul_div())
                          {
                            r_child->gen_gas_mul_div("");
                          }
                          if (r_child->is_relational_op())
                          {
                            r_child->gen_gas_relation("");
                          }
                          op_ofs << "popl %eax" << endl;
                          op_ofs << "# gen code (OP): " << l_child->str() << " " << str() << " " << r_child->str() << endl;
                        }
                        else if (NAME == r_child->ast_type())
                             {
                               op_ofs << "# gen code (NAME) : " << l_child->str() << " " << str() << " " << r_child->str() << endl;
                               auto node = local_symbol_table.lookup(r_child->str());
                               op_ofs << "movl " << node->local_var_addr_ << ", %eax # var: " << r_child->str() << endl;

                             }
                             else // int
                             {
                               op_ofs << "# gen code (int): " << l_child->str() << " " << str() << " " << r_child->str() << endl;
                               cout << "mov $" << r_child->str() << " ,%eax" << endl;
                               op_ofs << "mov $" << r_child->str() << " ,%eax" << endl;
                             }
     
                        //movl    $1, -4(%ebp)
                        auto node = local_symbol_table.lookup(l_child->str());
                        cout << "movl %eax, " << node->local_var_addr_ << endl;


                        op_ofs << "movl %eax, " << node->local_var_addr_ << " # var: " << l_child->str() << endl;
                        #if 0
                        if (NAME != r_child->ast_type())
                        {
                        }
                        else
                        {
                          op_ofs << "movl %ebx, " << node->local_var_addr_ << " # var: " << l_child->str() << endl;
                          op_ofs << "movl %eax, %ebx" << endl;
                        }
                        #endif

                        code_gen_state_ = STATEMENT;
                        cout << "exit assign" << endl;
                        return;
                      }
                      else if (is_relational_op())
                           {
                             gen_gas_relation("");
                             update_stack_usage();

                             op_ofs << "popl %eax" << endl;
                             cur_need_stack_size -= 4;

                             return;
                           }
                           else if (is_add_sub())
                                {
                                  gen_gas_add_sub("%eax");
                                  op_ofs << "popl %eax" << endl;

                                  update_stack_usage();
                                  cur_need_stack_size -= 4;

                                  cout << "max_need_stack_size: " << max_need_stack_size << endl;
                                  cout << "cur_need_stack_size: " << cur_need_stack_size << endl;
                                  cout << "add complete" << endl;
                                  return;
                                }
                                else if (is_mul_div())
                                     {
                                       gen_gas_mul_div(""); 
                                       op_ofs << "popl %eax" << endl;

                                       update_stack_usage();
                                       cur_need_stack_size -= 4;

                                       return;
                                     }
                                     else
                                     {
                                       cout << "do nothing" << endl;
                                     }
  
  for (auto &i : children())
  {

    //if (i->ast_type() != ADD)
    i->gen_gas_syntax();
  }
  if (ast_type() == VAR)
  {
    //code_gen_state_ = NORMAL;
    code_gen_state_ = STATEMENT;
    cout << "exit var" << endl;
  }

  if (str() == func_name)
  {
    cout << "  exit func: " << func_name << endl;
    cur_occupy_size = 0;

    func_ofs << "subl $" << max_need_stack_size << ", %esp # reserve temp object size" << endl; // reserve max_need_stack_size_ byte stack for temp object
    func_ofs << "subl $" << local_symbol_table.occupy_size() << ", %esp # reserve local variable" << endl; // reserve local variable

    op_ofs.close();
    func_ofs.close();
    data_ofs.close();
    func_call_ofs.close();

    system("cat data.s func.s op.s func_call.s");
    system("cat data.s func.s op.s func_call.s > r.S");
    text_section += "leave\n";
    text_section += "ret\n";
    system("echo leave >> r.S");
    system("echo ret >> r.S");
  }
}

#if 0
void ASTNode::code_gen()
{
  const u8 var_size = 4;

  cout << "cg: " << str() << " : " << type_str() << endl;
  if (ast_type() == FUNC_NAME) 
  {
    func_name = str();
  }

  if (ast_type() == FUNC_BODY) 
  {
    text_section += ".text\n";
    text_section += ".global " + func_name + "\n";
    text_section += func_name + ":\n";
    text_section += "pushl %ebp\n";
    text_section += "movl %esp, %ebp\n";

    cout << ".text" << endl;
    cout << ".global " << func_name << endl;
    cout << func_name << ": " << endl;
    cout << "pushl %ebp" << endl;
    cout << "movl %esp, %ebp" << endl;

    for (auto &i : children())
    {
     //cout << "func body: " << i->str() << " : " << i->type_str() << endl;

  if (i->ast_type() == VAR) // var declare
  {
    int offset = -4;

    auto var_num = i->children().size();
    cout << "subl $" << var_num * var_size << " , %esp" << endl;
    text_section += "subl $" + to_string(var_num * var_size) + " , %esp\n";

    for (auto &j : i->children())
    {
      alloc_stack.insert({j->str(), offset});
      offset -= 4;
    }
  }
  else if (i->ast_type() == FUNC_CALL) 
       {
         auto rit = i->children().rbegin();
         for ( ; rit != i->children().rend() ; ++rit)
         {
           //cout << (*rit)->str() << " : " << (*rit)->type_str() << endl;
           if ((*rit)->ast_type() == NAME)
           {
             auto it = alloc_stack.find( (*rit)->str() );
             if (it != alloc_stack.end()) // find it
             {
               cout << "pushl " << it->second << "(%ebp)" << endl;
               text_section += "pushl " + to_string(it->second) + "(%ebp)\n";
             }
           }
           else if ((*rit)->ast_type() == STRING)
                {
                  string new_str;
                  //.string "%d\n"
                  new_str = replace_backslash((*rit)->str());
                  ro_data_section += R"(.string ")" + new_str + R"(")" + "\n";

                  cout << "pushl $.LC0\n";
                  text_section += "pushl $.LC0\n";

                  //cout << "ro_data_section:\n" << ro_data_section << endl;
                  //data_section >> str;
                  //cout << "str: " << str << endl;


                }
         }

         cout << "call " << i->str() << endl;
         text_section += "call " + i->str() + "\n";
         cout << "add $" << i->children().size() << ", %esp" << endl;
         text_section += "add $" + to_string(i->children().size() * var_size) + ", %esp\n";


       }
       else if (i->ast_type() == ASSIGN) 
       {
         if (i->children().size() != 2)
         {
           cout << "'=' should has 2 children" << endl;
           exit(9);
         }

         ASTNode *l = i->children()[0];
         ASTNode *r = i->children()[1];

         auto it = alloc_stack.find(l->str());

         if (r->ast_type() == ADD)
         {
           
         }
         else 
         {


         if (it != alloc_stack.end()) // find it
         {
           cout << "movl $" << r->str() << ", " << it->second << "(%ebp)" << endl;
           text_section += "movl $" + r->str() + ", " + to_string(it->second) + "(%ebp)\n";
         }

         }
         
       }
       else if (i->ast_type() == ADD) 
            {
              ASTNode *l = i->children()[0];
              ASTNode *r = i->children()[1];
              cout << "left node: " << l->str() << " : " << l->type_str() << endl;
              cout << "right node: " << r->str() << " : " << r->type_str() << endl;

#if 0
              auto it = alloc_stack.find( (*rit)->str() );
             if (it != alloc_stack.end()) // find it
             {
               cout << "pushl " << it->second << "(%ebp)" << endl;
               text_section += "pushl " + to_string(it->second) + "(%ebp)\n";
             }
#endif

            }

    }
    cout << "leave" << endl;
    text_section += "leave\n";
    cout << "ret" << endl;
    text_section += "ret\n";
  }
  for (auto &i : children())
  {
    i->code_gen();
  }
}
#endif


void ASTNode::init()
{
  id_ = no_;
  ++no_;
  eval_result_ = 0;
  max_need_stack_size = cur_need_stack_size = 0;
  code_gen_state_ = NORMAL;

  gen_gas_op_[ADD] = &ASTNode::gen_gas_add_sub; // member function pointer
  gen_gas_op_[SUB] = &ASTNode::gen_gas_add_sub; // member function pointer
  gen_gas_op_[MUL] = &ASTNode::gen_gas_mul_div; // member function pointer
  gen_gas_op_[LESS] = &ASTNode::gen_gas_relation; // member function pointer
  gen_gas_op_[GREAT] = &ASTNode::gen_gas_relation; // member function pointer
  gen_gas_op_[EQUAL] = &ASTNode::gen_gas_relation; // member function pointer
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
    ASTNode *c1 = children_[0]->eval(env);

    uintptr_t addr_val = stoul(c1->str());
    //cout << "DEREF: " << hex << addr_val << dec << endl;
    string str = *((string*)addr_val);
    //cout << "xx str: " << str << endl;

    Token t(str, NUMBER);
    return new ASTNode(t);

#if 0
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
      ASTNode *f = env->lookup(children_[0]->str());
    if (f)
    {
    }
    else
    {
      cout << "can not found value!" << endl;
      return this;
    }

      void *addr = &f->token_.str_;
      uintptr_t addr_val = (uintptr_t)addr;
      #if 0
      cout << "yy addr: " << addr << endl;
      cout << "yy addr_val: " << hex << addr_val << dec << endl;
      #endif
      Token t(std::to_string(addr_val), NUMBER);
      return new ASTNode(t);
      //return children_[0];
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
        fmt.replace(pos, s1.length(), R"(%x)");
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
        #ifdef DEBUG_MSG
           cout << "op is =" << endl;
        #endif
           // add var/val to env
           ASTNode *c1 = children_[1]->eval(env);

           ASTNode *c0 = children_[0];

           if (c0->ast_type() != NAME)
           {
             cout << "= error: left side should be variable name" << endl;
             return this;
           }


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
           string s{c0->str()};

           #ifdef DEBUG_MSG
           cout << "s: " << s << endl;
           #endif
           ASTNode *f = env->lookup(c0->str());
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
                          ASTNode *then_node = 0;
                          ASTNode *else_node = 0;
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
                                                ASTNode *eval_while = 0;
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
