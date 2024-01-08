#include "account.h"
// Init link list
Account **createAccountList()
{
    Account **head = (Account **)malloc(sizeof(Account *));
    *head = NULL;

    return head;
}
// find account by username
Account *findaccbyusername(Account **head, char *username)
{

    Account *ptr = NULL;
    for (ptr = *head; ptr != NULL; ptr = ptr->next)
    {
        if (strcmp(ptr->username, username) == 0)
            return ptr;
    }
    return NULL;
}

Account **readAccount(Account *acc)
{
    FILE *f = fopen("account.txt", "r");
    Account **p;
    if (f == NULL)
    {
        printf("can't read file account.txt");
    }
    else
    {
        while (!feof(f))
        {
            Account *temp = (Account *)malloc(sizeof(Account));
            fscanf(f, "%s %s %d", temp->username, temp->password, &temp->accountStatus);
            temp->IsLogIn = 0;
            temp->next = NULL;

            if (acc == NULL)
            {
                acc = temp;
                *p = acc;
            }
            else
            {
                acc->next = temp;
                acc = acc->next;
            }
        }
    }
    fclose(f);
    return p;
}
// create node
Account *newAccount(char *username, char *password, int accountStatus)
{
    Account *new = (Account *)malloc(sizeof(Account));
    strcpy(new->username, username);
    strcpy(new->password, password);
    new->accountStatus = accountStatus;
    new->IsLogIn = 0;
    // new->status = OFFLINE;
    new->next = NULL;
    return new;
}
// add node
void addAccount(Account **head, Account *newAcc)
{
    // Account *new = newAccount(username, password, accountStatus, position);
    Account *current = (*head);

    if (*head == NULL)
    {
        newAcc->next = NULL;
        (*head) = newAcc;
    }
    else
    {
        while (current->next != NULL)
            current = current->next;
        newAcc->next = NULL;
        current->next = newAcc;
    }
}
// printf all account
void printListAccount(Account **head)
{
    Account *ptr = NULL;
    for (ptr = *head; ptr != NULL; ptr = ptr->next)
    {
        printf("Username: %s\n", ptr->username);
        printf("Password: %s\n", ptr->password);
        printf("Account status: %s\n", ptr->accountStatus == ACTIVE ? "Active" : "Blocked");
        // printf("Status : %s\n", ptr->status == ONLINE ? "Online" : "Offline");
        printf("\n");
    }
}
// printf all account
int getNumofAccount(Account **head)
{
    if (head == NULL) return 0;
    Account *ptr = NULL;
    int res = 0;
    for (ptr = *head; ptr != NULL; ptr = ptr->next)
    {
        res++;
    }
    return res;
}

void save_account(Account *acc)
{
    FILE *f = fopen("account.txt", "a");
    if (f == NULL)
    {
        printf("don't save account !\n");
    }
    else
    {
        fprintf(f, "\n%s %s %d", acc->username, acc->password, acc->accountStatus);
    }
    fclose(f);
}

Account **copyAccList(Account **x)
{
    Account **y = createAccountList();
    Account *current = *x;
    while (current->next != NULL)
    {
        addAccount(y, newAccount(current->password, current->password, current->accountStatus));
        current = current->next;
    }
    return y;
}