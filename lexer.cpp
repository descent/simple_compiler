#include <cstdio>
#include <string>
#include <cctype>

#include <iostream>
#include <vector>
#include <deque>

#include "mytype.h"
#include "lexer.h"

using namespace std;

//#define LEX_COLOR
//#define DEBUG_LEXER_MSG

#if 0
#define ESCAPE_CHAR(n)
case n:
{
  token.push_back(n); 
  break;
}
#endif


static inline int isascii_ex(int c) 
{
  if (c == '"')
    return 0;
  else
    return isascii(c);
}

static inline int isgraph_include_space(int c) 
{
  if (c == ' ')
    return 1;
  
  return isgraph(c);
}

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

int la=-1; // look ahead

int ungetchar_la(int c)
{
    if (c != EOF)
      la = c;
}

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

// " asdf "
int get_string_token(string &token)
{
  int c;

#if 0
  do
  {
    c = getchar_la();
  }while(isspace(c));
#endif

  c = getchar_la();
  while (isascii_ex(c))
  {
    if (c=='\\')
    {
      c = getchar_la();
      switch (c)
      {
        case 'a':
        {
          token.push_back('\a'); 
          break;
        }
        case 'b':
        {
          token.push_back('\b'); 
          break;
        }
        case 'f':
        {
          token.push_back('\f'); 
          break;
        }
        case 'n':
        {
          token.push_back('\n'); 
          break;
        }
        case 'r':
        {
          token.push_back('\r'); 
          break;
        }
        case 't':
        {
          token.push_back('\t'); 
          break;
        }
        case 'v':
        {
          token.push_back('\v'); 
          break;
        }
        default:
        {
          token.push_back(c); 
          break;
        }
      }
    }
    else
    {
      token.push_back(c); 
    }
    c = getchar_la();
  }

  if (c=='"')
  {
    //token.push_back(c); 
    return OK;
  }
  else
  {
    printf("should \"\n");
    if (c != EOF)
      la = c;
    return ERR;
  }

  return OK; 
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
#ifdef LEX_COLOR
//  这句话, 决定是否打印空格，如果不输出空格，source code 中空格将被去掉,
//  所有 source code 挤在一起
    if(isspace(c)) 
      printf("%c", c);
#endif
    
#ifdef GET_EOL
  }while(c == ' ');
#else
  }while(isspace(c));
