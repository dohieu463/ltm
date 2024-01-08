
#include "protocol.h"
bool isValidDate(char *dateString)
{

    int day, month, year;

    // Sử dụng sscanf để lấy các thành phần ngày tháng
    if (sscanf(dateString, "%d/%d/%d", &year, &month, &day) != 3)
    {
        return false;
    }

    // Kiểm tra từng thành phần
    if (year < 1900 || year > 9999)
    {
        return false;
    }

    if (month < 1 || month > 12)
    {
        return false;
    }

    if (day < 1 || day > 31)
    {
        return false;
    }

    // Sử dụng mktime để kiểm tra ngày tháng
    struct tm t;
    memset(&t, 0, sizeof(struct tm));
    t.tm_year = year - 1900;
    t.tm_mon = month - 1;
    t.tm_mday = day;

    if (mktime(&t) == -1)
    {
        return false;
    }

    return true;
}

char *mk_pj(char *pj_name, char *end_time, int maxmems, Account *temp)
{
    //Account *temp = findaccbyusername(root_acc, owner);
    
    if (strcmp(pj_name, "") == 0 || strcmp(end_time, "") == 0 || temp == NULL)
    {
        return "3112\r\n";
    }
    if (isValidDate(end_time) == false)
    {
        return "3111\r\n";
    }
    time_t now = time(NULL);

    int day, month, year;
    sscanf(end_time, "%d/%d/%d", &year, &month, &day);

    struct tm tm;
    memset(&tm, 0, sizeof(struct tm));
    tm.tm_year = year - 1900;
    tm.tm_mon = month - 1;
    tm.tm_mday = day;

    time_t date = mktime(&tm);
    if (date <= now)
    {
        return "3110\r\n";
    }
    char start_date[12];
    strftime(start_date, sizeof(start_date), "%Y/%m/%d", localtime(&now));
    Project *newpj = newProject(pj_name, start_date, end_time, temp, maxmems, createAccountList());
    addProject(root_pj, newpj);
    return "3100\r\n";
}
char *ls_pj(Account *user)
{

    //Account *user = findaccbyusername(root_acc, user_name);

    if (user == NULL)
    {
        return "3210\r\n";
    }
    Project **list = findpjbyuser(root_pj, user);
    if (list == NULL)
    {
        return "3211\r\n";
    }
    char *res = NULL;
    int len = snprintf(NULL, 0, "3200 %s", printListProject(list));
    res = (char *)malloc(len + 1);
    sprintf(res, "3200 %s\r\n", printListProject(list));
    return res;
}
char *addmem_pj(Account *user, int pj_id, char *mem_name)
{
    Project *pj = findpjbyid(root_pj, pj_id);
    if (pj == NULL)
    {
        return "3312\r\n";
    }
    Account *acc = findaccbyusername(root_acc, mem_name);
    if (acc->accountStatus == BLOCKED || acc == NULL || checkMeminPj(pj, acc) == true)
    {
        return "3311\r\n";
    }
    if (strcmp(user->username, pj->owner->username) != 0)
    {
        return "3310\r\n";
    }
    if (addMemtoPj(pj, acc) == false)
        return "3311\r\n";
    else
        return "3300\r\n";
}
char *find_pj(char *pj_name)
{
    if (strlen(pj_name) > 30)
        return "3503  ";
    Project **list = findpjbyname(root_pj, pj_name);
    if (list == NULL)
    {
        return "3502 ";
    }
    char *res = NULL;
    int len = snprintf(NULL, 0, "3500 %s\r\n", printListProject(list));
    res = (char *)malloc(len + 1);
    sprintf(res, "3500 %s\r\n", printListProject(list));
    return res;
}
char *chk_pj(Account *acc, int pj_id)
{
    Project *pj = findpjbyid(root_pj, pj_id);
    //Account *acc = findaccbyusername(root_acc, user_name);
    if (pj == NULL || acc == NULL)
    {
        return "3610\r\n";
    }
    if (checkMeminPj(pj, acc) == false)
    {
        return "3611\r\n";
    }
    char *res = NULL;
    int len = snprintf(NULL, 0, "3600 %s", print_detail(pj));
    res = (char *)malloc(len + 1);

    sprintf(res, "3600 %s\r\n", print_detail(pj));
    return res;
}
char *mk_task(int pj_id, char *task_name, char *end_time, Account *temp)
{
    //Account *temp = findaccbyusername(root_acc, owner);
    Project *pj = findpjbyid(root_pj, pj_id);
    if (pj == NULL || strcmp(end_time, "") == 0 || temp == NULL)
    {
        return "2112\r\n";
    }
    if (isValidDate(end_time) == false)
    {
        return "2111\r\n";
    }
    time_t now = time(NULL);

    int day, month, year;
    sscanf(end_time, "%d/%d/%d", &year, &month, &day);

    struct tm tm;
    memset(&tm, 0, sizeof(struct tm));
    tm.tm_year = year - 1900;
    tm.tm_mon = month - 1;
    tm.tm_mday = day;

    time_t date = mktime(&tm);
    if (date <= now)
    {
        return "2110\r\n";
    }
    char start_date[12];
    strftime(start_date, sizeof(start_date), "%Y/%m/%d", localtime(&now));
    Task *tsk = newTask(task_name, start_date, end_time, NULL);
    addTask(pj->listTask, tsk);
    return "2100\r\n";
}

