#include <cstdio>
#include <string>
#include <cctype>

#include <iostream>
#include <deque>

using namespace std;

#include "astnode.h"
#include "parser.h"
#include "token.h"

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
  }
  else if (token.type_ == NUMBER) // number
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
  }

  return left;
}

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


#ifdef DEBUG_PARSER
int main(int argc, char *argv[])
{
  int lexer();
  lexer(); 
  ASTNode* root = expression();
  cout << "ast node type: " << root->type_str() << endl;
  root->print();
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
