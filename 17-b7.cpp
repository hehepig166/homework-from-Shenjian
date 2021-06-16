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

#if defined(__linux) || defined(__linux__)
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
//#include <sys/io.h>
#else
#include <io.h>
#endif

using namespace std;

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


//通用链表节点(需要自己对头节点release())
template<class T>
class node {
protected:
    node *nxt;

public:
    T data;

    node() :nxt(NULL) {}
    ~node() {}

    void release()
    {
        node *cur = nxt;
        while (cur) {
            node *tmp = cur->nxt;
            delete cur;
            cur = tmp;
        }
        nxt = NULL;
    }

    void push(const T &val)
    {
        node<T> *np = new(nothrow) node<T>;
        if (np == NULL) {
            haltErr("No enough memory space");
        }
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


//一次测试的信息
struct testdata {
    string courseid;        //课号
    string filename;        //检查文件名称，若为 all 则意思是该课号下所有文件
    bool use_firstline;
    bool use_courseid;
    bool use_filename;

    testdata() :use_firstline(0), use_courseid(0), use_filename(0) {}
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
        if (filename != "all" && !goodtype(filename.c_str())) {
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

    friend ostream &operator<<(ostream &out, const studata &stu)
    {
        out << (stu.number + "-" + stu.name + "-" + stu.course_full);
        return out;
    }
};


//一个班级的信息
struct coursedata {
    string courseid;
    node<studata> stu_head;
    ~coursedata()
    {
        stu_head.release();     //重要
    }
};



void getTestdata(int argc, const char *argv[], testdata &testinfo);     //识别 main 函数参数，存到 testinfo 中

void getCoursedata(string filepath, coursedata &courseinfo);      //从路径 filepath 所指的课程学生文件读取 courseinfo 信息（该课程里的学生信息），存到 courseinfo 中

void searchAllFiles(string path, const testdata &testinfo, const studata &stuinfo);     //遍历文件夹下所有文件

void touchOneFile(const string &path, const string &fname, const testdata &testinfo, const studata &stuinfo);      //负责检查格式、调用检查函数以及格式化输出检查结果到stdout

string testFirstline(string filepath, const studata &stuinfo);        //检查首行，返回检查结果字符串

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



int main(const int argc, const char *argv[])
{
    testdata testinfo;
    coursedata courseinfo;

    getTestdata(argc, argv, testinfo);

    getCoursedata("./source/" + testinfo.courseid + ".dat", courseinfo);

    //以下已经保证 testinfo 和 courseinfo 里面都是正常的值

    courseinfo.stu_head.sort(cmp);

    for (node<studata> *p = courseinfo.stu_head.next(); p != NULL; p = p->next()) {

        string stupath = "./source/" + testinfo.courseid + "-" + p->data.number;

        if (testinfo.filename == "all") {
            searchAllFiles(stupath, testinfo, p->data);
        }
        else {
            touchOneFile(stupath, testinfo.filename, testinfo, p->data);
        }

    }

    return 0;
}

















void searchAllFiles(string path, const testdata &testinfo, const studata &stuinfo)
{
#if defined(__linux) || defined(__linux__)
    int flag = 0;
    DIR *pDir = NULL;
    struct dirent *pFile = NULL;

    pDir = opendir(path.c_str());
    if (pDir == NULL)
        return;

    while ((pFile = readdir(pDir)) != NULL) {

        if (pFile->d_type & DT_DIR) {   //文件夹
            if (strcmp(pFile->d_name, ".") != 0 && strcmp(pFile->d_name, "..") != 0) {
                searchAllFiles(path + "/" + pFile->d_name, testinfo, stuinfo);
            }
        }
        else {      //文件
            touchOneFile(path, pFile->d_name, testinfo, stuinfo);
            flag = 1;
        }

    }

    flag ? (cout << endl) : cout;      //若有检查过文件，就在后面多加一个空行
    closedir(pDir);  //结束查找
#else
    long hFile = 0; //文件信息
    _finddata_t fileinfo;    //存储文件信息的结构体
    string p;

    int flag = 0;   //用于记录是否有检查过文件

    if ((hFile = _findfirst(p.assign(path).append("/*").c_str(), &fileinfo)) != -1) {  //第一次查找

        do {

            if ((fileinfo.attrib & _A_SUBDIR)) {    //若找到的是文件夹

                if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) {

                    searchAllFiles(p.assign(path).append("/").append(fileinfo.name), testinfo, stuinfo);      //递归进入文件夹

                }

            }
            else {  //若找到的不是文件夹

                touchOneFile(path, fileinfo.name, testinfo, stuinfo);
                flag = 1;

            }

        } while (_findnext(hFile, &fileinfo) == 0);

    }

    flag ? (cout << endl) : cout;      //若有检查过文件，就在后面多加一个空行
    _findclose(hFile);  //结束查找
#endif
}




void getTestdata(int argc, const char *argv[], testdata &testinfo)
{
    string USAGE_STR =
        "\n\nUsage: " + string(argv[0]) + " --firstline | --courseid cid | --filename name / all\n" +
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
        haltErr(USAGE_STR);
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

        if (istr.fail() || !((istr >> tmpstr).fail())) {  //dat文件某一行信息数量不对
            haltErr(filepath + "中有错( " + tmpline + ")");
        }
        courseinfo.stu_head.push(tmpstu);   //加入链表
    }

    courseinfo.stu_head.sort(cmp);  //按学号小到大排序

    fin.close();
}

void touchOneFile(const string &path, const string &fname, const testdata &testinfo, const studata &stuinfo)
{
    string filepath = path + "/" + fname;
    if (!goodtype(filepath.c_str())) {
        return;     //不是需要检查的文件，就不做任何操作
    }

    cout << stuinfo << " : " << fname << " : " << testFirstline(filepath, stuinfo) << endl;
}

string testFirstline(string filepath, const studata &stuinfo)
{
    ifstream fin(filepath);
    if (!fin.is_open()) {       //找不到文件
        return "未提交";
    }

    string strline;
    while (getline(fin, strline).good() && str_is_blank(strline))
        continue;

    fin.close();    //重要

    if (str_is_blank(strline)) {    //是纯空白字符文件
        return "交个空文件给我，想干啥？";
    }

    istringstream istr(strline);

    node<string> toks;
    int tokcnt = 0;
    string tmpstr;
    string ret;

    //分析首行（若出现多组/**/，只处理第一组）
#define FAIL_ANALYSE_FIRSTLINE toks.release(); return "未取到首行信息"
    int f = 0; //0:前面为空  1:前面有/*  2:前面有//
    while (1) {
        istr >> tmpstr;
        if (istr.fail()) {
            if (f == 1) {       //首行包含不完全的/*注释
                FAIL_ANALYSE_FIRSTLINE;
            }
            break;  //否则说明正常读取完了
        }

        while (!tmpstr.empty()) {
            if (f == 0) {
                int is1 = 0, is2 = 0;
                unsigned long long pos1(tmpstr.find("/*")), pos2(tmpstr.find("//"));    //64位系统的 npos 是64位……
                if ((pos1 != tmpstr.npos && ++is1 && pos1 != 0) || (pos2 != tmpstr.npos && ++is2 && pos2 != 0) || (!is1 && !is2)) {    //出现非注释内容
                    FAIL_ANALYSE_FIRSTLINE;
                }
                f = (is1 ? 1 : 2);
                tmpstr = (tmpstr.c_str() + 2);    //删掉注释符号
            }
            else if (f == 1) {
                unsigned long long pos = tmpstr.find("*/");
                if (pos == tmpstr.npos) {       //注释里面的字符串
                    toks.push(tmpstr);
                    tokcnt++;
                    break;
                }
                tmpstr[(unsigned int)pos] = 0;
                string tmptok(tmpstr.c_str());
                tmpstr[(unsigned int)pos] = '*';
                tmpstr = (tmpstr.c_str() + pos + 2);    //消除注释符号以及前面被注释了的东西
                f = 0;  //恢复 f
                if (!tmptok.empty()) {      //注释里面的字符串
                    toks.push(tmptok);
                    tokcnt++;
                }
            }
            else {      //注释里面的字符串
                toks.push(tmpstr);
                tokcnt++;
                break;
            }
        }
    }

    if (f == 1) {   //该行内还有未匹配的注释符号
        FAIL_ANALYSE_FIRSTLINE;
    }
#undef FAIL_ANALYSE_FIRSTLINE

    //检查首行
    if (tokcnt < 3) {
        ret = "首行信息不全";
    }
    else if (tokcnt > 3) {
        ret = "首行信息过多";
    }
    else {
        int flag = 0;
        if (toks.find(stuinfo.number) == NULL) {
            flag++;
            ret += "学号不匹配 ";
        }
        if (toks.find(stuinfo.name) == NULL) {
            flag++;
            ret += "姓名不匹配 ";
        }
        if (toks.find(stuinfo.course_full) == NULL && toks.find(stuinfo.course_abbr) == NULL) {
            flag++;
            ret += "班级不匹配 ";
        }

        if (flag == 0) {
            ret = "通过";
        }
    }

    toks.release();     //重要
    return ret;
}









/*
void test_node()  //用于测试 node
{
    node<long long> A;
    int cnt = 0;
    getchar();
    for (long long i = 1; i <= 1000000; i++) {
        if (i % (1024 * 1024) == 0) {
            cout << ++cnt << endl;
        }
        A.push(i);
    }
    puts("done");
    getchar();
    A.release();
    puts("end");
}
*/