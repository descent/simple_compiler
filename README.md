This is simple c compiler, but current only support interpreter.
===========
A simple c implementation by c++.

current feature:
* char, int, pointer type
* function call
* if/else
* while
* // comment
* support printf

[eval expression] (http://test-template0.blogspot.tw/2016/07/smple-c-interpreter-1.html) 


[display AST] (http://test-template0.blogspot.tw/2016/07/eval-12345.html)


x86 32bit machine code generator:
  tmp.s is gas AT&T asm syntax format.
  as -32 -o tmp.o tmp.s 
  gcc -m32 tmp.o -o tmp
  
run tmp
  ./tmp

