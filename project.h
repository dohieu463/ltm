#pragma once
#ifndef __PROJECT__
#define __PROJECT__
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "account.h"
#include "task.h"

#define MAX_TASK 30

#define pj_file "../data/project.txt"
typedef struct Project
{
    int id;
    char pj_name[30];
    char start_date[11];
    char end_date[11];
    Task** listTask;
    Account* owner;
    Account **members;
    int maxmems;
    struct Project *next;
} Project;

Project **createProjectList();                                                  //Init link list NULL


Project *newProject(char *pj_name, char *start_date, char *end_date, Account *owner, int maxmems, Account** member);             // create node
void addProject(Project **head, Project* newPj); //add node to link list
char* printListProject(Project **head);                                                            // print link list
Project **findpjbyname(Project **head, char *pj_name);
Project *findpjbyid(Project **head, int id);
Project **findpjbyuser(Project **head, Account *acc);
bool addMemtoPj(Project* pj, Account* member);
void assignTask(Project* pj, int tsk_id, Account* member);
bool checkMeminPj(Project *pj, Account *member);
#endif
