#ifndef __ACCOUNT__
#define __ACCOUNT__
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define BLOCKED 0
#define ACTIVE 1

#define MAX 1024

typedef struct Account 
{
  char username[MAX];
  char password[MAX];
  int accountStatus; 
  int IsLogIn ;  
  struct Account *next;
} Account;

typedef struct
{
	
    Account *account;
    int socket; 
    //int IsLogIn; 
    //pthread_cond_t login_cond;

} Session;

Account **createAccountList();
Account **createAccountList();                                                                       // Init link list NULL
Account *findaccbyusername(Account **head, char *username);  
Account **readAccount(Account *acc);                                   // find Account By Username
Account *newAccount(char *username, char *password, int accoountStatus);             // create node
void addAccount(Account **head, Account* newAcc); //add node to link list
void printListAccount(Account **head); 
void save_account(Account *acc);
Account **copyAccList(Account **x);
#endif
