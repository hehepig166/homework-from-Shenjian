/* 朱泽凯 2051995 济美 */
#define _CRT_SECURE_NO_WARNINGS

//#define DEBUG_HEHEPIG
/****
*
* 分析 main 参数
*
* 根据参数读取学生名单
*
* 在聊天记录文件中一行行读
*   若匹配 时间 学生 成功，则在学生名单中匹配该学生
*       若匹配成功，则为该学生加一
*
* 不好处理，所以还是在各种函数一旦出错，就调用 exit_err 中止进程
* 
* 注意，导出文件中，有可能是两种情况
*   2021-06-04 13:00:46 1953080-计科-田宇<renxin-ty@qq.com>
*   2021-06-04 12:40:02 1853659-计科-叶茂尧(1974330833)
*
****/

#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <iomanip>
using namespace std;

#define GOOD (0)
#define FAIL (1)
#define WRONG (1)
#define NO_ENOUGH_MEMORY (2)
#define WRONG_IN_FILE_STULIST (4)
#define WRONG_IN_FILE_QQRECORD (8)
#define UNKNOWN_VAR (16)

const char *default_fname_stulist = "stulist.txt";
const char *default_fname_qq_record = "2020级-高程[计荣].txt";

void print_usage();

void exit_err(const char *debugstr = NULL, int code = FAIL)
{
    print_usage();
    if (debugstr != NULL)
        cout << debugstr << endl;
    exit(code);
}





class qtime {
public:
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;

    qtime() :year(-1) {}
    //qtime(const char *str); // "yy-mm-dd hh-mm-ss"

    friend istream &operator>> (istream &in, qtime &T);
    bool operator< (const qtime &T) const
    {
#define __time_check(xx) if (xx!=T.xx) return xx<T.xx

        __time_check(year);
        __time_check(month);
        __time_check(day);
        __time_check(hour);
        __time_check(minute);
        __time_check(second);

#undef __time_check

        return true;
    }

    bool good()   //检查时间是否合法
    {
        int ddd[13] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };
        if (year < 0)
            return false;

        ddd[2] += (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0));
        if (month < 0 || month>12)
            return false;

        if (day<0 || day>ddd[month])
            return false;

        if (hour < 0 || hour>23 || minute < 0 || minute>59 || second < 0 || second>59)
            return false;

        return true;
    }
};

class student {
public:
    string number;
    string name;
    string class_name;
    int cnt;
    //bool good;

    //student() :good(true), cnt(0) {}
     student() :cnt(0) {}
    //student(const string &number_, const string &name_, const string class_name_) : good(true), number(number_), name(name_), class_name(class_name_), cnt(0) {}
     student(const string &number_, const string &name_, const string class_name_) : number(number_), name(name_), class_name(class_name_), cnt(0) {}

     /*
    friend istream &operator>> (istream &in, student &stu)
    {
        int tmpcnt = 0;
        char tmpnum[50] = "", tmpclass[50] = "", tmpname[50] = "";
        string tmpstr;
        in >> tmpstr;
        if ((tmpcnt = sscanf(tmpstr.c_str(), "%s-%s-%s(", tmpnum, tmpclass, tmpname)) == 3) {
            stu.number = tmpnum;
            stu.class_name = tmpclass;
            stu.name = tmpname;
            stu.good = true;
        }
        else {
            stu.number = "~";
            stu.good = false;
        }
        return in;
    }
    */

    bool operator==(const student &B) const
    {
        return (number == B.number && name == B.name && class_name == B.class_name);
    }

    bool operator<(const student &B) const
    {
        return this->number < B.number;
    }
};

class studentset {
protected:
    int _size;
    int _cnt;
    student *data;

public:
    const int IDfail = -1;

    studentset()
    {
        _size = 1;
        _cnt = 0;
        data = new(nothrow) student[_size];
        if (data == NULL)
            exit_err("no memory", NO_ENOUGH_MEMORY);
    };

    ~studentset()
    {
        delete[]data;
    };

    student &operator[](int index)
    {
        return data[index];
    }

    int size() const
    {
        return _cnt;
    }

