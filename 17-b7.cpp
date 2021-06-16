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

#if defined(__linux) || defined(__linux__)
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
//#include <sys/io.h>
#else
#include <io.h>
#endif

using namespace std;

//���ִ���ʱ���õ��жϺ���
void haltErr(const string &errinfo = "", int errcode = -1)
{
    cerr << errinfo << endl;
    exit(errcode);
}

//ֻ���� .cpp/.c/.h �ļ�
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


//ͨ������ڵ�(��Ҫ�Լ���ͷ�ڵ�release())
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


//һ�β��Ե���Ϣ
struct testdata {
    string courseid;        //�κ�
    string filename;        //����ļ����ƣ���Ϊ all ����˼�Ǹÿκ��������ļ�
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
            cerr << "����: û�в��� --firstline" << endl;
            return false;
        }
        if (!use_courseid) {
            cerr << "����: û�в��� --courseid" << endl;
            return false;
        }
        if (!use_filename) {
            cerr << "����: û�в��� --filename" << endl;
            return false;
        }
        if (filename != "all" && !goodtype(filename.c_str())) {
            cerr << "����: " << filename << " ���ǿɽ��ܵ�Դ�����ļ�" << endl;
            return false;
        }
        return true;
    }
};


//һ��ѧ������Ϣ
struct studata {
    string number;          //ѧ��
    string name;            //����
    string course_full;      //�༶ȫ��
    string course_abbr;      //�༶���

    friend ostream &operator<<(ostream &out, const studata &stu)
    {
        out << (stu.number + "-" + stu.name + "-" + stu.course_full);
        return out;
    }
};


//һ���༶����Ϣ
struct coursedata {
    string courseid;
    node<studata> stu_head;
    ~coursedata()
    {
        stu_head.release();     //��Ҫ
    }
};



void getTestdata(int argc, const char *argv[], testdata &testinfo);     //ʶ�� main �����������浽 testinfo ��

void getCoursedata(string filepath, coursedata &courseinfo);      //��·�� filepath ��ָ�Ŀγ�ѧ���ļ���ȡ courseinfo ��Ϣ���ÿγ����ѧ����Ϣ�����浽 courseinfo ��

void searchAllFiles(string path, const testdata &testinfo, const studata &stuinfo);     //�����ļ����������ļ�

void touchOneFile(const string &path, const string &fname, const testdata &testinfo, const studata &stuinfo);      //�������ʽ�����ü�麯���Լ���ʽ������������stdout

string testFirstline(string filepath, const studata &stuinfo);        //������У����ؼ�����ַ���

bool cmp(const studata &X, const studata &Y)    //ѧ����ѧ������С����ǰ
{
    return X.number < Y.number;
}

bool str_is_blank(const string &str)   //����str�Ƿ�ȫ�ɿհ��ַ����
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

    //�����Ѿ���֤ testinfo �� courseinfo ���涼��������ֵ

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

        if (pFile->d_type & DT_DIR) {   //�ļ���
            if (strcmp(pFile->d_name, ".") != 0 && strcmp(pFile->d_name, "..") != 0) {
                searchAllFiles(path + "/" + pFile->d_name, testinfo, stuinfo);
            }
        }
        else {      //�ļ�
            touchOneFile(path, pFile->d_name, testinfo, stuinfo);
            flag = 1;
        }

    }

    flag ? (cout << endl) : cout;      //���м����ļ������ں�����һ������
    closedir(pDir);  //��������
#else
    long hFile = 0; //�ļ���Ϣ
    _finddata_t fileinfo;    //�洢�ļ���Ϣ�Ľṹ��
    string p;

    int flag = 0;   //���ڼ�¼�Ƿ��м����ļ�

    if ((hFile = _findfirst(p.assign(path).append("/*").c_str(), &fileinfo)) != -1) {  //��һ�β���

        do {

            if ((fileinfo.attrib & _A_SUBDIR)) {    //���ҵ������ļ���

                if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) {

                    searchAllFiles(p.assign(path).append("/").append(fileinfo.name), testinfo, stuinfo);      //�ݹ�����ļ���

                }

            }
            else {  //���ҵ��Ĳ����ļ���

                touchOneFile(path, fileinfo.name, testinfo, stuinfo);
                flag = 1;

            }

        } while (_findnext(hFile, &fileinfo) == 0);

    }

    flag ? (cout << endl) : cout;      //���м����ļ������ں�����һ������
    _findclose(hFile);  //��������
#endif
}