char *lstask(int pj_id, Account *mem)
{
    Project *pj = findpjbyid(root_pj, pj_id);
    //Account *mem = findaccbyusername(root_acc, member);
    if (pj == NULL || mem == NULL)
        return "2212\r\n";
    else if (pj->listTask == NULL)
        return "2211\r\n";
    else if (checkMeminPj(pj, mem) == false)
        return "2210\r\n";
    Task **temp = copyTaskList(pj->listTask);
    char *res = NULL;
    int len = snprintf(NULL, 0, "2200 %s\r\n", printListTask(temp));
    res = (char *)malloc(len + 1);
    sprintf(res, "2200 %s\r\n", printListTask(temp));
    return res;
}
char *updtask(int pj_id, char *member, int task_id, Account *own)
{
    Project *pj = findpjbyid(root_pj, pj_id);
    Account *mem = findaccbyusername(root_acc, member);
    //Account *own = findaccbyusername(root_acc, owner);
    Task *tsk = findtaskbyid(pj->listTask, task_id);
    if (pj == NULL || own == NULL || tsk == NULL)
        return "2312\r\n";
    if (mem != NULL && strcmp(pj->owner->username, own->username) != 0)
        return "2310\r\n";
    if (tsk->task_status == COMPLETED || check_outdate(tsk) == true)
        return "2311\r\n";
    tsk->task_status = COMPLETED;
    return "2300\r\n";
}
char *settask(int pj_id, char *member, int task_id, Account *own)
{

    Project *pj = findpjbyid(root_pj, pj_id);
    Account *mem = findaccbyusername(root_acc, member);
    //Account *own = findaccbyusername(root_acc, owner);
    Task *tsk = findtaskbyid(pj->listTask, task_id);
    if (pj == NULL || own == NULL || tsk == NULL || mem == NULL)
        return "2412\r\n";
    if (pj->owner != own || checkMeminPj(pj, mem) == false)
        return "2410\r\n";
    if (tsk->task_status == COMPLETED || check_outdate(tsk) == true)
        return "2411\r\n";
    if (tsk->curator != NULL)
        return "2413\r\n";
    tsk->curator = mem;
}
char* rv_task(int pj_id, int task_id, Account *own){
    Project *pj = findpjbyid(root_pj, pj_id);
    Task *tsk = findtaskbyid(pj->listTask, task_id);
    if (pj == NULL || own == NULL || tsk == NULL)
        return "3412\r\n";
    if (pj->owner != own)
        return "3411\r\n";
    if (tsk->task_status == INPROGRESS || check_outdate(tsk) == true || tsk->curator == NULL)
        return "3410\r\n";
    strcpy(tsk->rv, "Tốt");
    return "3400\r\n";
}