    int push(const student stu)
    {
        if (_size <= _cnt) {    //扩容
            student *tmp = new(nothrow) student[_size += 10];
            if (tmp == NULL) {
                cout << "no memory" << endl;
                return FAIL;    //这里不exit了
            }
            for (int i = 0; i < _cnt; i++)
                tmp[i] = data[i];
            delete[]data;
            data = tmp;
        }

        data[_cnt++] = stu;     //添加

        return GOOD;
    }

    void sort()
    {
        for (int i = 0; i < _cnt; i++) {
            int maxid = i;
            for (int j = i + 1; j < _cnt; j++)
                if (data[j] < data[maxid])
                    maxid = j;
            swap(data[i], data[maxid]);
        }
    }

    //返回编号，若没找到，则返回 IDfail
    int find(const student &stu) const
    {
        int ret = IDfail;
        int L = 0, R = _cnt - 1, M;

        while (L <= R) {
            M = (L + R) >> 1;
            if (data[M] < stu) {
                L = M + 1;
            }
            else if (stu < data[M]) {
                R = M - 1;
            }
            else {
                ret = (data[M] == stu) ? M : IDfail;
                break;
            }
        }

        return ret;
    }
};

class main_arg {
public:
    int use_stulist;
    int use_qq_record;
    int use_begin;
    int use_end;
    string fname_stulist;
    string fname_qq_record;
    qtime time_begin;
    qtime time_end;

    int use_stuno;
    int use_only_zero;
    int use_more_than_zero;
    string stuno;

    void init()
    {
        use_stulist = use_qq_record = use_begin = use_end = 0;
        use_stuno = use_only_zero = use_more_than_zero = 0;
    }

};










//检查并将 main 参数存到 marg 中
int analyze_main(main_arg &marg, int argc, const char *argv[]);

//根据 marg 中的要求，从学生信息文件中读取班级学生信息到 stuset 中
int get_stuset(main_arg &marg, studentset &stuset);

//判断并分析一行，若符合格式，则将识别出的时间和学生信息存到 tm 和 stu 中
bool analyze_line(const char *strline, qtime &tm, student &stu);

//打开聊天记录文件并根据 marg 中的要求进行统计，统计结果更新在 stulist 中
int analyze_qqlines(const main_arg &marg, studentset &stuset);

//输出
int print_stuset(const main_arg &marg, studentset &stuset);

int main(const int argc, const char *argv[])
{

#ifdef DEBUG_HEHEPIG
    for (int i = 0; i < argc; i++) {
        cout << i << "\t " << "[" << argv[i] << "]" << endl;
    }
    cout << endl;
#endif


    main_arg marg;
    studentset stuset;

    analyze_main(marg, argc, argv);

    get_stuset(marg, stuset);

    stuset.sort();

#ifdef DEBUG_HEHEPIG
    for (int i = 0, mi = stuset.size(); i < mi; i++)
        cout << setw(3) << i << " " << stuset[i].number << " " << setw(8) << stuset[i].name << " " << stuset[i].class_name << endl;
    cout << endl;
#endif

    analyze_qqlines(marg, stuset);

    print_stuset(marg, stuset);

    return 0;
}


















bool analyze_line(const char *strline, qtime &tm, student &stu)
{
    qtime curtime;
    char qqname[50];
    string tmpstuinfo[3];
    int cnt(0), cntstuinfo(0);

    cnt = sscanf(strline, "%d-%d-%d %d:%d:%d %s",
        &curtime.year, &curtime.month, &curtime.day, &curtime.hour, &curtime.minute, &curtime.second, qqname);
    if (cnt != 7 || !curtime.good())
        return false;
    tm = curtime;

    for (char *p = qqname; *p; p++) {
        if (*p == '-') {
            if (++cntstuinfo >= 3)
                return false;
        }
        else if (*p == '(' || *p == '<') {
            break;
        }
        else {
            tmpstuinfo[cntstuinfo] += *p;
        }
    }
    if (cntstuinfo != 2)
        return false;

    stu.number = tmpstuinfo[0];
    stu.class_name = tmpstuinfo[1];
    stu.name = tmpstuinfo[2];

    return true;
}