#endif

  if (c == EOF)
    return EOF;
  else if (isdigit(c))
       {
         do
         {
           token.str_.push_back(c);
           c = getchar_la();
         }while(isdigit(c));
         token.ast_type_ = NUMBER;
       }
       else if (isalpha(c))
            {
              do
              {
                token.str_.push_back(c); 
                c = getchar_la();
              } while(isalnum(c));
              if (token.str() == "char")
                token.ast_type_ = CHAR;
              else if (token.str() == "int")
                     token.ast_type_ = INT;
                   else if (token.str() == "enum")
                          token.ast_type_ = ENUM;
                        else if (token.str() == "if")
                               token.ast_type_ = IF;
                             else if (token.str() == "while")
                                    token.ast_type_ = WHILE;
                                  else
                                    token.ast_type_ = NAME;
                   
            }
            else if (c == '"')
            {
              int ret = get_string_token(token.str_);

              if (ret == OK)
              {
                token.ast_type_ = STRING;
              }

              return ret;
            }
            #if 1
            else if (c == '\'') // 'c' char
                 {
                   c = getchar_la();
                   if (isgraph_include_space(c))
                   {
                     token.str_.push_back(c); 
                     token.ast_type_ = S8;
                   }
                   else
                   {
                     ungetchar_la(c);
                     return ERR;
                   }
                   c = getchar_la();
                   if ('\'' != c)
                   {
                     ungetchar_la(c);
                     return ERR;
                   }
                   else
                     return OK;
                 }
                 #endif
                 else
       {
         switch (c)
         {
           case '&':
           {
             token.str_ = "&";
             token.ast_type_ = ADDR_OF;
             break;
           }
           case '<':
           {
             token.str_ = "<";
             token.ast_type_ = LESS;
             break;
           }
           case '>':
           {
             token.str_ = ">";
             token.ast_type_ = GREAT;
             break;
           }
           case '+':
           {
             token.str_ = "+";
             token.ast_type_ = ADD;
             break;
           }
           case '-':
           {
             token.str_ = "-";
             token.ast_type_ = SUB;
             break;
           }
           case '*':
           {
             token.str_ = "*";
             token.ast_type_ = MUL;
             break;
           }
           case '/':
           {
             c = getchar_la();
             if (c == '/') // comment symbol: //
             {
               do
               {
                 c = getchar_la();
               } while(c != '\n' && c != EOF);
               token.ast_type_ = COMMENT;
             }
             else
             {
               token.str_ = "/";
               token.ast_type_ = DIV;
             }
             la = c; // put back
             break;
           }
#ifdef GET_EOL
           case '\n':
           {
             token.str_ = "\n";
             token.ast_type_ = EOL;
             break;
           }
#endif
           case ',':
           {
             token.str_ = ",";
             token.ast_type_ = SEP;
             break;
           }
           case ';':
           {
             token.str_ = ";";
             token.ast_type_ = SEP;
             break;
           }
           case '{':
           {
             token.str_ = "{";
             token.ast_type_ = SEP;
             break;
           }
           case '}':
           {
             token.str_ = "}";
             token.ast_type_ = SEP;
             break;
           }
           case '(':
           {
             token.str_ = "(";
             token.ast_type_ = SEP;
             break;
           }
           case ')':
           {
             token.str_ = ")";
             token.ast_type_ = SEP;
             break;
           }
           case '=':
           {
             c = getchar_la();
             if (c == '=')
             {
               token.str_ = "==";
               token.ast_type_ = EQUAL;
             }
             else
             {
               la = c;
               token.str_ = "=";
               token.ast_type_ = ASSIGN;
             }
             break;
           }
           default:
           {
             token.str_.push_back(c); 
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
      if (token.ast_type() != COMMENT)
      {
        tokens.push_back(token);
  #ifdef DEBUG_LEXER_MSG
        if (token.str_ == "\n")
          cout << "token: eol";
        else
          cout << "token: " << token.str_;
        cout << " / " << token.ast_type_str() << endl;
  #endif
      }
    }
    else
    {
      if (token.str_ == "\n")
        cout << "error token: eol" << endl;
      else
      cout << "error token: " << token.str_ << endl;
    }
    // token.str_.clear();
  }

  return 0;
}

int lexer_color()
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
      if (ENUM <= token.ast_type() && token.ast_type() <= WHILE)
      {
        cout << LIGHTBLUE(token.str());
      }
      else
        cout << token.str();
    }
    else
    {
      if (token.str_ == "\n")
        cout << "error token: eol" << endl;
      else
      cout << "error token: " << token.str_ << endl;
    }
    // token.str_.clear();
  }

  return 0;
}

#ifdef DEBUG_LEXER


//ref: http://stackoverflow.com/questions/11672876/colored-console-output-in-linux
//printf("\x1b[31m%s\x1b[0m\n", i.str().c_str());
//cout << "\x1b[34m" << i.str() << "\x1b[0m" << endl;

int main(int argc, char *argv[])
{
  //int a,b;
  //a++ + ++b;

#ifdef LEX_COLOR
  lexer_color();
#else
  lexer();
#endif
  // http://misc.flogisoft.com/bash/tip_colors_and_formatting

#if 0
  for (auto i : tokens)
  {
    if (ENUM <= i.type() && i.type() <= WHILE)
      cout << LIGHTBLUE(i.str()) << endl;
    else
      cout << i.str() << endl;
  }
#endif
  return 0;
}
#endif
