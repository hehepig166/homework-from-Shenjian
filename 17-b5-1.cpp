/* 2051995 ���� ���� */

//c++ -Wall -lncurses -o 17-b5 17-b5-1.cpp

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <iomanip>

using namespace std;


//������Ļ���������ʱ�������ֵĶ���
ostream &operator<<(ostream &out, const unsigned char &x)
{
    out << int(x);
    return out;
}
istream &operator>>(istream &in, unsigned char &x)
{
    int tmp;
    in >> tmp;
    x = (unsigned char)tmp;
    return in;
}


class TYPE_NAME {
public:
    char data[16] = { 0 };
    //Ҫ�Ƕ������ȴ���15�ģ��������� data[0]=0��Ȼ���ǳ����ﳤ����С��1�����ܲ�ͨ����
    TYPE_NAME() {}
    TYPE_NAME(int len)
    {
        data[len] = 0;
        while (len--)
            data[len] = '#';
    }
    friend istream &operator>>(istream &in, TYPE_NAME &X)
    {
        string tmp;
        in >> tmp;
        if (tmp.size() > 15)
            X.data[0] = 0;
        else
            strcpy(X.data, tmp.c_str());
        return in;
    }
    friend ostream &operator<<(ostream &out, TYPE_NAME &X)
    {
        out << X.data;
        return out;
    }
    operator int() const
    {
        return strlen(data);
    }
};

//start_addr �� 0 ��ʼ����
class ITEM_ALL {
public:
    int start_addr;
    string name;

    ITEM_ALL(int start_addr_, const string &name_) :start_addr(start_addr_), name(name_) {}
    virtual void print(const void *from) const = 0;
    virtual void modify(const void *from, const void *to) const = 0;
    virtual void ask_modify(void *target) const = 0;
};


template<typename T>
class ITEM :public ITEM_ALL {
public:
    T Min, Max;

    ITEM(int start_addr_, const string &name_, T Min_, T Max_) : ITEM_ALL(start_addr_, name_), Min(Min_), Max(Max_) {}

    virtual void print(const void *from) const
    {
        cout << *(T *)from;
    }
    virtual void modify(const void *from, const void *to) const
    {
        *(T *)to = *(const T *)from;
    }
    virtual void ask_modify(void *target) const
    {
        T tmp;
        cout << name << "����ǰֵ= ";
        print(target);
        cout << "��Χ[" << Min << ".." << Max << "]����������ֵ" << endl;
        putchar('>');
        putchar(' ');
        cin >> tmp;

        while (!cin.good() || tmp<Min || tmp>Max) {
            cin.clear();
            cin.ignore(1926, '\n');
            cout << "�Ƿ���" << name << "���úÿ�����Χ���ٴ�����" << endl;
            putchar('>');
            putchar(' ');
            cin >> tmp;
        }

        modify(&tmp, target);   //��ȡ�ɹ�����������
    }
};



#if 1
static const ITEM<TYPE_NAME> _tn[] = { {0,"����ǳ�",1,15} };
static const ITEM<unsigned char> _uc[] = {
    {44,"�ƶ��ٶ�",0,100}, {45,"�����ٶ�",0,100}, {46,"������Χ",0,100}, {52,"���ݶ�",0,100}, {53,"����",0,100},
    {54,"����",0,100}, {55,"�ȼ�",0,100}, {58,"����ħ��ֵ",0,100}, {59,"ħ���˺���",0,100}, {60,"������",0,100},
    {61,"ħ��������",0,100}, {62,"������",0,100}, {63,"����",0,100} };
static const ITEM<unsigned short> _us[] = {
    {16,"����ֵ",0,10000}, {18,"����ֵ",0,10000}, {20,"����",0,8192}, {22,"����",0,1024}, {48,"������",0,2000},
    {50,"������",0,2000}, {56,"ħ��ֵ",0,10000} };
static const ITEM<unsigned int> _uint[] = { {24,"��Ǯ����",0ll,100000000}, {28,"����ֵ",0,1000000} , {32,"����ֵ",0,1000000} };
static const ITEM<unsigned long long> _ull[] = { {36,"��Ϸ�ۼ�ʱ��",0,10000000000000000ll} };
static const ITEM_ALL *item_p[] = {
    &_tn[0],  &_us[0],  &_us[1], &_us[2], &_us[3],
    &_uint[0],&_uint[1], &_uint[2], &_ull[0], &_uc[0],
    &_uc[1], &_uc[2], &_us[4], &_us[5], &_uc[3],
    &_uc[4], &_uc[5], &_uc[6], &_us[6], &_uc[7],
    &_uc[8], &_uc[9], &_uc[10], &_uc[11], &_uc[12]
};
static const int item_cnt = sizeof(item_p) / sizeof(void *);
#endif


//�˴�Ϊʾ���������޸Ľṹ�����ƣ������޸Ľṹ���еĳ�Ա���ݣ�Ҫ��sizeof������64
struct demo {
    char data[64];
};


#if 1

