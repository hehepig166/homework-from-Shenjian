/* 朱泽凯 2051995 济美 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct student {
    int *no;
    char *name;
    int *score;
    struct student *next;
};


/// 遍历并输出
void output(struct student *head)
{
    for (struct student *cur = head; cur != NULL; cur = cur->next)
        printf("%d %s %d\n", *(cur->no), cur->name, *(cur->score));
}

/// 新建节点，返回指针
/// 中途失败不考虑释放
struct student *newStudent(const int _no, const char *_name, const int _score)
{
    struct student *p;
    int len = strlen(_name);
    p = (struct student *)malloc(sizeof(struct student));
    if (p == NULL)
        return NULL;

    p->no = (int *)malloc(sizeof(int));
    if (p->no == NULL)
        return NULL;
    else
        *(p->no) = _no;

    p->name = (char*)malloc((len+1) * sizeof(char));
    if (p->name == NULL)
        return NULL;
    else
        strcpy(p->name, _name);

    p->score = (int *)malloc(sizeof(int));
    if (p->score == NULL)
        return NULL;
    else
        *(p->score) = _score;

    p->next = NULL;

    return p;
}

void releaseStudent(struct student *p)
{
    if (p == NULL)
        return;
    //puts("free no");
    free(p->no);
    //puts("free name");
    free(p->name);
    //puts("free score");
    free(p->score);
    //puts("free p");
    free(p);
}

/// 释放
void release(struct student *head)
{
    //puts("release begin");
    struct student *cur, *nxt;
    for (cur = head; cur != NULL; cur = nxt) {
        nxt = cur->next;
        //printf("  release %s\n", cur->name);
        releaseStudent(cur);
        //free(cur);
    }
}



/// 返回头指针(指向第一个student)
struct student *input()
{
    int _no;
    char _name[9];
    int _score;

    struct student *head = NULL, *cur = NULL, *tail = NULL;

    FILE *fin;
    if ((fin = fopen("list.txt", "r")) == NULL) {
        fprintf(stderr, "打开文件 list.txt 失败\n");
        return NULL;
    }

    while (1) {
        if (fscanf(fin, "%d", &_no) != 1) {
            fclose(fin);
            return NULL;
        }
        if (_no == 9999999) {   //输入结束，返回指针
            break;
        }
        if (fscanf(fin, "%s%d", _name, &_score) != 2) {
            fclose(fin);
            return NULL;
        }

        //新建节点
        cur = newStudent(_no, _name, _score);
        if (cur == NULL) {
            //申请失败，不考虑释放
            fclose(fin);
            return NULL;
        }

        //连接节点
        if (head == NULL) { //新链
            head = tail = cur;
        }
        else {  //旧链
            tail->next = cur;
            tail = cur;
        }
    }

    fclose(fin);
    return head;
}


int main()
{
    struct student *p;
    p = input();
    if (p == NULL) {    //申请失败直接返回-1
        return -1;
    }

    output(p);

    release(p);

    return 0;
}
