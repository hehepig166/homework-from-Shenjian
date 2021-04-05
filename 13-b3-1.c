/* 朱泽凯 2051995 济美 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct student {
    int no;
    char name[9];
    int score;
    struct student *next;
};


/// 遍历并输出
void output(struct student *head)
{
    for (struct student *cur = head; cur != NULL; cur = cur->next)
        printf("%d %s %d\n", cur->no, cur->name, cur->score);
}

/// 释放
void release(struct student *head)
{
    struct student *cur, *nxt;
    for (cur = head; cur != NULL; cur = nxt) {
        nxt = cur->next;
        free(cur);
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
        cur = malloc(sizeof(struct student));
        if (cur == NULL) {
            //申请失败，不考虑释放
            fclose(fin);
            return NULL;
        }
        cur->no = _no;
        strcpy(cur->name, _name);
        cur->score = _score;
        cur->next = NULL;

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
