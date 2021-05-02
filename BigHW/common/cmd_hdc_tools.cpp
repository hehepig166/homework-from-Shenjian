/* 朱泽凯 2051995 济美 */

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include <cmath>
#include "../include/cmd_hdc_tools.h"
using namespace std;

#if !HDC_SERIES_BY_TEACHER	//未定义的情况下才打开条件编译

/* 允许添加需要的头文件 */

extern "C" WINBASEAPI HWND WINAPI GetConsoleWindow(); //VS中此处会有波浪线，不影响正常使用

/* 已定义的静态全局变量 */
static HWND hWnd = GetConsoleWindow();
static HDC hdc = NULL;
static const double PI = 3.14159;
static int _BgColor_, _FgColor_, _Width_, _High;
static int _Delay_ms;

/* 允许适度使用静态全局变量，但仅限static，即外部不可见 */

/* 此处允许添加自定义函数，但仅限static，即外部不可见 */
static inline void hdc_base_line(const int x1, const int y1, const int x2, const int y2);

//从 angle_begin 一直画到 angle_end，折线近似圆弧
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

        /* 注意，系统的坐标轴，0°在圆的最下方 */
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

/* 下面给出了几个基本函数的完整实现，不要改动 */
/***************************************************************************
  函数名称：
  功    能：初始化
  输入参数：const int bgcolor：背景色
            const int fgcolor：前景色
            const int width  ：屏幕宽度（点阵）
            const int high   ：屏幕高度（点阵）
  返 回 值：
  说    明：
***************************************************************************/
void hdc_init(const int bgcolor, const int fgcolor, const int width, const int high)
{
    /* 先释放，防止不release而再次init（hdc_release可重入） */
    hdc_release();

    /* 窗口init后，用一个静态全局量记录，后续hdc_cls()会用到 */
    _BgColor_ = bgcolor;
    _FgColor_ = fgcolor;
    _Width_ = width;
    _High = high;

    hdc = GetDC(hWnd);

    cct_setcursor(CURSOR_INVISIBLE);
    cct_setcolor(bgcolor, fgcolor);
    cct_setfontsize("点阵字体", 16);
    cct_setconsoleborder(width / 8 + !!(width % 8), high / 16 + !!(high % 16)); //将点阵的宽度及高度转换为特定字体的行列数，!!的含义：如果不是8/16的倍数，行列多+1
    cct_cls();
}

/***************************************************************************
  函数名称：
  功    能：释放画图资源
  输入参数：
  返 回 值：
  说    明：可重入
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
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_set_delay(int ms)
{
    if (ms > 0)
        _Delay_ms = ms;
    else
        _Delay_ms = 0;
}

/***************************************************************************
  函数名称：
  功    能：设置画笔颜色，传入RGB值
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_set_pencolor(const int RGB_value)
{
    SelectObject(hdc, GetStockObject(DC_PEN));
    SetDCPenColor(hdc, RGB_value);
}

/***************************************************************************
  函数名称：
  功    能：设置画笔颜色，传入RGB三色，值0-255
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_set_pencolor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
    hdc_set_pencolor(RGB(red, green, blue));
}

/***************************************************************************
  函数名称：
  功    能：清除屏幕上现有的图形
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_cls()
{
    /* 发现一定要换一种颜色初始化才能清除像素点，找到更简便方法的同学可以通知我 */
    hdc_init(_BgColor_, (_FgColor_ + 1) % 16, _Width_, _High);

    hdc_init(_BgColor_, _FgColor_, _Width_, _High);

    /* 部分机器上运行demo时，会出现hdc_cls()后第一根线有缺线的情况，加延时即可
       如果部分机器运行还有问题，调高此延时值 */
    Sleep(30);
}

/***************************************************************************
  函数名称：
  功    能：在(x,y)位置处用指定颜色画出一个像素点
  输入参数：const int x：x坐标，左上角为(0,0)
            const int y：y坐标，左上角为(0,0)
  返 回 值：
  说    明：颜色直接用当前设定，可以在调用处指定
***************************************************************************/
static inline void hdc_base_point(const int x, const int y)
{
    MoveToEx(hdc, x - 1, y - 1, NULL);
    LineTo(hdc, x, y);
    if (_Delay_ms > 0)
        Sleep(_Delay_ms);
}

