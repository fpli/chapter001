
struct Account
{
    int No;
    char *name;
    char sex;
    int age;
    char *address;
    char email[20];
};

int addAccount(struct Account account);

void listAccounts(struct Account accounts[], int size);

int deleteAccount(struct Account *account, int size, int no);
