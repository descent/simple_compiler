#include <cstdio>
#include <string>
#include <cctype>

#include <iostream>
#include <deque>
#include <map>
#include <string>

#define PRINT_TREE_STRING
//#define WARN_MSG
//#define DEBUG_MSG
//#define DEBUG_LEXER_MSG

using namespace std;

bool need = true;

#include "lexer.h"
#include "parser.h"
#include "token.h"

// ref: http://lotabout.me/2016/write-a-C-interpreter-5/
/*
 * program ::= {global_declaration}+
 * modify program ::= global_declaration {global_declaration}
 * global_declaration ::= enum_decl | variable_decl | function_decl
 * enum_decl ::= 'enum' [id] '{' id ['=' 'num'] {',' id ['=' 'num'] '}'
 * variable_decl ::= type {'*'} id { ',' {'*'} id } ';'
 * function_decl ::= type {'*'} id '(' parameter_decl ')' '{' body_decl '}'
 * parameter_decl ::= type {'*'} id {',' type {'*'} id}
 * body_decl ::= {variable_decl}, {statement}
 * statement ::= non_empty_statement | empty_statement
 * non_empty_statement ::= if_statement | while_statement | '{' statement '}'
 *                      | 'return' expression | expression ';'
 * if_statement ::= 'if' '(' expression ')' statement ['else' non_empty_statement]
 * while_statement ::= 'while' '(' expression ')' non_empty_statement
*/


#define OP_PRECEDENCE

void err(const char *msg, const std::string &str, bool end=true)
{
  if (str=="\n")
    printf("%s, token: eol\n", msg);
  else
    printf("%s, token: %s\n", msg, str.c_str());
  if (end)
    exit(1);
}

std::map<std::string, Precedence*> operators;




