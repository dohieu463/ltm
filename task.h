#pragma once
#ifndef __TASK__
#define __TASK__
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

typedef enum
{
    COMPLETED,
    INPROGRESS,

} STATUS;

typedef struct Task
{
    int id;
    char task_name[20];
    char start_date[11];
    char end_date[11];
    Account *curator;
    STATUS task_status;
    char rv[100];
    struct Task *next;
} Task;

Task **createTaskList();                                                  //Init link list NULL


Task *newTask(char *pj_name, char* start_date, char *end_date, Account *curator) ;             // create node
void addTask(Task **head, Task* newPj); //add node to link list
char* printListTask(Task **head);                                                            // print link list
Task *findtaskbyid(Task **head, int id);
Task **findtaskbyuser(Task **head, Account *acc);
int getNumofTask(Task **head);
void reviewTask(Task* tsk, char* rv);
Task **copyTaskList(Task **x);
#endif