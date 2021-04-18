/* 2051995 朱泽凯 济美 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <climits>
#include <conio.h>
#include <cstring>
#include <Windows.h>
#include "../include/cmd_console_tools.h"
#include "../include/cmd_gmw_tools.h"
using namespace std;

/* --------------------------------------------------
        此处可以给出需要的静态全局变量（尽可能少，最好没有）、静态全局只读变量/宏定义（个数不限）等
   -------------------------------------------------- */

   // cct上网格左上角坐标（即拐角那个字符）
#define START_X (pCGI->start_x + (pCGI->draw_frame_with_row_no << 1))
#define START_Y (pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no)

   /* --------------------------------------------------
           此处可以给出需要的内部辅助工具函数
           1、函数名不限，建议为 gmw_inner_*
           2、个数不限
           3、必须是static函数，确保只在本源文件中使用
      -------------------------------------------------- */

      /// 计算并更新一个CFI(游戏主框架)结构体中的参数(只会改变该CFI中的值)
static int calc_CFI(CONSOLE_GRAPHICS_INFO *const pCGI)
{
    CONSOLE_FRAME_INFO *const pCFI = &(pCGI->CFI);

    // 每个色块附加宽高度
    pCFI->block_width_ext = pCFI->separator ? 2 : 0;
    pCFI->block_high_ext = pCFI->separator ? 1 : 0;

    // 每行列含分隔线的总宽度
    pCFI->bwidth = (pCFI->block_width + pCFI->block_width_ext) * pCGI->col_num + (pCGI->CFI.separator ? 2 : 4);
    pCFI->bhigh = (pCFI->block_high + pCFI->block_high_ext) * pCGI->row_num + (pCGI->CFI.separator ? 1 : 2);

    return 0;
}

/// 计算并更新一个SLI(状态栏信息)结构体中的参数(只会改变该SLI中的值)
/// 尽量在调用calc_CFI之后调用
static int calc_SLI(CONSOLE_GRAPHICS_INFO *const pCGI)
{
    // 状态栏位置
    pCGI->SLI.top_start_x = pCGI->extern_left_cols;
    pCGI->SLI.top_start_y = pCGI->extern_up_lines;
    pCGI->SLI.lower_start_x = pCGI->extern_left_cols;
    pCGI->SLI.lower_start_y = pCGI->extern_up_lines + pCGI->top_status_line + pCGI->draw_frame_with_col_no + pCGI->CFI.bhigh;

    // 状态栏长度=框架长度+行标长度
    pCGI->SLI.width = pCGI->CFI.bwidth + (pCGI->draw_frame_with_row_no ? 2 : 0);

    return 0;
}

/// 计算并更新CGI中的所有参数
static int calc_CGI(CONSOLE_GRAPHICS_INFO *const pCGI)
{
    calc_CFI(pCGI);
    calc_SLI(pCGI);

    // 整个图形界面的主框架区域
    pCGI->start_x = pCGI->extern_left_cols;
    pCGI->start_y = pCGI->extern_up_lines;

    // cmd窗口大小
    pCGI->lines = pCGI->extern_up_lines + pCGI->top_status_line + pCGI->draw_frame_with_col_no + pCGI->CFI.bhigh + pCGI->lower_status_line + pCGI->extern_down_lines + 4;
    pCGI->cols = pCGI->extern_left_cols + (pCGI->draw_frame_with_row_no << 1) + pCGI->CFI.bwidth + pCGI->extern_right_cols + 1;
    return 0;
}

/// 从 fromStr 中复制前 len 个字符(不包括'\0')给 toStr，不足补空格，最后加个 '\0'
static int myStrCpy(char *toStr, const char *fromStr, int len = -1)
{
    if (len >= 0) {
        while (*fromStr && len--)
            *toStr++ = *fromStr++;
        while (len-- > 0)
            *toStr++ = ' ';
        *toStr = 0;
    }
    else {
        while (*fromStr)
            *toStr++ = *fromStr++;
        *toStr = 0;
    }

    return 0;
}

/// (0,0)为框架左上角(即默认是"╔"字符的那个位置)
/// _x 为列坐标，_y 为行坐标
static void myGotoXY(const CONSOLE_GRAPHICS_INFO *const pCGI, int _x, int _y)
{
    // myGotoXY 中的变换后坐标
    static int x, y;

    // 一个坐标占两个char
    _x <<= 1;

    // x偏移 = (全区域最小列坐标 + 行标预留)
    x = _x + START_X;//pCGI->start_x + (pCGI->draw_frame_with_row_no << 1);
    // y偏移 = (全区域最小行坐标 + 上状态栏预留 + 列标预留)
    y = _y + START_Y;//pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no;

    cct_gotoxy(x, y);
}

static char rowNo(int rol)
{
    if (rol < 26)
        return rol + 'A';
    else if (rol < 26 + 26)
        return rol - 26 + 'a';
    else
        return '*';
}

/// 画一个网格
/// ilen,jlen: 网格总大小(包括四周边框)
/// di,dj:(格内长度+separator)
static int draw_grid(const CONSOLE_GRAPHICS_INFO *const pCGI,
    int imin, int jmin, int imax, int jmax, int di, int dj,
    int bgcolor, int fgcolor, bool separator,
    const char *top_left, const char *lower_left, const char *top_right, const char *lower_right,
    const char *h_normal, const char *v_normal,
    int delay_ms = 0,
    const char *h_lower_separator = NULL, const char *h_top_separator = NULL, const char *v_left_separator = NULL, const char *v_right_separator = NULL, const char *mid_separator = NULL)
{

    if (imin > imax || jmin > jmax || di - separator > imax - imin - 1) {    //参数对不上，退出
        return -1;
    }


    //画框架
    cct_setcolor(bgcolor, fgcolor);		//设成框架颜色

    //框架四角
    myGotoXY(pCGI, jmin, imin);
    puts(top_left);
    myGotoXY(pCGI, jmin, imax);
    puts(lower_left);
    myGotoXY(pCGI, jmax, imin);
    puts(top_right);
    myGotoXY(pCGI, jmax, imax);
    puts(lower_right);

    Sleep(delay_ms);

    //框架四周
    for (int i = 1, ii = imin + 1; ii < imax; i++, ii++) {
        myGotoXY(pCGI, jmin, ii);
        puts((separator && i % di == 0) ? v_left_separator : v_normal);
        myGotoXY(pCGI, jmax, ii);
        puts((separator && i % di == 0) ? v_right_separator : v_normal);
    }
    for (int j = 1, jj = jmin + 1; jj < jmax; j++, jj++) {
        myGotoXY(pCGI, jj, imin);
        puts((separator && j % dj == 0) ? h_top_separator : h_normal);
        myGotoXY(pCGI, jj, imax);
        puts((separator && j % dj == 0) ? h_lower_separator : h_normal);
    }

    Sleep(delay_ms);

    //框架中间
    if (separator) {
        for (int ii = imin + di; ii < imax; ii += di) {
            for (int jj = jmin + 1; jj < jmax; jj++) {
                myGotoXY(pCGI, jj, ii);
                puts((jj - jmin) % dj == 0 ? mid_separator : h_normal);
            }
        }
        for (int jj = jmin + dj; jj < jmax; jj += dj) {
            for (int ii = imin + 1; ii < imax; ii++) {
                myGotoXY(pCGI, jj, ii);
                puts((ii - imin) % di == 0 ? mid_separator : v_normal);
            }
        }
    }

    cct_setcolor();   //恢复默认颜色
    return 0;
}