void getTestdata(int argc, const char *argv[], testdata &testinfo)
{
    string USAGE_STR =
        "\n\nUsage: " + string(argv[0]) + " --firstline | --courseid cid | --filename name / all\n" +
        "  --firstline : ���м��\n" +
        "  --courseid  : �κ�\n" +
        "  --filename  : �ļ���(all��ʾȫ��)\n" +
        "\n" +
        "e.g.   " + string(argv[0]) + " --firstline --courseid 100692 --filename 3-b1.cpp : ���100692�γ̵�3-b1.cpp������\n" +
        "       " + string(argv[0]) + " --firstline --courseid 100692 --filename all : ���100692�γ̵�ȫ���ļ�������";

    testinfo.init();    //����λ����

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
                haltErr("����courseidֵ����\n" + USAGE_STR);
            }
        }
        else if (strcmp(argv[i], "--filename") == 0) {
            if (++i < argc) {
                testinfo.filename = argv[i];
                testinfo.use_filename = 1;
            }
            else {
                haltErr("����filenameֵ����\n" + USAGE_STR);
            }
        }
        else {
            haltErr(string("δ֪�Ĳ��� ") + argv[i]);
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
        if (str_is_blank(tmpline))  //�հ���������
            continue;
        istringstream istr(tmpline);

        istr >> tmpstu.number >> tmpstu.name >> tmpstu.course_full >> tmpstu.course_abbr;

        if (istr.fail() || !((istr >> tmpstr).fail())) {  //dat�ļ�ĳһ����Ϣ��������
            haltErr(filepath + "���д�( " + tmpline + ")");
        }
        courseinfo.stu_head.push(tmpstu);   //��������
    }

    courseinfo.stu_head.sort(cmp);  //��ѧ��С��������

    fin.close();
}

void touchOneFile(const string &path, const string &fname, const testdata &testinfo, const studata &stuinfo)
{
    string filepath = path + "/" + fname;
    if (!goodtype(filepath.c_str())) {
        return;     //������Ҫ�����ļ����Ͳ����κβ���
    }

    cout << stuinfo << " : " << fname << " : " << testFirstline(filepath, stuinfo) << endl;
}

string testFirstline(string filepath, const studata &stuinfo)
{
    ifstream fin(filepath);
    if (!fin.is_open()) {       //�Ҳ����ļ�
        return "δ�ύ";
    }

    string strline;
    while (getline(fin, strline).good() && str_is_blank(strline))
        continue;

    fin.close();    //��Ҫ

    if (str_is_blank(strline)) {    //�Ǵ��հ��ַ��ļ�
        return "�������ļ����ң����ɶ��";
    }

    istringstream istr(strline);

    node<string> toks;
    int tokcnt = 0;
    string tmpstr;
    string ret;

    //�������У������ֶ���/**/��ֻ�����һ�飩
#define FAIL_ANALYSE_FIRSTLINE toks.release(); return "δȡ��������Ϣ"
    int f = 0; //0:ǰ��Ϊ��  1:ǰ����/*  2:ǰ����//
    while (1) {
        istr >> tmpstr;
        if (istr.fail()) {
            if (f == 1) {       //���а�������ȫ��/*ע��
                FAIL_ANALYSE_FIRSTLINE;
            }
            break;  //����˵��������ȡ����
        }

        while (!tmpstr.empty()) {
            if (f == 0) {
                int is1 = 0, is2 = 0;
                unsigned long long pos1(tmpstr.find("/*")), pos2(tmpstr.find("//"));    //64λϵͳ�� npos ��64λ����
                if ((pos1 != tmpstr.npos && ++is1 && pos1 != 0) || (pos2 != tmpstr.npos && ++is2 && pos2 != 0) || (!is1 && !is2)) {    //���ַ�ע������
                    FAIL_ANALYSE_FIRSTLINE;
                }
                f = (is1 ? 1 : 2);
                tmpstr = (tmpstr.c_str() + 2);    //ɾ��ע�ͷ���
            }
            else if (f == 1) {
                unsigned long long pos = tmpstr.find("*/");
                if (pos == tmpstr.npos) {       //ע��������ַ���
                    toks.push(tmpstr);
                    tokcnt++;
                    break;
                }
                tmpstr[(unsigned int)pos] = 0;
                string tmptok(tmpstr.c_str());
                tmpstr[(unsigned int)pos] = '*';
                tmpstr = (tmpstr.c_str() + pos + 2);    //����ע�ͷ����Լ�ǰ�汻ע���˵Ķ���
                f = 0;  //�ָ� f
                if (!tmptok.empty()) {      //ע��������ַ���
                    toks.push(tmptok);
                    tokcnt++;
                }
            }
            else {      //ע��������ַ���
                toks.push(tmpstr);
                tokcnt++;
                break;
            }
        }
    }

    if (f == 1) {   //�����ڻ���δƥ���ע�ͷ���
        FAIL_ANALYSE_FIRSTLINE;
    }
#undef FAIL_ANALYSE_FIRSTLINE

    //�������
    if (tokcnt < 3) {
        ret = "������Ϣ��ȫ";
    }
    else if (tokcnt > 3) {
        ret = "������Ϣ����";
    }
    else {
        int flag = 0;
        if (toks.find(stuinfo.number) == NULL) {
            flag++;
            ret += "ѧ�Ų�ƥ�� ";
        }
        if (toks.find(stuinfo.name) == NULL) {
            flag++;
            ret += "������ƥ�� ";
        }
        if (toks.find(stuinfo.course_full) == NULL && toks.find(stuinfo.course_abbr) == NULL) {
            flag++;
            ret += "�༶��ƥ�� ";
        }

        if (flag == 0) {
            ret = "ͨ��";
        }
    }

    toks.release();     //��Ҫ
    return ret;
}









/*
void test_node()  //���ڲ��� node
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