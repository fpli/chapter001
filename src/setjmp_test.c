#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

void jmpfunction(jmp_buf env_buf);

int main()
{
    int val;
    jmp_buf env_buffer;

    /* 保存 longjmp 的调用环境 */
    val = setjmp(env_buffer);
    if (val != 0)
    {
        printf("the return value from longjmp() = %d\n", val);
        //exit(0);
    } else {
        printf("++++++++++++++\n");
        printf("will longjmp \n");
        jmpfunction(env_buffer);
    }
    
    printf("----------******--------\n");
    return (0);
}

void jmpfunction(jmp_buf env_buf)
{
    longjmp(env_buf, 1);
    // dead code 
    // printf("---------\n");
    //longjmp(env_buf, 2);
}