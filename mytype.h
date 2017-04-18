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

#if 0
enum ASTType 
{
  INVALID, ROOT, PROG, COMMENT,
  TRUE, FALSE,
  ENUM, INT, CHAR, S8,
  ADDR_OF, DEREF,
  IF, WHILE, WHILE_BLOCK, RETURN, RETURN_VAL,
  THEN_BLOCK, ELSE_BLOCK, SEP, NEG, ASSIGN, EQUAL, 
  NAME, GLOBAL_VAR, VAR, PARA,
  FUNC_CALL, FUNC_BODY, FUNC_NAME, FUNC_PARA,
  ADD, SUB, MUL, DIV, GREAT, LESS, NUMBER, STRING, EOL, LAST
};
#endif

// ref: http://stackoverflow.com/questions/9907160/how-to-convert-enum-names-to-string-in-c
// thanks for coldnew

#define FOREACH_FRUIT(FRUIT) \
        FRUIT(INVALID)   \
        FRUIT(ROOT)  \
        FRUIT(PROG)   \
        FRUIT(COMMENT)  \
        FRUIT(TRUE)  \
        FRUIT(FALSE)  \
        FRUIT(ENUM)  \
        FRUIT(INT)  \
        FRUIT(CHAR)  \
        FRUIT(S8)  \
        FRUIT(ADDR_OF)  \
        FRUIT(DEREF)  \
        FRUIT(IF)  \
        FRUIT(WHILE)  \
        FRUIT(WHILE_BLOCK)  \
        FRUIT(RETURN)  \
        FRUIT(RETURN_VAL)  \
        FRUIT(THEN_BLOCK)  \
        FRUIT(ELSE_BLOCK)  \
        FRUIT(SEP)  \
        FRUIT(NEG)  \
        FRUIT(ASSIGN)  \
        FRUIT(EQUAL)  \
        FRUIT(NAME)  \
        FRUIT(GLOBAL_VAR)  \
        FRUIT(VAR)  \
        FRUIT(PARA)  \
        FRUIT(FUNC_CALL)  \
        FRUIT(FUNC_BODY)  \
        FRUIT(FUNC_NAME)  \
        FRUIT(FUNC_PARA)  \
        FRUIT(ADD)  \
        FRUIT(SUB)  \
        FRUIT(MUL)  \
        FRUIT(DIV)  \
        FRUIT(GREAT)  \
        FRUIT(LESS)  \
        FRUIT(NUMBER)  \
        FRUIT(STRING)  \
        FRUIT(EOL)  \
        FRUIT(LAST)

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,

enum ASTType 
{
  FOREACH_FRUIT(GENERATE_ENUM)
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
