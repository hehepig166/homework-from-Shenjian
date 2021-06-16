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


//ͨ������ڵ�
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


//һ�β��Ե���Ϣ
struct testdata {
    string courseid;        //�κ�
    string filename;        //����ļ����ƣ���Ϊ all ����˼�Ǹÿκ��������ļ�
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
        if (filename != "all" && goodtype(filename.c_str())) {
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
};


//һ���༶����Ϣ
struct coursedata {
    string courseid;
    node<studata> stu_head;
};



void getTestdata(int argc, const char *argv[], testdata &testinfo);     //ʶ�� main �����������浽 testinfo ��

void getCoursedata(string filepath, coursedata &courseinfo);      //��·�� filepath ��ָ�Ŀγ�ѧ���ļ���ȡ courseinfo ��Ϣ���ÿγ����ѧ����Ϣ�����浽 courseinfo ��

void searchAllFiles(string path, const testdata &testinfo, const studata &stuinfo);     //�����ļ����������ļ�

void touchOneFile(string filepath, const testdata &testinfo, const studata &stuinfo);      //�ж�·�� filpath ��ָ�ļ��Ƿ�Ҫ��飬���ļ��Ƿ���ڡ��ļ���ʽ�Ƿ�Ҫ���ȣ��ù��ɵ������ļ��ĸ�ʽ��麯��

void testFirstline(string filepath, const studata &stuinfo);        //������У������������� stdout

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

    //�����Ѿ���֤ testinfo �� courseinfo ���涼��������ֵ

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
    long hFile = 0; //�ļ���Ϣ
    _finddata_t fileinfo;    //�洢�ļ���Ϣ�Ľṹ��
    string p;

    if ((hFile = _findfirst(p.assign(path).append("/*").c_str(), &fileinfo)) != -1) {  //��һ�β���

        do {

            if ((fileinfo.attrib & _A_SUBDIR)) {    //���ҵ������ļ���

                if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) {

                    searchAllFiles(p.assign(path).append("/").append(fileinfo.name), testinfo, stuinfo);      //�ݹ�����ļ���

                }

            }
            else {  //���ҵ��Ĳ����ļ���

                touchOneFile(path + "/" + fileinfo.name, testinfo, stuinfo);

            }

        } while (_findnext(hFile, &fileinfo) == 0);

    }

    _findclose(hFile);  //��������
}




void getTestdata(int argc, const char *argv[], testdata &testinfo)
{
    string USAGE_STR =
        "Usage: " + string(argv[0]) + " --firstline | --courseid cid | --filename name / all\n" +
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
        if (str_is_blank(tmpline))  //�հ���������
            continue;
        istringstream istr(tmpline);

        istr >> tmpstu.number >> tmpstu.name >> tmpstu.course_full >> tmpstu.course_abbr;

        if (istr.fail() || !istr.eof()) {  //dat�ļ�ĳһ����Ϣ��������
            haltErr(filepath + "���д�( " + tmpline + ")");
        }
        courseinfo.stu_head.push(tmpstu);   //��������
    }

    courseinfo.stu_head.sort(cmp);  //��ѧ��С��������

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