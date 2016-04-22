#include <cstdio>
#include <string>
#include <cctype>

#include <iostream>
#include <deque>
#include <map>
#include <string>

using namespace std;

#include "astnode.h"
#include "parser.h"
#include "token.h"
#include "op.h"

#define OP_PRECEDENCE

std::map<std::string, Precedence*> operators;

extern std::deque <Token> tokens;



Token peek_token()
{
  if (tokens.size() == 0)
    return invalid_token;
  else
    return tokens[0];
}

Token pop_token()
{
  if (tokens.size() == 0)
    return invalid_token;
  else
  {
    Token t = tokens.front();
    tokens.pop_front();
    return t;
  }
}

bool is_token(const std::string &str)
{
  Token t = peek_token();
  if (t.str_ == str)
    return true;
  else
    return false;
}


ASTNode* factor()
{
  Token token = peek_token(); 
  if (token.str_ == "(")
  {
    Token t = pop_token();
    ASTNode *e = expression();
    t = pop_token();
    if (t.str_ != ")")
    {
      cout << "fail: should ')'" << endl;
      exit(1);
    }
    return e;
  }
  else if (token.type_ == NUMBER || token.type_ == NAME) // number || variable name
       {
         Token t = pop_token();
         return new ASTNode(t);
       }
       else
       {
       }
  return 0;
}

ASTNode* term()
{
  ASTNode *left = factor();
  while(is_token("*") || is_token("/"))
  {
    Token t = pop_token();
    ASTNode *op = new ASTNode(t);
    ASTNode *right = factor();
    op->add_child(left, right);
    left = op;
  }

  return left;
}

#ifdef OP_PRECEDENCE

/* p 45 EBNF
 * primary   : "(" expr ")" | NUMBER | IDENTIFIER | STRING
 * factor    : "-" primary | primary
 * expr      : factor { OP factor}
 * block     : "{" [ statement ] { ("; " | EOL) [ statement ]} "}"
 * simple    : expr
 * statement :   "if" expr block [ "else" block ] 
 *             | "while" expr block
 *             | simple
 * program   : [ statement ] ("; " | EOL)
 */

// operator precedence parsing
/*
 *  factor: NUMBER | "(" expression ")"
 *  expression: factor { OP factor}
 */

Precedence* next_op()
{
  Token t = peek_token();
  auto it = operators.find(t.str_);

  if (it != operators.end())
    return it->second;
  else
    return 0;
}

bool right_is_expr(int prec, Precedence* next_prec)
{
  if (next_prec->left_assoc_)
    return prec < next_prec->value_;
  else
    return prec <= next_prec->value_;
}

ASTNode* do_shift(ASTNode* l, int prec)
{
  Token t = pop_token();
  ASTNode *op = new ASTNode(t);
  ASTNode *r = factor();
  Precedence *next;

  while((next = next_op()) != 0 && right_is_expr(prec, next))
  {
    r = do_shift(r, next->value_);
  }

  op->add_child(l, r);
  return op;
}

ASTNode* expression()
{
  ASTNode *r = factor();
  Precedence *next;

  while((next = next_op()) != 0)
  {
    r = do_shift(r, next->value_);
    // left = new ASTNode(left, , right);
  }
  #if 0
  #endif
  return r;
}
#else
/*
 *  factor: NUMBER | "(" expression ")"
 *  term: factor { ("*" | "/") factor}
 *  expression: term {("+" | "-") term}
 */

ASTNode* expression()
{
  ASTNode *left = term();
  while(is_token("+") || is_token("-"))
  {
    Token t = pop_token();
    ASTNode *op = new ASTNode(t);
    ASTNode *right = factor();
    op->add_child(left, right);
    left = op;
    // left = new ASTNode(left, , right);
  }

  return left;
}

#endif


#ifdef DEBUG_PARSER
int main(int argc, char *argv[])
{
  operators.insert({"+", new Precedence{1, true}});
  operators.insert({"*", new Precedence{3, true}});
  operators.insert({"=", new Precedence{0, false}});
  operators.insert({"==", new Precedence{-1, true}});

  int lexer();
  lexer(); 
  ASTNode* root = expression();
  cout << "ast node type: " << root->type_str() << endl;
  root->print();
  cout << endl;
#if 0
  while(1)
  {
    string token;

    //int ret = get_token(token);
    int ret = get_se_token(token);
    if (ret == EOF)
    {
      break;
    }
    if (ret == OK)
    {
      cout << "token: " << token << endl;
    }
    else
    {
      cout << "token error" << endl;
    }
    token.clear();
  }
#endif
  return 0;
}
#endif
