/* ���� 2051995 ���� */
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <sstream>
#include <Windows.h>
#include <iomanip>

#include "../include/cmd_console_tools.h"
#include "../include/hehepigMenu.h"
#include "../include/CODE_GB2312.h"
#include "../include/hehepigIO.h"
#include "90-02-b4-pro.h"
#include "../include/cfg_file_tools.h"
using namespace std;

//#define RGB(x,y,z) (((x)<<4)|((y)<<2)|((z)))

const char *CFG_FILE = "led.cfg";

void test()
{
    char tmpstr[20];
    bool f[16][16];
    while (1) {
        puts("\n����һ��ȫ���ַ���������0�˳�");
        cin >> tmpstr;
        if (tmpstr[0] == '0')
            break;
        //show_pixels(tmpstr);
        to_image(f, tmpstr, 0);
        for (int i = 0; i < 16; i++) {
            for (int j = 0; j < 16; j++) {
                putchar(' ');
                putchar(f[i][j] ? '#' : ' ');
            }
            puts("");
        }
    }
}


const char *const texts[] = {
        "��С��ս���ܸ�������С��ս���ܸ�������С��ս���ܸ�������С��ս���ܸ�������С��ս���ܸ�������С��ս���ܸ�����",
        "�����й侯̷�ǣ�\n·��Υ�°ѻ��ġ�\n����ѯ��Ҫȥ�ģ�\n�ɻ���������š�",
        "����մ���ţүү˵������˵��ͼɶ�أ���",
        "�����Ǻð������ǻ�Ԫ����̫����������������\n�ղ��и�������������ʦ����ʲô���ˣ���˵��ô���£����ҷ���һ���Ž�ͼ��\n��һ����໣�ԭ�������죬�����������ˣ���ʮ���꣬һ�����ؾ�ʮ�๫�һ�����ذ�ʮ�๫�",
        "ɡ��һ��¬��ΰ׼������������\n",
        "���ǿ��ܲ�֪��ֻ�ö�ʮ��Ӯ�������ʮ������ʲô�������һ��ֻ���������������������ˣ����Ĺ֡���",
        "�����̵�����ðɣ����̸��㵹һ��������ŵ�������̵�һ��������ŵ����ʼ���ը���㣬ը��ը����Ư����ʮ�������������ң�������ɱ�ң����������ʮ�����ư�¬��ΰ���ˣ��ң����������Ͱ����������Ļ�Ե�������",
        "����ᣬ���ز������ر���ײ�ش������ؼ������عң��ױ��ǣ������磬���ضԣ�Ĺ�ر������ز��������̣��ز��ϣ�����ˣ��ײ��ţ����ؼӣ����������ѱ�ƽ��ϡ��ֽ�������Σ���������û�ذˣ������У�Ͷ�ضϣ��Ը�ҽ������ȫ���㲻Ѫ��Hת�����Ʋ��������س�",
        "Ⱥ��������㣬\n�������ž��ӡ�\n����ô��˯���ž���",
        "�ֲ������ķ������ɶ��\n�����û"
};

