/* ���� 2051995 ���� */
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


/// ���������
void output(struct student *head)
{
    for (struct student *cur = head; cur != NULL; cur = cur->next)
        printf("%d %s %d\n", cur->no, cur->name, cur->score);
}

/// �ͷ�
void release(struct student *head)
{
    struct student *cur, *nxt;
    for (cur = head; cur != NULL; cur = nxt) {
        nxt = cur->next;
        free(cur);
    }
}



/// ����ͷָ��(ָ���һ��student)
struct student *input()
{
    int _no;
    char _name[9];
    int _score;

    struct student *head = NULL, *cur = NULL, *tail = NULL;

    FILE *fin;
    if ((fin = fopen("list.txt", "r")) == NULL) {
        fprintf(stderr, "���ļ� list.txt ʧ��\n");
        return NULL;
    }

    while (1) {
        if (fscanf(fin, "%d", &_no) != 1) {
            fclose(fin);
            return NULL;
        }
        if (_no == 9999999) {   //�������������ָ��
            break;
        }
        if (fscanf(fin, "%s%d", _name, &_score) != 2) {
            fclose(fin);
            return NULL;
        }

        //�½��ڵ�
        cur = malloc(sizeof(struct student));
        if (cur == NULL) {
            //����ʧ�ܣ��������ͷ�
            fclose(fin);
            return NULL;
        }
        cur->no = _no;
        strcpy(cur->name, _name);
        cur->score = _score;
        cur->next = NULL;

        //���ӽڵ�
        if (head == NULL) { //����
            head = tail = cur;
        }
        else {  //����
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
    if (p == NULL) {    //����ʧ��ֱ�ӷ���-1
        return -1;
    }

    output(p);
    release(p);

    return 0;
}
