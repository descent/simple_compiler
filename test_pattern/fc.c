
char func123()
{
  char c;
  int i;

  c='a';
  i = 567;
  printf("i: %d, c: %d\n", i, c);
}

char func678(char c)
{
  //c='a';
  printf("c: %c\n", c);
}

int main()
{
  //char func678(char c);

  char a;

  a='1';

  func678('5');
  func123();
  //func678(3+8);
  return 0;
}
