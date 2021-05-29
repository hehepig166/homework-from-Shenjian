/* 2051995 ���� ���� */
/* 1853769 ��� ; 2053642 ��� ; 2052147 �ÿ�Ϊ */

/*

��ʽ��
ÿһ�У����ǳ���8��һ�����ֱ�ʾ��ַ��Ȼ�������ո���16�����2������8 9 �м��и����ţ�Ȼ������ո񣬺Ͷ�Ӧ���ַ�
���־�Ϊʮ�����ƣ�Сд��ĸ

00000000  30 30 30 30 30 30 30 30 - 20 20 33 31 20 33 32 20     00000000..31.32.
00000010  33 33 20 33 34 20 30 64 - 20 30 61 20 33 31 20 33     33.34.0d.0a.31.3
00000020  32 20 2d 20 33 34 20 36 - 65 20 36 36 20 36 34 20     2.-.34.6e.66.64.
00000030  30 64 20 30 61 20 65 35 - 20 39 33 20 20 20 20 20     0d.0a.e5.93.....
00000040  31 32 33 34 2e 2e 31 32 - 34 6e 66 64 2e 2e 2e 2e     1234..124nfd....
00000050  0d 0a 30 30 30 30 30 30 - 31 30 20 20 38 38 20 65     ..00000010..88.e
00000060  38 20 39 62 20 61 34 20 - 30 64 20 30 61 20 30 64     8.9b.a4.0d.0a.0d
00000070  20 30 61 20 2d 20 65 37 - 20 62 62 20 39 39 20 20     .0a.-.e7.bb.99..
00000080  20 20 20 20 20 20 20 20 - 20 20 20 20 20 20 20 20     ................
00000090  20 20 2e 2e 2e 2e 2e 2e - 2e 2e 2e 2e 2e 0d 0a 0d     ................
000000a0  0a                                                    .

*/

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <iomanip>
using namespace std;

const char *HINT_USG = "Usage : 17-b4 --infile a.docx\n        17-b4 --infile a.docx --outfile a.hex";

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


//ʶ��ɹ�����0��ʧ�ܷ��� -1
int analyze_main(int argc, const char *argv[], DATA &data);

//��֤ data ӵ�� ifname���� has_infile Ϊ��
void convert(DATA &data);

void base_convert(istream &in, ostream &out = cout);

void test()
{
    DATA data;
    data.has_infile = data.has_outfile = 1;
    data.ifname = "std1.txt";
    data.ofname = "he.txt";
    convert(data);
    exit(0);
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

    if (!data.has_infile && !data.has_outfile) {
        exit_err(HINT_USG);
    }

    return 0;
}


void convert(DATA &data)
{
    ifstream fin(data.ifname);     //����binary
    if (!fin.is_open()) {
        exit_err("�����ļ���ʧ��");
    }

    if (data.has_outfile) {
        ofstream fout(data.ofname, ios::binary);     //�� binary
        if (!fout.is_open()) {
            fin.close();    //�ر�fin
            exit_err("����ļ���ʧ��");
        }
        base_convert(fin, fout);

        //�ر��ļ�
        fin.close();
        fout.close();
    }
}


void base_convert(istream &in, ostream &out)
{
    int tmp;
    char tmpstr[100];

    for (in.getline(tmpstr, 90); !in.eof(); in.getline(tmpstr, 90)) {
        for (int i = 0, p = 10, len = strlen(tmpstr); p < len && i < 16; i++, p += 3) {
            if (i == 8)
                p += 2; //�����Ǹ�����
            if (tmpstr[p] == ' ')
                break;
            tmp = ((isalpha(tmpstr[p]) ? tmpstr[p] - 'a' + 10 : tmpstr[p] - '0') << 4) |
                (isalpha(tmpstr[p + 1]) ? tmpstr[p + 1] - 'a' + 10 : tmpstr[p + 1] - '0');
            out.put(tmp);
        }
    }
}


