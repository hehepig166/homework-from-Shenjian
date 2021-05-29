/* 2051995 朱泽凯 济美 */
/* 1853769 许辰昊 ; 2053642 李博宇 ; 2052147 雒俊为 */

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <iomanip>
using namespace std;

const char *HINT_USG = "Usage : 17-b3 --infile 原始文件 [ --outfile hex格式文件 ]\n        17-b3 --infile a.docx\n        17-b3 --infile a.docx --outfile a.hex";

void exit_err(const char *str, int exit_code = -1)
{
    cout << str << endl;
    exit(exit_code);
}

struct DATA {
    bool has_infile;
    bool has_outfile;
    string ifname;
    string ofname;
};


//识别成功返回0，失败返回 -1
int analyze_main(int argc, const char *argv[], DATA &data);

//保证 data 拥有 ifname，即 has_infile 为真
void convert(DATA &data);

void base_convert(istream &in, ostream &out = cout);


void test()
{
    DATA data;
    data.has_infile = true;
    data.has_outfile = true;
    data.ifname = "5.txt";
    data.ofname = "he.txt";
    convert(data);
}


int main(int argc, const char *argv[])
{

    //test();

    DATA data;

    if (analyze_main(argc, argv, data) < 0) {
        exit_err(HINT_USG);
    }

    convert(data);

    return 0;
}



int analyze_main(int argc, const char *argv[], DATA &data)
{
    int dep = 0;

    data.has_infile = data.has_outfile = false;

    while (++dep < argc) {
        if (strcmp(argv[dep], "--infile") == 0) {
            if (++dep < argc) {
                data.has_infile = true;
                data.ifname = argv[dep];
            }
            else {
                exit_err(HINT_USG);
                return -1;
            }
        }
        else if (strcmp(argv[dep], "--outfile") == 0) {
            if (++dep < argc) {
                data.has_outfile = true;
                data.ofname = argv[dep];
            }
            else {
                exit_err(HINT_USG);
                return -1;
            }
        }
        else {
            exit_err(HINT_USG);
            return -1;
        }
    }

    if (!data.has_infile) {
        exit_err(HINT_USG);
    }

    return 0;
}


void convert(DATA &data)
{
    ifstream fin(data.ifname, ios::binary);
    if (!fin.is_open()) {
        exit_err("输入文件打开失败");
    }

    if (data.has_outfile) {
        ofstream fout(data.ofname);     //不用 binary
        if (!fout.is_open()) {
            fin.close();    //关闭fin
            exit_err("输出文件打开失败");
        }
        base_convert(fin, fout);

        //关闭文件
        fin.close();
        fout.close();
    }
    else {
        base_convert(fin);

        //关闭文件
        fin.close();
    }
}


void base_convert(istream &in, ostream &out)
{
    unsigned int cnt = 0;
    int pbuf, buf[16];
    int tmp;

    for (tmp = in.get(), pbuf = 0; !in.eof(); tmp = in.get()) {
        buf[pbuf++] = tmp;
        if (pbuf == 16) {
            out << setw(8) << setfill('0') << hex << cnt << ' ';
            for (int i = 0; i < 16; i++) {
                if (i == 8)
                    out << " -";
                out << ' ' << setw(2) << buf[i];
            }
            out << "     ";
            for (int i = 0; i < 16; i++)
                out << char(buf[i] >= 33 && buf[i] <= 126 ? buf[i] : '.');
            out << endl;

            cnt += 16;
            pbuf = 0;
        }
    }

    if (pbuf > 0) {
        out << setw(8) << setfill('0') << hex << cnt << ' ';
        for (int i = 0; i < 16; i++) {
            if (i == 8)
                out << (i<pbuf ? " -" : "  ");
            out << ' ';
            i<pbuf ? (out<< setw(2) << buf[i]) : (out <<"  ");
        }
        out << "     ";
        for (int i = 0; i < pbuf; i++)
            out << char(buf[i] >= 33 && buf[i] <= 126 ? buf[i] : '.');
        out << endl;

        cnt += pbuf;
    }

    out << endl;
}


