/* 朱泽凯 2051995 济美 */

/******************************
* 注：本程序对于错误处理，都是调用 haltErr 直接输出提示信息，然后 exit 了
*
*
* 若 dat 中有该同学的信息，且filename是 all，且没有他的文件夹，则不输出他的结果
*
* 判断规则：
*
* 只匹配 .cpp/.c/.h 三种文件
*
* 若 filename 是 all
*   根据 dat 中的信息查找所有在课的学生，按学生学号排序，将该课该学生对应文件夹内所有匹配文件检查并输出
*
* 若 filename 不是 all 且是可匹配的文件配型
*   根据 dat 中信息查找所有在课学生的该文件，没有则输出未提交，有则检查并输出
*
******************************/

#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <io.h>
using namespace std;

int cccccnt;

//出现错误时调用的中断函数
void haltErr(const string &errinfo = "", int errcode = -1)
{
    cerr << errinfo << endl;
    exit(errcode);
}


//通用链表节点
template<class T>
class node {
protected:
    node *nxt;

public:
    T data;

    ~node()
    {
        int tmp = ++cccccnt;
        cout << "start " << tmp << endl;
        if (nxt != NULL)
            delete nxt;
        cout << "end " << tmp << endl;
    }

    node() :nxt(NULL) {}

    void push(const T &val)
    {
        node<T> *np = new node<T>;
        np->data = val;
        np->nxt = this->nxt;
        this->nxt = np;
    }

    //cmp(cur, nxt) == true
    void sort(bool cmp(const T &, const T &))
    {
        node *cur, *p;

        for (cur = this->nxt; cur != NULL; cur = cur->nxt) {
            for (p = cur->nxt; p != NULL; p = p->nxt) {
                if (!cmp(cur->data, p->data)) {
                    swap(cur->data, p->data);
                }
            }
        }

    }

    node *next()
    {
        return nxt;
    }
    node *find(const T &val) const
    {
        node *p = this->nxt;
        while (p != NULL && !(p->data == val))
            p = p->nxt;
        return p;
    }
    node *find(const T &val, bool *equl(const T &, const T &)) const
    {
        node *p = this->nxt;
        while (p != NULL && !(equl(this->data, val)))
            p = p->nxt;
        return p;
    }

   
};


//一个学生的信息
struct studata {
    string number;          //学号
    string name;            //名字
    string course_full;      //班级全称
    string course_abbr;      //班级简称
};


//一个班级的信息
struct coursedata {
    string courseid;
    node<studata> stu_head;
};

bool cmp(const studata &X, const studata &Y)    //学生按学号排序，小号在前
{
    return X.number < Y.number;
}


void fun()
{
    node<long long> A;
    int cnt = 0;
    puts(".");
    getchar();
    for (long long i = 1; i <= 1000; i++) {
        if (i % (1024 * 1024) == 0) {
            cout << ++cnt << endl;
        }
        A.push(i);
    }
    puts("done");
    getchar();
}

int main(const int argc, const char *argv[])
{

    fun();
    puts("\nendfun");
    getchar();
    return 0;

    return 0;
}

