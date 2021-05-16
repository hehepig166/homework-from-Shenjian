/* ���� 2051995 ���� */

/*********************************************************
˵����
    GB2312����
    16*16�����ֿ�

    ��λ�룺 qh         wh
    �����룺 qh+0xa0    2h+0xa0
    ƫ������ (94*(qh-1)+(wh-1))*32  bit
    ÿ������ռ 32bit��ÿ��2�ֽڣ������� unsigned char ��

    ����������

    ��λ�룺16-01   (0x10-0x01)
    �����룺0xb0a1
    ƫ������0xb040  ((94*15+0)*32)

    ������Ϣ��˳��0xb040-0xb041 0xb042-0xb043 ... 0xb05e-0xb05f��
    00 04 2f 7e f9 04 a9 04 aa 14 aa 7c ac 54 aa 54 aa 54 a9 54 e9 74 ad 54 0a 04 08 04 08 14 08 0c
*********************************************************/
#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <iostream>

#include "../include/CODE_GB2312.h"
using namespace std;

//const unsigned char *HZK16_DEF = "HZK16";
//const unsigned char *HZK16F_DEF = "HZK16F";


#define offset(incode) (((94*(((unsigned char)incode[0]-0xa0)-1))+(((unsigned char)incode[1]-0xa0)-1))*32)


//��ʶ�𷵻� 1�����򷵻� 0
bool check_code(const char *incode)
{
    unsigned char qh = (unsigned char)incode[0] - 0xa0;
    unsigned char wh = (unsigned char)incode[1] - 0xa0;
    return qh <= 87 && wh <= 94;
}

void get_pixels(unsigned char *out, const char *incode, const int is_fanti)
{
    if (!check_code(incode)) {
        cout << "����ʶ��ʧ��" << endl;
        return;
    }

    const char *infile = is_fanti ? "HZK16F" : "HZK16";

    FILE *fin;
    if ((fin = fopen(infile, "rb")) == NULL) {
        cout << "���ļ� " << infile << " ʧ��" << endl;
        return;
    }

    fseek(fin, offset(incode), SEEK_SET);

    for (int i = 0; i < 32; i++) {
        out[i] = getc(fin);
    }

    fclose(fin);
}

void to_image(bool out[16][16], const char *incode, const int is_fanti)
{
    if (!check_code(incode)) {
        cout << "����ʶ��ʧ��" << endl;
        return;
    }

    const char *infile = is_fanti ? "HZK16F" : "HZK16";

    FILE *fin;
    if ((fin = fopen(infile, "rb")) == NULL) {
        cout << "���ļ� " << infile << " ʧ��" << endl;
        return;
    }

    fseek(fin, offset(incode), SEEK_SET);

    for (int i = 0; i < 16; i++) {
        unsigned char x1, x2;
        x1 = getc(fin);
        x2 = getc(fin);

        for (int j = 0; j < 8; j++)
            out[i][7-j] = x1 & (1 << j);
        for (int j = 0; j < 8; j++)
            out[i][15-j] = x2 & (1 << j);
    }

    fclose(fin);
}


void show_line(const unsigned char x1, const unsigned char x2)
{
    for (int j = 7; j >= 0; j--) {
        putchar(' ');
        putchar((x1 & (1 << j)) ? '*' : ' ');
    }
    for (int j = 7; j >= 0; j--) {
        putchar(' ');
        putchar((x2 & (1 << j)) ? '*' : ' ');
    }
}


void show_pixels(const char *incode, const int is_fanti)
{
    unsigned char tmp[32];
    get_pixels(tmp, incode, is_fanti);

    cout << offset(incode) << endl;

    for (int i = 0; i < 32; i += 2) {
        putchar('\n');
        //printf("%02x %02x ", tmp[i], tmp[i + 1]);
        show_line(tmp[i], tmp[i | 1]);
    }
    putchar('\n');
}
