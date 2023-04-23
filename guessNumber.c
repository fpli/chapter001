#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

/**
 * @brief
 *
 * @param argc
 * @param argv
 * @param envp
 * @return int
 */
int main(int argc, char *argv[], char *envp[])
{
    srand(time(NULL));
    int r = rand();
    int i = r % 10000;
    int array1[4] = {
        i / 1000 % 10,
        i / 100 % 10,
        i / 10 % 10,
        i / 1 % 10};

    int a;
    while (1)
    {
        printf("please input your number:");
        scanf("%d", &a);
        printf("a = %d\n", a);
        int array2[4] = {
            a / 1000 % 10,
            a / 100 % 10,
            a / 10 % 10,
            a / 1 % 10};
        int n = 0;

        for (int j = 0; j < 4; j++)
        {
            // printf("array1[%d] = %d \n", j, array1[j]);
            if (array2[j] < array1[j])
            {
                printf("the %d is less than. \n", j + 1);
            }
            else if (array2[j] > array1[j])
            {
                printf("the %d is greater than. \n", j + 1);
            }
            else
            {
                printf("the %d is equales. \n\n", j + 1);
                n++;
            }
        }
        if (4 == n)
        {
            printf("you are right. good.\n");
            break;
        }
        sleep(2);
    }

    return 0;
}