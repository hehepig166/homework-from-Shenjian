/* 1051995 朱泽凯 济美 */


/*
判断是 Windows 格式还是 Linux 格式

所有 0x0A 前都有 0x0D ，则认为是 Windows 格式
所有 0x0A 前都没 0x0D ，则认为是 Linux 格式

17-b2 --check file1
17-b2 --convert wtol file1 file2
    将 Windows 格式文件 file1 转换为 Linux 格式文件 file2，
    不符合格式则输出 "文件格式无法识别"。
17-b2 --convert ltow file1 file2
    将 Linux 格式文件 file1 转换为 Windows 格式文件 file2，
    不符合格式则输出 "文件格式无法识别"。
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


//操作匹配成功返回对应 opr_index(0,1,2...)，失败返回 -1
int analyze_main(int argc, const char *argv[], DATA_17_b2 &data);


//运行成功则返回 0
int opr_check(DATA_17_b2 &data);
int opr_convert_wtol(DATA_17_b2 &data);
int opr_convert_ltow(DATA_17_b2 &data);


//返回对应的 FORMAT_xxx 宏定义
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
        cout << "Windows格式" << endl;
    else if (tmp == FORMAT_LINUX)
        cout << "Linux格式" << endl;
    else
        cout << "文件格式无法识别";

    return 0;
}
int opr_convert_wtol(DATA_17_b2 &data)
{
    if (base_check(data.fname_in.c_str()) != FORMAT_WINDOWS) {
        //cout << "待转换文件并不是 windows 格式" << endl;
        cout << "文件格式无法识别" << endl;
        return -1;
    }

    ifstream fin(data.fname_in.c_str(), ios::binary);
    if (!fin.is_open()) {
        exit_err("文件无法打开");
    }

    ofstream fout(data.fname_out.c_str(), ios::binary);
    if (!fout.is_open()) {
        fin.close();
        exit_err("文件无法打开");
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

    cout << "转换成功，删除 " << cnt << "个 0x0D" << endl;

    fin.close();
    fout.close();
    return 0;
}
int opr_convert_ltow(DATA_17_b2 &data)
{
    if (base_check(data.fname_in.c_str()) != FORMAT_LINUX) {
        //cout << "待转换文件并不是 linux 格式" << endl;
        cout << "文件格式无法识别" << endl;
        return -1;
    }

    ifstream fin(data.fname_in.c_str(), ios::binary);
    if (!fin.is_open()) {
        exit_err("文件无法打开");
    }

    ofstream fout(data.fname_out.c_str(), ios::binary);
    if (!fout.is_open()) {
        fin.close();
        exit_err("文件无法打开");
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

    cout << "转换成功，添加 " << cnt << "个 0x0D" << endl;

    fin.close();
    fout.close();
    return 0;
}


int base_check(const char *fname_in)
{
    ifstream fin(fname_in, ios::binary);

    if (!fin.is_open()) {
        exit_err("文件无法打开");
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