/// 画一个信息方块，在抽象了的坐标(startx, starty)为左上角第一个画的位置
static int draw_block(const CONSOLE_GRAPHICS_INFO *const pCGI, const BLOCK_DISPLAY_INFO *const pBDI, int starti, int startj, int ilen, int jlen)
{


    int bgcolor = pBDI->bgcolor == -1 ? pCGI->CFI.bgcolor : pBDI->bgcolor;
    int fgcolor = pBDI->value == BDI_VALUE_BLANK ? pCGI->CFI.bgcolor :      //空白快特殊处理(前景设成背景色以达到空白效果)
        (pBDI->fgcolor == -1 ? pCGI->CFI.fgcolor : pBDI->fgcolor);          //-1则为游戏区默认颜色

    //预填充
    cct_setcolor(bgcolor, fgcolor);
    for (int i = 0, ii = starti; i < ilen; i++, ii++) {
        for (int j = 0, jj = startj; j < jlen; j++, jj++) {
            myGotoXY(pCGI, jj, ii);
            puts("  ");
        }
    }

    //边框
    if (pCGI->CBI.block_border) {
        draw_grid(pCGI, starti, startj, starti + ilen - 1, startj + jlen - 1, ilen - 2, jlen - 2,
            bgcolor, fgcolor, 0,
            pCGI->CFI.top_left, pCGI->CFI.lower_left, pCGI->CFI.top_right, pCGI->CFI.lower_right,
            pCGI->CFI.h_normal, pCGI->CFI.v_normal, pCGI->delay_of_draw_block
        );
    }

    //内容，若是结束那个（也就是没定义过的值）的话就啥都不输出
    if (pBDI->value != BDI_VALUE_END) {

        cct_setcolor(bgcolor, fgcolor);
        const char *pContent = pBDI->content;
        char numberStr[30];
        if (pContent == NULL) { //Contet指向NULL说明是直接显示数字


            int cnt = 0, i;
            for (i = pBDI->value; i; i /= 10)
                numberStr[cnt++] = i % 10 + '0';
            numberStr[cnt] = 0;
            for (i = 0, cnt--; cnt > i; i++, cnt--)
                swap(numberStr[i], numberStr[cnt]);
            pContent = numberStr;

        }
        //起始位置，讨论块内边框以及居中、最长长度，只输出一行（串内部有'\n'等不管他，出问题了也不管……）
        myGotoXY(pCGI, startj + pCGI->CBI.block_border + max(0, (jlen - 1 - (pCGI->CBI.block_border << 1)) / 2 - (int(strlen(pContent)) - 1) / 2), starti + (ilen - 1) / 2);
        //注意最长输出，讨论块内边框
        for (int j = 0, jm = ((jlen - (pCGI->CBI.block_border << 1)) << 1); j < jm && pContent[j]; j++)
            putchar(pContent[j]);
    }

    return 0;
}


/// 返回pCGI中边框图形在抽象了的坐标(x,y)处应显示的字符串
static const char *const getFramePix(const CONSOLE_GRAPHICS_INFO *const pCGI, int x, int y)
{
    int imax = pCGI->CFI.bhigh - 1;
    int jmax = (pCGI->CFI.bwidth >> 1) - 1;
    int di = pCGI->CFI.block_high + pCGI->CFI.separator;
    int dj = (pCGI->CFI.block_width >> 1) + pCGI->CFI.separator;

    if (x<0 || x>jmax || y<0 || y>imax)
        return NULL;

    //框架四角
    if (x == 0 && y == 0)
        return pCGI->CFI.top_left;
    if (x == 0 && y == imax)
        return pCGI->CFI.lower_left;
    if (x == jmax && y == 0)
        return pCGI->CFI.top_right;
    if (x == jmax && y == imax)
        return pCGI->CFI.lower_right;

    //框架四周
    if (x == 0 && y > 0 && y < imax)
        return ((pCGI->CFI.separator && y % di == 0) ? pCGI->CFI.v_left_separator : pCGI->CFI.v_normal);
    if (x == jmax && y > 0 && y < imax)
        return ((pCGI->CFI.separator && y % di == 0) ? pCGI->CFI.v_right_separator : pCGI->CFI.v_normal);
    if (y == 0 && x > 0 && x < jmax)
        return ((pCGI->CFI.separator && x % dj == 0) ? pCGI->CFI.h_top_separator : pCGI->CFI.h_normal);
    if (y == imax && x > 0 && x < jmax)
        return ((pCGI->CFI.separator && x % dj == 0) ? pCGI->CFI.h_lower_separator : pCGI->CFI.h_normal);

    //框架中间
    if (pCGI->CFI.separator) {
        if (y % di == 0 && y < imax && x>0 && x < jmax)
            return ((x % dj == 0) ? pCGI->CFI.mid_separator : pCGI->CFI.h_normal);
        if (x % dj == 0 && x < jmax && y>0 && y < imax)
            return ((y % di == 0) ? pCGI->CFI.mid_separator : pCGI->CFI.v_normal);
    }
    return "  ";
}

/// 画一个边框像素（抽象了的像素，其实是两个char）
static int drawFramePix(const CONSOLE_GRAPHICS_INFO *const pCGI, int x, int y)
{
    cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
    myGotoXY(pCGI, x, y);
    puts(getFramePix(pCGI, x, y));
    return 0;
}


