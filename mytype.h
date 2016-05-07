#ifndef MY_TYPE_H
#define MY_TYPE_H

#define OK 0
#define ERR 1

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

enum ASTType {INVALID, ROOT, ENUM, INT, CHAR, THEN_BLOCK, ELSE_BLOCK, SEP, NEG, ASSIGN, EQUAL, NAME, VAR, ADD, MIN, MUL, DIV, GREAT, LESS, NUMBER, STRING, IF, WHILE, EOL, LAST};



#endif
