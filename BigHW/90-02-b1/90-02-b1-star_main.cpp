/* ���� 2051995 ���� */
#include "../include/cmd_console_tools.h"
#include "../include/hehepigMenu.h"
#include "./90-02-b1-star_play.h"
#include <iostream>
using namespace std;

int menu();

int main() {
    while (menu()) {
        cct_cls();
    }
}

int menu() {
    const char* hintStr[] = {
        "�������ҳ����������ʶ",
        "���������һ���������ֲ�����ʾ��",
        "���������һ�أ��ֲ�����ʾ��",
        "αͼ�ν����������ѡ��һ��ɫ�飨�޷ָ��ߣ�",
        "αͼ�ν����������ѡ��һ��ɫ�飨�зָ��ߣ�",
        "αͼ�ν������һ���������ֲ��裩",
        "αͼ�ν���������",
        "�˳�"
    };

    int m = Menu_char(8, "ABCDEFGQ", hintStr);

    m = toupper(m);

    if (m >= 'A' && m <= 'C') {
        cct_cls();
        DigitalPlay().Begin(m);
    }
    else if (m >= 'D' && m <= 'G') {

    }
    else if (m == 'Q') {
        return 0;
    }

    cout << m << endl;
    return m;
}