#ifndef MY_TYPE_H
#define MY_TYPE_H

#define OK 0
#define ERR 1

// ref: http://jafrog.com/2013/11/23/colors-in-terminal.html
// \x1b[0m means “reset all attributes”.
#define RED(str) "\x1b[31m" << str << "\x1b[0m"
#define BLUE(str) "\x1b[34m" << str << "\x1b[0m"
#define LIGHTBLUE(str) "\x1b[94m" << str << "\x1b[0m"
#define LIGHTYELLOW(str) "\x1b[93m" << str << "\x1b[0m"

typedef signed char s8;
typedef signed short s16;
typedef signed int s32;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

typedef unsigned long long u64;

enum ASTType 
{
  INVALID, ROOT, PROG, COMMENT,
  TRUE, FALSE,
  ENUM, INT, CHAR, 
  ADDR_OF, DEREF,
  IF, WHILE, RETURN, RETURN_VAL,
  THEN_BLOCK, ELSE_BLOCK, SEP, NEG, ASSIGN, EQUAL, NAME, GLOBAL_VAR, VAR, 
  FUNC_CALL, FUNC_BODY, FUNC_NAME, FUNC_PARA,
  ADD, MIN, MUL, DIV, GREAT, LESS, NUMBER, STRING, EOL, LAST
};

enum ErrorType
{
  FUNC_PARA_ARGU_NOT_MATCH=1, VAR_NOT_DEFINE, FUNC_NOT_DEFINE, FUNC_REDEFINITION, NEG_ERR,
};

enum CodeGenState
{
  NORMAL, DECLARE_VAR, STATEMENT, 
};

#endif
