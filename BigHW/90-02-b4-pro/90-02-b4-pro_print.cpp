/* ���� 2051995 ���� */
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <Windows.h>
#include "90-02-b4-pro.h"
#include "../include/CODE_GB2312.h"
#include "../include/cmd_console_tools.h"
#include "../include/hehepigIO.h"
#include "../include/cmd_hdc_tools.h"
using namespace std;

#define DEF_SZ_D int sz = size & 0xff, hdc_sz = sz, hdc_d = size >> 8

//�� (startX,startY) Ϊ���Ͻǣ���*дһ��16*16�� incode��
//ִ��ʱ���ı�������ɫ�ȣ���Щ��Ҫ���ú���ǰ����
inline static void print_single_star(const char *incode, int is_fan, int startX, int startY)
{
    unsigned char tmp[35];
    get_pixels(tmp, incode, is_fan);
    for (int i = 0; i < 32; i += 2) {
        cct_gotoxy(startX, startY + (i >> 1));
        show_line(tmp[i], tmp[i | 1]);
    }
}

//size�ǵ�����һ��Բ���ֱ��
//Ҫ��incode��NULL���ǿո����bgcolor
inline static void print_single_hdc(const char *incode, int is_fan, int startX, int startY, int size, int d, int bgcolor, int fgcolor, int delay_ms)
{
    if (incode == NULL) {
        for (int i = 0; i < 32; i += 2) {

            for (int j = 0; j < 16; j++) {
                hdc_point(startX + (j * size), startY + (i / 2 * size), d, bgcolor);
            }

            Sleep(delay_ms);

        }
        return;
    }


    unsigned char tmp[35];
    get_pixels(tmp, incode, is_fan);
    for (int i = 0; i < 32; i += 2) {

        for (int j = 0; j < 8; j++) {
            hdc_point(startX + (j * size), startY + (i / 2 * size), d, (tmp[i] & (0x80 >> j)) ? fgcolor : bgcolor);
            //Sleep(delay_ms);
        }

        for (int j = 0; j < 8; j++) {
            hdc_point(startX + ((8 + j) * size), startY + (i / 2 * size), d, (tmp[i | 1] & (0x80 >> j)) ? fgcolor : bgcolor);
            //Sleep(delay_ms);
        }

        Sleep(delay_ms);

    }
}

//�� use_hdc���� color ���� RGB ֵ�� ���õ��������*�������color���� 1..14 ��һ������
//ignore_enter��\n �������У�����0����
inline static void print_text_basic(const char *text, int is_fan, int num_row, int num_col, int bgcolor, int fgcolor, int size, bool use_hdc, int hdc_delay_ms, bool ignore_enter)
{
    const char *p = text;
    int row = 0, col = 0;

    //int sz = size & 0xff, hdc_sz = sz * 8, hdc_d = size >> 8;
    DEF_SZ_D;

    while (*p) {
        //�Է����ֵ��ַ�
        if (*p == 0)
            break;
        else if (*p < 0) {

            //����Ҫ���»���
            if (col >= num_col) {
                row++;
                col = 0;
            }
            if (row >= num_row) {
                break;      //���ģʽ�³�����Ļ�ľͲ����ˣ�LED���������޴��
            }

            if (use_hdc)
                print_single_hdc(p, is_fan, (col * 16 + 2) * hdc_sz, (row * 17 + 2) * hdc_sz, hdc_sz, hdc_d, bgcolor, fgcolor, hdc_delay_ms);
            else
                print_single_star(p, is_fan, col * 32 + 2, row * 17 + 2);
            p += 2;
            col++;
        }
        else {
            if (!ignore_enter && *p == '\n' && col < num_col) {      //���Ѿ���һ�еĸտ�ͷ���Ͳ����ٻ�����
                if (use_hdc) {
                    while (col < num_col) {
                        print_single_hdc(NULL, is_fan, (col * 16 + 2) * hdc_sz, (row * 17 + 2) * hdc_sz, hdc_sz, hdc_d, bgcolor, fgcolor, hdc_delay_ms);
                        col++;
                    }
                }
                row++;
                col = 0;
            }
            p += 1;
        }
    }

    if (use_hdc && row<num_row) {
        while (col < num_col) {
            print_single_hdc(NULL, is_fan, (col * 16 + 2) * hdc_sz, (row * 17 + 2) * hdc_sz, hdc_sz, hdc_d, bgcolor, fgcolor, hdc_delay_ms);
            col++;
        }
    }
}


