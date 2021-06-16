/* ���� 2051995 ���� */

/******************************
* ע����������ڴ��������ǵ��� haltErr ֱ�������ʾ��Ϣ��Ȼ�� exit ��
*
*
* �� dat ���и�ͬѧ����Ϣ����filename�� all����û�������ļ��У���������Ľ��
*
* �жϹ���
*
* ֻƥ�� .cpp/.c/.h �����ļ�
*
* �� filename �� all
*   ���� dat �е���Ϣ���������ڿε�ѧ������ѧ��ѧ�����򣬽��ÿθ�ѧ����Ӧ�ļ���������ƥ���ļ���鲢���
*
* �� filename ���� all ���ǿ�ƥ����ļ�����
*   ���� dat ����Ϣ���������ڿ�ѧ���ĸ��ļ���û�������δ�ύ�������鲢���
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

//���ִ���ʱ���õ��жϺ���
void haltErr(const string &errinfo = "", int errcode = -1)
{
    cerr << errinfo << endl;
    exit(errcode);
}


//ͨ������ڵ�
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


//һ��ѧ������Ϣ
struct studata {
    string number;          //ѧ��
    string name;            //����
    string course_full;      //�༶ȫ��
    string course_abbr;      //�༶���
};


//һ���༶����Ϣ
struct coursedata {
    string courseid;
    node<studata> stu_head;
};

bool cmp(const studata &X, const studata &Y)    //ѧ����ѧ������С����ǰ
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

