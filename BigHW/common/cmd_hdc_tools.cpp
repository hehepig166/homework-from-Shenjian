/* ���� 2051995 ���� */

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include <cmath>
#include "../include/cmd_hdc_tools.h"
using namespace std;

#if !HDC_SERIES_BY_TEACHER	//δ���������²Ŵ���������

/* ���������Ҫ��ͷ�ļ� */

extern "C" WINBASEAPI HWND WINAPI GetConsoleWindow(); //VS�д˴����в����ߣ���Ӱ������ʹ��

/* �Ѷ���ľ�̬ȫ�ֱ��� */
static HWND hWnd = GetConsoleWindow();
static HDC hdc = NULL;
static const double PI = 3.14159;
static int _BgColor_, _FgColor_, _Width_, _High;
static int _Delay_ms;

/* �����ʶ�ʹ�þ�̬ȫ�ֱ�����������static�����ⲿ���ɼ� */

/* �˴���������Զ��庯����������static�����ⲿ���ɼ� */
static inline void hdc_base_line(const int x1, const int y1, const int x2, const int y2);

//�� angle_begin һֱ���� angle_end�����߽���Բ��
static void my_base_arc(const int point_x, const int point_y, const int radius, const int angle_begin, const int angle_end, int thickness)
{
    int angle, da = angle_end > angle_begin ? 1 : -1, ea = angle_end + da;
    int x, y, old_x, old_y;
    double Sin, Cos, old_Sin, old_Cos;
    int tn_end = thickness >> 1;
    int R = radius + (thickness >> 1);

    //old_x = point_x + (int)(radius * (old_Sin = sin(angle_begin * PI / 180)));
    //old_y = point_y - (int)(radius * (old_Cos = cos(angle_begin * PI / 180)));
    old_x = point_x + (int)(R * (old_Sin = sin(angle_begin * PI / 180)));
    old_y = point_y - (int)(R * (old_Cos = cos(angle_begin * PI / 180)));

    for (angle = angle_begin + da; angle != ea; angle += da) {

        /* ע�⣬ϵͳ�������ᣬ0����Բ�����·� */
        //x = point_x + (int)(radius * (Sin = sin(angle * PI / 180)));
        //y = point_y - (int)(radius * (Cos = cos(angle * PI / 180)));
        x = point_x + (int)(R * (Sin = sin(angle * PI / 180)));
        y = point_y - (int)(R * (Cos = cos(angle * PI / 180)));
        if (x != old_x || y != old_y) {
            hdc_base_line(old_x, old_y, x, y);

            for (int t = 1; t < thickness; t++) {
                //hdc_base_line((int)(old_x + old_Sin * t), (int)(old_y - old_Cos * t), (int)(x + Sin * t), (int)(y - Cos * t));
                hdc_base_line((int)(old_x - old_Sin * t), (int)(old_y + old_Cos * t), (int)(x - Sin * t), (int)(y + Cos * t));
                hdc_base_line(old_x, old_y, (int)(x - Sin * t), (int)(y + Cos * t));
                hdc_base_line((int)(old_x - old_Sin * t), (int)(old_y + old_Cos * t), x, y);
                //hdc_base_line((int)(old_x + old_Sin * t), (int)(old_y - old_Cos * t), (int)(x - Sin * t), (int)(y + Cos * t));
                //hdc_base_line((int)(old_x - old_Sin * t), (int)(old_y + old_Cos * t), (int)(x + Sin * t), (int)(y - Cos * t));
            }

            old_x = x;
            old_y = y;
            old_Sin = Sin;
            old_Cos = Cos;
        }
    }
}