//�����в�������������Ҫ���������ǰ����
inline static void _print_basic(const char *text, int is_fan, int num_row, int num_col, int bgcolor, int fgcolor, int size, bool use_hdc, int delay_ms, bool ignore_enter)
{
    //int sz = size & 0xff, hdc_sz = sz * 8, hdc_d = size >> 8;
    DEF_SZ_D;


    if (use_hdc) {
        hdc_init(0, 7, (num_col * 16 + 3) * hdc_sz, (num_row * 17 + 3) * hdc_sz);
    }
    else {
        cct_setcolor(bgcolor, fgcolor);
        cct_setfontsize("������", sz);
        cct_setconsoleborder((num_col * 32) + 3, (num_row * 17) + 3, (num_col * 32) + 3, 9001);
    }


    print_text_basic(text, is_fan, num_row, num_col, bgcolor, fgcolor, size, use_hdc, delay_ms, ignore_enter);
}



//�������ı�������Ļ״̬ת��bool
//���out����Ч�����д��� &ROW, &COL ��
//ignore_enter��\n �������У�����0����
template<int n, int m>
inline static void to_text_image(bool(&out)[n][m], const char *text, int is_fan, int num_row, int num_col, int &ROW, int &COL, bool ignore_enter)
{
    ROW = num_row * 16, COL = num_col * 16;     //����out�Ĵ�С
    if (ROW > n || COL > m) {   //����out���鲻����ֱ���˳�
        puts("���鲻��");
        return;
    }
    int row = 0, col = 0;   //��ǰλ��
    const char *p = text;
    bool tmp[16][16];
    while (*p) {
        //�Է����ֵ��ַ�
        if (*p == 0)
            break;
        else if (*p < 0) {

            //����Ҫ���»���
            if (col >= num_col) {
                row++;
                col = 0;
            }
            if (row >= num_row) {
                break;      //���ģʽ�³�����Ļ�ľͲ����ˣ�LED���������޴��
            }

            to_image(tmp, p, is_fan);
            for (int i = 0; i < 16; i++)
                for (int j = 0; j < 16; j++)
                    out[(row * 16) + i][(col * 16) + j] = tmp[i][j];

            p += 2;
            col++;
        }
        else {
            if (!ignore_enter && *p == '\n' && col < num_col) {      //���Ѿ���һ�еĸտ�ͷ���Ͳ����ٻ�����

                while (col < num_col) {
                    for (int i = 0; i < 16; i++)
                        for (int j = 0; j < 16; j++)
                            out[(row * 16) + i][(col * 16) + j] = 0;
                    col++;
                }

                row++;
                col = 0;
            }
            p += 1;
        }
    }
    //���һ�в���
    while (row<num_row && col < num_col) {
        for (int i = 0; i < 16; i++)
            for (int j = 0; j < 16; j++)
                out[(row * 16) + i][(col * 16) + j] = 0;
        col++;
    }
}














//===========================================================================
//����ģʽ
//===========================================================================


void print_basic(const char *text, int is_fan, int num_row, int num_col, int bgcolor, int fgcolor, int size, bool use_hdc)
{

    cct_cls();
    _print_basic(text, is_fan, num_row, num_col, bgcolor, fgcolor, size, use_hdc, use_hdc, 0);     //��hdc��delay 1ms


    GetKey('\r', '\r');

    cct_setconsoleborder(120, 30, 120, 9001);
    cct_setfontsize("������", 16);
    cct_setcolor();
    cct_cls();

    puts("\n��c����");
    GetKey('c', 'c', 0);
}



void print_colorful(const char *text, int is_fan, int num_row, int num_col, int size)
{
    int key;
    hdc_cls();
    while ((key = AskKey()) != '\r' && key != 'c') {

        _print_basic(text, is_fan, num_row, num_col, RGB(rand() % 64, rand() % 64, rand() % 64), RGB(rand() % 32 * 8, rand() % 32 * 8, rand() % 32 * 8), size, 1, 0, 0);
        Sleep(100);
    }

    cct_setconsoleborder(120, 30, 120, 9001);
    cct_setfontsize("������", 16);
    cct_setcolor();
    cct_cls();

    puts("\n��c����");
    GetKey('c', 'c', 0);
}