int analyze_qqlines(const main_arg &marg, studentset &stuset)
{
    ifstream fin(marg.fname_qq_record, ios::in);
    if (!fin.is_open())
        exit_err(string("打开聊天记录文件 ").append(marg.fname_qq_record).append(" 失败").c_str(), WRONG_IN_FILE_QQRECORD);

    string strline;
    student tmpstu;
    qtime tmptm;
    while (getline(fin, strline).good()) {
        if (analyze_line(strline.c_str(), tmptm, tmpstu)) {

            if (marg.time_end < tmptm) {
                break;
            }
            else if (tmptm < marg.time_begin) {
                continue;
            }
            else {
                int id = stuset.find(tmpstu);
                if (id != stuset.IDfail)
                    stuset[id].cnt++;
            }
        }
    }

    fin.close();    //重要
    return GOOD;
}



int get_stuset(main_arg &marg, studentset &stuset)
{

    ifstream fin(marg.fname_stulist, ios::in);
    if (!fin.is_open())
        exit_err(string("打开学生信息文件 ").append(marg.fname_stulist).append(" 失败").c_str(), WRONG_IN_FILE_STULIST);

    if (!marg.use_stuno) {   //所有学生

        student tmpstu;
        while ((fin >> tmpstu.number >> tmpstu.name >> tmpstu.class_name).good()) {
            stuset.push(tmpstu);
        }
    }
    else {      //单个学生
        student tmpstu;
        while ((fin >> tmpstu.number >> tmpstu.name >> tmpstu.class_name).good()) {
            if (tmpstu.number == marg.stuno) {
                stuset.push(tmpstu);
                break;
            }
        }
    }

#ifdef DEBUG_HEHEPIG
    cout << "\n读取到的学生人数 " << stuset.size() << endl;
#endif

    fin.close();    //重要
    return GOOD;
}






int analyze_main(main_arg &marg, int argc, const char *argv[])
{
    marg.init();

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--stulist") == 0) {
            if (++i < argc)
                marg.fname_stulist = argv[i];
            else
                exit_err("错误: 参数 [--stulist] 信息不够");

            if (++marg.use_stulist > 1)
                exit_err("错误: 参数 [--stulist] 重复");
        }
        else if (strcmp(argv[i], "--qq_record") == 0) {
            if (++i < argc)
                marg.fname_qq_record = argv[i];
            else
                exit_err("错误: 参数 [--qq_record] 信息不够");

            if (++marg.use_qq_record > 1)
                exit_err("错误: 参数 [--qq_record] 重复");
        }
        else if (strcmp(argv[i], "--begin") == 0) {
            if (++i < argc) {
                qtime &tt = marg.time_begin;
                if (sscanf(argv[i], "%d-%d-%d %d:%d:%d", &tt.year, &tt.month, &tt.day, &tt.hour, &tt.minute, &tt.second) != 6)
                    exit_err("错误: [--begin] 格式错误");
                if (!tt.good())
                    exit_err("错误: [--begin] 格式错误(范围错了)");
            }
            else
                exit_err("错误: 参数 [--begin] 信息不够");

            if (++marg.use_begin > 1)
                exit_err("错误: 参数 [--begin] 重复");
        }
        else if (strcmp(argv[i], "--end") == 0) {
            if (++i < argc) {
                qtime &tt = marg.time_end;
                if (sscanf(argv[i], "%d-%d-%d %d:%d:%d", &tt.year, &tt.month, &tt.day, &tt.hour, &tt.minute, &tt.second) != 6)
                    exit_err("错误: [--end] 格式错误");
                if (!tt.good())
                    exit_err("错误: [--end] 格式错误(范围错了)");
            }
            else
                exit_err("错误: 参数 [--end] 信息不够");

            if (++marg.use_end > 1)
                exit_err("错误: 参数 [--end] 重复");
        }
        else if (strcmp(argv[i], "--stuno") == 0) {
            if (++i < argc)
                marg.stuno = argv[i];
            else
                exit_err("错误: 参数 [--stuno] 信息不够");

            if (++marg.use_stuno > 1)
                exit_err("错误: 参数 [--stuno] 重复");
        }
        else if (strcmp(argv[i], "--only_zero") == 0) {
            if (++marg.use_only_zero > 1)
                exit_err("错误: 参数 [--only_zero] 重复");
        }
        else if (strcmp(argv[i], "--more_than_zero") == 0) {
            if (++marg.use_more_than_zero > 1)
                exit_err("错误: 参数 [--more_than_zero] 重复");
        }
        else {
            exit_err(string("错误: 未知参数 [").append(argv[i]).append("]").c_str(), UNKNOWN_VAR);
        }
    }

    if (!marg.use_stulist && ++marg.use_stulist)
        marg.fname_stulist = default_fname_stulist;
    if (!marg.use_qq_record && ++marg.use_qq_record)
        marg.fname_qq_record = default_fname_qq_record;

    if (marg.use_stuno + marg.use_only_zero + marg.use_more_than_zero > 1) {
        string tmpstr;

        tmpstr += (marg.use_stuno) ? " [--stuno]" : "";
        tmpstr += (marg.use_only_zero) ? " [--only_zero]" : "";
        tmpstr += (marg.use_more_than_zero) ? " [--more_than_zero]" : "";
        exit_err(string("错误: 参数冲突 {").append(tmpstr).append(" }").c_str());
    }

    if (!(marg.use_stulist && marg.use_qq_record && marg.use_begin && marg.use_end)) {
        exit_err();
    }

    return GOOD;
}