static void modify(const char *(&text), int &is_fan, int &num_row, int &num_col, unsigned  int &bgcolor, unsigned int &fgcolor, int &size)
{
    static char static_text[105];
    puts("\nmodify");
    int opr;
    const char *modify_hint[] = {
        "�޸���ʾ�ı�����",
        "�л���",
        "�޸���Ļ��ʾ������",
        "����ģʽ 1��*����������ɫ",
        "����ģʽ 2��hdc��������ɫ",
        "�޸������С",
        "�޸�ģʽ2��С���ֱ��",
        "����ģʽ 2 (hdc) ������ɫ",
        "���ز˵�����",

        "һЩ�����ı�",
        "�Լ������ı�"
    };

    while (1) {
        opr = Menu_char(9, "12345678c", modify_hint);

        if (opr == '1') {
            opr = Menu_char(2, "12", &modify_hint[8]);
            if (opr == '1') {
                char tmp[100];
                sprintf(tmp, "���������ı�������%d-%d��\n> ", 0, sizeof(texts) / sizeof(char *) - 1);
                text = texts[GetInt(tmp, 0, sizeof(texts) / sizeof(char *) - 1)];
            }
            else if (opr == '2') {
                puts("����Ҫ��ʾ���ı���ֻ�ܰ���������һ�������ַ����س����������ɳ���50���֣�\n> ");
                cin >> static_text;
                text = static_text;
            }
            continue;
        }
        else if (opr == '2') {
            is_fan ^= 1;
            cout << "֪���ˣ������л�Ϊ " << ((is_fan & 1) ? "��" : "��") << endl;
            continue;
        }
        else if (opr == '3') {
            cout << "��ǰ������Ϣ��[�У���] = [" << num_row << "��" << num_col << "]" << endl;
            num_row = GetInt("�У�1..10��\n> ", 1, 10);
            num_col = GetInt("�У�1..10��\n> ", 1, 10);
            continue;
        }
        else if (opr == '4') {
            cout << "��ǰ��ɫ��� " << (fgcolor >> 24) << endl;
            fgcolor &= 0xffffff;
            fgcolor |= GetInt("������ɫ��ţ�1..14��\n> ", 1, 14) << 24;
            continue;
        }
        else if (opr == '5') {
            cout << "��ǰ��ɫRGB " << ((fgcolor & 0xff0000) >> 16) << ' ' << ((fgcolor & 0xff00) >> 8) << ' ' << (fgcolor & 0xff) << endl;
            fgcolor &= (~0xffffff);
            fgcolor |= (GetInt("R > ", 0, 255)) << 16;
            fgcolor |= (GetInt("G > ", 0, 255)) << 8;
            fgcolor |= (GetInt("B > ", 0, 255));
            continue;
        }
        else if (opr == '6') {
            cout << "��ǰ�����С " << (size & 0xff) << endl;
            size &= (~0xff);
            size |= GetInt("���������С��1..16��\n> ", 1, 16);
            continue;
        }
        else if (opr == '7') {
            cout << "��ǰֱ�� " << ((size & 0xff00) >> 8) << endl;
            size &= (~0xff00);
            size |= GetInt("����ֱ����1..50��\n> ", 1, 50) << 8;
            continue;
        }
        else if (opr == '8') {
            cout << "��ǰ��ɫRGB " << ((bgcolor & 0xff0000) >> 16) << ' ' << ((bgcolor & 0xff00) >> 8) << ' ' << (bgcolor & 0xff) << endl;
            bgcolor &= (~0xffffff);
            bgcolor |= (GetInt("R > ", 0, 255)) << 16;
            bgcolor |= (GetInt("G > ", 0, 255)) << 8;
            bgcolor |= (GetInt("B > ", 0, 255));
        }
        else if (opr == 'c') {
            break;
        }
    }
}






static void read_from_file(const char *(&text), int &is_fan, int &num_row, int &num_col, unsigned  int &bgcolor, unsigned int &fgcolor, int &size)
{
    CFT cft(CFG_FILE, 0);

    if (!cft.is_open()) {
        cout << "�޷��������ļ� " << CFG_FILE << " ����ʹ��Ĭ��ֵ�����س�����" << endl;
        GetKey('\r');
        return;
    }

    int tmp_int;
    string tmp_string;

    puts("��ȡȫ��������");

    if (cft.item_get_value("ȫ������", "����", tmp_int)) {
        num_row = tmp_int;
        cout << "���� " << num_row << endl;
    }
    if (cft.item_get_value("ȫ������", "����", tmp_int)) {
        num_col = tmp_int;
        cout << "���� " << num_col << endl;
    }
    if (cft.item_get_value("ȫ������", "����ɫ", tmp_string)) {
        istringstream is(tmp_string);
        is >> hex >> tmp_int;
        if (tmp_int >= 0 && tmp_int <= 0xFFFFFF) {
            bgcolor = tmp_int;
            cout << "����ɫ " << bgcolor << endl;
        }
    }
    if (cft.item_get_value("ȫ������", "��", tmp_string) && (tmp_string == "��" || tmp_string == "��")) {
        is_fan = tmp_string == "��";
        cout << "�� " << tmp_string << endl;
    }

    cft.close();

    puts("��ȡ�������س�����");
    GetKey('\r');
}


