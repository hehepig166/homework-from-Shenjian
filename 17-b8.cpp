/* ���� 2051995 ���� */
#define _CRT_SECURE_NO_WARNINGS

//#define DEBUG_HEHEPIG
/****
*
* ���� main ����
*
* ���ݲ�����ȡѧ������
*
* �������¼�ļ���һ���ж�
*   ��ƥ�� ʱ�� ѧ�� �ɹ�������ѧ��������ƥ���ѧ��
*       ��ƥ��ɹ�����Ϊ��ѧ����һ
*
* ���ô������Ի����ڸ��ֺ���һ�������͵��� exit_err ��ֹ����
* 
* ע�⣬�����ļ��У��п������������
*   2021-06-04 13:00:46 1953080-�ƿ�-����<renxin-ty@qq.com>
*   2021-06-04 12:40:02 1853659-�ƿ�-ҶïҢ(1974330833)
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
const char *default_fname_qq_record = "2020��-�߳�[����].txt";

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

    bool good()   //���ʱ���Ƿ�Ϸ�
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
        if (_size <= _cnt) {    //����
            student *tmp = new(nothrow) student[_size += 10];
            if (tmp == NULL) {
                cout << "no memory" << endl;
                return FAIL;    //���ﲻexit��
            }
            for (int i = 0; i < _cnt; i++)
                tmp[i] = data[i];
            delete[]data;
            data = tmp;
        }

        data[_cnt++] = stu;     //���

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

    //���ر�ţ���û�ҵ����򷵻� IDfail
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










//��鲢�� main �����浽 marg ��
int analyze_main(main_arg &marg, int argc, const char *argv[]);

//���� marg �е�Ҫ�󣬴�ѧ����Ϣ�ļ��ж�ȡ�༶ѧ����Ϣ�� stuset ��
int get_stuset(main_arg &marg, studentset &stuset);

//�жϲ�����һ�У������ϸ�ʽ����ʶ�����ʱ���ѧ����Ϣ�浽 tm �� stu ��
bool analyze_line(const char *strline, qtime &tm, student &stu);

//�������¼�ļ������� marg �е�Ҫ�����ͳ�ƣ�ͳ�ƽ�������� stulist ��
int analyze_qqlines(const main_arg &marg, studentset &stuset);

//���
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
        exit_err(string("�������¼�ļ� ").append(marg.fname_qq_record).append(" ʧ��").c_str(), WRONG_IN_FILE_QQRECORD);

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

    fin.close();    //��Ҫ
    return GOOD;
}



int get_stuset(main_arg &marg, studentset &stuset)
{

    ifstream fin(marg.fname_stulist, ios::in);
    if (!fin.is_open())
        exit_err(string("��ѧ����Ϣ�ļ� ").append(marg.fname_stulist).append(" ʧ��").c_str(), WRONG_IN_FILE_STULIST);

    if (!marg.use_stuno) {   //����ѧ��

        student tmpstu;
        while ((fin >> tmpstu.number >> tmpstu.name >> tmpstu.class_name).good()) {
            stuset.push(tmpstu);
        }
    }
    else {      //����ѧ��
        student tmpstu;
        while ((fin >> tmpstu.number >> tmpstu.name >> tmpstu.class_name).good()) {
            if (tmpstu.number == marg.stuno) {
                stuset.push(tmpstu);
                break;
            }
        }
    }

#ifdef DEBUG_HEHEPIG
    cout << "\n��ȡ����ѧ������ " << stuset.size() << endl;
#endif

    fin.close();    //��Ҫ
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
                exit_err("����: ���� [--stulist] ��Ϣ����");

            if (++marg.use_stulist > 1)
                exit_err("����: ���� [--stulist] �ظ�");
        }
        else if (strcmp(argv[i], "--qq_record") == 0) {
            if (++i < argc)
                marg.fname_qq_record = argv[i];
            else
                exit_err("����: ���� [--qq_record] ��Ϣ����");

            if (++marg.use_qq_record > 1)
                exit_err("����: ���� [--qq_record] �ظ�");
        }
        else if (strcmp(argv[i], "--begin") == 0) {
            if (++i < argc) {
                qtime &tt = marg.time_begin;
                if (sscanf(argv[i], "%d-%d-%d %d:%d:%d", &tt.year, &tt.month, &tt.day, &tt.hour, &tt.minute, &tt.second) != 6)
                    exit_err("����: [--begin] ��ʽ����");
                if (!tt.good())
                    exit_err("����: [--begin] ��ʽ����(��Χ����)");
            }
            else
                exit_err("����: ���� [--begin] ��Ϣ����");

            if (++marg.use_begin > 1)
                exit_err("����: ���� [--begin] �ظ�");
        }
        else if (strcmp(argv[i], "--end") == 0) {
            if (++i < argc) {
                qtime &tt = marg.time_end;
                if (sscanf(argv[i], "%d-%d-%d %d:%d:%d", &tt.year, &tt.month, &tt.day, &tt.hour, &tt.minute, &tt.second) != 6)
                    exit_err("����: [--end] ��ʽ����");
                if (!tt.good())
                    exit_err("����: [--end] ��ʽ����(��Χ����)");
            }
            else
                exit_err("����: ���� [--end] ��Ϣ����");

            if (++marg.use_end > 1)
                exit_err("����: ���� [--end] �ظ�");
        }
        else if (strcmp(argv[i], "--stuno") == 0) {
            if (++i < argc)
                marg.stuno = argv[i];
            else
                exit_err("����: ���� [--stuno] ��Ϣ����");

            if (++marg.use_stuno > 1)
                exit_err("����: ���� [--stuno] �ظ�");
        }
        else if (strcmp(argv[i], "--only_zero") == 0) {
            if (++marg.use_only_zero > 1)
                exit_err("����: ���� [--only_zero] �ظ�");
        }
        else if (strcmp(argv[i], "--more_than_zero") == 0) {
            if (++marg.use_more_than_zero > 1)
                exit_err("����: ���� [--more_than_zero] �ظ�");
        }
        else {
            exit_err(string("����: δ֪���� [").append(argv[i]).append("]").c_str(), UNKNOWN_VAR);
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
        exit_err(string("����: ������ͻ {").append(tmpstr).append(" }").c_str());
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
                  ��ѡ�\n\
                     --stulist filename   : ѧ���б��ļ���(ȱʡ:stulist.txt)\n\
                     --qq_record filename : �����������¼�ļ���(ȱʡ:2020��-�߳�[����].txt)\n\
                     --begin timestr      : ��ʼʱ��(ʱ�䴮��ʽΪyyyy-mm-dd hh:mm:ss��������)\n\
                     --end timestr        : ����ʱ��(ʱ�䴮��ʽΪyyyy-mm-dd hh:mm:ss��������)\n\
                     ע��δ���ʱ�䴮��ȷ��\n\
                  ��ѡ�\n\
                     --stuno stuno        : ָ��ĳ��ѧ��(�޴�ѡ����ȫ��ѧ��)\n\
                     --only_zero          : ����ʾ0����ѧ��(ָ��--stunoʱ������Ч)\n\
                     --more_than_zero     : ����ʾ�з���ѧ��(ָ��--stunoʱ������Ч������--only_zero����)\n\
\n\
e.g.  17-b8 --begin \"2021-06-04 10:00:00\" --end \"2021-06-04 11:55:00\"\n\
                               : ͳ��\"2020��-�߳�[����].txt\"����stulist.txt�б��е�ѧ����2021.6.4 10:00~11:55��ķ�������\n\
      17-b8 --begin \"2021-06-04 10:00:00\" --end \"2021-06-04 11:55:00\" --only_zero\n\
                               : ͳ��\"2020��-�߳�[����].txt\"����stulist.txt�б��е�ѧ����2021.6.4 10:00~11:55��0��������\n\
    ");
}


int print_stuset(const main_arg &marg, studentset &stuset)
{
    if (marg.use_only_zero) {
        cout << "����                     ���Դ���" << endl;
        for (int i = 0, mi = stuset.size(); i < mi; i++) {
            if (stuset[i].cnt == 0) {
                string tmpstr = stuset[i].number + "-" + stuset[i].class_name + "-" + stuset[i].name;
                cout << setw(25) << left << tmpstr << stuset[i].cnt << endl;
            }
        }
    }
    else if (marg.use_more_than_zero) {
        cout << "����                     ���Դ���" << endl;
        for (int i = 0, mi = stuset.size(); i < mi; i++) {
            if (stuset[i].cnt > 0) {
                string tmpstr = stuset[i].number + "-" + stuset[i].class_name + "-" + stuset[i].name;
                cout << setw(25) << left << tmpstr << stuset[i].cnt << endl;
            }
        }
    }
    else {
        cout << "����                     ���Դ���" << endl;
        for (int i = 0, mi = stuset.size(); i < mi; i++) {
            string tmpstr = stuset[i].number + "-" + stuset[i].class_name + "-" + stuset[i].name;
            cout << setw(25) << left << tmpstr << stuset[i].cnt << endl;
        }
    }

    return GOOD;
}