/* -----------------------------------------------
        实现下面给出的函数（函数声明不准动）
   ----------------------------------------------- */
   /***************************************************************************
     函数名称：
     功    能：设置游戏主框架的行列数
     输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
               const int row						：行数(错误则为0，不设上限，人为保证正确性)
               const int col						：列数(错误则为0，不设上限，人为保证正确性)
     返 回 值：
     说    明：1、指消除类游戏的矩形区域的行列值
               2、行列的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
   ***************************************************************************/
int gmw_set_rowcol(CONSOLE_GRAPHICS_INFO *const pCGI, const int row, const int col)
{
    /* 防止在未调用 gmw_init 前调用其它函数 */
    if (pCGI->inited != CGI_INITED)
        return -1;

    pCGI->row_num = (row < 0 ? 0 : row);
    pCGI->col_num = (col < 0 ? 0 : col);

    calc_CGI(pCGI);

    return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置整个窗口（含游戏区、附加区在内的整个cmd窗口）的颜色
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
           const int bg_color					：前景色（缺省COLOR_BLACK）
           const int fg_color					：背景色（缺省COLOR_WHITE）
           const bool cascade					：是否级联（缺省为true-级联）
  返 回 值：
  说    明：1、cascade = true时
                同步修改游戏主区域的颜色
                同步修改上下状态栏的正常文本的背景色和前景色，醒目文本的背景色（前景色不变）
            2、不检查颜色值错误及冲突，需要人为保证
                例：颜色非0-15
                    前景色背景色的值一致导致无法看到内容
                    前景色正好是状态栏醒目前景色，导致无法看到醒目提示
                    ...
***************************************************************************/
int gmw_set_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int bgcolor, const int fgcolor, const bool cascade)
{
    /* 防止在未调用 gmw_init 前调用其它函数 */
    if (pCGI->inited != CGI_INITED)
        return -1;

    pCGI->area_bgcolor = bgcolor;
    pCGI->area_fgcolor = fgcolor;

    if (cascade) {
        //游戏主区域
        pCGI->CFI.bgcolor = bgcolor;
        pCGI->CFI.fgcolor = fgcolor;

        //状态栏
        gmw_set_status_line_color(pCGI, TOP_STATUS_LINE, bgcolor, fgcolor, bgcolor);
        gmw_set_status_line_color(pCGI, LOWER_STATUS_LINE, bgcolor, fgcolor, bgcolor);
    }

    return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置窗口的字体
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
           const char *fontname					：字体名称（只能是"Terminal"和"新宋体"两种，错误则返回-1，不改变字体）
           const int fs_high					：字体高度（缺省及错误为16，不设其它限制，人为保证）
           const int fs_width					：字体高度（缺省及错误为8，不设其它限制，人为保证）
  返 回 值：
  说    明：1、与cmd_console_tools中的setfontsize相似，目前只支持“点阵字体”和“新宋体”
            2、若设置其它字体则直接返回，保持原字体设置不变
***************************************************************************/
int gmw_set_font(CONSOLE_GRAPHICS_INFO *const pCGI, const char *fontname, const int fs_high, const int fs_width)
{
    /* 防止在未调用 gmw_init 前调用其它函数 */
    if (pCGI->inited != CGI_INITED)
        return -1;

    pCGI->CFT.font_size_high = (fs_high < 0 ? 16 : fs_high);
    pCGI->CFT.font_size_width = (fs_width < 0 ? 8 : fs_width);

    if (strcmp(fontname, "Terminal") == 0 || strcmp(fontname, "新宋体") == 0)
        strcpy(pCGI->CFT.font_type, fontname);
    else
        return -1;

    return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置延时
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
           const int type						：延时的类型（目前为3种）
           const int delay_ms					：以ms为单位的延时
               画边框的延时：0 ~ 不设上限，人为保证正确（<0则置0）
               画色块的延时：0 ~ 不设上限，人为保证正确（<0则置0）
               色块移动的延时：BLOCK_MOVED_DELAY_MS ~ 不设上限，人为保证正确（ <BLOCK_MOVED_DELAY_MS 则置 BLOCK_MOVED_DELAY_MS）
  返 回 值：
  说    明：
***************************************************************************/
int gmw_set_delay(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const int delay_ms)
{
    /* 防止在未调用 gmw_init 前调用其它函数 */
    if (pCGI->inited != CGI_INITED)
        return -1;

    int t = (delay_ms < 0 ? 0 : delay_ms);

    switch (type) {
        case DELAY_OF_DRAW_FRAME:
            pCGI->delay_of_draw_frame = t;
            break;
        case DELAY_OF_DRAW_BLOCK:
            pCGI->delay_of_draw_block = t;
            break;
        case DELAY_OF_BLOCK_MOVED:
            pCGI->delay_of_block_moved = t;
            break;
    }

    return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  输入参数：设置游戏主框架结构之外需要保留的额外区域
  功    能：CONSOLE_GRAPHICS_INFO *const pCGI	：
           const int up_lines					：上部额外的行（缺省及错误为0，不设上限，人为保证）
           const int down_lines				：下部额外的行（缺省及错误为0，不设上限，人为保证）
           const int left_cols					：左边额外的列（缺省及错误为0，不设上限，人为保证）
           const int right_cols				：右边额外的列（缺省及错误为0，不设上限，人为保证）
  返 回 值：
  说    明：额外行列的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_ext_rowcol(CONSOLE_GRAPHICS_INFO *const pCGI, const int up_lines, const int down_lines, const int left_cols, const int right_cols)
{
    /* 防止在未调用 gmw_init 前调用其它函数 */
    if (pCGI->inited != CGI_INITED)
        return -1;

    pCGI->extern_up_lines = (up_lines < 0 ? 0 : up_lines);
    pCGI->extern_down_lines = (down_lines < 0 ? 0 : down_lines);
    pCGI->extern_left_cols = (left_cols < 0 ? 0 : left_cols);
    pCGI->extern_right_cols = (right_cols < 0 ? 0 : right_cols);

    return calc_CGI(pCGI);

    //return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_FRAME_TYPE 结构中的11种线型（缺省4种）
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
            const int type						：1 - 全双线 2 - 全单线 3 - 横双竖单 4 - 横单竖双
  返 回 值：
  说    明：
***************************************************************************/
int gmw_set_frame_default_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const int type)
{
    /* 防止在未调用 gmw_init 前调用其它函数 */
    if (pCGI->inited != CGI_INITED)
        return -1;

    switch (type) {
        case 1:
        default:
            gmw_set_frame_linetype(pCGI);
            break;
        case 2:
            gmw_set_frame_linetype(pCGI, "┏", "┗", "┓", "┛", "━", "┃", "┳", "┻", "┣", "┫", "╋");
            break;
        case 3:
            gmw_set_frame_linetype(pCGI, "╒", "╘", "╕", "╛", "═", "┃", "╤", "╧", "╞", "╡", "╋");
            break;
        case 4:
            gmw_set_frame_linetype(pCGI, "╓", "╙", "╖", "╜", "─", "║", "╥", "╨", "╟", "╢", "╫");
            break;
    }

    return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_FRAME_TYPE 结构中的11种线型
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
            const char *...						：共11种，具体见.h，此处略
  返 回 值：
  说    明：约定为一个中文制表符，可以使用其它内容，人为保证2字节
            1、超过2字节则只取前2字节
            2、如果给NULL，用两个空格替代
            3、如果给1字节，则补一个空格，如果因此而导致显示乱，不算错
***************************************************************************/
int gmw_set_frame_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const char *top_left, const char *lower_left, const char *top_right,
    const char *lower_right, const char *h_normal, const char *v_normal, const char *h_top_separator,
    const char *h_lower_separator, const char *v_left_separator, const char *v_right_separator, const char *mid_separator)
{
    /* 防止在未调用 gmw_init 前调用其它函数 */
    if (pCGI->inited != CGI_INITED)
        return -1;

    myStrCpy(pCGI->CFI.top_left, top_left, 2);
    myStrCpy(pCGI->CFI.lower_left, lower_left, 2);
    myStrCpy(pCGI->CFI.top_right, top_right, 2);
    myStrCpy(pCGI->CFI.lower_right, lower_right, 2);
    myStrCpy(pCGI->CFI.h_normal, h_normal, 2);
    myStrCpy(pCGI->CFI.v_normal, v_normal, 2);
    myStrCpy(pCGI->CFI.h_top_separator, h_top_separator, 2);
    myStrCpy(pCGI->CFI.h_lower_separator, h_lower_separator, 2);
    myStrCpy(pCGI->CFI.v_left_separator, v_left_separator, 2);
    myStrCpy(pCGI->CFI.v_right_separator, v_right_separator, 2);
    myStrCpy(pCGI->CFI.mid_separator, mid_separator, 2);

    return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_FRAME_TYPE 结构中的色块数量大小、是否需要分隔线等
  输入参数：输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
            const int block_width						：宽度（错误及缺省2，因为约定表格线为中文制表符，如果给出奇数，要+1）
            const int block_high						：高度（错误及缺省1）
            const bool separator						：是否需要分隔线（缺省为true，需要分隔线）
  返 回 值：
  说    明：框架大小/是否需要分隔线等的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_frame_style(CONSOLE_GRAPHICS_INFO *const pCGI, const int block_width, const int block_high, const bool separator)
{
    /* 防止在未调用 gmw_init 前调用其它函数 */
    if (pCGI->inited != CGI_INITED)
        return -1;

    pCGI->CFI.block_width = (block_width < 0 ? 2 : block_width + (block_width & 1));
    pCGI->CFI.block_high = (block_high < 0 ? 1 : block_high);
    pCGI->CFI.separator = separator;

    calc_CGI(pCGI);

    return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_BORDER_TYPE 结构中的颜色
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
            const int bg_color					：背景色（缺省 -1表示用窗口背景色）
            const int fg_color					：前景色（缺省 -1表示用窗口前景色）
  返 回 值：
  说    明：不检查颜色值错误及冲突，需要人为保证
                例：颜色非0-15，前景色背景色的值一致导致无法看到内容等
***************************************************************************/
int gmw_set_frame_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int bgcolor, const int fgcolor)
{
    /* 防止在未调用 gmw_init 前调用其它函数 */
    if (pCGI->inited != CGI_INITED)
        return -1;

    pCGI->CFI.bgcolor = bgcolor;
    pCGI->CFI.fgcolor = fgcolor;

    return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_BLOCK_INFO 结构中的6种线型（缺省4种）
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
            const int type						：1 - 全双线 2 - 全单线 3 - 横双竖单 4 - 横单竖双
  返 回 值：
  说    明：
***************************************************************************/
int gmw_set_block_default_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const int type)
{
    /* 防止在未调用 gmw_init 前调用其它函数 */
    if (pCGI->inited != CGI_INITED)
        return -1;

    switch (type) {
        case 1:
        default:
            gmw_set_block_linetype(pCGI);
        case 2:
            gmw_set_block_linetype(pCGI, "┏", "┗", "┓", "┛", "━", "┃");
        case 3:
            gmw_set_block_linetype(pCGI, "╒", "╘", "╕", "╛", "═", "┃");
        case 4:
            gmw_set_block_linetype(pCGI, "╓", "╙", "╖", "╜", "─", "║");
    }

    return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_BLOCK_INFO 结构中的6种线型
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
           const char *...					：共6种，具体见.h，此处略
  返 回 值：
  说    明：约定为一个中文制表符，可以使用其它内容，人为保证2字节
            1、超过2字节则只取前2字节
            2、如果给NULL，用两个空格替代
            3、如果给1字节，则补一个空格，如果因此而导致显示乱，不算错
***************************************************************************/
int gmw_set_block_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const char *top_left, const char *lower_left, const char *top_right, const char *lower_right, const char *h_normal, const char *v_normal)
{
    /* 防止在未调用 gmw_init 前调用其它函数 */
    if (pCGI->inited != CGI_INITED)
        return -1;

    myStrCpy(pCGI->CBI.top_left, top_left, 2);
    myStrCpy(pCGI->CBI.lower_left, lower_left, 2);
    myStrCpy(pCGI->CBI.top_right, top_right, 2);
    myStrCpy(pCGI->CBI.lower_right, lower_right, 2);
    myStrCpy(pCGI->CBI.h_normal, h_normal, 2);
    myStrCpy(pCGI->CBI.v_normal, v_normal, 2);

    return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置每个游戏色块(彩球)是否需要小边框
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
           const bool on_off					：true - 需要 flase - 不需要（缺省false）
  返 回 值：
  说    明：边框约定为中文制表符，双线
***************************************************************************/
int gmw_set_block_border_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
    /* 防止在未调用 gmw_init 前调用其它函数 */
    if (pCGI->inited != CGI_INITED)
        return -1;

    pCGI->CBI.block_border = on_off;

    return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置是否显示上下状态栏
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
            const int type						：状态栏类型（上/下）
            const bool on_off					：true - 需要 flase - 不需要（缺省true）
  返 回 值：
  说    明：1、状态栏的相关约定如下：
               1.1、上状态栏只能一行，在主区域最上方框线/列标的上面，为主区域的最开始一行（主区域的左上角坐标就是上状态栏的坐标）
               1.2、下状态栏只能一行，在主区域最下方框线的下面
               1.3、状态栏的宽度为主区域宽度，如果信息过长则截断
           2、行列的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_status_line_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const bool on_off)
{
    /* 防止在未调用 gmw_init 前调用其它函数 */
    if (pCGI->inited != CGI_INITED)
        return -1;

    if (type == TOP_STATUS_LINE) {
        pCGI->SLI.is_top_status_line = on_off;
        pCGI->top_status_line = on_off;
    }
    else if (type == LOWER_STATUS_LINE) {
        pCGI->SLI.is_lower_status_line = on_off;
        pCGI->lower_status_line = on_off;
    }

    calc_CGI(pCGI);

    return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置上下状态栏的颜色
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
            const int type						：状态栏类型（上/下）
            const int normal_bgcolor			：正常文本背景色（缺省 -1表示使用窗口背景色）
            const int normal_fgcolor			：正常文本前景色（缺省 -1表示使用窗口前景色）
            const int catchy_bgcolor			：醒目文本背景色（缺省 -1表示使用窗口背景色）
            const int catchy_fgcolor			：醒目文本前景色（缺省 -1表示使用亮黄色）
  输入参数：
  返 回 值：
  说    明：不检查颜色值错误及冲突，需要人为保证
                例：颜色非0-15，前景色背景色的值一致导致无法看到内容等
***************************************************************************/
int gmw_set_status_line_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const int normal_bgcolor, const int normal_fgcolor, const int catchy_bgcolor, const int catchy_fgcolor)
{
    /* 防止在未调用 gmw_init 前调用其它函数 */
    if (pCGI->inited != CGI_INITED)
        return -1;

    if (type == TOP_STATUS_LINE) {
        pCGI->SLI.top_normal_bgcolor = (normal_bgcolor == -1 ? pCGI->area_bgcolor : normal_bgcolor);
        pCGI->SLI.top_normal_fgcolor = (normal_fgcolor == -1 ? pCGI->area_fgcolor : normal_fgcolor);
        pCGI->SLI.top_catchy_bgcolor = (catchy_bgcolor == -1 ? pCGI->area_bgcolor : catchy_bgcolor);
        pCGI->SLI.top_catchy_fgcolor = (catchy_fgcolor == -1 ? COLOR_HYELLOW : catchy_fgcolor);
    }
    else if (type == LOWER_STATUS_LINE) {
        pCGI->SLI.lower_normal_bgcolor = (normal_bgcolor == -1 ? pCGI->area_bgcolor : normal_bgcolor);
        pCGI->SLI.lower_normal_fgcolor = (normal_fgcolor == -1 ? pCGI->area_fgcolor : normal_fgcolor);
        pCGI->SLI.lower_catchy_bgcolor = (catchy_bgcolor == -1 ? pCGI->area_bgcolor : catchy_bgcolor);
        pCGI->SLI.lower_catchy_fgcolor = (catchy_fgcolor == -1 ? COLOR_HYELLOW : catchy_fgcolor);
    }

    return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置是否显示行号
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
            const bool on_off					：true - 显示 flase - 不显示（缺省false）
  返 回 值：
  说    明：1、行号约定为字母A开始连续排列（如果超过26，则从a开始，超过52的统一为*，实际应用不可能）
            2、是否显示行号的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_rowno_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
    /* 防止在未调用 gmw_init 前调用其它函数 */
    if (pCGI->inited != CGI_INITED)
        return -1;

    pCGI->draw_frame_with_row_no = on_off;

    calc_CGI(pCGI);

    return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置是否显示列标
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
            const bool on_off					：true - 显示 flase - 不显示（缺省false）
  返 回 值：
  说    明：1、列标约定为数字0开始连续排列（数字0-99，超过99统一为**，实际应用不可能）
            2、是否显示列标的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_colno_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
    /* 防止在未调用 gmw_init 前调用其它函数 */
    if (pCGI->inited != CGI_INITED)
        return -1;

    pCGI->draw_frame_with_col_no = on_off;

    calc_CGI(pCGI);

    return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：打印 CONSOLE_GRAPHICS_INFO 结构体中的各成员值
  输入参数：
  返 回 值：
  说    明：1、仅供调试用，打印格式自定义
            2、本函数测试用例中未调用过，可以不实现
***************************************************************************/
int gmw_print(const CONSOLE_GRAPHICS_INFO *const pCGI)
{
    /* 防止在未调用 gmw_init 前调用其它函数 */
    if (pCGI->inited != CGI_INITED)
        return -1;

    return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：将 CONSOLE_GRAPHICS_INFO 结构体用缺省值进行初始化
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI：整体结构指针
           const int row					：游戏区域色块行数（缺省10）
           const int col					：游戏区域色块列数（缺省10）
           const int bgcolor				：整个窗口背景色（缺省 COLOR_BLACK）
           const int fgcolor				：整个窗口背景色（缺省 COLOR_WHITE）
  返 回 值：
  说    明：窗口背景黑/前景白，点阵16*8，上下左右无额外行列，上下状态栏均有，无行号/列标，框架线型为双线，色块宽度2/高度1/无小边框，颜色略
***************************************************************************/
int gmw_init(CONSOLE_GRAPHICS_INFO *const pCGI, const int row, const int col, const int bgcolor, const int fgcolor)
{
    /* 首先置标记 */
    pCGI->inited = CGI_INITED;

    // 四周空位extern_udlr_lc
    gmw_set_ext_rowcol(pCGI);

    // 游戏区色块行列
    gmw_set_rowcol(pCGI, row, col);

    // 全窗口颜色
    gmw_set_color(pCGI, 0, 7, 1);

    // 行列标号
    gmw_set_colno_switch(pCGI);
    gmw_set_rowno_switch(pCGI);

    // 延时时间设置
    gmw_set_delay(pCGI, DELAY_OF_DRAW_FRAME, 0);
    //pCGI->delay_of_draw_frame = 0;
    gmw_set_delay(pCGI, DELAY_OF_DRAW_BLOCK, 0);
    //pCGI->delay_of_draw_block = 0;
    gmw_set_delay(pCGI, DELAY_OF_BLOCK_MOVED, 0);
    //pCGI->delay_of_block_moved = 0;

    // 主框架信息 CFI
    gmw_set_frame_default_linetype(pCGI, 1);
    gmw_set_frame_style(pCGI);
    gmw_set_frame_color(pCGI);

    // 色块信息 CBI
    gmw_set_block_default_linetype(pCGI, 1);
    gmw_set_block_border_switch(pCGI);

    // 状态栏 SLI
    gmw_set_status_line_switch(pCGI, TOP_STATUS_LINE, 1);
    gmw_set_status_line_switch(pCGI, LOWER_STATUS_LINE, 1);
    gmw_set_status_line_color(pCGI, TOP_STATUS_LINE);
    gmw_set_status_line_color(pCGI, LOWER_STATUS_LINE);

    // 字体 CFT
    gmw_set_font(pCGI);


    calc_CGI(pCGI);		//计算其它参数


    return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：画主游戏框架
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
  返 回 值：
  说    明：具体可参考demo的效果
***************************************************************************/
int gmw_draw_frame(const CONSOLE_GRAPHICS_INFO *const pCGI)
{
    /* 防止在未调用 gmw_init 前调用其它函数 */
    if (pCGI->inited != CGI_INITED)
        return -1;

    //设置窗口
    cct_setfontsize(pCGI->CFT.font_type, pCGI->CFT.font_size_high, pCGI->CFT.font_size_width);
    cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);
    cct_cls();
    cct_setconsoleborder(pCGI->cols, pCGI->lines, pCGI->cols, pCGI->lines);
    cct_setconsoletitle("hehepig's present o7");

    int imax = pCGI->CFI.bhigh - 1;
    int jmax = (pCGI->CFI.bwidth >> 1) - 1;
    int di = pCGI->CFI.block_high + pCGI->CFI.separator;
    int dj = (pCGI->CFI.block_width >> 1) + pCGI->CFI.separator;

    draw_grid(pCGI, 0, 0, imax, jmax, di, dj,
        pCGI->CFI.bgcolor, pCGI->CFI.fgcolor, pCGI->CFI.separator,
        pCGI->CFI.top_left, pCGI->CFI.lower_left, pCGI->CFI.top_right, pCGI->CFI.lower_right,
        pCGI->CFI.h_normal, pCGI->CFI.v_normal, pCGI->delay_of_draw_frame,
        pCGI->CFI.h_lower_separator, pCGI->CFI.h_top_separator, pCGI->CFI.v_left_separator, pCGI->CFI.v_right_separator,
        pCGI->CFI.mid_separator
    );

    //标尺
    cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);	//恢复背景颜色
    if (pCGI->draw_frame_with_row_no) {
        for (int i = (di + 1) / 2, ii = 0; ii < pCGI->row_num; i += di, ii++) {
            myGotoXY(pCGI, -1, i);
            putchar(rowNo(ii));
        }
    }
    if (pCGI->draw_frame_with_col_no) {
        for (int j = (dj + 1) / 2, jj = 0; jj < pCGI->col_num; j += dj, jj++) {
            myGotoXY(pCGI, j, -1);
            if (jj < 100)
                cout << jj;
            else
                puts("**");
        }
    }

    cct_gotoxy(0, 0);   //光标复位

    return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：在状态栏上显示信息
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
           const int type							：指定是上/下状态栏
           const char *msg						：正常信息
           const char *catchy_msg					：需要特别标注的信息（在正常信息前显示）
  返 回 值：
  说    明：1、输出宽度限定为主框架的宽度（含行号列标位置），超出则截去
            2、如果最后一个字符是某汉字的前半个，会导致后面乱码，要处理
***************************************************************************/
int gmw_status_line(const CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const char *msg, const char *catchy_msg)
{
    /* 防止在未调用 gmw_init 前调用其它函数 */
    if (pCGI->inited != CGI_INITED)
        return -1;


    int cnt_all = 1, cnt_cn;
    int stx = 0, sty = 0, c_bgc = 0, c_fgc = 0, bgc = 0, fgc = 0;

    if (type == TOP_STATUS_LINE) {
        if (!pCGI->top_status_line) {	//不需输出
            return 0;
        }
        stx = pCGI->SLI.top_start_x;
        sty = pCGI->SLI.top_start_y;
        c_bgc = pCGI->SLI.top_catchy_bgcolor;
        c_fgc = pCGI->SLI.top_catchy_fgcolor;
        bgc = pCGI->SLI.top_normal_bgcolor;
        fgc = pCGI->SLI.top_normal_fgcolor;
    }
    else if (type == LOWER_STATUS_LINE) {
        if (!pCGI->lower_status_line) {		//不需输出
            return 0;
        }
        stx = pCGI->SLI.lower_start_x;
        sty = pCGI->SLI.lower_start_y;
        c_bgc = pCGI->SLI.lower_catchy_bgcolor;
        c_fgc = pCGI->SLI.lower_catchy_fgcolor;
        bgc = pCGI->SLI.lower_normal_bgcolor;
        fgc = pCGI->SLI.lower_normal_fgcolor;
    }
    else {
        return 2;
    }

    cct_gotoxy(stx, sty);
    cct_setcolor(c_bgc, c_fgc);
    if (catchy_msg) {
        for (cnt_cn = 0; cnt_all <= pCGI->SLI.width && *catchy_msg; catchy_msg++, cnt_all++) {
            cnt_cn += (*catchy_msg < 0);
            if (cnt_all == pCGI->SLI.width && (cnt_cn & 1))
                break;
            putchar(*catchy_msg);
        }
    }
    cct_setcolor(bgc, fgc);
    if (msg) {
        for (cnt_cn = 0; cnt_all <= pCGI->SLI.width && *msg; msg++, cnt_all++) {
            cnt_cn += (*msg < 0);
            if (cnt_all == pCGI->SLI.width && (cnt_cn & 1))
                break;
            putchar(*msg);
        }
    }
    while (cnt_all++ <= pCGI->SLI.width)
        putchar(' ');

    cct_gotoxy(0, 0);   //光标复位

    return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：显示某一个色块(内容为字符串，坐标为row/col)
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
           const int row_no						：行号（从0开始，人为保证正确性，程序不检查）
           const int col_no						：列号（从0开始，人为保证正确性，程序不检查）
           const int bdi_value						：需要显示的值
           const BLOCK_DISPLAY_INFO *const bdi		：存放该值对应的显示信息的结构体数组
  返 回 值：
  说    明：1、BLOCK_DISPLAY_INFO 的含义见头文件，用法参考测试样例
            2、bdi_value为 BDI_VALUE_BLANK 表示空白块，要特殊处理
***************************************************************************/
int gmw_draw_block(const CONSOLE_GRAPHICS_INFO *const pCGI, const int row_no, const int col_no, const int bdi_value, const BLOCK_DISPLAY_INFO *const bdi)
{
    /* 防止在未调用 gmw_init 前调用其它函数 */
    if (pCGI->inited != CGI_INITED)
        return -1;

    //此处i,j都是抽象了的坐标（两个char为一个点）
    int starti, startj;
    int ilen, jlen;
    starti = 1 + row_no * (pCGI->CFI.block_high + pCGI->CFI.block_high_ext);
    startj = 1 + col_no * (pCGI->CFI.block_width + pCGI->CFI.block_width_ext) / 2;
    ilen = pCGI->CFI.block_high;
    jlen = pCGI->CFI.block_width / 2;

    const BLOCK_DISPLAY_INFO *pBDI = bdi;
    while (pBDI->value != BDI_VALUE_END && pBDI->value != bdi_value)
        pBDI++;

    draw_block(pCGI, pBDI, starti, startj, ilen, jlen);

    cct_setcolor(); //恢复默认颜色

    cct_gotoxy(0, 0);   //光标复位

    Sleep(pCGI->delay_of_draw_block);   //延时

    return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：移动某一个色块
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
           const int row_no						：行号（从0开始，人为保证正确性，程序不检查）
           const int col_no						：列号（从0开始，人为保证正确性，程序不检查）
           const int bdi_value						：需要显示的值
           const int blank_bdi_value				：移动过程中用于动画效果显示时用于表示空白的值（一般为0，此处做为参数代入，是考虑到可能出现的特殊情况）
           const BLOCK_DISPLAY_INFO *const bdi		：存放显示值/空白值对应的显示信息的结构体数组
           const int direction						：移动方向，一共四种，具体见cmd_gmw_tools.h
           const int distance						：移动距离（从1开始，人为保证正确性，程序不检查）
  返 回 值：
  说    明：0:up 1:down 2:left 3:right，若移动多格距离，经过的方格都会被 blank_bdi_value 覆盖（留下轨迹），所以建议一格一格地用
***************************************************************************/
int gmw_move_block(const CONSOLE_GRAPHICS_INFO *const pCGI, const int row_no, const int col_no, const int bdi_value, const int blank_bdi_value, const BLOCK_DISPLAY_INFO *const bdi, const int direction, const int distance)
{
    /* 防止在未调用 gmw_init 前调用其它函数 */
    if (pCGI->inited != CGI_INITED)
        return -1;

    const int fx[4][2] = { {0,-1},{0,1},{-1,0},{1,0} };

    //此处i,j都是抽象了的坐标（两个char为一个点）
    int starti = 1 + row_no * (pCGI->CFI.block_high + pCGI->CFI.block_high_ext);
    int startj = 1 + col_no * (pCGI->CFI.block_width + pCGI->CFI.block_width_ext) / 2;
    int ilen = pCGI->CFI.block_high;
    int jlen = pCGI->CFI.block_width / 2;
    int len = (direction < 2 ? (pCGI->CFI.block_high + pCGI->CFI.block_high_ext) : (pCGI->CFI.block_width + pCGI->CFI.block_width_ext) / 2);

    const BLOCK_DISPLAY_INFO *pBDI = bdi;
    while (pBDI->value != BDI_VALUE_END && pBDI->value != bdi_value)
        pBDI++;

    int lstRow = row_no, lstCol = col_no;
    for (int disCnt = 0; disCnt < distance; disCnt++) {
        for (int k = 0; k < len; k++) {
            //框架，只补充移动一像素导致多出来的那一小段
            if (pCGI->CFI.separator) {
                if (direction == 0) {    //UP
                    for (int j = 0, ii = starti + ilen - 1; j < jlen; j++)
                        drawFramePix(pCGI, j + startj, ii);
                }
                else if (direction == 1) {  //DOWN
                    for (int j = 0; j < jlen; j++)
                        drawFramePix(pCGI, j + startj, starti);
                }
                else if (direction == 2) {  //LEFT
                    for (int i = 0, jj = startj + jlen - 1; i < ilen; i++)
                        drawFramePix(pCGI, jj, i + starti);
                }
                else {  //RIGHT
                    for (int i = 0; i < ilen; i++)
                        drawFramePix(pCGI, startj, i + starti);
                }
            }

            //空白值
            gmw_draw_block(pCGI, lstRow, lstCol, blank_bdi_value, bdi);

            //移动覆盖
            startj += fx[direction][0];
            starti += fx[direction][1];
            draw_block(pCGI, pBDI, starti, startj, ilen, jlen);

            //延迟
            Sleep(pCGI->delay_of_block_moved);
        }
        lstCol += fx[direction][0];
        lstRow += fx[direction][1];
    }

    cct_gotoxy(0, 0);   //光标复位

    return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：读键盘或鼠标
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
           int &MAction							：如果返回 CCT_MOUSE_EVENT，则此值有效，为 MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK/MOUSE_RIGHT_BUTTON_CLICK 三者之一
                                                       如果返回 CCT_KEYBOARD_EVENT，则此值无效
           int &MRow								：如果返回 CCT_MOUSE_EVENT 且 MAction = MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK，则此值有效，表示左键选择的游戏区域的行号（从0开始）
                                                  其余情况此值无效（如果访问无效值导致错误，不是本函数的错!!!）
           int &MCol								：如果返回 CCT_MOUSE_EVENT 且 MAction = MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK，则此值有效，表示左键选择的游戏区域的列号（从0开始）
                                                  其余情况此值无效（如果访问无效值导致错误，不是本函数的错!!!）
           int &KeyCode1							：如果返回 CCT_KEYBOARD_EVENT，则此值有效，为读到的键码（如果双键码，则为第一个）
                                                  其余情况此值无效（如果访问无效值导致错误，不是本函数的错!!!）
           int &KeyCode2							：如果返回 CCT_KEYBOARD_EVENT，则此值有效，为读到的键码（如果双键码，则为第二个，如果是单键码，则为0）
                                                  其余情况此值无效（如果访问无效值导致错误，不是本函数的错!!!）
           const bool update_lower_status_line		：鼠标移动时，是否要在本函数中显示"[当前光标] *行*列/位置非法"的信息（true=显示，false=不显示，缺省为true）
  返 回 值：函数返回约定
           1、如果是鼠标移动，得到的MRow/MCol与传入的相同(鼠标指针微小的移动)，则不返回，继续读
                              得到行列非法位置，则不返回，根据 update_lower_status_line 的设置在下状态栏显示"[当前光标] 位置非法"
                              得到的MRow/MCol与传入的不同(行列至少一个变化)，根据 update_lower_status_line 的设置在下状态栏显示"[当前光标] *行*列"，再返回??MOUSE_ONLY_MOVED（有些游戏返回后要处理色块的不同颜色显示）
           2、如果是按下鼠标左键，且当前鼠标指针停留在主游戏区域的*行*列上，则返回 CCT_MOUSE_EVENT ，MAction 为 MOUSE_LEFT_BUTTON_CLICK, MRow 为行号，MCol 为列标
                                  且当前鼠标指针停留在非法区域（非游戏区域，游戏区域中的分隔线），则不返回，根据 update_lower_status_line 的设置在下状态栏显示"[当前光标] 位置非法"
           3、如果是按下鼠标右键，则不判断鼠标指针停留区域是否合法，直接返回 CCT_MOUSE_EVENT ，MAction 为 MOUSE_RIGHT_BUTTON_CLICK, MRow、MCol取当前值
           4、如果按下键盘上的某键（含双键码按键），则直接返回 CCT_KEYBOARD_EVENT，KeyCode1/KeyCode2中为对应的键码值
 说    明：通过调用 cmd_console_tools.cpp 中的 read_keyboard_and_mouse 函数实现
***************************************************************************/
int gmw_read_keyboard_and_mouse(const CONSOLE_GRAPHICS_INFO *const pCGI, int &MAction, int &MRow, int &MCol, int &KeyCode1, int &KeyCode2, const bool update_lower_status_line)
{
    /* 防止在未调用 gmw_init 前调用其它函数 */
    if (pCGI->inited != CGI_INITED)
        return -1;

    int cctRet, mx = -1, my = -1, mrow, mcol, bx, by;
    bx = (pCGI->CFI.block_width + pCGI->CFI.block_width_ext);   //一格（扩展）长度
    by = (pCGI->CFI.block_high + pCGI->CFI.block_high_ext);     //...

    char msg[100];

    cct_enable_mouse(); //启用鼠标
    while (1) {

        cctRet = cct_read_keyboard_and_mouse(mx, my, MAction, KeyCode1, KeyCode2);


        if (cctRet == CCT_KEYBOARD_EVENT) {
            if (update_lower_status_line) {
                if (KeyCode2 == 0) {    //单字符键位
                    sprintf(msg, "[读到键盘] %d", KeyCode1);
                }
                else if (KeyCode1 == 0xe0) {
                    switch (KeyCode2) {
                        case KB_ARROW_UP:
                            sprintf(msg, "[读到键盘] ↑");
                            break;
                        case KB_ARROW_DOWN:
                            sprintf(msg, "[读到键盘] ↓");
                            break;
                        case KB_ARROW_LEFT:
                            sprintf(msg, "[读到键盘] ←");
                            break;
                        case KB_ARROW_RIGHT:
                            sprintf(msg, "[读到键盘] →");
                            break;
                    }
                }
                else {
                    sprintf(msg, "[读到键盘] 未知");
                }
                gmw_status_line(pCGI, LOWER_STATUS_LINE, msg);
            }

            break;
        }

        //以下全是CCT_MOUSE_EVENT

        //鼠标右键
        if (MAction == MOUSE_RIGHT_BUTTON_CLICK) {
            if (update_lower_status_line)
                gmw_status_line(pCGI, LOWER_STATUS_LINE, "[读到鼠标右键]");
            break;
        }

        //计算 mcol, mrow
        mx -= START_X + 2;    //+2是游戏区的外框架
        my -= START_Y + 1;
        mcol = mx < 0 ? -1 : mx / bx;
        mrow = my < 0 ? -1 : my / by;


        if (mcol < 0 || mcol >= pCGI->col_num || (pCGI->CFI.separator && (mx + 1) % bx == 0) || mrow < 0 || mrow >= pCGI->row_num || (pCGI->CFI.separator && (my + 1) % by == 0)) {
            //非法位置
            if (update_lower_status_line) {
                sprintf(msg, "[当前光标] 位置非法");
                gmw_status_line(pCGI, LOWER_STATUS_LINE, msg);
            }
            continue;   //继续读
        }
        else {
            //合法位置
            if (update_lower_status_line) {
                sprintf(msg, "[当前光标] %c行%d列", rowNo(mrow), mcol);
                gmw_status_line(pCGI, LOWER_STATUS_LINE, msg);
            }

            //鼠标左键点击 或 左右一起按 或 滚轮单击
            if (MAction == MOUSE_LEFT_BUTTON_CLICK || MAction==MOUSE_LEFTRIGHT_BUTTON_CLICK || MAction==MOUSE_WHEEL_CLICK) {
                MRow = mrow;
                MCol = mcol;
                break;
            }

            //鼠标移动
            if (MAction == MOUSE_ONLY_MOVED) {
                if (mcol == MCol && mrow == MRow) {
                    continue;
                }

                MRow = mrow;
                MCol = mcol;
                break;
            }
        }
    }

    cct_disable_mouse();	//禁用鼠标
    return cctRet;

    return 0; //此句可根据需要修改
}


//bool &top_status_line = SLI.is_top_status_line;
//bool &lower_status_line = SLI.is_lower_status_line;