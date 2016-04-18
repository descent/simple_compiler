#include <cstdio>
#include <string>
#include <cctype>

#include <iostream>
#include <vector>
#include <deque>

#include "mytype.h"
#include "token.h"

using namespace std;


// printable ascii, but not (, )
static inline int isgraph_ex(int c) 
{
#if 1
  if (c == '(')
    return 0;
  if (c == ')')
    return 0;
#endif
  return isgraph(c);
}

int la=-1;

int getchar_la()
{
  if (la != -1)
  {
    int tmp=la;
    la = -1;
    return tmp;
  }
  else
    return getchar();
}

int get_token(string &token)
{
  int c;

  do
  {
    c = getchar_la();
  }while(isspace(c));

  if (c == EOF)
    return EOF;
  else if (isdigit(c))
       {
         do
         {
           token.push_back(c); 
           c = getchar_la();
         }while(isdigit(c));
       }
       else if (isalpha(c))
            {
              do
              {
                token.push_back(c); 
                c = getchar_la();
              } while(isalnum(c));
            }
            else if (c == '=')
                 {
                   c = getchar_la();
                   if (c == '=')
                   {
                     token = "==";
                   }
                   else
                   {
                     la = c;
                     token = "=";
                   }
                   return OK;
                 }
                 else
                 {
                   return ERR;
                 }
  if (c != EOF)
    la = c;
  return OK;
}

int get_se_token(string &token)
{
  int c;

  do
  {
    c = getchar_la();
  }while(isspace(c));

  if (c == EOF)
    return EOF;
  else if (c == '(')
       {
         token = '(';
         return OK;
       }
       else if (c == ')')
            {
              token = ')';
              return OK;
            }
            else if (isgraph_ex(c)) // printable ascii, but not (, )
                 {
                   do
                   {
                     token.push_back(c); 
                     c = getchar_la();
                   }while(isgraph_ex(c));
                 }
                 else
                 {
                   return ERR;
                 }


  if (c != EOF)
    la = c;
  return OK;
}

// 1+2
int get_token(Token &token)
{
  int c;

  do
  {
    c = getchar_la();
  }while(isspace(c));

  if (c == EOF)
    return EOF;
  else if (isdigit(c))
       {
         do
         {
           token.str_.push_back(c);
           c = getchar_la();
         }while(isdigit(c));
         token.type_ = NUMBER;
       }
       else 
       {
         switch (c)
         {
           case '+':
           {
             token.str_ = "+";
             token.type_ = ADD;
             break;
           }
           case '-':
           {
             token.str_ = "-";
             token.type_ = MIN;
             break;
           }
           case '*':
           {
             token.str_ = "*";
             token.type_ = MUL;
             break;
           }
           case '/':
           {
             token.str_ = "/";
             token.type_ = DIV;
             break;
           }
           case '(':
           {
             token.str_ = "(";
             token.type_ = SEP;
             break;
           }
           case ')':
           {
             token.str_ = ")";
             token.type_ = SEP;
             break;
           }
           default:
           {
             return ERR;
           }
         }
         return OK;
       }

  if (c != EOF)
    la = c;
  return OK;
}

std::deque <Token> tokens;

int lexer()
{
  while(1)
  {
    //string token;
    Token token;

    int ret = get_token(token);
    //int ret = get_se_token(token);
    if (ret == EOF)
    {
      break;
    }
    if (ret == OK)
    {
      tokens.push_back(token);
      cout << "token: " << token.str_ << endl;
    }
    else
    {
      cout << "token error" << endl;
    }
    token.str_.clear();
  }

  return 0;
}

#ifdef DEBUG_LEXER

int main(int argc, char *argv[])
{
  lexer();
  return 0;
}
#endif
