/* ���� 2051995 ���� */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

struct student {
    int *no;
    char *name;
    int *score;
    struct student *next;
};


/// ���������
void output(struct student *head)
{
    for (struct student *cur = head; cur != NULL; cur = cur->next)
        cout << *(cur->no) << ' ' << cur->name << ' ' << *(cur->score) << endl;
}

/// �½��ڵ㣬����ָ��
/// ��;ʧ�ܲ������ͷſռ�
struct student *newStudent(const int _no, const char *_name, const int _score)
{
    struct student *p;
    int len = strlen(_name);
    p = new(nothrow) struct student;
    if (p == NULL)
        return NULL;

    p->no = new(nothrow) int;
    if (p->no == NULL)
        return NULL;
    else
        *(p->no) = _no;

    p->name = new(nothrow)char[len + 1];
    if (p->name == NULL)
        return NULL;
    else
        strcpy(p->name, _name);

    p->score = new(nothrow)int;
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
    delete p->no;
    delete[]p->name;
    delete p->score;
    delete p;
}


/// �ͷ�
void release(struct student *head)
{
    struct student *cur, *nxt;
    for (cur = head; cur != NULL; cur = nxt) {
        nxt = cur->next;
        releaseStudent(cur);
        //delete cur;
    }
}


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
        //cur = new(nothrow) struct student;
        cur = newStudent(_no, _name, _score);
        if (cur == NULL) {
            //����ʧ�ܣ��������ͷ�
            fin.close();
            return NULL;
        }

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
