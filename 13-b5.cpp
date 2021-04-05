/* 朱泽凯 2051995 济美 */
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
        //cout << "空间释放" << id << " " << name << " " << school << endl;
        if (id != NULL)
            delete[]id;
        if (name != NULL)
            delete[]name;
        if (school != NULL)
            delete[]school;
    }

    /// 从 fin 读入一个 student 信息（报名号，姓名，学校）
    /// 成功则返回 1
    int fread(ifstream &_fin)
    {
        static char _id[15], _name[30], _school[50];
        //cout << "开始读入一个 student 信息" << endl;
        _fin >> _id >> _name >> _school;
        if (_fin.fail()) {
            cout << "数据错误，读入失败" << endl;
            return 0;
        }

        id = new(nothrow) char[(strlen(_id) + 1) * sizeof(char)];
        if (id == NULL) {
            cout << "为" << _id << '(' << _name << ")的 id 申请空间失败" << endl;
            return 0;
        }
        else
            strcpy(id, _id);

        name = new(nothrow) char[(strlen(_name) + 1) * sizeof(char)];
        if (name == NULL) {
            cout << "为" << _id << '(' << _name << ")的 name 申请空间失败" << endl;
            return 0;
        }
        else
            strcpy(name, _name);

        school = new(nothrow) char[(strlen(_school) + 1) * sizeof(char)];
        if (school == NULL) {
            cout << "为" << _id << '(' << _name << ")的 school 申请空间失败" << endl;
            return 0;
        }
        else
            strcpy(school, _school);

        //cout << "读入成功" << endl;
        return 1;
    }
};

student *input(int &n, int &m)
{
    ifstream fin;
    fin.open("stulist.txt", ios::in);
    if (fin.is_open() == 0) {
        cout << "打开文件 stulist.txt 失败" << endl;
        return NULL;
    }

    fin >> n >> m;
    if (n > m) {
        cout << "数据错误：选出人数大于总人数" << endl;
        fin.close();
        return NULL;
    }

    //分配空间
    student *ret;
    ret = new(nothrow) student[m];
    if (ret == NULL) {
        cout << "为 " << m << " 个 student 申请空间失败，需要多于 " << 1ll * m * sizeof(student) << "字节" << endl;
        fin.close();    //关闭文件
        return NULL;
    }

    //读 m 个 student
    for (int i = 0; i < m; i++)
        if (!ret[i].fread(fin)) {
            fin.close();
            return NULL;
        }

    fin.close();    //关闭文件
    return ret;
}

/// a 的 m个里随机选n个输出到result.txt中
/// 注意：会损坏 a 中的内容
void sortWrite(int n, int m, student *a)
{

    //打开写文件
    ofstream fout;
    fout.open("result.txt", ios::out);
    if (fout.is_open() == 0) {
        cout << "打开写文件 result.txt 失败" << endl;
        return;
    }

    srand((unsigned)time(0));
    int flag = (m - n > n);   //1:抽选的 0:抽不选的
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

    fout.close();   //关闭写文件
}

int main()
{
    student *stu;
    int N, M;

    cout << "尝试从文件 stulist.txt 载入..." << endl;
    stu = input(N, M);
    if (stu == NULL) {
        return -1;
    }

    /*
    for (int i = 0; i < M; i++) {
        cout << stu[i].id << " " << stu[i].name << endl;
    }
    */

    cout << "尝试抽卡并输出到文件 result.txt..." << endl;
    sortWrite(N, M, stu);

    delete[]stu;
    return 0;
}

/****************************
*
*
*  采取直接申请连续M个student空间当数组使用。
*  因为随机选则需要频繁随机访问学生信息，连续空间便于访问。
*  而且一个 student 占不超过一百字节，该地初中学生再怎样也不会超过十万人，这样下来需要的空间也不会大于 10Mb。
*
*  而随机与输出，就不再额外要空间了，选中的就在原来申请的变量上标记一下，然后输出到文件里就行了
*
* */