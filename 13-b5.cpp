/* ���� 2051995 ���� */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>
using namespace std;

struct student {
    char *id;
    char *name;
    char *school;

public:
    student() : id(NULL), name(NULL), school(NULL) {}
    ~student()
    {
        //cout << "�ռ��ͷ�" << id << " " << name << " " << school << endl;
        if (id != NULL)
            delete[]id;
        if (name != NULL)
            delete[]name;
        if (school != NULL)
            delete[]school;
    }

    /// �� fin ����һ�� student ��Ϣ�������ţ�������ѧУ��
    /// �ɹ��򷵻� 1
    int fread(ifstream &_fin)
    {
        static char _id[15], _name[30], _school[50];
        //cout << "��ʼ����һ�� student ��Ϣ" << endl;
        _fin >> _id >> _name >> _school;
        if (_fin.fail()) {
            cout << "���ݴ��󣬶���ʧ��" << endl;
            return 0;
        }

        id = new(nothrow) char[(strlen(_id) + 1) * sizeof(char)];
        if (id == NULL) {
            cout << "Ϊ" << _id << '(' << _name << ")�� id ����ռ�ʧ��" << endl;
            return 0;
        }
        else
            strcpy(id, _id);

        name = new(nothrow) char[(strlen(_name) + 1) * sizeof(char)];
        if (name == NULL) {
            cout << "Ϊ" << _id << '(' << _name << ")�� name ����ռ�ʧ��" << endl;
            return 0;
        }
        else
            strcpy(name, _name);

        school = new(nothrow) char[(strlen(_school) + 1) * sizeof(char)];
        if (school == NULL) {
            cout << "Ϊ" << _id << '(' << _name << ")�� school ����ռ�ʧ��" << endl;
            return 0;
        }
        else
            strcpy(school, _school);

        //cout << "����ɹ�" << endl;
        return 1;
    }
};

student *input(int &n, int &m)
{
    ifstream fin;
    fin.open("stulist.txt", ios::in);
    if (fin.is_open() == 0) {
        cout << "���ļ� stulist.txt ʧ��" << endl;
        return NULL;
    }

    fin >> n >> m;
    if (n > m) {
        cout << "���ݴ���ѡ����������������" << endl;
        fin.close();
        return NULL;
    }

    //����ռ�
    student *ret;
    ret = new(nothrow) student[m];
    if (ret == NULL) {
        cout << "Ϊ " << m << " �� student ����ռ�ʧ�ܣ���Ҫ���� " << 1ll * m * sizeof(student) << "�ֽ�" << endl;
        fin.close();    //�ر��ļ�
        return NULL;
    }

    //�� m �� student
    for (int i = 0; i < m; i++)
        if (!ret[i].fread(fin)) {
            fin.close();
            return NULL;
        }

    fin.close();    //�ر��ļ�
    return ret;
}

/// a �� m�������ѡn�������result.txt��
/// ע�⣺���� a �е�����
void sortWrite(int n, int m, student *a)
{

    //��д�ļ�
    ofstream fout;
    fout.open("result.txt", ios::out);
    if (fout.is_open() == 0) {
        cout << "��д�ļ� result.txt ʧ��" << endl;
        return;
    }

    srand((unsigned)time(0));
    int flag = (m - n > n);   //1:��ѡ�� 0:�鲻ѡ��
    int tmp;
    if (flag) {
        while (n--) {
            for (tmp = rand() % m; a[tmp].id == NULL; tmp = rand() % m)
                continue;
            fout << a[tmp].id << ' ' << a[tmp].name << ' ' << a[tmp].school << endl;
            a[tmp].id = NULL;
        }
    }
    else {
        n = m - n;
        while (n--) {
            for (tmp = rand() % m; a[tmp].id == NULL; tmp = rand() % m)
                continue;
            a[tmp].id = NULL;
        }
        for (int i = 0; i < m; i++)
            if (a[i].id != NULL)
                fout << a[i].id << ' ' << a[i].name << ' ' << a[i].school << endl;
    }

    fout.close();   //�ر�д�ļ�
}

int main()
{
    student *stu;
    int N, M;

    cout << "���Դ��ļ� stulist.txt ����..." << endl;
    stu = input(N, M);
    if (stu == NULL) {
        return -1;
    }

    /*
    for (int i = 0; i < M; i++) {
        cout << stu[i].id << " " << stu[i].name << endl;
    }
    */

    cout << "���Գ鿨��������ļ� result.txt..." << endl;
    sortWrite(N, M, stu);

    delete[]stu;
    return 0;
}

/****************************
*
*
*  ��ȡֱ����������M��student�ռ䵱����ʹ�á�
*  ��Ϊ���ѡ����ҪƵ���������ѧ����Ϣ�������ռ���ڷ��ʡ�
*  ����һ�� student ռ������һ���ֽڣ��õس���ѧ��������Ҳ���ᳬ��ʮ���ˣ�����������Ҫ�Ŀռ�Ҳ������� 10Mb��
*
*  �������������Ͳ��ٶ���Ҫ�ռ��ˣ�ѡ�еľ���ԭ������ı����ϱ��һ�£�Ȼ��������ļ��������
*
* */