static void write_to_file(const char *(&text), int &is_fan, int &num_row, int &num_col, unsigned  int &bgcolor, unsigned int &fgcolor, int &size)
{
    CFT cft(CFG_FILE, 1);

    if (!cft.is_open()) {
        return;
    }

    puts("�޸������ļ�");

    cft.group_add("ȫ������");

    if (cft.item_update("ȫ������", "����", num_row)) {
        puts("�޸�ȫ�������ɹ�");
    }
    if (cft.item_update("ȫ������", "����", num_col)) {
        puts("�޸�ȫ�������ɹ�");
    }
    ostringstream os;
    os << hex << setw(6) << setfill('0') << bgcolor << setfill(' ');
    if (cft.item_update("ȫ������", "����ɫ", os.str())) {
        puts("�޸�ȫ�ֱ���ɫ�ɹ�");
    }
    if (cft.item_update("ȫ������", "��", is_fan ? "��" : "��")) {
        puts("�޸�ȫ�ּ򷱳ɹ�");
    }

    cft.close();

    puts("�޸Ľ������س�����");
    GetKey('\r', '\r');
}



int main()
{
    const char *text = texts[6];
    //size:     0xff ��size��0xff00��ֱ��
    //color:    0xffffff ��RGB��0xff000000 ��cct_setcolor
    int is_fan = 1, num_row = 4, num_col = 10, size = (5 << 8) | 6;
    unsigned int bgcolor = 0, fgcolor = (COLOR_PINK << 24) | 0xE066FF;

    read_from_file(text, is_fan, num_row, num_col, bgcolor, fgcolor, size);


    cct_setconsoleborder(120, 30, 120, 9001);
    cct_setfontsize("������", 16);
    cct_setcolor();
    cct_cls();

    const char *hint[] = {
        "ģ��LED��Ϣ��ʾ-��*��ʾÿ����",
        "ģ��LED��Ϣ��ʾ-��hdc_point��ÿ����",
        "ģ��LED��Ϣ��ʾ-�任��ɫ",
        "ģ��LED��Ϣ��ʾ-������ɢ",
        "ģ��LED��Ϣ��ʾ-�ܽ�",
        "ģ��LED��Ϣ��ʾ-���޳��ı�ѭ�����ţ����з���ʧЧ��",
        "������",
        "�޸���Ļ״̬",
        "�˳�"
    };

    int done = 0;

    while (!done) {
        cct_setcolor();
        cct_cls();
        int opr = Menu_char(9, "1234560cz", hint);

        switch (opr) {
            case('0'):
                test();
                break;
            case ('1'):
                print_basic(text, is_fan, num_row, num_col, bgcolor >> 24, fgcolor >> 24, size, 0);
                break;
            case ('2'):
                print_basic(text, is_fan, num_row, num_col, bgcolor & 0xffffff, fgcolor & 0xffffff, size, 1);
                break;
            case('3'):
                print_colorful(text, is_fan, num_row, num_col, size);
                break;
            case('4'):
                print_zoom(text, is_fan, num_row, num_col, bgcolor & 0xffffff, fgcolor & 0xffffff, size);
                break;
            case('5'):
                print_dissolve(text, is_fan, num_row, num_col, bgcolor & 0xffffff, fgcolor & 0xffffff, size);
                break;
            case('6'):
                print_inf(text, is_fan, num_row, num_col, bgcolor & 0xffffff, fgcolor & 0xffffff, size);
                break;
            case('c'):
                modify(text, is_fan, num_row, num_col, bgcolor, fgcolor, size);
                break;
            case('z'):
                int update_cft = GetInt("�Ƿ�Ҫ���������ļ���(0��Ҫ��1Ҫ)", 0, 1);
                if (update_cft) {
                    write_to_file(text, is_fan, num_row, num_col, bgcolor, fgcolor, size);
                }

                done = 1;
                break;
        }
    }



    return 0;
}