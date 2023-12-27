#include <stdio.h>
#include <stdlib.h>

typedef struct gfun
{
    int (*pfun)(int, int);
} gfun;

int myfun(int d, int n)
{
    printf("myfun data: %d , n = %d \n", d, n);
    return d * n;
}

int rt_data(int data, int (*tr_fun)(int, int))
{
    int result = tr_fun(data, 2);
    printf("rt_data result = %d \n", result);
    return (*tr_fun)(data, 2);
}
// 在 main 函数中将参数100与函数gf.pfun传递给函数rt_data，函数rt_data调用函数gf.pfun；函数rt_data调用的动作称为回调,函数gf.pfun称为回调函数
int main(int argc, char *argv[])
{
    int ret;
    printf("myfun = %p \n", myfun);
    gfun gf;
    gf.pfun = myfun;
    ret = rt_data(10, myfun);   // myfun callback
    ret = rt_data(100, gf.pfun); // gf.pfun callback
    printf("return data: %d \n", ret);
    return 0;
}