/* ���� 2051995 ���� */
#include "../include/hehepigMenu.h"
#include <iostream>
using namespace std;

int main() {
    int ret;

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

    ret = Menu_char(8, "ABCDEFGQ", hintStr);

    cout << (char)ret << endl;
}