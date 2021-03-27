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
    //int cols, lines, buffer_cols, buffer_lines;
    //cct_getconsoleborder(cols, lines, buffer_cols, buffer_lines);   //��ȡ��������


    const char* hintStr[] = {
        "�������ҳ����������ʶ",
        "���������һ���������ֲ�����ʾ��",
        "���������һ�أ��ֲ�����ʾ��",
        "αͼ�ν����������ѡ��һ��ɫ�飨�޷ָ��ߣ�",
        "αͼ�ν����������ѡ��һ��ɫ�鲢���һ�֣��зָ��ߣ�",
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
        cct_cls();
        GraphicalPlay().Begin(m);
    }
    else if (m == 'Q') {
        //
        //cct_setconsoleborder(cols, lines, buffer_cols, buffer_lines);
        return 0;
    }

    //cct_setconsoleborder(cols, lines, buffer_cols, buffer_lines);   //�������Ը�λ
    
    return m;
}