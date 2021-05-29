/* 1051995 ���� ���� */


/*
�ж��� Windows ��ʽ���� Linux ��ʽ

���� 0x0A ǰ���� 0x0D ������Ϊ�� Windows ��ʽ
���� 0x0A ǰ��û 0x0D ������Ϊ�� Linux ��ʽ

17-b2 --check file1
17-b2 --convert wtol file1 file2
    �� Windows ��ʽ�ļ� file1 ת��Ϊ Linux ��ʽ�ļ� file2��
    �����ϸ�ʽ����� "�ļ���ʽ�޷�ʶ��"��
17-b2 --convert ltow file1 file2
    �� Linux ��ʽ�ļ� file1 ת��Ϊ Windows ��ʽ�ļ� file2��
    �����ϸ�ʽ����� "�ļ���ʽ�޷�ʶ��"��
*/

#include <fstream>
#include <cstring>
#include <string>
#include <iostream>
using namespace std;

#define FORMAT_WINDOWS  1
#define FORMAT_LINUX    2
#define FORMAT_UNKNOWN  0

void exit_err(const char *str, int exit_code = -1)
{
    cout << str << endl;
    exit(exit_code);
}


struct DATA_17_b2 {
    string fname_in;
    string fname_out;
};


//����ƥ��ɹ����ض�Ӧ opr_index(0,1,2...)��ʧ�ܷ��� -1
int analyze_main(int argc, const char *argv[], DATA_17_b2 &data);


//���гɹ��򷵻� 0
int opr_check(DATA_17_b2 &data);
int opr_convert_wtol(DATA_17_b2 &data);
int opr_convert_ltow(DATA_17_b2 &data);


//���ض�Ӧ�� FORMAT_xxx �궨��
int base_check(const char *fname_in);


int main(int argc, const char *argv[])
{
    int opr_index;
    DATA_17_b2 data;

    int (*opr[])(DATA_17_b2 &) = { opr_check, opr_convert_wtol, opr_convert_ltow };

    if ((opr_index = analyze_main(argc, argv, data)) < 0) {
        exit_err("Usage: 17-b2 --check file  |  --convert < wtol | ltow > file1 file2", -1);
    }

    opr[opr_index](data);

    return 0;
}







int analyze_main(int argc, const char *argv[], DATA_17_b2 &data)
{
    //cout << argc << endl;
    int dep = 0;

    if (++dep && --argc <= 0)
        return -1;

    if (strcmp(argv[dep], "--check") == 0) {

        if (++dep && --argc <= 0)
            return -1;

        data.fname_in = argv[dep];

        return (--argc == 0) ? 0 : -1;
    }
    else if (strcmp(argv[dep], "--convert") == 0) {
        if (++dep && --argc <= 0)
            return -1;

        if (strcmp(argv[dep], "wtol") == 0) {
            if (--argc == 2) {
                data.fname_in = argv[++dep];
                data.fname_out = argv[++dep];
                return 1;
            }
            return -1;
        }
        else if (strcmp(argv[dep], "ltow") == 0) {
            if (--argc == 2) {
                data.fname_in = argv[++dep];
                data.fname_out = argv[++dep];
                return 2;
            }
            return -1;
        }
    }

    return -1;
}


int opr_check(DATA_17_b2 &data)
{
    int tmp = base_check(data.fname_in.c_str());

    if (tmp == FORMAT_WINDOWS)
        cout << "Windows��ʽ" << endl;
    else if (tmp == FORMAT_LINUX)
        cout << "Linux��ʽ" << endl;
    else
        cout << "�ļ���ʽ�޷�ʶ��";

    return 0;
}
int opr_convert_wtol(DATA_17_b2 &data)
{
    if (base_check(data.fname_in.c_str()) != FORMAT_WINDOWS) {
        //cout << "��ת���ļ������� windows ��ʽ" << endl;
        cout << "�ļ���ʽ�޷�ʶ��" << endl;
        return -1;
    }

    ifstream fin(data.fname_in.c_str(), ios::binary);
    if (!fin.is_open()) {
        exit_err("�ļ��޷���");
    }

    ofstream fout(data.fname_out.c_str(), ios::binary);
    if (!fout.is_open()) {
        fin.close();
        exit_err("�ļ��޷���");
    }

    int cnt = 0;
    char cur, nxt = fin.get();
    while (!fin.eof()) {
        cur = nxt;
        nxt = fin.get();
        if (cur == 0x0D && nxt == 0x0A) {
            cnt++;
            continue;
        }
        else {
            fout.put(cur);
        }
    }

    cout << "ת���ɹ���ɾ�� " << cnt << "�� 0x0D" << endl;

    fin.close();
    fout.close();
    return 0;
}
int opr_convert_ltow(DATA_17_b2 &data)
{
    if (base_check(data.fname_in.c_str()) != FORMAT_LINUX) {
        //cout << "��ת���ļ������� linux ��ʽ" << endl;
        cout << "�ļ���ʽ�޷�ʶ��" << endl;
        return -1;
    }

    ifstream fin(data.fname_in.c_str(), ios::binary);
    if (!fin.is_open()) {
        exit_err("�ļ��޷���");
    }

    ofstream fout(data.fname_out.c_str(), ios::binary);
    if (!fout.is_open()) {
        fin.close();
        exit_err("�ļ��޷���");
    }

    int cnt = 0;
    char cur=fin.get();
    while (!fin.eof()) {
        if (cur == 0x0A) {
            cnt++;
            fout.put(0x0D);
        }
        fout.put(cur);
        cur = fin.get();
    }

    cout << "ת���ɹ������ " << cnt << "�� 0x0D" << endl;

    fin.close();
    fout.close();
    return 0;
}


int base_check(const char *fname_in)
{
    ifstream fin(fname_in, ios::binary);

    if (!fin.is_open()) {
        exit_err("�ļ��޷���");
    }

    int lst = -1, cur = -1;

    bool not_windows = false, not_linux = false;

    while (!fin.eof()) {
        if (cur == 0x0A) {
            if (lst == 0x0D)
                not_linux = true;
            else
                not_windows = true;
            if (not_windows && not_linux)
                break;
        }
        lst = cur;
        cur = fin.get();
    }

    fin.close();
        
    if (!not_windows && not_linux)
        return FORMAT_WINDOWS;
    else if (!not_linux && not_windows)
        return FORMAT_LINUX;
    else
        return FORMAT_UNKNOWN;
}