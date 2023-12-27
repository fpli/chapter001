#include <stdio.h>

#include "common/listAccount.h"

static const int size = 100;
struct Account accounts[size];
int count = 0;


int main(int argc, char *args[])
{
    printf("------------>-------");
    char key;
    int loop = 1;
    do
    {
        printf("please chouse an item: l, a, d, q. \n");
        scanf("%c", &key);
        getchar();
        switch (key)
        {
        case 'l':
            printf("list all accounts. \n");
            listAccounts(accounts, count);
            break;

        case 'a':
            printf("add  an account. \n");
            struct Account a = {1, "zhangsan", 'm', 20, "shanghai", "zhangsan@gmail.com"};
            addAccount(a);
            break;

        case 'd':
            printf("delete an account. \n");
            deleteAccount(accounts, count, 1);
            break;

        case 'q':
            printf("quit system. \n");
            loop = 0;
            break;

        default:
            break;
        }

    } while (loop);
    
    return 0;
}

int addAccount(struct Account account)
{
    accounts[count] = account;
    count++;
    return count;
}

void listAccounts(struct Account accounts[], int size)
{
    for (size_t i = 0; i < count; i++)
    {
       struct Account a = accounts[i];
       printf("name = %s.\n", a.name);
    }
    
}

int deleteAccount(struct Account *account, int size, int no)
{
    int position = -1;
    for (size_t i = 0; i < count; i++)
    {
        struct Account a = accounts[i];
        if (a.No == no)
        {
            position = i;
            break;
        }  
    }

    if (position == -1){
        return -1;
    }

    while (position <= count)
    {
        accounts[position] = accounts[position+1];
    }
    
    return 1;
}
