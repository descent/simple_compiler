
int f1(int i, int j)
{
  int c;

  c=i+j;
  return c;
}

int main()
{
  int x;
  int a,b;

  a=5;
  b=6;

  f1(a, b);

  x = -1+(-a+b);
  printf("x: %d\n", x);

}
