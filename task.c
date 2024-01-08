#define _CRT_SECURE_NO_WARNINGS
#include "task.h"
#include "account.h"

// Init link list
Task **createTaskList()
{
    Task **head = (Task **)malloc(sizeof(Task *));
    *head = NULL;

    return head;
}
// create node
Task *newTask(char *tsk_name, char *start_date, char *end_date, Account *curator)
{
    Task *new = (Task *)malloc(sizeof(Task));
    strcpy(new->task_name, tsk_name);
    strcpy(new->end_date, end_date);
    strcpy(new->start_date, start_date);
    new->curator = curator;
    new->task_status = INPROGRESS;
    return new;
}
// add node
void addTask(Task **head, Task *new)
{
    // Task *new = newTask(pj_name, end_date, owner);
    Task *current = (*head);

    if (*head == NULL)
    {
        new->id = 1;
        new->next = NULL;
        (*head) = new;
    }
    else
    {
        while (current->next != NULL)
        {
            current = current->next;
        }
        if (new->id == 0)
            new->id = current->id + 1;
        new->next = NULL;
        current->next = new;
    }
}
// printf all account
char *printListTask(Task **head)
{
    Task *ptr;
    char *res = NULL;
    int len = 0;
    len += snprintf(NULL, 0, "%20.20s|%20.20s|%20.20s|%20.20s|%20.20s|%20.20s\n",
                    "ID", "Task Name", "Start Date", "End Date", "Curator", "Status");
    len += snprintf(NULL, 0, "-----------------------------------------------------------------------------------------------------------------------------------------------\n");
    res = (char *)malloc(len + 1);
    sprintf(res, "%20.20s|%20.20s|%20.20s|%20.20s|%20.20s|%20.20s\n",
            "ID", "Task Name", "Start Date", "End Date", "Curator", "Status");
    strcat(res, "-----------------------------------------------------------------------------------------------------------------------------------------------\n");
    for (ptr = *head; ptr != NULL; ptr = ptr->next)
    {
        char target1[10], target3[20];

        memset(target1, '\0', sizeof(target1));
        memset(target3, '\0', sizeof(target3));
        sprintf(target1, "%d", ptr->id);
        sprintf(target3, "%s", (ptr->task_status == COMPLETED) ? "Completed" : "In progress");

        int addlen = snprintf(NULL, 0, "%20.20s|%20.20s|%20.20s|%20.20s|%20.20s|%20.20s\n",
                              target1,
                              ptr->task_name,
                              ptr->start_date,
                              ptr->end_date,
                              ptr->curator->username,
                              target3);

        char *newres = (char *)malloc(len + addlen + 1);
        memcpy(newres, res, len);

        sprintf(newres + len, "%20.20s|%20.20s|%20.20s|%20.20s|%20.20s|%20.20s\n",
                target1,
                ptr->task_name,
                ptr->start_date,
                ptr->end_date,
                ptr->curator->username,
                target3);
        len += addlen;
        free(res);
        res = newres;
    }
    // printf("%s", res);
    return res;
}
Task **findtaskbyuser(Task **head, Account *acc)
{
    Task **res = createTaskList();
    Task *ptr = NULL;
    for (ptr = *head; ptr != NULL; ptr = ptr->next)
    {
        if (strcmp(ptr->curator->username, acc->username) == 0)
        {
            Task *new = newTask(ptr->task_name, ptr->start_date, ptr->end_date, ptr->curator);
            new->id = ptr->id;
            addTask(res, new);
        }
    }
    return res;
}
Task *findtaskbyid(Task **head, int id)
{

    Task *ptr = NULL;
    for (ptr = *head; ptr != NULL; ptr = ptr->next)
    {
        if (ptr->id == id)
        {
            return ptr;
        }
    }
    return NULL;
}
int getNumofTask(Task **head)
{
    if (head == NULL) return 0;
    Task *ptr = NULL;
    int res = 0;
    for (ptr = *head; ptr != NULL; ptr = ptr->next)
    {

        res++;
    }
    return res;
}
void reviewTask(Task *tsk, char *rv)
{
}
Task **copyTaskList(Task **x)
{
    Task **y = createTaskList();
    if (*x != NULL)
    {
        Task *current = *x;
        do
        {
            Task* temp = newTask(current->task_name, current->start_date, current->end_date, current->curator);
            temp->task_status = current->task_status;
            addTask(y, temp);
            current = current->next;
        }while (current != NULL);
    }

    return y;
}
bool check_outdate(Task* tsk){
    time_t now = time(NULL);

    int day, month, year;
    sscanf(tsk->end_date, "%d/%d/%d", &year, &month, &day);

    struct tm tm;
    memset(&tm, 0, sizeof(struct tm));
    tm.tm_year = year - 1900;
    tm.tm_mon = month - 1;
    tm.tm_mday = day;

    time_t date = mktime(&tm);
    if (date <= now)
    {
        return true;
    }
    return false;
}
