/* ���� 2051995 ���� */
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

/// ����ͷָ��(ָ���һ��student)
struct student* input()
{
    int _no;
    char _name[9];
    int _score;

    struct student *head=NULL, *cur=NULL, *tail=NULL;

    ifstream fin;
    fin.open("list.txt");
    if (fin.fail()) {
        fprintf(stderr, "���ļ� list.txt ʧ��\n");
        return NULL;
    }

    while (1) {
        fin >> _no;
        if (_no == 9999999) {   //�������������ָ��
            break;
        }
        fin >>_name >> _score;

        

        //�½��ڵ�
        cur = new(nothrow) struct student;
        if (cur == NULL) {
            //����ʧ�ܣ��������ͷ�
            fin.close();
            return NULL;
        }
        cur->no = _no;
        strcpy(cur->name, _name);
        cur->score = _score;
        cur->next=NULL;

        //���ӽڵ�
        if (head == NULL) { //����
            head = tail = cur;
        }
        else {  //����
            tail->next = cur;
            tail = cur;
        }
    }

    fin.close();
    return head;
}

/// ���������
void output(struct student *head)
{
    for (struct student *cur = head; cur!=NULL; cur=cur->next)
        cout << cur->no << ' ' << cur->name << ' ' << cur->score << endl;
}

/// �ͷ�
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
    if (p == NULL) {    //����ʧ��ֱ�ӷ���-1
        return -1;
    }

    output(p);
    release(p);

    return 0;
}
