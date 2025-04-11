#include <stdio.h>
#include <pthread.h>

typedef int (*pfun)(int);

typedef struct
{
  int i;
  int out;
  int (*p)(int);
} Task, *Task_Point;

int myfun(int data)
{
  return data * 10;
}

pfun gf()
{
  return myfun;
}

void f1(int a, char *b, char *c)
{
  printf("a = %d, b = %s, c = %s\n", a, b, c);
}

int a()
{
  printf("---a-----------\n");
  return 0;
}

char *b()
{
  printf("-----b--------\n");
  return "b";
}

char *c()
{
  printf("-----c--------\n");
  return "c";
}

static long facttail(long n, int a)
{
  if (n < 0 || n == 0 || n == 1)
  {
    return a;
  }
  else
  {
    return facttail(n - 1, a * n);
  }
}

long factorial(long n)
{
  return facttail(n, 1);
}

long f(int n)
{
  if (n == 1)
  {
    return 10L;
  }
  else
  {
    return f(n-1) * 2 + 6;
  }
}

long ff(int n)
{
  long a = 10;
  for (int i = 0; i < n; i++)
  {
    /* code */
    a = (a + 3) * 2;
  }
  return a;
}

int main(int argc, char *args[])
{
  long ret = factorial(5L);
  printf("ret = %ld", ret);

  char *str1 = "abc";
  char str2[] = "abc";
  printf("str1: %p, str2: %p \n", str1, str2);
  f1(a(), b(), c());
  pfun p = myfun;
  int result = p(10);
  printf("result = %d \n", result);
  printf("-------\n");
  unsigned int a = 1;
  unsigned int b = 2;
  printf("compatibility\n");
  printf("a - b = %d\n", a - b);
  printf("a - b = %u\n", a - b);
  for (int i = 0; i < 5; i++)
  {
    printf("i = %d\n", i);
  }
  int m = 30;
  int *pt = &m;
  printf("*pt = %d \n", *pt);

  Task t = {
      1, 2, myfun};

  Task *ptask = &t;
  m = ptask->p(30);
  Task_Point ptt = &t;
  printf("--- %d, pt->i =%d \n", m, ptt->i);
  getchar();
  return 0;
}

