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

//只接受 .cpp/.c/.h 文件
bool goodtype(const char *str)
{
    if (str == NULL || *str == 0)
        return false;
    int len = strlen(str);
    if (str[len - 1] == 'p')
        return len >= 4 && !strcmp(str + len - 4, ".cpp");
    else if (str[len - 1] == 'h')
        return len >= 2 && !strcmp(str + len - 2, ".h");
    else if (str[len - 1] == 'c')
        return len >= 2 && !strcmp(str + len - 2, ".c");
    else
        return false;
}


//通用链表节点
template<class T>
class node {
protected:
    node *nxt;
    void release()
    {
        if (nxt != NULL) {
            nxt->release();
            delete nxt;
            nxt = NULL;
        }
    }

public:
    T data;

    ~node()
    {
        //release();
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
        while (p != NULL && !(p->data == T))
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


//一次测试的信息
struct testdata {
    string courseid;        //课号
    string filename;        //检查文件名称，若为 all 则意思是该课号下所有文件
    bool use_firstline;
    bool use_courseid;
    bool use_filename;

    testdata() :use_courseid(0), use_filename(0) {}
    void init()
    {
        use_firstline = use_courseid = use_filename = 0;
    }
    bool good()
    {
        if (!use_firstline) {
            cerr << "错误: 没有参数 --firstline" << endl;
            return false;
        }
        if (!use_courseid) {
            cerr << "错误: 没有参数 --courseid" << endl;
            return false;
        }
        if (!use_filename) {
            cerr << "错误: 没有参数 --filename" << endl;
            return false;
        }
        if (filename != "all" && goodtype(filename.c_str())) {
            cerr << "错误: " << filename << " 不是可接受的源程序文件" << endl;
            return false;
        }
        return true;
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



void getTestdata(int argc, const char *argv[], testdata &testinfo);     //识别 main 函数参数，存到 testinfo 中

void getCoursedata(string filepath, coursedata &courseinfo);      //从路径 filepath 所指的课程学生文件读取 courseinfo 信息（该课程里的学生信息），存到 courseinfo 中

void searchAllFiles(string path, const testdata &testinfo, const studata &stuinfo);     //遍历文件夹下所有文件

void touchOneFile(string filepath, const testdata &testinfo, const studata &stuinfo);      //判断路径 filpath 所指文件是否要检查，如文件是否存在、文件格式是否要检查等，用过渡到单个文件的格式检查函数

void testFirstline(string filepath, const studata &stuinfo);        //检查首行，并输出检查结果到 stdout

bool cmp(const studata &X, const studata &Y)    //学生按学号排序，小号在前
{
    return X.number < Y.number;
}

bool str_is_blank(const string &str)   //返回str是否全由空白字符组成
{
    for (int i = str.length() - 1; i >= 0; i--)
        if (str[i] != ' ' && str[i] != '\r' && str[i] != '\n' && str[i] != '\t')
            return false;
    return true;
}

void fun()
{
    node<long long> A;
    int cnt = 0;
    getchar();
    for (long long i = 1; i <= 100000; i++) {
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
    getchar();
    return 0;
    
    testdata testinfo;
    coursedata courseinfo;

    getTestdata(argc, argv, testinfo);

    getCoursedata("./source/" + testinfo.courseid + ".dat", courseinfo);

    //以下已经保证 testinfo 和 courseinfo 里面都是正常的值

    courseinfo.stu_head.sort(cmp);

    for (node<studata> *p = courseinfo.stu_head.next(); p != NULL; p = p->next()) {

        string stupath = "./source/" + testinfo.courseid + "-" + p->data.number;

        cout << endl << p->data.number << " " << p->data.name << endl;

        if (testinfo.filename == "all") {
            searchAllFiles(stupath, testinfo, p->data);
        }
        else {
            touchOneFile(stupath, testinfo, p->data);
        }

    }

    return 0;
}














void searchAllFiles(string path, const testdata &testinfo, const studata &stuinfo)
{
    long hFile = 0; //文件信息
    _finddata_t fileinfo;    //存储文件信息的结构体
    string p;

    if ((hFile = _findfirst(p.assign(path).append("/*").c_str(), &fileinfo)) != -1) {  //第一次查找

        do {

            if ((fileinfo.attrib & _A_SUBDIR)) {    //若找到的是文件夹

                if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) {

                    searchAllFiles(p.assign(path).append("/").append(fileinfo.name), testinfo, stuinfo);      //递归进入文件夹

                }

            }
            else {  //若找到的不是文件夹

                touchOneFile(path + "/" + fileinfo.name, testinfo, stuinfo);

            }

        } while (_findnext(hFile, &fileinfo) == 0);

    }

    _findclose(hFile);  //结束查找
}




void getTestdata(int argc, const char *argv[], testdata &testinfo)
{
    string USAGE_STR =
        "Usage: " + string(argv[0]) + " --firstline | --courseid cid | --filename name / all\n" +
        "  --firstline : 首行检查\n" +
        "  --courseid  : 课号\n" +
        "  --filename  : 文件名(all表示全部)\n" +
        "\n" +
        "e.g.   " + string(argv[0]) + " --firstline --courseid 100692 --filename 3-b1.cpp : 检查100692课程的3-b1.cpp的首行\n" +
        "       " + string(argv[0]) + " --firstline --courseid 100692 --filename all : 检查100692课程的全部文件的首行";

    testinfo.init();    //所有位清零

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--firstline") == 0) {
            testinfo.use_firstline = 1;
        }
        else if (strcmp(argv[i], "--courseid") == 0) {
            if (++i < argc) {
                testinfo.courseid = argv[i];
                testinfo.use_courseid = 1;
            }
            else {
                haltErr("参数courseid值错误\n" + USAGE_STR);
            }
        }
        else if (strcmp(argv[i], "--filename") == 0) {
            if (++i < argc) {
                testinfo.filename = argv[i];
                testinfo.use_filename = 1;
            }
            else {
                haltErr("参数filename值错误\n" + USAGE_STR);
            }
        }
        else {
            haltErr(string("未知的参数 ") + argv[i]);
        }
    }

    if (!testinfo.good()) {
        haltErr();
    }

}



void getCoursedata(string filepath, coursedata &courseinfo)
{
    ifstream fin(filepath, ios::in);
    if (!fin.is_open()) {
        haltErr("Can't open" + filepath);
    }

    string tmpstr;
    string tmpline;
    studata tmpstu;

    while (getline(fin, tmpline).good()) {
        if (str_is_blank(tmpline))  //空白行则跳过
            continue;
        istringstream istr(tmpline);

        istr >> tmpstu.number >> tmpstu.name >> tmpstu.course_full >> tmpstu.course_abbr;

        if (istr.fail() || !istr.eof()) {  //dat文件某一行信息数量不对
            haltErr(filepath + "中有错( " + tmpline + ")");
        }
        courseinfo.stu_head.push(tmpstu);   //加入链表
    }

    courseinfo.stu_head.sort(cmp);  //按学号小到大排序

    node<studata> *p = courseinfo.stu_head.next();
    while (p) {
        cout << p->data.number << " " << p->data.name << endl;
        p = p->next();
    }

    fin.close();
}

void touchOneFile(string filepath, const testdata &testinfo, const studata &stuinfo)
{
    cout << "touchOneFile " << filepath << endl;
}

void testFirstline(string filepath, const studata &stuinfo)
{
    cout << "testFirstline " << filepath << endl;
}