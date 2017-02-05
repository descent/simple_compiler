int main()
{
  int i;
  int j;
  printf("1<2 : %d\n", 1<2);
  printf("1>2 : %d\n", 1>2);

  i=5;
  j=9;
  printf("i = %d\n", i);
  printf("5+i = %d\n", 5+i);
  printf("j = %d\n", j);
  printf("i < j : %d\n", i < j);
  printf("i > j : %d\n", i > j);

  //i=23;
  //j=5678;
  //printf("i = %d\n", i+j);
  //printf("j = %d\n", j);
  i=2+9;
  j=1;
  //printf("i+j = %d\n", i+j);

  //printf("i+5 = %d\n", i+5);

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