void print_zoom(const char *text, int is_fan, int num_row, int num_col, int bgcolor, int fgcolor, int size)
{
    //int sz = size & 0xff, hdc_sz = sz * 8, hdc_d = size >> 8;
    DEF_SZ_D;
    hdc_init(0, 7, (num_col * 16 + 3) * hdc_sz, (num_row * 17 + 3) * hdc_sz);

    //û�п��Ƕ�̬���룬ֱ��д����һ���Ƚϴ�����飬Ҫ����Ļ����ͻ������
    static bool img[640][640], vis[645][645];

    int ROW = 0, COL = 0;

    to_text_image(img, text, is_fan, num_row, num_col, ROW, COL, 0);


    int fx[4][2] = { {0,1}, {1,0}, {0,-1}, {-1,0} };
    int curI = 0, curJ = 0, nxtI, nxtJ, curF = 0, cnt = ROW * COL;
    //vis��ʼ������
    for (int i = 0; i < ROW; i++)
        for (int j = 0; j < COL; j++)
            vis[i][j] = 0;
    while (cnt--) {
        hdc_point((curJ + 1) * hdc_sz, (curI + 1) * hdc_sz, hdc_d, img[curI][curJ] ? fgcolor : bgcolor);
        vis[curI][curJ] = 1;

        nxtI = curI + fx[curF][0];
        nxtJ = curJ + fx[curF][1];
        if (nxtI < 0 || nxtI >= ROW || nxtJ < 0 || nxtJ >= COL || vis[nxtI][nxtJ])
            curF = (curF + 1) % 4;
        curI += fx[curF][0];
        curJ += fx[curF][1];

    }



    GetKey('\r', '\r');
    cct_setconsoleborder(120, 30, 120, 9001);
    cct_setfontsize("������", 16);
    cct_setcolor();
    cct_cls();

    puts("\n��c����");
    GetKey('c', 'c', 0);
}


void print_dissolve(const char *text, int is_fan, int num_row, int num_col, int bgcolor, int fgcolor, int size)
{
    //int sz = size & 0xff, hdc_sz = sz * 8, hdc_d = size >> 8;
    DEF_SZ_D;
    hdc_init(0, 7, (num_col * 16 + 3) * hdc_sz, (num_row * 17 + 3) * hdc_sz);

    //û�п��Ƕ�̬���룬ֱ��д����һ���Ƚϴ�����飬Ҫ����Ļ����ͻ������
    static bool img[640][640];

    struct node {
        int i, j, hash;
        bool operator<(const node &y)
        {
            return hash < y.hash;
        }
    };
    static node pix[640 * 640];

    int ROW = 0, COL = 0;

    to_text_image(img, text, is_fan, num_row, num_col, ROW, COL, 0);

    int curI = 0, curJ = 0, cnt = 0;

    //�������hash�Ա�������˳��ó�
    for (int i = 0; i < ROW; i++)
        for (int j = 0; j < COL; j++) {
            cnt++;
            pix[cnt].i = i;
            pix[cnt].j = j;
            pix[cnt].hash = rand() % 1000;
        }

    //���˳��
    sort(pix, pix + cnt - 1);

    while (cnt) {
        curI = pix[cnt].i;
        curJ = pix[cnt].j;
        hdc_point((curJ + 1) * hdc_sz, (curI + 1) * hdc_sz, hdc_d, img[curI][curJ] ? fgcolor : 0xffffffff);
        cnt--;
    }

    GetKey('\r', '\r');


    cct_setconsoleborder(120, 30, 120, 9001);
    cct_setfontsize("������", 16);
    cct_setcolor();
    cct_cls();

    puts("\n��c����");
    GetKey('c', 'c', 0);
}



void print_inf(const char *text, int is_fan, int num_row, int num_col, int bgcolor, int fgcolor, int size)
{
    int key;
    //hdc_cls();
    const char *p = text;

    while ((key = AskKey()) != '\r' && key != 'c') {
        hdc_cls();
        _print_basic(p, is_fan, num_row, num_col, RGB(rand() % 64, rand() % 64, rand() % 64), RGB(rand() % 32 * 8, rand() % 32 * 8, rand() % 32 * 8), size, 1, 0, 1);

        for (int i = num_col*num_row; *p > 0 || i>0; ) {
            if (*p < 0) {
                p += 2;
                i--;
            }
            else if (*p > 0) {
                p++;
            }
            else {
                p = text;
                break;
            }
        }

        Sleep(1000);
    }

    cct_setconsoleborder(120, 30, 120, 9001);
    cct_setfontsize("������", 16);
    cct_setcolor();
    cct_cls();

    puts("\n��c����");
    GetKey('c', 'c', 0);
}