void demo_show(const demo &D)
{
    for (int i = 0; i < item_cnt; i++) {
        cout << setw(20) << item_p[i]->name << "��";
        item_p[i]->print(D.data + item_p[i]->start_addr);
        cout << endl;
    }
}


/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ��������������ֻ�������һ��open��һ��read����Ϊ����������������close��
***************************************************************************/
int read()
{
    /* ��������ֻ������һ�� ifstream�����󣬲����������κ���ʽ��fstream/ifstream/ofstream������Ҳ������ʹ��C��ʽ���ļ����� */
    ifstream gfile;

    /* �ļ��򿪣�����Ҫ��Ϊ��
       1��Ҫ���Զ���ʽ�򿪣��򿪷�ʽ***����ָ��
       2��������open�⣬�����������ط��������ٳ���open  */
    gfile.open("game.dat", ios::binary | ios::in);

    /* ���к����������������������ļ�����ʾ����Ϸ���ֵ���ر��ļ��ȣ�������ú���
       ���У�ֻ������һ���Զ�ȡ64�ֽڵķ�����game.dat�����ݶ���***�����������ơ��ṹ����������ָ����
                 gfile.read(***, sizeof(demo));
    */
    if (!gfile.is_open()) {
        puts("���ļ� game.dat ʧ��");
        return -1;
    }
    gfile.seekg(0, ios::end);
    if (gfile.tellg() != 64) {
        cout << "data.dat ��С�쳣��Ӧ����64byte��ʵ����" << gfile.tellg() << "byte" << endl;
        return -1;
    }
    gfile.seekg(0, ios::beg);


    demo D;
    gfile.read((char *)&D, sizeof(demo));

    demo_show(D);

    gfile.close();
    return 0;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ��������������ֻ�������һ��open��һ��read��һ��write����Ϊ����������������close��
***************************************************************************/
int modify()
{
    /* ��������ֻ������һ�� fstream�����󣬲����������κ���ʽ��fstream/ifstream/ofstream������Ҳ������ʹ��C��ʽ���ļ����� */
    fstream gfile;

    /* �ļ��򿪣�����Ҫ��Ϊ��
       1��Ҫ���Զ�д��ʽ�򿪣��򿪷�ʽ***����ָ��
       2��������open�⣬�����������ط��������ٳ���open  */
    gfile.open("game.dat", ios::binary | ios::in | ios::out);

    /* ���к����������������������ļ�����ʾ����Ϸ���ֵ���ر��ļ��ȣ�������ú���
       ���У�ֻ������һ���Զ�ȡ64�ֽڵķ�����game.dat�����ݶ���***�����������ơ��ṹ����������ָ����
                 gfile.read(***, sizeof(demo));
             ֻ������һ����д��64�ֽڵķ�����***������д��game.dat�У����������ơ��ṹ����������ָ����
                 gfile.write(***, sizeof(demo));
    */
    if (!gfile.is_open()) {
        puts("���ļ� game.dat ʧ��");
        return -1;
    }
    gfile.seekg(0, ios::end);
    if (gfile.tellg() != 64) {
        cout << "data.dat ��С�쳣��Ӧ����64byte��ʵ����" << gfile.tellg() << "byte" << endl;
        return -1;
    }
    gfile.seekg(0, ios::beg);

    demo D;
    gfile.read(D.data, sizeof(demo));


    //------------------------------
    //�浵�޸�ģ��
    //------------------------------
    int opr;
    while (1) {
        puts("");
        puts("--------------------------------------");
        puts("  ��Ϸ�浵�ļ��޸Ĺ���");
        puts("--------------------------------------");
        for (int i = 0; i < item_cnt; i++) {
            cout << "  " << (char)(i + 'a') << '.' << left << setw(16) << item_p[i]->name << right;
            item_p[i]->print(D.data + item_p[i]->start_addr);
            cout << endl;
        }
        puts("--------------------------------------");
        puts("  0.�����޸�");
        puts("  1.�����˳�");
        puts("--------------------------------------");
        puts("��ѡ��[a..y, 0..1]");
        putchar('>');
        putchar(' ');
        do {
            opr = cin.get();
        } while (opr <= ' ' || opr > '~');
        cin.ignore(1926, '\n');
        puts("");

        if (opr == '0') {
            break;
        }
        else if (opr == '1') {
            gfile.seekp(0, ios::beg);
            gfile.write(D.data, sizeof(demo));
            break;
        }
        else if (isalpha(opr) && ((opr = (tolower(opr) - 'a')) < item_cnt)) {
            item_p[opr]->ask_modify(D.data + item_p[opr]->start_addr);
        }
    }
    //------------------------------

    gfile.close();
    return 0;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����main�������������������������ļ���д
***************************************************************************/
int main(int argc, const char *argv[])
{

    if (argc == 2 && strcmp(argv[1], "--read") == 0)
        read();
    else if (argc == 2 && strcmp(argv[1], "--modify") == 0)
        modify();
    else
        puts("usage : 17-b5-demo --modify | --read");

    return 0;
}


#endif