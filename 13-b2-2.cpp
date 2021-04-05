/* 朱泽凯 2051995 济美 */
#include <fstream>
#include <iostream>
#include <cstdlib>
using namespace std;

/// 存一个学生的信息
struct student {
    int no;         //学号，不考虑0开头
    char name[9];   //姓名，最长4个双字节汉字
    int score;      //成绩，不考虑小数点
    int rank;       //名次
};

/// 排序
/// [ begin, end )
/// cmp(*begin, *end) != 0
void student_sort(struct student *begin, struct student *end, int (*_cmp)(const struct student *lhs, const struct student *rhs))
{
    struct student *i, *j, *tmp;
    struct student buf;
    for (i = begin; i < end; i++) {

        //找更优
        for (j = tmp = i; j < end; j++)
            if (_cmp(j, tmp))
                tmp = j;

        //交换
        if (i != tmp) {
            buf = *i;
            *i = *tmp;
            *tmp = buf;
        }
    }
}

/// 第一关键字为 score (大更优)
/// 第二关键字为 name  (小更优)
int cmp(const struct student *lhs, const struct student *rhs)
{
    return lhs->score == rhs->score ? lhs->no < rhs->no : lhs->score > rhs->score;
}


/// 按 no 排序(小更优)
int cmp_no(const struct student *lhs, const struct student *rhs)
{
    return lhs->no < rhs->no;
}


/// 排序并计算rank
/// 要求 [begin,end) 地址合法
void getrank(struct student *begin, struct student *end)
{
    //排序
    student_sort(begin, end, cmp);

    //计算rank
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

    //按学号排序
    student_sort(begin, end, cmp_no);
}

/// 初始化，读入 *_n(人数)，并读入_n个人的信息返回一个数组头指针
struct student *input(int *_n)
{
    ifstream fin;
    struct student *p;

    //打开文件
    fin.open("student.txt");
    if (fin.fail()) {
        fprintf(stderr, "打开文件 student.txt 失败\n");
        return NULL;
    }

    //分配空间
    fin >> *_n;
    if (fin.fail()) {
        fin.close();
        return NULL;
    }
    if ((p = new(nothrow)student[*_n]) == NULL) {
        fprintf(stderr, "分配空间失败\n");
        fin.close();
        return NULL;
    }

    //读入数据
    for (int i = 0; i < *_n; i++)
        fin >> p[i].no >> p[i].name >> p[i].score;
    if (fin.fail()) {
        delete[]p;
        fin.close();
        return NULL;
    }

    //返回指针
    fin.close();
    return p;
}

int main()
{
    int n;
    struct student *p;

    //读入
    if ((p = input(&n)) == NULL)
        return 1;

    //排序并计算rank
    getrank(p, p + n);

    //输出
    for (int i = 0; i < n; i++)
        cout << p[i].no << ' ' << p[i].name << ' ' << p[i].score << ' ' << p[i].rank << endl;

    //释放
    delete[]p;

    return 0;
}