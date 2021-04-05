/* ���� 2051995 ���� */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

/// ��һ��ѧ������Ϣ
struct student {
    int no;         //ѧ�ţ�������0��ͷ
    char name[9];   //�������4��˫�ֽں���
    int score;      //�ɼ���������С����
    int rank;       //����
};

/// ����
/// [ begin, end )
/// cmp(*begin, *end) != 0
void student_sort(struct student *begin, struct student *end, int (*_cmp)(const struct student *lhs, const struct student *rhs))
{
    struct student *i, *j, *tmp;
    struct student buf;
    for (i = begin; i < end; i++) {

        //�Ҹ���
        for (j = tmp = i; j < end; j++)
            if (_cmp(j, tmp))
                tmp = j;

        //����
        if (i != tmp) {
            buf = *i;
            *i = *tmp;
            *tmp = buf;
        }
    }
}

/// ��һ�ؼ���Ϊ score (�����)
/// �ڶ��ؼ���Ϊ name  (С����)
int cmp(const struct student *lhs, const struct student *rhs)
{
    return lhs->score == rhs->score ? lhs->no < rhs->no : lhs->score > rhs->score;
}

/// ���򲢼���rank
/// Ҫ�� [begin,end) ��ַ�Ϸ�
void getrank(struct student *begin, struct student *end)
{
    //����
    student_sort(begin, end, cmp);

    //����rank
    int cnt;
    struct student *lst;
    begin->rank = cnt = 1;
    lst = begin;
    for (begin++, cnt++; begin < end; cnt++,begin++) {
        if (begin->score == lst->score) {
            begin->rank = lst->rank;
        }
        else {
            begin->rank = cnt;
            lst = begin;
        }
    }
}

/// ��ʼ�������� *_n(����)��������_n���˵���Ϣ����һ������ͷָ��
struct student *input(int *_n)
{
    FILE *fin;
    struct student *p;

    //���ļ�
    if ((fin = fopen("student.txt", "r")) == NULL) {
        fprintf(stderr, "���ļ� student.txt ʧ��\n");
        return NULL;
    }

    //����ռ�
    if (fscanf(fin, "%d", _n) != 1) {
        fclose(fin);
        return NULL;
    }
    if ((p = (struct student*)malloc(*_n * sizeof(struct student))) == NULL) {
        fprintf(stderr, "����ռ�ʧ��\n");
        fclose(fin);
        return NULL;
    }

    //��������
    for (int i = 0; i < *_n; i++)
        if (fscanf(fin, "%d%s%d", &p[i].no, p[i].name, &p[i].score) != 3) {
            free(p);
            fclose(fin);
            return NULL;
        }

    //����ָ��
    fclose(fin);
    return p;
}

int main()
{
    int n;
    struct student *p;

    //����
    if ((p = input(&n)) == NULL)
        return 1;

    //���򲢼���rank
    getrank(p, p + n);

    //�õ�rank

    //���
    for (int i = 0; i < n; i++)
        printf("%d %s %d %d\n", p[i].no, p[i].name, p[i].score, p[i].rank);

    //�ͷ�
    free(p);

    return 0;
}