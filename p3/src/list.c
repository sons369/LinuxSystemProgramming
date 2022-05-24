#include "../includes/ssu_sfinder.h"

/* insert node function */
void insert(t_myStatptr *sPtr, char *path, char *hash, long size)
{
    t_myStatptr newPtr;
    t_myStatptr previousPtr;
    t_myStatptr currentPtr;
    t_stat statbuf;

    newPtr = malloc(sizeof(t_myStat));

    if (newPtr != NULL)
    {
        /* copy informations to node */
        if ((stat(path, &statbuf)) == -1)
            perror("stat link");
        strcpy(newPtr->real_path, path);
        strcpy(newPtr->hash, hash);
        newPtr->atim = get_string_time(statbuf, 1);
        newPtr->mtim = get_string_time(statbuf, 2);
        newPtr->size = size;
        newPtr->next = NULL;

        previousPtr = NULL;
        currentPtr = *sPtr;
        /*find node position order to ASCII and PATH*/
        while (currentPtr != NULL && newPtr->size > currentPtr->size)
        {
            previousPtr = currentPtr;
            currentPtr = currentPtr->next;
        }
        while (currentPtr != NULL && sort_filter(newPtr->real_path, currentPtr->real_path) > 0)
        {

            if (!strcmp(newPtr->hash, currentPtr->hash))
            {
                previousPtr = currentPtr;
                currentPtr = currentPtr->next;
            }
            else
                break;
        }
        if (previousPtr == NULL)
        {
            newPtr->next = *sPtr;
            *sPtr = newPtr;
        }
        else
        {
            previousPtr->next = newPtr;
            newPtr->next = currentPtr;
        }
    }
}

/* free head and head point NULL */
void free_all_node(t_myStatptr *sPtr)
{
    t_myStatptr tmp;

    while (*sPtr != NULL)
    {
        tmp = *sPtr;
        *sPtr = (*sPtr)->next;
        free(tmp->atim);
        free(tmp->mtim);
        free(tmp);
    }
    *sPtr = NULL;
}

/* if user input index then get idx's node */
int is_set_idx_node(int set, int idx)
{
    t_myStatptr sPtr;

    sPtr = g_head;
    while (sPtr != NULL)
    {
        if (sPtr->set == set && sPtr->idx == idx)
            return 1;
        sPtr = sPtr->next;
    }
    return 0;
}

/* 해당 세트번호가 링크드리스트에 있는지 체크 */
int is_set_node(int set)
{
    t_myStatptr sPtr;

    sPtr = g_head;
    while (sPtr != NULL)
    {
        if (sPtr->set == set)
            return 1;
        sPtr = sPtr->next;
    }
    return 0;
}

/* 해당 해쉬값이 링크드 리스트에 있는지 체크 */
int is_hash(char *hash)
{
    t_myStatptr sPtr;
    sPtr = g_head;
    if (sPtr == NULL)
        return 0;
    while (sPtr->next != NULL)
    {
        sPtr = sPtr->next;
        if (!strcmp(sPtr->hash, hash))
            return 1;
    }
    return 0;
}

/* 해당 세트에 몇개의 노드가 있는지 세어줌 */
int cnt_set_node(int set)
{
    t_myStatptr sPtr;
    int cnt;

    cnt = 0;
    sPtr = g_head;
    if (sPtr == NULL)
        return 0;
    while (sPtr != NULL)
    {
        if (sPtr->set == set)
        {
            cnt++;
        }
        sPtr = sPtr->next;
    }
    return cnt;
}

/* 해당 해쉬값이 몇개인지 세어줌 */
int cnt_hash(char *hash)
{
    t_myStatptr sPtr;
    int cnt;

    cnt = 0;
    sPtr = g_head;
    if (sPtr == NULL)
        return 0;
    while (sPtr != NULL)
    {
        if (!strcmp(sPtr->hash, hash))
            cnt++;
        sPtr = sPtr->next;
    }
    return cnt;
}

/* 노드 출력 */
/* 만약 해쉬값이 1개뿐이라면 set와 idx값을 0으로 바꿔주고 출력에서 생략 */
void print_node(t_myStatptr sPtr)
{
    int flag;
    char hash_buf[50];
    char atol_num[30];
    long size_buf;
    int set;
    int i;

    flag = 0;
    set = 1;
    while (sPtr != NULL)
    {
        if (cnt_hash(sPtr->hash) == 1)
        {
            sPtr->set = 0;
            sPtr->idx = 0;
            sPtr = sPtr->next;
            continue;
        }
        if (flag == 0)
        {
            make_comma_num(sPtr->size, atol_num);
            printf("\n---- Identical files #%d (%s bytes - %s) ----\n", set, atol_num, sPtr->hash);
            flag = 1;
            i = 1;
            set++;
        }
        sPtr->set = set - 1;
        sPtr->idx = i;
        if (sPtr->next != NULL && strcmp(sPtr->hash, sPtr->next->hash))
        {
            flag = 0;
        }
        print_file(*sPtr, i);
        i++;
        sPtr = sPtr->next;
    }
}