/***************************************************************************
  函数名称：
  功    能：在(x1,y1)-(x2,y2)之间画出一个像素点的连线
  输入参数：const int x1：起点x坐标，左上角为(0,0)
            const int y1：起点y坐标，左上角为(0,0)
            const int x2：终点y坐标，左上角为(0,0)
            const int y2：终点y坐标，左上角为(0,0)
  返 回 值：
  说    明：颜色直接用当前设定
***************************************************************************/
static inline void hdc_base_line(const int x1, const int y1, const int x2, const int y2)
{
    MoveToEx(hdc, x1, y1, NULL);
    LineTo(hdc, x2, y2);
    if (_Delay_ms > 0)
        Sleep(_Delay_ms);
}

/***************************************************************************
  函数名称：
  功    能：在(x,y)位置处用指定颜色画一个指定粗细的点(用画实心圆来模拟)
  输入参数：const int x			：x坐标，左上角为(0,0)
            const int y			：y坐标，左上角为(0,0)
            const int thickness	：点的粗细，下限位1，上限不限(如过大会导致不完全填充)（有缺省值）
            const int RGB_value	：点的颜色（有缺省值）
  返 回 值：
  说    明：改进运行效率
***************************************************************************/
void hdc_point(const int x, const int y, const int thickness, const int RGB_value)
{
    int tn = thickness;
    if (tn < 1)
        tn = 1;		//处理下限
    /* 不卡点直径的上限，但是单上限超过一定大小是，画出的圆部分位置未实心 */

    const int tn_end = (tn <= 1) ? 1 : tn / 2;

    int angle, level;
    int old_x1 = INT_MAX, old_y1 = INT_MAX, x1, y1;
    int count = 0;

    if (RGB_value != INVALID_RGB)
        hdc_set_pencolor(RGB_value);

    /* 用画圆的方式模拟粗点 */
    for (level = 1; level <= tn_end; level++) {
        for (angle = 0; angle <= 360; angle++) {
            /* 注意，系统的坐标轴，0°在圆的最下方 */
            x1 = x + (int)(level * sin(angle * PI / 180));
            y1 = y - (int)(level * cos(angle * PI / 180));

            /* 当半径很小时，角度变化不会引起int型的x1/y1变化，因此加判断语句，避免重复画同一像素点 */
            if (x1 != old_x1 || y1 != old_y1) {
                old_x1 = x1;
                old_y1 = y1;
                hdc_base_point(x1, y1);
                ++count;
            }
        }
    }
#if 0
    /* 放开此注释，可以看到一个粗点用了几个像素点组成 */
    cct_gotoxy(0, 41);
    printf("count=%d           ", count);
    getchar();
#endif
}

