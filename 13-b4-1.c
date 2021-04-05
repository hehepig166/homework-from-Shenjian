/* ���� 2051995 ���� */
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


/// ���������
void output(struct student *head)
{
    for (struct student *cur = head; cur != NULL; cur = cur->next)
        printf("%d %s %d\n", *(cur->no), cur->name, *(cur->score));
}

/// �½��ڵ㣬����ָ��
/// ��;ʧ�ܲ������ͷ�
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

/// �ͷ�
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
        cur = newStudent(_no, _name, _score);
        if (cur == NULL) {
            //����ʧ�ܣ��������ͷ�
            fclose(fin);
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