void print_file(t_myStat file, int i)
{
    printf("[%d] %s (mtime : %s) (atime : %s)\n", i, file.real_path, file.mtim, file.atim);
}

/* Linked list sort filter */
/* sort by number of '/' */
int sort_filter(char *s1, char *s2)
{
    int i;
    int cnt1;
    int cnt2;

    i = 0;
    cnt1 = 0;
    cnt2 = 0;
    while (s1[i])
    {
        if (s1[i] == '/')
            cnt1++;
        i++;
    }
    i = 0;
    while (s2[i])
    {
        if (s2[i] == '/')
            cnt2++;
        i++;
    }

    return (cnt1 - cnt2);
}

/* mtim, atim을 출력 양식으로 바꿔줌 */
char *get_string_time(t_stat st_t, int flag)
{
    char *s;
    struct tm *t;

    s = (char *)malloc(sizeof(char) * 20);
    if (flag == 1)
        t = localtime(&st_t.st_atim.tv_sec);
    else if (flag == 2)
        t = localtime(&st_t.st_mtim.tv_sec);
    // strftime(s, 30, "%y-%m-%d %H:%M", t);
    sprintf(s, "%04d-%02d-%02d %02d:%02d:%02d",
            t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
            t->tm_hour, t->tm_min, t->tm_sec);
    return s;
}

/* 파일 사이즈 3자리마다 , 찍어주는 함수 */
void make_comma_num(long num, char *result)
{
    long i;
    char str[30], *p;

    if (num < 0)
    {
        num *= -1;
        *result++ = '-';
    }
    else if (num == 0)
    {
        *result++ = '0';
        *result = 0;
        return;
    }

    p = str;
    for (i = 0; i < 16 && num > 0; i++)
    {
        if (i && (i % 3) == 0)
            *p++ = ',';
        *p++ = (num % 10) + '0';
        num /= 10;
    }
    p--;
    while (p >= str)
        *result++ = *p--;
    *result = 0;
}

/* 총 세트 갯수와 노드 갯수를 세어줌 */
void cnt_set_idx_num(t_myStatptr sPtr)
{
    int flag;

    flag = 0;
    g_total_set = 0;
    g_total_node = 0;
    while (sPtr != NULL)
    {
        if (flag == 0)
        {
            g_total_set++;
            flag = 1;
        }
        if (sPtr->next != NULL && strcmp(sPtr->hash, sPtr->next->hash))
        {
            flag = 0;
        }
        g_total_node++;
        sPtr = sPtr->next;
    }
}

/* 해당 세트의 인덱스 노드의 path 정보를 건네주는 함수 */
char *get_node_path(int set, int idx)
{
    t_myStatptr sPtr;
    char *path;

    sPtr = g_head;
    while (sPtr != NULL)
    {
        if (sPtr->set == set && sPtr->idx == idx)
        {
            path = (char *)malloc(4048);
            strcpy(path, sPtr->real_path);
            return path;
        }
        sPtr = sPtr->next;
    }
    return NULL;
}

/* 링크드 리스트 노드 제거 */
char *delete_node(t_myStatptr *sPtr, int set, int idx)
{
    t_myStatptr previous;
    t_myStatptr current;
    t_myStatptr temp;
    char *path;

    path = (char *)malloc(4048);
    if ((*sPtr)->set == set && (*sPtr)->idx == idx)
    {
        temp = *sPtr;
        strcpy(path, temp->real_path);
        *sPtr = (*sPtr)->next;
        free(temp->atim);
        free(temp->mtim);
        free(temp);
        return path;
    }
    else
    {
        previous = *sPtr;
        current = (*sPtr)->next;
        while (current != NULL && (current->set != set || current->idx != idx))
        {
            previous = current;
            current = current->next;
        }
        if (current != NULL)
        {
            temp = current;
            strcpy(path, temp->real_path);
            previous->next = current->next;
            free(temp->atim);
            free(temp->mtim);
            free(temp);
            return path;
        }
    }
    return NULL;
}

/* 가장 최근에 변경된 파일의 idx값을 넘겨줌 */
int get_latest_mtim_idx(int set)
{
    t_myStatptr sPtr;
    char latest[20];
    int idx;

    idx = 1;
    sPtr = g_head;
    memset(latest, 0, 20);
    while (sPtr != NULL)
    {
        if (sPtr->set == set)
        {
            if (strcmp(latest, sPtr->mtim) < 0)
                strcpy(latest, sPtr->mtim);
        }
        sPtr = sPtr->next;
    }
    sPtr = g_head;
    while (sPtr != NULL)
    {
        if (sPtr->set == set)
        {
            if (!strcmp(latest, sPtr->mtim))
                break;
        }
        sPtr = sPtr->next;
    }
    return sPtr->idx;
}

/* 해당 세트의 해당 노드의 mtim 값을 넘겨줌 */
char *get_latest_mtim(int set, int idx)
{
    t_myStatptr sPtr;
    char *latest;

    sPtr = g_head;
    latest = (char *)malloc(20);
    while (sPtr != NULL)
    {
        if (sPtr->set == set && sPtr->idx == idx)
            strcpy(latest, sPtr->mtim);
        sPtr = sPtr->next;
    }
    return latest;
}