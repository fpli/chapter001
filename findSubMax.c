#include <stdio.h>

int main(int argc, char const *argv[], char const *evnp[])
{
    int array[] = {20, 40, 32, 12, 42, 59, 100, 300, 10, 500};
    int m1, m2;
    if (*array > *(array + 1))
    {
        m1 = *array;
        m2 = *(array + 1);
    }
    else
    {
        m1 = *(array + 1);
        m2 = *(array);
    }

    int n = sizeof(array) / sizeof(array[0]);
    int i = 0;
    for (i = 2; i < n; i++)
    {
        if (*(array + i) > m1)
        {
            m2 = m1;
            m1 = *(array + i);
        }
        else if (*(array + i) < m1 && *(array + i) > m2)
        {
            m2 = *(array + i);
        }
    }

    printf("the first big data: %d, the second big data: %d", m1, m2);
    return 0;
}