/* ��������˼�����������������ʵ�֣���Ҫ�Ķ� */
/***************************************************************************
  �������ƣ�
  ��    �ܣ���ʼ��
  ���������const int bgcolor������ɫ
            const int fgcolor��ǰ��ɫ
            const int width  ����Ļ��ȣ�����
            const int high   ����Ļ�߶ȣ�����
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_init(const int bgcolor, const int fgcolor, const int width, const int high)
{
    /* ���ͷţ���ֹ��release���ٴ�init��hdc_release�����룩 */
    hdc_release();

    /* ����init����һ����̬ȫ������¼������hdc_cls()���õ� */
    _BgColor_ = bgcolor;
    _FgColor_ = fgcolor;
    _Width_ = width;
    _High = high;

    hdc = GetDC(hWnd);

    cct_setcursor(CURSOR_INVISIBLE);
    cct_setcolor(bgcolor, fgcolor);
    cct_setfontsize("��������", 16);
    cct_setconsoleborder(width / 8 + !!(width % 8), high / 16 + !!(high % 16)); //������Ŀ�ȼ��߶�ת��Ϊ�ض��������������!!�ĺ��壺�������8/16�ı��������ж�+1
    cct_cls();
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��ͷŻ�ͼ��Դ
  ���������
  �� �� ֵ��
  ˵    ����������
***************************************************************************/
void hdc_release()
{
    if (hdc) {
        ReleaseDC(hWnd, hdc);
        hdc = NULL;
        cct_setcursor(CURSOR_VISIBLE_NORMAL);
    }
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_set_delay(int ms)
{
    if (ms > 0)
        _Delay_ms = ms;
    else
        _Delay_ms = 0;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����û�����ɫ������RGBֵ
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_set_pencolor(const int RGB_value)
{
    SelectObject(hdc, GetStockObject(DC_PEN));
    SetDCPenColor(hdc, RGB_value);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����û�����ɫ������RGB��ɫ��ֵ0-255
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_set_pencolor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
    hdc_set_pencolor(RGB(red, green, blue));
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������Ļ�����е�ͼ��
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_cls()
{
    /* ����һ��Ҫ��һ����ɫ��ʼ������������ص㣬�ҵ�����㷽����ͬѧ����֪ͨ�� */
    hdc_init(_BgColor_, (_FgColor_ + 1) % 16, _Width_, _High);

    hdc_init(_BgColor_, _FgColor_, _Width_, _High);

    /* ���ֻ���������demoʱ�������hdc_cls()���һ������ȱ�ߵ����������ʱ����
       ������ֻ������л������⣬���ߴ���ʱֵ */
    Sleep(30);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���(x,y)λ�ô���ָ����ɫ����һ�����ص�
  ���������const int x��x���꣬���Ͻ�Ϊ(0,0)
            const int y��y���꣬���Ͻ�Ϊ(0,0)
  �� �� ֵ��
  ˵    ������ɫֱ���õ�ǰ�趨�������ڵ��ô�ָ��
***************************************************************************/
static inline void hdc_base_point(const int x, const int y)
{
    MoveToEx(hdc, x - 1, y - 1, NULL);
    LineTo(hdc, x, y);
    if (_Delay_ms > 0)
        Sleep(_Delay_ms);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���(x1,y1)-(x2,y2)֮�仭��һ�����ص������
  ���������const int x1�����x���꣬���Ͻ�Ϊ(0,0)
            const int y1�����y���꣬���Ͻ�Ϊ(0,0)
            const int x2���յ�y���꣬���Ͻ�Ϊ(0,0)
            const int y2���յ�y���꣬���Ͻ�Ϊ(0,0)
  �� �� ֵ��
  ˵    ������ɫֱ���õ�ǰ�趨
***************************************************************************/
static inline void hdc_base_line(const int x1, const int y1, const int x2, const int y2)
{
    MoveToEx(hdc, x1, y1, NULL);
    LineTo(hdc, x2, y2);
    if (_Delay_ms > 0)
        Sleep(_Delay_ms);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���(x,y)λ�ô���ָ����ɫ��һ��ָ����ϸ�ĵ�(�û�ʵ��Բ��ģ��)
  ���������const int x			��x���꣬���Ͻ�Ϊ(0,0)
            const int y			��y���꣬���Ͻ�Ϊ(0,0)
            const int thickness	����Ĵ�ϸ������λ1�����޲���(�����ᵼ�²���ȫ���)����ȱʡֵ��
            const int RGB_value	�������ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    �����Ľ�����Ч��
***************************************************************************/
void hdc_point(const int x, const int y, const int thickness, const int RGB_value)
{
    int tn = thickness;
    if (tn < 1)
        tn = 1;		//��������
    /* ������ֱ�������ޣ����ǵ����޳���һ����С�ǣ�������Բ����λ��δʵ�� */

    const int tn_end = (tn <= 1) ? 1 : tn / 2;

    int angle, level;
    int old_x1 = INT_MAX, old_y1 = INT_MAX, x1, y1;
    int count = 0;

    if (RGB_value != INVALID_RGB)
        hdc_set_pencolor(RGB_value);

    /* �û�Բ�ķ�ʽģ��ֵ� */
    for (level = 1; level <= tn_end; level++) {
        for (angle = 0; angle <= 360; angle++) {
            /* ע�⣬ϵͳ�������ᣬ0����Բ�����·� */
            x1 = x + (int)(level * sin(angle * PI / 180));
            y1 = y - (int)(level * cos(angle * PI / 180));

            /* ���뾶��Сʱ���Ƕȱ仯��������int�͵�x1/y1�仯����˼��ж���䣬�����ظ���ͬһ���ص� */
            if (x1 != old_x1 || y1 != old_y1) {
                old_x1 = x1;
                old_y1 = y1;
                hdc_base_point(x1, y1);
                ++count;
            }
        }
    }
#if 0
    /* �ſ���ע�ͣ����Կ���һ���ֵ����˼������ص���� */
    cct_gotoxy(0, 41);
    printf("count=%d           ", count);
    getchar();
#endif
}

/* -------------------------------------------------------------------------
    �������к�����ʵ�֣���������������׼�Ķ�
    1������Ҫ����ϵͳ�������й�ͼ�β����ĺ���
    2�����к�����ʵ�ֶ�����hdc_base_point/hdc_base_line���������
    3����һ�룬�ǲ������к�������Ҫ����������ʵ�ֹ��̣�Ӧ����ʵ����Щ������
    4�������ʵ������ӵĲ���
    5��ϵͳ�ĽǶ��뺯�������еĽǶȺ������180��
   ------------------------------------------------------------------------- */

   /***************************************************************************
     �������ƣ�
     ��    �ܣ����߶�
     ���������const int x1		������x
               const int y1		������y
               const int x2		���յ��x
               const int y2		���յ��y
               const int thickness	���߶εĴ�ϸ����ȱʡֵ��
               const int RGB_value	���߶ε���ɫ����ȱʡֵ��
     �� �� ֵ��
     ˵    ���������������û���hdc_point����ϣ��ٶ�����
                     Ҳ������hdc_base_point/hdc_base_line����ϣ��ٶȿ죩
   ***************************************************************************/
void hdc_line(const int x1, const int y1, const int x2, const int y2, const int thickness, const int RGB_value)
{
    if (RGB_value != INVALID_RGB)
        hdc_set_pencolor(RGB_value);

    if (x1 == x2 && y1 == y2) {
        hdc_point(x1, y1, thickness, RGB_value);
        return;
    }

    int tn = thickness;
    if (tn < 1)
        tn = 1;		//��������

    const int tn_end = tn >> 1;

    hdc_base_line(x1, y1, x2, y2);	//����
    if (tn_end == 0) {
        return;
    }

    //�Ӵ�

    hdc_circle(x1, y1, thickness>>1, true);
    hdc_circle(x2, y2, thickness >> 1, true);

    int x = x2 - x1, y = y2 - y1;	//�߶�����
    double len = sqrt(x * x + y * y);
    double dx = -y / len, dy = x / len, k = -(double)x / y; 	//��ֱ��λ�������ۼƱ�����ȷ��dx������
    if (dx < 0) {
        dx = -dx;
        dy = -dy;
    }

    if (x == 0) {
        for (int i = 1; i <= tn_end; i++) {
            hdc_base_line(x1 + i, y1, x2 + i, y2);
            hdc_base_line(x1 - i, y1, x2 - i, y2);
        }
        return;
    }
    if (y == 0) {
        for (int i = 1; i <= tn_end; i++) {
            hdc_base_line(x1, y1 + i, x2, y2 + i);
            hdc_base_line(x1, y1 - i, x2, y2 - i);
        }
        return;
    }

    //ע��ȷ��dx������

    /*
    int di = dx > 0 ? 1 : -1;
    int dj = dy > 0 ? 1 : -1;
    int si = -int(dx * tn_end), mi = (int)round(dx * tn_end);
    int sj = -int(dy * tn_end), mj = (int)round(dy * tn_end);
    for (int i = si,j=sj,tj; i != mi+di; i+=di,j-=dj) {
        for (tj = int(k * i)+dj; j != tj && j != mj+dj; j+=dj) {
            hdc_base_line(x1 + i, y1 + j, x2 + i, y2 + j);
        }
    }
    */

    for (int i = 0, dj = (dy > 0 ? 1 : -1), j = dj, mi = abs((int)round(tn_end * dx)), mj = (int)(tn_end * dy) + dj; i < mi || j != mj; ) {
        i++;
        j -= dj;
        for (int jj = abs(i * x / y); jj >= 0 && j != mj; j += dj, jj--) {
            hdc_base_line(x1 + i, y1 + j, x2 + i, y2 + j);
            hdc_base_line(x1 - i, y1 - j, x2 - i, y2 - j);
        }
    }

}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�������������꣬��һ��������
  ���������const int x1		����1�����x
            const int y1		����1�����y
            const int x2		����2�����x
            const int y2		����2�����y
            const int x3		����3�����x
            const int y3		����3�����y
            bool filled			���Ƿ���Ҫ��䣨��ȱʡֵ��
            const int thickness	���ߵĴ�ϸ����ȱʡֵ��
            const int RGB_value	����ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    �������ж������Ƿ��ߣ�������ߣ�����һ��ֱ�߼���
***************************************************************************/
void hdc_triangle(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3, bool filled, const int thickness, const int RGB_value)
{
    if (RGB_value != INVALID_RGB)
        hdc_set_pencolor(RGB_value);

    //���㹲��
    if ((x2 - x1) * (x3 - x1) + (y2 - y1) * (y3 - y1) == 0) {
        if ((x1 - x2) * (x1 - x3) >= 0 || (y1 - y2) * (y1 - y3) >= 0) {
            hdc_line(x2, y2, x3, y3, thickness);
            return;
        }
        else if ((x2 - x1) * (x2 - x3) >= 0 || (y2 - y1) * (y2 - y3) >= 0) {
            hdc_line(x1, y1, x3, y3, thickness);
            return;
        }
        else {
            hdc_line(x1, y1, x2, y2, thickness);
            return;
        }
    }


    hdc_line(x1, y1, x2, y2, thickness);
    hdc_line(x2, y2, x3, y3, thickness);
    hdc_line(x3, y3, x1, y1, thickness);

    if (filled) {
        int X0, Y0, X1, Y1, X2, Y2, x, y, dx, dy;
        double k;
        int d3 = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
        int d2 = (x1 - x3) * (x1 - x3) + (y1 - y3) * (y1 - y3);
        int d1 = (x2 - x3) * (x2 - x3) + (y2 - y3) * (y2 - y3);
        if (d1 <= d2 && d1 <= d3) {
            X0 = x1, Y0 = y1;
            X1 = x2, Y1 = y2;
            X2 = x3, Y2 = y3;
        }
        else {
            if (d2 < d3) {
                X0 = x2, Y0 = y2;
                X1 = x1, Y1 = y1;
                X2 = x3, Y2 = y3;
            }
            else {
                X0 = x3, Y0 = y3;
                X1 = x1, Y1 = y1;
                X2 = x2, Y2 = y2;
            }
        }

        if (X1 == X2) {
            for (int yy = Y1; yy <= Y2; yy++)
                hdc_base_line(X0, Y0, X1, yy);
        }

        x = X2 - X1, y = Y2 - Y1;
        dx = x > 0 ? 1 : -1;
        dy = y > 0 ? 1 : -1;
        k = double(y) / double(x);

        for (int xx = X1, yy = Y1, my; xx != X2 + dx; xx += dx, yy -= dy) {
            for (my = int(k * (xx - X1) + Y1 + dy); yy != my && yy != Y2 + dy; yy += dy) {
                hdc_base_line(X0, Y0, xx, yy);
            }
        }
    }
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��������Ͻ����꼰��ȡ��߶ȣ�����һ��������
  ���������const int left_up_x			�����Ͻ�x
            const int left_up_y			�����Ͻ�y
            const int width				�����
            const int high				���߶�
            const int rotation_angles	�������Ͻ�Ϊ֧�㣬��x�����ת��ǣ�����תΪ����
            bool filled					���Ƿ���Ҫ��䣨��ȱʡֵ��
            const int thickness			���ߵĴ�ϸ����ȱʡֵ��
            const int RGB_value			����ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_rectangle(const int left_up_x, const int left_up_y, const int width, const int high, const int rotation_angles, const bool filled, const int thickness, const int RGB_value)
{
    if (RGB_value != INVALID_RGB)
        hdc_set_pencolor(RGB_value);

    double Cos = cos(rotation_angles * PI / 180), Sin = sin(rotation_angles * PI / 180);
    int Wx = (int)round(width * Cos), Wy = (int)round(width * Sin);
    int X2 = left_up_x + int(width * Cos);
    int Y2 = left_up_y + int(width * Sin);
    int X3 = left_up_x + int(-high * Sin);
    int Y3 = left_up_y + int(high * Cos);
    int X4 = left_up_x + int(width * Cos - high * Sin);
    int Y4 = left_up_y + int(width * Sin + high * Cos);

    //�ı�
    hdc_line(left_up_x, left_up_y, X2, Y2, thickness);
    hdc_line(X2, Y2, X4, Y4, thickness);
    hdc_line(X4, Y4, X3, Y3, thickness);
    hdc_line(X3, Y3, left_up_x, left_up_y, thickness);

    //���
    if (filled) {
        double dHx = -Sin, dHy = Cos, k = dHy / dHx;
        int curX = left_up_x, curY = left_up_y, nxtY;
        int dx = dHx > 0 ? 1 : -1;
        int dy = dHy > 0 ? 1 : -1;

        while (1) {
            //hdc_base_line(curX, curY, curX + Wx, curY + Wy);
            nxtY = left_up_y + int((curX - left_up_x) * k);
            do {
                hdc_base_line(curX, curY, curX + Wx, curY + Wy);
            } while (curY != nxtY && curY != Y3 && (curY += dy, 1));

            if (curX != X3) {
                curX += dx;
            }
            else {
                while (curY != Y3) {
                    curY += dy;
                    hdc_base_line(curX, curY, curX + Wx, curY + Wy);
                }
                break;
            }

        }
    }
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��������Ͻ����꼰�߳�������һ��������
  ���������const int left_up_x			�����Ͻ�x
            const int left_up_y			�����Ͻ�y
            const int length			���߳�
            const int rotation_angles	�������Ͻ�Ϊ֧�㣬��x�����ת��ǣ�����תΪ��������ȱʡֵ��
            const bool filled			���Ƿ���Ҫ��䣨��ȱʡֵ��
            const int thickness			���ߵĴ�ϸ����ȱʡֵ��
            const int RGB_value			����ɫ����ȱʡֵ��
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_square(const int left_up_x, const int left_up_y, const int length, const int rotation_angles, const bool filled, const int thickness, const int RGB_value)
{
    hdc_rectangle(left_up_x, left_up_y, length, length, rotation_angles, filled, thickness, RGB_value);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ո����Ĳ�����һ��Բ��
  ���������const int point_x		��Բ�ĵ�x
            const int point_y		��Բ�ĵ�y
            const int radius		���뾶
            const int angle_begin	����ʼ�Ƕȣ���Բ�����Ϸ�Ϊ0�㣬˳ʱ��Ϊ����ע�⣺��ϵͳ�������180��!!!������ȱʡֵ��
            const int angle_end		�������Ƕȣ�ͬ�ϣ�����ȱʡֵ��
            const int thickness		����ϸ����ȱʡֵ��
            const int RGB_value		����ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_arc(const int point_x, const int point_y, const int radius, const int angle_begin, const int angle_end, const int thickness, const int RGB_value)
{
    if (RGB_value != INVALID_RGB)
        hdc_set_pencolor(RGB_value);

    int angle_b, angle_e;
    angle_b = angle_begin % 360;
    angle_e = angle_end % 360;
    if (angle_e - angle_b > 360) {
        angle_e -= 360;
    }
    else if (angle_e - angle_b <= 0) {
        angle_e += 360;
    }

    my_base_arc(point_x, point_y, radius, angle_b, angle_e, thickness);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ո����Ĳ�����һ������
  ���������const int point_x		��Բ�ĵ�x
            const int point_y		��Բ�ĵ�y
            const int radius		���뾶
            const int angle_begin	����ʼ�Ƕȣ���Բ�����Ϸ�Ϊ0�㣬˳ʱ��Ϊ����ע�⣺��ϵͳ�������180��!!!������ȱʡֵ��
            const int angle_end		�������Ƕȣ�ͬ�ϣ�����ȱʡֵ��
            const bool filled		���Ƿ���Ҫ��䣨��ȱʡֵ��
            const int thickness		����ϸ����ȱʡֵ��
            const int RGB_value		����ɫ����ȱʡֵ��
  ���������
  �� �� ֵ��
  ˵    ��������ʼ/�����ǶȲ�ֵΪ360�ı���ʱ������������
***************************************************************************/
void hdc_sector(const int point_x, const int point_y, const int radius, const int angle_begin, const int angle_end, const bool filled, const int thickness, const int RGB_value)
{
    if (RGB_value != INVALID_RGB)
        hdc_set_pencolor(RGB_value);

    int angle_b, angle_e;
    angle_b = (angle_begin) % 360;
    angle_e = (angle_end) % 360;
    if (angle_e - angle_b > 360) {
        angle_e -= 360;
    }
    else if (angle_e - angle_b <= 0) {
        angle_e += 360;
    }

    if (angle_e - angle_b != 360) {
        hdc_line(point_x, point_y, point_x + int(radius * sin(angle_b * PI / 180)), point_y - int(radius * cos(angle_b * PI / 180)),thickness);
        hdc_line(point_x, point_y, point_x + int(radius * sin(angle_e * PI / 180)), point_y - int(radius * cos(angle_e * PI / 180)),thickness);
    }

    my_base_arc(point_x, point_y, radius, angle_b, angle_e, thickness);

    if (filled) {

        if (angle_e - angle_b == 360) {
            hdc_circle(point_x, point_y, radius, true);
        }
        else {
            my_base_arc(point_x, point_y, radius / 2, angle_b, angle_e, radius + 1);
        }
        /*
        for (int angle = angle_b+1; angle < angle_e; angle++) {
            hdc_line(point_x, point_y, point_x + int(radius * sin(angle * PI / 180)), point_y - int(radius * cos(angle * PI / 180)));
        }
        */
    }
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ո����Ĳ�����һ��Բ
  ���������const int point_x		��Բ�ĵ�x
            const int point_y		��Բ�ĵ�y
            const int radius		���뾶
            const bool filled		���Ƿ���Ҫ��䣨��ȱʡֵ��
            const int thickness		����ϸ����ȱʡֵ��
            const int RGB_value		����ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_circle(const int point_x, const int point_y, const int radius, const bool filled, const int thickness, const int RGB_value)
{
    if (RGB_value != INVALID_RGB)
        hdc_set_pencolor(RGB_value);

    if (!filled) {
        my_base_arc(point_x, point_y, radius, 0, 360, thickness);;
        return;
    }

    hdc_base_line(point_x, point_y - radius, point_x, point_y + radius);
    for (int i = radius, j; i > 0; i--) {
        j = (int)sqrt(radius * radius - i * i);
        hdc_base_line(point_x + i, point_y + j, point_x + i, point_y - j - 1);
        hdc_base_line(point_x - i, point_y + j, point_x - i, point_y - j - 1);
    }
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ո����Ĳ�����һ����Բ
  ���������const int point_x			��Բ�ĵ�x
            const int point_y			��Բ�ĵ�y
            const int radius_a			��ƽ����X��İ뾶
            const int radius_b			��ƽ����Y��İ뾶
            const int rotation_angles	����Բ��Ϊ֧�㣬��x�����ת��ǣ�����תΪ��������ȱʡֵ��
            const bool filled			���Ƿ���Ҫ��䣨��ȱʡֵ��
            const int thickness			����ϸ����ȱʡֵ��
            const int RGB_value			����ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_ellipse(const int point_x, const int point_y, const int radius_a, const int radius_b, const int rotation_angles, const bool filled, const int thickness, const int RGB_value)
{
    if (RGB_value != INVALID_RGB)
        hdc_set_pencolor(RGB_value);

    int angle;
    int x, y, old_x, old_y;
    double Sin = sin(rotation_angles*PI/180), Cos = cos(rotation_angles*PI/180);
    
    if (!filled) {
        int rx, ry;     //��ת�����꣨ԭ��Ϊ���ģ�
        old_x = int(radius_a*Cos), old_y =  int(radius_a*Sin);
        for (angle = 1; angle <= 360; angle++) {
            x = int(radius_a * cos(angle * PI / 180));
            y = int(radius_b * sin(angle * PI / 180));

            rx = int(x * Cos - y * Sin);
            ry = int(x * Sin + y * Cos);

            if (x != old_x || y != old_y) {
                hdc_line(point_x+old_x, point_y+old_y, point_x+rx, point_y+ry, thickness);
            }
            old_x = rx;
            old_y = ry;
        }
        return;
    }

    int ri1, rj1, ri2, rj2; //��ת������꣨ԭ��Ϊ���ģ�

    hdc_base_line(int(point_x-radius_b*Sin), int(point_y + radius_b*Cos), int(point_x + radius_b * Sin), int(point_y - radius_b * Cos));
    for (int j = radius_b, i; j > 0; j--) {
        i = (int)ceil(sqrt(radius_a * radius_a - double(radius_a * radius_a)*j*j / radius_b / radius_b));
        ri1 = int(i * Cos - j * Sin);
        rj1 = int(i * Sin + j * Cos);
        ri2 = int(-i * Cos - j * Sin);
        rj2 = int(-i * Sin + j * Cos);
        hdc_line(point_x + ri1, point_y + rj1, point_x + ri2, point_y + rj2, 4);
        hdc_line(point_x - ri1, point_y - rj1, point_x - ri2, point_y - rj2, 4);
    }
}

#endif !HDC_SERIES_BY_TEACHER