/* -------------------------------------------------------------------------
    给出下列函数的实现，函数名及参数表不准改动
    1、不需要调用系统的其他有关图形操作的函数
    2、下列函数的实现都基于hdc_base_point/hdc_base_line函数的组合
    3、想一想，是不是所有函数都需要给出独立的实现过程？应该先实现哪些函数？
    4、填充是实现中最复杂的部分
    5、系统的角度与函数参数中的角度含义相差180°
   ------------------------------------------------------------------------- */

   /***************************************************************************
     函数名称：
     功    能：画线段
     输入参数：const int x1		：起点的x
               const int y1		：起点的y
               const int x2		：终点的x
               const int y2		：终点的y
               const int thickness	：线段的粗细（有缺省值）
               const int RGB_value	：线段的颜色（有缺省值）
     返 回 值：
     说    明：本函数可以用基于hdc_point的组合（速度慢）
                     也可以用hdc_base_point/hdc_base_line的组合（速度快）
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
        tn = 1;		//处理下限

    const int tn_end = tn >> 1;

    hdc_base_line(x1, y1, x2, y2);	//主干
    if (tn_end == 0) {
        return;
    }

    //加粗

    hdc_circle(x1, y1, thickness>>1, true);
    hdc_circle(x2, y2, thickness >> 1, true);

    int x = x2 - x1, y = y2 - y1;	//线段向量
    double len = sqrt(x * x + y * y);
    double dx = -y / len, dy = x / len, k = -(double)x / y; 	//垂直单位向量与累计变量，确保dx是正的
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

    //注意确保dx是正的

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
  函数名称：
  功    能：给出三点的坐标，画一个三角形
  输入参数：const int x1		：第1个点的x
            const int y1		：第1个点的y
            const int x2		：第2个点的x
            const int y2		：第2个点的y
            const int x3		：第3个点的x
            const int y3		：第3个点的y
            bool filled			：是否需要填充（有缺省值）
            const int thickness	：边的粗细（有缺省值）
            const int RGB_value	：颜色（有缺省值）
  返 回 值：
  说    明：不判断三点是否共线，如果共线，划出一条直线即可
***************************************************************************/
void hdc_triangle(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3, bool filled, const int thickness, const int RGB_value)
{
    if (RGB_value != INVALID_RGB)
        hdc_set_pencolor(RGB_value);

    //三点共线
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
  函数名称：
  功    能：给出左上角坐标及宽度、高度，画出一个长方形
  输入参数：const int left_up_x			：左上角x
            const int left_up_y			：左上角y
            const int width				：宽度
            const int high				：高度
            const int rotation_angles	：以左上角为支点，与x轴的旋转倾角（向下转为正）
            bool filled					：是否需要填充（有缺省值）
            const int thickness			：边的粗细（有缺省值）
            const int RGB_value			：颜色（有缺省值）
  返 回 值：
  说    明：
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

    //四边
    hdc_line(left_up_x, left_up_y, X2, Y2, thickness);
    hdc_line(X2, Y2, X4, Y4, thickness);
    hdc_line(X4, Y4, X3, Y3, thickness);
    hdc_line(X3, Y3, left_up_x, left_up_y, thickness);

    //填充
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
  函数名称：
  功    能：给出左上角坐标及边长，画出一个长方形
  输入参数：const int left_up_x			：左上角x
            const int left_up_y			：左上角y
            const int length			：边长
            const int rotation_angles	：以左上角为支点，与x轴的旋转倾角（向下转为正）（有缺省值）
            const bool filled			：是否需要填充（有缺省值）
            const int thickness			：边的粗细（有缺省值）
            const int RGB_value			：颜色（有缺省值）
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_square(const int left_up_x, const int left_up_y, const int length, const int rotation_angles, const bool filled, const int thickness, const int RGB_value)
{
    hdc_rectangle(left_up_x, left_up_y, length, length, rotation_angles, filled, thickness, RGB_value);
}

/***************************************************************************
  函数名称：
  功    能：按照给出的参数画一段圆弧
  输入参数：const int point_x		：圆心的x
            const int point_y		：圆心的y
            const int radius		：半径
            const int angle_begin	：起始角度（以圆的最上方为0°，顺时针为正，注意：与系统坐标差了180°!!!）（有缺省值）
            const int angle_end		：结束角度（同上）（有缺省值）
            const int thickness		：粗细（有缺省值）
            const int RGB_value		：颜色（有缺省值）
  返 回 值：
  说    明：
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
  函数名称：
  功    能：按照给出的参数画一个扇形
  输入参数：const int point_x		：圆心的x
            const int point_y		：圆心的y
            const int radius		：半径
            const int angle_begin	：起始角度（以圆的最上方为0°，顺时针为正，注意：与系统坐标差了180°!!!）（有缺省值）
            const int angle_end		：结束角度（同上）（有缺省值）
            const bool filled		：是否需要填充（有缺省值）
            const int thickness		：粗细（有缺省值）
            const int RGB_value		：颜色（有缺省值）
  输入参数：
  返 回 值：
  说    明：当起始/结束角度差值为360的倍数时，不画两条边
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
  函数名称：
  功    能：按照给出的参数画一个圆
  输入参数：const int point_x		：圆心的x
            const int point_y		：圆心的y
            const int radius		：半径
            const bool filled		：是否需要填充（有缺省值）
            const int thickness		：粗细（有缺省值）
            const int RGB_value		：颜色（有缺省值）
  返 回 值：
  说    明：
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
  函数名称：
  功    能：按照给出的参数画一个椭圆
  输入参数：const int point_x			：圆心的x
            const int point_y			：圆心的y
            const int radius_a			：平行于X轴的半径
            const int radius_b			：平行于Y轴的半径
            const int rotation_angles	：以圆心为支点，与x轴的旋转倾角（向下转为正）（有缺省值）
            const bool filled			：是否需要填充（有缺省值）
            const int thickness			：粗细（有缺省值）
            const int RGB_value			：颜色（有缺省值）
  返 回 值：
  说    明：
***************************************************************************/
void hdc_ellipse(const int point_x, const int point_y, const int radius_a, const int radius_b, const int rotation_angles, const bool filled, const int thickness, const int RGB_value)
{
    if (RGB_value != INVALID_RGB)
        hdc_set_pencolor(RGB_value);

    int angle;
    int x, y, old_x, old_y;
    double Sin = sin(rotation_angles*PI/180), Cos = cos(rotation_angles*PI/180);
    
    if (!filled) {
        int rx, ry;     //旋转后坐标（原点为中心）
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

    int ri1, rj1, ri2, rj2; //旋转后的坐标（原点为中心）

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
