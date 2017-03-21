#include <stdio.h>

int main()
{
  int i;
  int j;
  int k;
  char c;

  i=8;
  j=3;
  k=5;
  c='~';
  printf("sizeof(c): %u\n", sizeof('~'));
  //c='x'; // current version will get segment fault

  //printf("i = %d\n", i);
  //printf("j = %d\n", j);
  i = j+3>5;
  //printf("i = %d, c = %c\n", i, c);
  printf("i = %d, j = %d, k = %d, c=%c\n", i, j, k, c);
  //i=j;
  //printf("i=j, i = %d\n", i);


  //printf("i < j: %d\n", i < j);
  //printf("i > j: %d\n", i > j);


  //printf("(2 < 3) < (6 < 8) : %d\n", (2 < 3) < (6 < 8));
  //printf("3 < 5 > 8 : %d\n", 3 < 5 > 8);
  //printf("3 < (5 > 8) : %d\n", 3 < (5 > 8));
//  1<2+3;
//  3 * 2 > 5;
//  3 * (2 > 5);
  //1+2+3;
//  printf("1<2 : %d\n", 1<2);
//  printf("1>2 : %d\n", 1>2);

//  i=5;
//  j=9;
//  printf("5+i = %d\n", 5+i);
//  printf("i < j : %d\n", i < j);
//  printf("i > j : %d\n", i > j);
//
//  i=23;
//  i>2;
//  j=5678;
  //printf("i = %d\n", i+j);
  //printf("j = %d\n", j);
  //i=6;
  //j=2;

//  i = 1>2*2;
//  1>2+3;



  //printf("i+5 = %d\n", i+5);
//  printf("i = %d\n", i);
//  printf("j = %d\n", j);
//  printf("i+j = %d\n", i+j);
  //printf("i+5 = %d\n", i+5);
  //printf("5+(1+2) = %d\n", 5+(1+2));
  //printf("5+i = %d\n", 5+i);
  //printf("i*j = %d\n", i*j);

  //printf("5*i = %d\n", 5*i);
  //printf("5*i+j = %d\n", 5*i+j);
  //printf("5*(i+j) = %d\n", 5*(i+j));
  //printf("j*7 = %d\n", j*7);

  //(1+2)+((3+4)+(5+6));
  //printf("1+2 = %d\n", 1+2);
  //printf("(1+2)+(3+4) = %d\n", (1+2)+(3+4));
  //printf("1+2*3 = %d\n", 1+2*3);
  //printf("xx 5*(6+7) = %d\n", 5*(6+7));
  //printf("yy %d\n", 1+2);
  //(2*3*4) * (5*(7*(8*9)));
  // (2*3)*(5*6);
  
  //3*9;
  //4*(2*3);
  //2-1-6;

  // toplevel if (child[0]->is_leaf() && child[1]->is_leaf() != true)
  // toplevel if (child[0]->is_leaf() != true && child[1]->is_leaf())
  //1+2+5;
  //5+(1+2);

  //printf("%d\n", (1+2+3)+ (5+6+7)+(9+10));

  // if (child[0]->is_leaf() != true && child[1]->is_leaf())
  //(9+10) + ((1+2+3)+ (5+6+7));
  //(1+2)+9+(3+5)+(7+8);

  // (child[0]->is_leaf() && child[1]->is_leaf() != true)
  //(3+(5+6)+9) + (1+2);
  //(1+2) + (9 + (3 +(5+6)));

  //(1+2)+(3+4+5);
  //(1+2)+(3+4)+(5+6) + (7+8);

  //1+2+3+4+5;
  //1+2;
  //1+(2+3);
  //1+2+3;
  //(1+2)+(3+4);
}

