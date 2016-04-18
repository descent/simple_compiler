#ifndef MY_TYPE_H
#define MY_TYPE_H

#define OK 0
#define ERR 1

typedef signed char s8;
typedef signed short s16;
typedef signed int s32;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

typedef unsigned long long u64;

enum ASTType {INVALID, SEP, ADD, MIN, MUL, DIV, NUMBER, LAST};


#endif
