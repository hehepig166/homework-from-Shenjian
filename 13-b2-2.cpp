/* ���� 2051995 ���� */
#include <fstream>
#include <iostream>
#include <cstdlib>
using namespace std;

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


/// �� no ����(С����)
int cmp_no(const struct student *lhs, const struct student *rhs)
{
    return lhs->no < rhs->no;
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
    struct student *cur = begin;
    cur->rank = cnt = 1;
    lst = cur;
    for (cur++, cnt++; cur < end; cnt++, cur++) {
        if (cur->score == lst->score) {
            cur->rank = lst->rank;
        }
        else {
            cur->rank = cnt;
            lst = cur;
        }
    }

    //��ѧ������
    student_sort(begin, end, cmp_no);
}

/// ��ʼ�������� *_n(����)��������_n���˵���Ϣ����һ������ͷָ��
struct student *input(int *_n)
{
    ifstream fin;
    struct student *p;

    //���ļ�
    fin.open("student.txt");
    if (fin.fail()) {
        fprintf(stderr, "���ļ� student.txt ʧ��\n");
        return NULL;
    }

    //����ռ�
    fin >> *_n;
    if (fin.fail()) {
        fin.close();
        return NULL;
    }
    if ((p = new(nothrow)student[*_n]) == NULL) {
        fprintf(stderr, "����ռ�ʧ��\n");
        fin.close();
        return NULL;
    }

    //��������
    for (int i = 0; i < *_n; i++)
        fin >> p[i].no >> p[i].name >> p[i].score;
    if (fin.fail()) {
        delete[]p;
        fin.close();
        return NULL;
    }

    //����ָ��
    fin.close();
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

    //���
    for (int i = 0; i < n; i++)
        cout << p[i].no << ' ' << p[i].name << ' ' << p[i].score << ' ' << p[i].rank << endl;

    //�ͷ�
    delete[]p;

    return 0;
}