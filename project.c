#define _CRT_SECURE_NO_WARNINGS
#include "project.h"
#include "account.h"

// Init link list
Project **createProjectList()
{
    Project **head = (Project **)malloc(sizeof(Project *));
    //memset(head, '\0', sizeof(head));
    *head = NULL;
    
    return head;
}
// create node
Project *newProject(char *pj_name, char *start_date, char *end_date, Account *owner, int maxmems, Account **member)
{
    Project *new = (Project *)malloc(sizeof(Project));
    memset(new, '\0', sizeof(sizeof(Project)));
    if (new)
    {
        strcpy(new->pj_name, pj_name);
        strcpy(new->end_date, end_date);
        strcpy(new->start_date, start_date);
        new->owner = owner;
        new->members = member;
        Account *temp = newAccount(owner->username, owner->password, owner->accountStatus);
        addAccount(new->members, temp);
        new->listTask = createTaskList();
        new->maxmems = maxmems;
    }
    return new;
}
// add node
void addProject(Project **head, Project *newPj)
{
    // Project *new = newProject(pj_name, end_date, owner);
    Project *current = (*head);

    if (*head == NULL)
    {
        newPj->id = 1;
        newPj->next = NULL;
        (*head) = newPj;
    }
    else
    {
        while (current->next != NULL)
        {
            current = current->next;
        }
        if (newPj->id == 0)
            newPj->id = current->id + 1;
        newPj->next = NULL;
        current->next = newPj;
    }
}
// printf all node
char *printListProject(Project **head)
{
    Project *ptr;
    char *res = NULL;
    int len = 0;
    len += snprintf(NULL, 0, "%20.20s|%20.20s|%20.20s|%20.20s|%20.20s|%20.20s|%20.20s\n",
                    "ID", "Project Name", "Start Date", "End Date", "Owner", "Members", "Task");
    len += snprintf(NULL, 0, "----------------------------------------------------------------------------------------------------------------------------------------------------\n");
    res = (char *)malloc(len + 1);
    sprintf(res, "%20.20s|%20.20s|%20.20s|%20.20s|%20.20s|%20.20s|%20.20s\n",
            "ID", "Project Name", "Start Date", "End Date", "Owner", "Members", "Task");

    strcat(res, "----------------------------------------------------------------------------------------------------------------------------------------------------\n");
    for (ptr = *head; ptr != NULL; ptr = ptr->next)
    {
        char target1[5], target2[10], target3[10];
        sprintf(target1, "%d", ptr->id);
        sprintf(target2, "%d/%d", getNumofAccount(ptr->members), ptr->maxmems);
        sprintf(target3, "%d", getNumofTask(ptr->listTask));
        int addlen = snprintf(NULL, 0, "%20.20s|%20.20s|%20.20s|%20.20s|%20.20s|%20.20s|%20.20s\n",
                              target1, ptr->pj_name, ptr->start_date, ptr->end_date, ptr->owner->username, target2, target3);
        char *newres = (char *)malloc(len + addlen + 1);
        memcpy(newres, res, len);
        sprintf(newres + len, "%20.20s|%20.20s|%20.20s|%20.20s|%20.20s|%20.20s|%20.20s\n",
                target1, ptr->pj_name, ptr->start_date, ptr->end_date, ptr->owner->username, target2, target3);
        len += addlen;
        free(res);
        res = newres;
    }
    return res;
}

Project **findpjbyuser(Project **head, Account *acc)
{
    Project **res = createProjectList();
    Project *ptr;
    for (ptr = *head; ptr != NULL; ptr = ptr->next)
    {
        if (checkMeminPj(ptr, acc))
        {
            /*Account** mem = createAccountList();
            Account* current = *(ptr->members);
            while (current->next != NULL){
                addAccount(mem, newAccount(current->password, current->password, current->accountStatus));
                current = current->next;
            }*/
            Account **mem = copyAccList(ptr->members);
            Task **lst = copyTaskList(ptr->listTask);
            Project *temp = newProject(ptr->pj_name, ptr->start_date, ptr->end_date, ptr->owner, ptr->maxmems, mem);
            temp->id = ptr->id;
            temp->listTask = lst;
            addProject(res, temp);
        }
    }
    return res;
}
Project *findpjbyid(Project **head, int id)
{

    Project *ptr;
    for (ptr = *head; ptr != NULL; ptr = ptr->next)
    {
        if (ptr->id == id)
        {
            return ptr;
        }
    }
    return NULL;
}
Project **findpjbyname(Project **head, char *pj_name)
{
    Project **res = createProjectList();
    Project *ptr;
    for (ptr = *head; ptr != NULL; ptr = ptr->next)
    {
        char *check = strstr(ptr->pj_name, pj_name);
        if (check)
        {
            Account **mem = copyAccList(ptr->members);
            Task **lst = copyTaskList(ptr->listTask);
            Project *temp = newProject(ptr->pj_name, ptr->start_date, ptr->end_date, ptr->owner, ptr->maxmems, mem);
            temp->id = ptr->id;
            temp->listTask = lst;
            addProject(res, temp);
        }
    }
    return res;
}

void assignTask(Project *pj, int tsk_id, Account *member)
{
    if (findaccbyusername(pj->members, member->username) == NULL)
    {
        printf("Nope\n");
    }
    else
        findtaskbyid(pj->listTask, tsk_id)->curator = member;
}
bool addMemtoPj(Project *pj, Account *member)
{
    if (getNumofAccount(pj->members) == pj->maxmems)
        return false;
    //Account *temp = newAccount(member->username, member->password, ACTIVE);
    addAccount(pj->members, member);
    return true;
}

bool checkMeminPj(Project *pj, Account *member)
{

    if (findaccbyusername(pj->members, member->username) != NULL)
        return true;

    return false;
}
char *print_detail(Project *pj)
{
    char *res = NULL;
    int len = 0;
    len += snprintf(NULL, 0, "Project id: %d\nProject name: %s\nStart date: %s\nEnd date: %s\nOwner: %s\nNumbers of members: %d\n\n", pj->id,
                    pj->pj_name, pj->start_date, pj->end_date, pj->owner->username, getNumofAccount(pj->members));
    res = (char *)malloc(len + 1);
    sprintf(res, "Project id: %d\nProject name: %s\nStart date: %s\nEnd date: %s\nOwner: %s\nNumbers of members: %d\n\n", pj->id,
            pj->pj_name, pj->start_date, pj->end_date, pj->owner->username, getNumofAccount(pj->members));

    return res;
}