void print_usage()
{
    puts("\
\n\
Usage: 17-b8 { --stulist | --qq_record | --begin | --end | {--stuno} | {--only_zero} }\n\
\n\
                  必选项：\n\
                     --stulist filename   : 学生列表文件名(缺省:stulist.txt)\n\
                     --qq_record filename : 导出的聊天记录文件名(缺省:2020级-高程[计荣].txt)\n\
                     --begin timestr      : 起始时间(时间串格式为yyyy-mm-dd hh:mm:ss，加引号)\n\
                     --end timestr        : 结束时间(时间串格式为yyyy-mm-dd hh:mm:ss，加引号)\n\
                     注：未检查时间串正确性\n\
                  可选项：\n\
                     --stuno stuno        : 指定某个学生(无此选项则全部学生)\n\
                     --only_zero          : 仅显示0发言学生(指定--stuno时本项无效)\n\
                     --more_than_zero     : 仅显示有发言学生(指定--stuno时本项无效，且与--only_zero互斥)\n\
\n\
e.g.  17-b8 --begin \"2021-06-04 10:00:00\" --end \"2021-06-04 11:55:00\"\n\
                               : 统计\"2020级-高程[计荣].txt\"中在stulist.txt列表中的学生在2021.6.4 10:00~11:55间的发言数量\n\
      17-b8 --begin \"2021-06-04 10:00:00\" --end \"2021-06-04 11:55:00\" --only_zero\n\
                               : 统计\"2020级-高程[计荣].txt\"中在stulist.txt列表中的学生在2021.6.4 10:00~11:55间0发言数量\n\
    ");
}


int print_stuset(const main_arg &marg, studentset &stuset)
{
    if (marg.use_only_zero) {
        cout << "名单                     发言次数" << endl;
        for (int i = 0, mi = stuset.size(); i < mi; i++) {
            if (stuset[i].cnt == 0) {
                string tmpstr = stuset[i].number + "-" + stuset[i].class_name + "-" + stuset[i].name;
                cout << setw(25) << left << tmpstr << stuset[i].cnt << endl;
            }
        }
    }
    else if (marg.use_more_than_zero) {
        cout << "名单                     发言次数" << endl;
        for (int i = 0, mi = stuset.size(); i < mi; i++) {
            if (stuset[i].cnt > 0) {
                string tmpstr = stuset[i].number + "-" + stuset[i].class_name + "-" + stuset[i].name;
                cout << setw(25) << left << tmpstr << stuset[i].cnt << endl;
            }
        }
    }
    else {
        cout << "名单                     发言次数" << endl;
        for (int i = 0, mi = stuset.size(); i < mi; i++) {
            string tmpstr = stuset[i].number + "-" + stuset[i].class_name + "-" + stuset[i].name;
            cout << setw(25) << left << tmpstr << stuset[i].cnt << endl;
        }
    }

    return GOOD;
}