/* 朱泽凯 2051995 济美 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

struct student {
    int no;
    char name[9];
    int score;
    struct student *next;
};

/// 返回头指针(指向第一个student)
struct student* input()
{
    int _no;
    char _name[9];
    int _score;

    struct student *head=NULL, *cur=NULL, *tail=NULL;

    ifstream fin;
    fin.open("list.txt");
    if (fin.fail()) {
        fprintf(stderr, "打开文件 list.txt 失败\n");
        return NULL;
    }

    while (1) {
        fin >> _no;
        if (_no == 9999999) {   //输入结束，返回指针
            break;
        }
        fin >>_name >> _score;

        

        //新建节点
        cur = new(nothrow) struct student;
        if (cur == NULL) {
            //申请失败，不考虑释放
            fin.close();
            return NULL;
        }
        cur->no = _no;
        strcpy(cur->name, _name);
        cur->score = _score;
        cur->next=NULL;

        //连接节点
        if (head == NULL) { //新链
            head = tail = cur;
        }
        else {  //旧链
            tail->next = cur;
            tail = cur;
        }
    }

    fin.close();
    return head;
}

/// 遍历并输出
void output(struct student *head)
{
    for (struct student *cur = head; cur!=NULL; cur=cur->next)
        cout << cur->no << ' ' << cur->name << ' ' << cur->score << endl;
}

/// 释放
void release(struct student *head)
{
    struct student *cur, *nxt;
    for (cur = head; cur != NULL; cur=nxt) {
        nxt = cur->next;
        delete cur;
    }
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