Token peek_token(int i=0)
{
  if (tokens.size() == 0)
    return invalid_token;
  else
    return tokens[i];
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

bool is_token(ASTType type)
{
  Token t = peek_token();
  if (t.type() == type)
    return true;
  else
    return false;
}

bool is_token(const std::string &str)
{
  Token t = peek_token();
  if (t.str_ == str)
    return true;
  else
    return false;
}

bool is_type()
{
  if(is_token("int") || is_token("char"))
    return true;
  else
    return false;
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

/*!
 * primary   : "(" expr ")" | NUMBER | IDENTIFIER | STRING
 */
ASTNode* primary()
{
  Token token = peek_token(); 
  if (token.str_ == "(")
  {
    Token t = pop_token();
    ASTNode *e = expr();
    t = pop_token();
    if (t.str_ != ")")
    {
      cout << "fail: should ')'" << endl;
      exit(1);
    }
    return e;
  }
  else if (token.type() == NUMBER || token.type() == NAME) // number || variable name
       {
         Token t = pop_token();
         return new ASTNode(t);
       }
       else if (token.type() == STRING)
            {
              Token t = pop_token();
              return new ASTNode(t);
            }
            else
            {
              if (need == true)
                err("primary: no match primer rule", token.str());
              else
                #ifdef WARN_MSG
                err("primary: warn!!", token.str_, false);
                #else
                ;
                #endif
            }
  return 0;
}

// factor    : "-" primary | primary
ASTNode* factor()
{
  ASTNode *op = 0;

  Token token = peek_token(); 
  if (token.str_ == "-")
  {
    Token t = pop_token();
    op = new ASTNode(t);
    t.type_ = NEG;
    ASTNode *p = primary();
    op->add_child(p);
  }
  else
  {
    op = primary();
  }

  return op;
}

// expr      : factor { OP factor} 
ASTNode* expr()
{
  ASTNode *r = factor();
  Precedence *next;

  while((next = next_op()) != 0)
  {
    r = do_shift(r, next->value_);
    // left = new ASTNode(left, , right);
  }
  return r;
}

// block     : "{" [ statement ] { (";" | EOL) [ statement ] } "}"
// modify - block     : "{" [ statement ] { ";" [ statement ] } "}"
// modify 2 - block     : "{" { statement } "}"
ASTNode* block()
{
  Token token = peek_token(); 

  ASTNode *b = new ASTNode();

  if (token.str_ == "{")
  {
    pop_token();
  
    while(is_token("}") == false)
    {
      need = false;
      ASTNode *s = statement();
      need = true;
      if (s)
        b->add_child(s);
    }


#if 0
    need = false;
    ASTNode *s = statement();
    if (s)
      b->add_child(s);
    need = true;

    while(is_token(";") || is_token("\n"))
    {
      pop_token();

      need = false;
      ASTNode *s = statement();
      if (s)
        b->add_child(s);
      #if 0
      need = true;
      if (s==0)
        continue;
      if (b)
        b->add_child(s);
      else
        b=s; // s is possiable 0??
      #endif
    }
#endif
    Token t = peek_token();
    if (t.str() == "}")
    {
      Token t = pop_token();
    }
    else
    {
      err("block: should '}'", t.str_);
    }


  }
  else
  {
    err("block: should '{'", token.str_);
  }
  return b;
}

/// func_call: NAME '(' [ (expr)  { ',' (expr) } ] ')'
ASTNode* func_call()
{
  ASTNode *fc = 0;
  Token t = peek_token();

  if (t.type() == NAME)
  {
    fc = new ASTNode(func_call_token);

    ASTNode *e=0;
    pop_token();
    t = peek_token();
    if (t.str() == ("("))
    {
      pop_token();

      //if((e = expr()) != 0)
      t = peek_token();
      if(t.str() != ")")
      {
        e = expr();
        if (e)
          fc->add_child(e);

        while (is_token(")") == false)
        {
            Token t = peek_token();
            if (t.str() == ",")
            {
              pop_token();
              e = expr();
              if (e);
                fc->add_child(e);
            }
            else
            {
              err("func_call: should ','", t.str());
            }

        } 

      }
      else // function call passes no argument
      {

      }

      t = peek_token();
      if (t.str() == (")"))
      {
        pop_token();
      }
      else
      {
        err("func_call: should )", t.str());
      }
    }
    else
    {
      err("func_call: should (", t.str());
    }
  }
  else
  {
    err("func_call: should NAME", t.str());
  }

  return fc;
}

// simple    : expr | func_call
ASTNode* simple()
{
  ASTNode *e=0;

  Token t = peek_token();
  if (t.type() == NAME)
  {
    t = peek_token(1);
    if (t.str() == "(") // func_call
    {
      e = func_call();
      return e;
    }
  }

  e = expr();

  #if 0
  while(is_token("\n"))
  {
    pop_token();
  }
  #endif
  return e;
}

#if 1
/*
 * statement :   "if" expr block [ "else" block ] 
 *               | "while" expr block
 *               | simple
 */

/*
 * modify - statement :   "if" "(" expr ")" block [ "else" block ] 
 *                        | "while" "(" expr ")" block
 *                        | simple ";"
 */
ASTNode* statement()
{
  ASTNode *s_node = 0; // statement node
  Token token = peek_token(); 

  if (token.str_ == "if")
  {
    Token t = pop_token();
    t.type_ = IF;
    s_node = new ASTNode(t);

    ASTNode *e = 0;
    t = peek_token(); 
    if (t.str_ == "(")
    {
      pop_token();
      e = expr();
    }
    else
    {
      err("statement: should '('", t.str_);
    }
    t = peek_token(); 
    if (t.str_ == ")")
    {
      pop_token();
    }
    else
    {
      err("statement: should ')'", t.str_);
    }

    ASTNode *then_b = block();
    then_b->set_token(then_block);

    s_node->add_child(e, then_b);
    //s_node->add_child(then_b->children());

    Token token = peek_token(); 
    if (token.str_ == "else")
    {
      Token t = pop_token();
      ASTNode *else_b = block();
      else_b->set_token(else_block);
      s_node->add_child(else_b);
    }
    #if 0
    else
    {
      err("statement: should 'else'", token.str_);
    }
    #endif

  }
  else if (token.str_ == "while")
       {
         Token t = pop_token();
         t.type_ = WHILE;
         s_node = new ASTNode(t);
         ASTNode *e = 0;
         t = peek_token(); 
         if (t.str_ == "(")
         {
           pop_token();
           e = expr();
         }
         else
         {
           err("statement: should '('", t.str_);
         }

         t = peek_token(); 
         if (t.str_ == ")")
         {
           pop_token();
         }
         else
         {
           err("statement: should ')'", t.str_);
         }

         ASTNode *b = block();
         s_node->add_child(e, b);
       }
       else // simple
       {
         s_node = simple();
         if (is_token(";"))
         {
           pop_token();
         }
         else
         {
           Token t = peek_token();
           err("statement|simple: should be ;", t.str());
         }

       }

  return s_node;
}
#endif

bool is_token_type(ASTType type)
{
}

ASTNode* non_empty_statement()
{
}

ASTNode* empty_statement()
{
}

#if 0
// statement ::= non_empty_statement | empty_statement
ASTNode* statement()
{
    // there are 6 kinds of statements here:
    // 1. if (...) <statement> [else <statement>]
    // 2. while (...) <statement>
    // 3. { <statement> }
    // 4. return xxx;
    // 5. <empty statement>;
    // 6. expression; (expression end with semicolon)
  ASTNode* s = 0;
  return s;
}
#endif

ASTNode* variable_decl()
{
}

// body_decl ::= {variable_decl}, {statement}
ASTNode* body_decl()
{
  ASTNode *body_node = new ASTNode(func_body_token);
  while(is_token("int") || is_token("char"))
  {
    pop_token();
    ASTNode *v = var_decl();
    if (v)
      body_node->add_child(v);
  }

  while (is_token("}") == false)
  {
    //body_node = new ASTNode(var_token); // new statement node
    //pop_token();
    ASTNode *s = statement();
    if (s)
      body_node->add_child(s);
  }

  return body_node;
}

// parameter_decl ::= type {'*'} id {',' type {'*'} id}
ASTNode* parameter_decl()
{
  ASTNode *var_node = 0;

  if(is_token("int") || is_token("char"))
  {
    var_node = new ASTNode(para_token);
    pop_token();

  while(is_token("*"))
  {
    pop_token();
  }
  if (is_token(NAME))
  {
    Token t = pop_token();
    ASTNode *v = new ASTNode(t);
    var_node->add_child(v);
  }

  while(is_token(","))
  {
    pop_token();

    if(is_token("int") || is_token("char"))
    {
      pop_token();
      while(is_token("*"))
        pop_token();
      if (is_token(NAME))
      {
        Token t = pop_token();
        ASTNode *v = new ASTNode(t);
        var_node->add_child(v);
      }
    }
    else
    {
      Token t = peek_token();
      err("parameter_decl: should be type (ex: int or char)", t.str());
    }
  }

  }
  #if 0
  else
  {
    Token token = peek_token(); 
    err("parameter_decl: should type(ex: int or char)", token.str());
  }
  #endif

  return var_node;
}

// function_decl ::= type {'*'} id '(' parameter_decl ')' '{' body_decl '}'
ASTNode* func_decl()
{
  ASTNode *func_node = new ASTNode(func_token);

  while(is_token("*"))
  {
    pop_token();
  }
  if (is_token(NAME))
  {
    Token t = pop_token();
  }
  else
  {
    Token t = peek_token();
    err("func_decl: should NAME\n", t.str());
  }

  if (is_token("("))
  {
    Token t = pop_token();
  }
  else
  {
    Token t = peek_token();
    err("func_decl: should (\n", t.str());
  }

  ASTNode *func_para = parameter_decl();

  if (is_token(")"))
  {
    Token t = pop_token();
  }
  else
  {
    Token t = peek_token();
    err("func_decl: should )\n", t.str());
  }

  if (is_token("{"))
  {
    Token t = pop_token();
  }
  else
  {
    Token t = peek_token();
    err("func_decl: should {\n", t.str());
  }
  if (func_para)
    func_node->add_child(func_para);

  ASTNode *func_body = body_decl();
  if (func_body)
    func_node->add_child(func_body);
    

  if (is_token("}"))
  {
    Token t = pop_token();
  }
  else
  {
    Token t = peek_token();
    err("func_decl: should }\n", t.str());
  }

  return func_node;
}

// variable_decl ::= type {'*'} id { ',' {'*'} id } ';'
ASTNode* var_decl()
{
  ASTNode *var_node = new ASTNode(var_token);

  while(is_token("*"))
  {
    pop_token();
  }
  if (is_token(NAME))
  {
    Token t = pop_token();
    ASTNode *v = new ASTNode(t);
    var_node->add_child(v);
  }

  while(is_token(","))
  {
    pop_token();
    while(is_token("*"))
      pop_token();
    if (is_token(NAME))
    {
      Token t = pop_token();
      ASTNode *v = new ASTNode(t);
      var_node->add_child(v);
    }
  }

  if (is_token(";"))
    pop_token();
  else
  {
    Token token = peek_token(); 
    err("var_decl: should ;", token.str_);
  }
  return var_node;
}

ASTNode* enum_decl()
{
}

// global_declaration ::= enum_decl | variable_decl | function_decl
ASTNode* global_declaration()
{
  ASTNode* g;

  if (is_token("enum"))
  {
    pop_token();
    enum_decl();
  }
  else if(is_token("int") || is_token("char"))
       {
         pop_token();
         int skip_start = 0; // skip *, **, *** ...
         Token t = peek_token(skip_start); 

         while (t.str() == "*")
         {
           ++skip_start;
           t = peek_token(skip_start); 
         }

         if (t.type()==NAME)
         {
           ++skip_start;
           Token t = peek_token(skip_start); // ll(n)
           if (t.str() == "(")
           {
             g = func_decl();
           }
           else // variable decl
             g = var_decl();
         }
         else 
         {
           Token t = peek_token();
           err("global_declaration: should var name", t.str());
         }
       }
       else
       {
         Token token = peek_token(); 
         err("global_declaration: no match global_declaration rule", token.str_);
       }
  return g;
}


// program ::= {global_declaration}+
// modify program ::= global_declaration {global_declaration}
ASTNode* program()
{
  ASTNode *p = new ASTNode(prog_token);
#if 0
  ASTNode *g = func_call();
  p->add_child(g);
#endif
#if 1
  ASTNode *g = global_declaration();
  if (g)
    p->add_child(g);
  Token token = peek_token(); 
  while(token.valid())
  {
    ASTNode *g = global_declaration();
    p->add_child(g);
    //pop_token();
    token = peek_token(); 
  }
#endif
  return p;
}

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
  {
#ifdef DEBUG_MSG
    cout << "cannot find op: " << t.str_ << endl;
#endif
    return 0;
  }
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

ASTNode *get_root()
{
  static ASTNode *root = 0;

  if (root == 0)
  {
    Token root_token("root", ROOT);
    root = new ASTNode(root_token);
  }

  return root;
}

#ifdef DEBUG_PARSER
int main(int argc, char *argv[])
{
  operators.insert({"=", new Precedence{1, false}});
  operators.insert({"==", new Precedence{2, true}});
  operators.insert({">", new Precedence{2, false}});
  operators.insert({"<", new Precedence{2, false}});
  operators.insert({"+", new Precedence{3, true}});
  operators.insert({"-", new Precedence{3, true}});
  operators.insert({"*", new Precedence{4, true}});

  lexer(); 

  ASTNode *root = get_root();
  ASTNode* n=0;

  while (tokens.size()>0 && (n = program()))
  {
    if (n)
      root->add_child(n);

#if 0
    if (n)
    {
      root->add_child(n);
#ifndef PRINT_TREE_STRING
      cout << "ast node type: " << root->type_str() << endl;
      root->print();
      cout << endl;
#else
      cout << "\\tree";
      root->print_tree();
    }
    cout << endl;
#endif
#endif
  }


#ifdef PRINT_TREE_STRING
  cout << "\\tree";
  root->print_tree();
  cout << endl;
#else
  cout << "ast node type: " << root->type_str() << endl;
  root->print();
  cout << endl;
#endif

  return 0;
}
#endif
