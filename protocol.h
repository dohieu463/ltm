#pragma once
#define _XOPEN_SOURCE 700
#ifndef __PROTOCOL__
#define __PROTOCOL__

#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include "account.h"
#include "project.h"
#include "task.h"


Project** root_pj;
Task** root_task;
Account** root_acc;

bool isValidDate(char *dateString);
char *mk_pj(char *pj_name, char *end_time, int maxmems, Account *temp);
char *ls_pj(Account *user);   //?
char *addmem_pj(Account *user, int pj_id, char *mem_name);
char *find_pj(char *pj_name);
char *chk_pj(Account *acc, int pj_id);
char *mk_task(int pj_id, char *task_name, char *end_time, Account *temp);
char *lstask(int pj_id, Account *mem);
char *updtask(int pj_id, char *member, int task_id, Account *own);
char *settask(int pj_id, char *member, int task_id, Account *own);

#endif
