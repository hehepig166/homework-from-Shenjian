/* 2051995 ���� ���� */
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
        �˴����Ը�����Ҫ�ľ�̬ȫ�ֱ������������٣����û�У�����̬ȫ��ֻ������/�궨�壨�������ޣ���
   -------------------------------------------------- */

   // cct���������Ͻ����꣨���ս��Ǹ��ַ���
#define START_X (pCGI->start_x + (pCGI->draw_frame_with_row_no << 1))
#define START_Y (pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no)

   /* --------------------------------------------------
           �˴����Ը�����Ҫ���ڲ��������ߺ���
           1�����������ޣ�����Ϊ gmw_inner_*
           2����������
           3��������static������ȷ��ֻ�ڱ�Դ�ļ���ʹ��
      -------------------------------------------------- */

      /// ���㲢����һ��CFI(��Ϸ�����)�ṹ���еĲ���(ֻ��ı��CFI�е�ֵ)
static int calc_CFI(CONSOLE_GRAPHICS_INFO *const pCGI)
{
    CONSOLE_FRAME_INFO *const pCFI = &(pCGI->CFI);

    // ÿ��ɫ�鸽�ӿ�߶�
    pCFI->block_width_ext = pCFI->separator ? 2 : 0;
    pCFI->block_high_ext = pCFI->separator ? 1 : 0;

    // ÿ���к��ָ��ߵ��ܿ��
    pCFI->bwidth = (pCFI->block_width + pCFI->block_width_ext) * pCGI->col_num + (pCGI->CFI.separator ? 2 : 4);
    pCFI->bhigh = (pCFI->block_high + pCFI->block_high_ext) * pCGI->row_num + (pCGI->CFI.separator ? 1 : 2);

    return 0;
}

/// ���㲢����һ��SLI(״̬����Ϣ)�ṹ���еĲ���(ֻ��ı��SLI�е�ֵ)
/// �����ڵ���calc_CFI֮�����
static int calc_SLI(CONSOLE_GRAPHICS_INFO *const pCGI)
{
    // ״̬��λ��
    pCGI->SLI.top_start_x = pCGI->extern_left_cols;
    pCGI->SLI.top_start_y = pCGI->extern_up_lines;
    pCGI->SLI.lower_start_x = pCGI->extern_left_cols;
    pCGI->SLI.lower_start_y = pCGI->extern_up_lines + pCGI->top_status_line + pCGI->draw_frame_with_col_no + pCGI->CFI.bhigh;

    // ״̬������=��ܳ���+�б곤��
    pCGI->SLI.width = pCGI->CFI.bwidth + (pCGI->draw_frame_with_row_no ? 2 : 0);

    return 0;
}

/// ���㲢����CGI�е����в���
static int calc_CGI(CONSOLE_GRAPHICS_INFO *const pCGI)
{
    calc_CFI(pCGI);
    calc_SLI(pCGI);

    // ����ͼ�ν�������������
    pCGI->start_x = pCGI->extern_left_cols;
    pCGI->start_y = pCGI->extern_up_lines;

    // cmd���ڴ�С
    pCGI->lines = pCGI->extern_up_lines + pCGI->top_status_line + pCGI->draw_frame_with_col_no + pCGI->CFI.bhigh + pCGI->lower_status_line + pCGI->extern_down_lines + 4;
    pCGI->cols = pCGI->extern_left_cols + (pCGI->draw_frame_with_row_no << 1) + pCGI->CFI.bwidth + pCGI->extern_right_cols + 1;
    return 0;
}

/// �� fromStr �и���ǰ len ���ַ�(������'\0')�� toStr�����㲹�ո����Ӹ� '\0'
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

/// (0,0)Ϊ������Ͻ�(��Ĭ����"�X"�ַ����Ǹ�λ��)
/// _x Ϊ�����꣬_y Ϊ������
static void myGotoXY(const CONSOLE_GRAPHICS_INFO *const pCGI, int _x, int _y)
{
    // myGotoXY �еı任������
    static int x, y;

    // һ������ռ����char
    _x <<= 1;

    // xƫ�� = (ȫ������С������ + �б�Ԥ��)
    x = _x + START_X;//pCGI->start_x + (pCGI->draw_frame_with_row_no << 1);
    // yƫ�� = (ȫ������С������ + ��״̬��Ԥ�� + �б�Ԥ��)
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

/// ��һ������
/// ilen,jlen: �����ܴ�С(�������ܱ߿�)
/// di,dj:(���ڳ���+separator)
static int draw_grid(const CONSOLE_GRAPHICS_INFO *const pCGI,
    int imin, int jmin, int imax, int jmax, int di, int dj,
    int bgcolor, int fgcolor, bool separator,
    const char *top_left, const char *lower_left, const char *top_right, const char *lower_right,
    const char *h_normal, const char *v_normal,
    int delay_ms = 0,
    const char *h_lower_separator = NULL, const char *h_top_separator = NULL, const char *v_left_separator = NULL, const char *v_right_separator = NULL, const char *mid_separator = NULL)
{

    if (imin > imax || jmin > jmax || di - separator > imax - imin - 1) {    //�����Բ��ϣ��˳�
        return -1;
    }


    //�����
    cct_setcolor(bgcolor, fgcolor);		//��ɿ����ɫ

    //����Ľ�
    myGotoXY(pCGI, jmin, imin);
    puts(top_left);
    myGotoXY(pCGI, jmin, imax);
    puts(lower_left);
    myGotoXY(pCGI, jmax, imin);
    puts(top_right);
    myGotoXY(pCGI, jmax, imax);
    puts(lower_right);

    Sleep(delay_ms);

    //�������
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

    //����м�
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

    cct_setcolor();   //�ָ�Ĭ����ɫ
    return 0;
}

/// ��һ����Ϣ���飬�ڳ����˵�����(startx, starty)Ϊ���Ͻǵ�һ������λ��
static int draw_block(const CONSOLE_GRAPHICS_INFO *const pCGI, const BLOCK_DISPLAY_INFO *const pBDI, int starti, int startj, int ilen, int jlen)
{


    int bgcolor = pBDI->bgcolor == -1 ? pCGI->CFI.bgcolor : pBDI->bgcolor;
    int fgcolor = pBDI->value == BDI_VALUE_BLANK ? pCGI->CFI.bgcolor :      //�հ׿����⴦��(ǰ����ɱ���ɫ�Դﵽ�հ�Ч��)
        (pBDI->fgcolor == -1 ? pCGI->CFI.fgcolor : pBDI->fgcolor);          //-1��Ϊ��Ϸ��Ĭ����ɫ

    //Ԥ���
    cct_setcolor(bgcolor, fgcolor);
    for (int i = 0, ii = starti; i < ilen; i++, ii++) {
        for (int j = 0, jj = startj; j < jlen; j++, jj++) {
            myGotoXY(pCGI, jj, ii);
            puts("  ");
        }
    }

    //�߿�
    if (pCGI->CBI.block_border) {
        draw_grid(pCGI, starti, startj, starti + ilen - 1, startj + jlen - 1, ilen - 2, jlen - 2,
            bgcolor, fgcolor, 0,
            pCGI->CFI.top_left, pCGI->CFI.lower_left, pCGI->CFI.top_right, pCGI->CFI.lower_right,
            pCGI->CFI.h_normal, pCGI->CFI.v_normal, pCGI->delay_of_draw_block
        );
    }

    //���ݣ����ǽ����Ǹ���Ҳ����û�������ֵ���Ļ���ɶ�������
    if (pBDI->value != BDI_VALUE_END) {

        cct_setcolor(bgcolor, fgcolor);
        const char *pContent = pBDI->content;
        char numberStr[30];
        if (pContent == NULL) { //Contetָ��NULL˵����ֱ����ʾ����


            int cnt = 0, i;
            for (i = pBDI->value; i; i /= 10)
                numberStr[cnt++] = i % 10 + '0';
            numberStr[cnt] = 0;
            for (i = 0, cnt--; cnt > i; i++, cnt--)
                swap(numberStr[i], numberStr[cnt]);
            pContent = numberStr;

        }
        //��ʼλ�ã����ۿ��ڱ߿��Լ����С�����ȣ�ֻ���һ�У����ڲ���'\n'�Ȳ���������������Ҳ���ܡ�����
        myGotoXY(pCGI, startj + pCGI->CBI.block_border + max(0, (jlen - 1 - (pCGI->CBI.block_border << 1)) / 2 - (int(strlen(pContent)) - 1) / 2), starti + (ilen - 1) / 2);
        //ע�����������ۿ��ڱ߿�
        for (int j = 0, jm = ((jlen - (pCGI->CBI.block_border << 1)) << 1); j < jm && pContent[j]; j++)
            putchar(pContent[j]);
    }

    return 0;
}


/// ����pCGI�б߿�ͼ���ڳ����˵�����(x,y)��Ӧ��ʾ���ַ���
static const char *const getFramePix(const CONSOLE_GRAPHICS_INFO *const pCGI, int x, int y)
{
    int imax = pCGI->CFI.bhigh - 1;
    int jmax = (pCGI->CFI.bwidth >> 1) - 1;
    int di = pCGI->CFI.block_high + pCGI->CFI.separator;
    int dj = (pCGI->CFI.block_width >> 1) + pCGI->CFI.separator;

    if (x<0 || x>jmax || y<0 || y>imax)
        return NULL;

    //����Ľ�
    if (x == 0 && y == 0)
        return pCGI->CFI.top_left;
    if (x == 0 && y == imax)
        return pCGI->CFI.lower_left;
    if (x == jmax && y == 0)
        return pCGI->CFI.top_right;
    if (x == jmax && y == imax)
        return pCGI->CFI.lower_right;

    //�������
    if (x == 0 && y > 0 && y < imax)
        return ((pCGI->CFI.separator && y % di == 0) ? pCGI->CFI.v_left_separator : pCGI->CFI.v_normal);
    if (x == jmax && y > 0 && y < imax)
        return ((pCGI->CFI.separator && y % di == 0) ? pCGI->CFI.v_right_separator : pCGI->CFI.v_normal);
    if (y == 0 && x > 0 && x < jmax)
        return ((pCGI->CFI.separator && x % dj == 0) ? pCGI->CFI.h_top_separator : pCGI->CFI.h_normal);
    if (y == imax && x > 0 && x < jmax)
        return ((pCGI->CFI.separator && x % dj == 0) ? pCGI->CFI.h_lower_separator : pCGI->CFI.h_normal);

    //����м�
    if (pCGI->CFI.separator) {
        if (y % di == 0 && y < imax && x>0 && x < jmax)
            return ((x % dj == 0) ? pCGI->CFI.mid_separator : pCGI->CFI.h_normal);
        if (x % dj == 0 && x < jmax && y>0 && y < imax)
            return ((y % di == 0) ? pCGI->CFI.mid_separator : pCGI->CFI.v_normal);
    }
    return "  ";
}

/// ��һ���߿����أ������˵����أ���ʵ������char��
static int drawFramePix(const CONSOLE_GRAPHICS_INFO *const pCGI, int x, int y)
{
    cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
    myGotoXY(pCGI, x, y);
    puts(getFramePix(pCGI, x, y));
    return 0;
}


/* -----------------------------------------------
        ʵ����������ĺ���������������׼����
   ----------------------------------------------- */
   /***************************************************************************
     �������ƣ�
     ��    �ܣ�������Ϸ����ܵ�������
     ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
               const int row						������(������Ϊ0���������ޣ���Ϊ��֤��ȷ��)
               const int col						������(������Ϊ0���������ޣ���Ϊ��֤��ȷ��)
     �� �� ֵ��
     ˵    ����1��ָ��������Ϸ�ľ������������ֵ
               2�����еı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
   ***************************************************************************/
int gmw_set_rowcol(CONSOLE_GRAPHICS_INFO *const pCGI, const int row, const int col)
{
    /* ��ֹ��δ���� gmw_init ǰ������������ */
    if (pCGI->inited != CGI_INITED)
        return -1;

    pCGI->row_num = (row < 0 ? 0 : row);
    pCGI->col_num = (col < 0 ? 0 : col);

    calc_CGI(pCGI);

    return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������������ڣ�����Ϸ�������������ڵ�����cmd���ڣ�����ɫ
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
           const int bg_color					��ǰ��ɫ��ȱʡCOLOR_BLACK��
           const int fg_color					������ɫ��ȱʡCOLOR_WHITE��
           const bool cascade					���Ƿ�����ȱʡΪtrue-������
  �� �� ֵ��
  ˵    ����1��cascade = trueʱ
                ͬ���޸���Ϸ���������ɫ
                ͬ���޸�����״̬���������ı��ı���ɫ��ǰ��ɫ����Ŀ�ı��ı���ɫ��ǰ��ɫ���䣩
            2���������ɫֵ���󼰳�ͻ����Ҫ��Ϊ��֤
                ������ɫ��0-15
                    ǰ��ɫ����ɫ��ֵһ�µ����޷���������
                    ǰ��ɫ������״̬����Ŀǰ��ɫ�������޷�������Ŀ��ʾ
                    ...
***************************************************************************/
int gmw_set_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int bgcolor, const int fgcolor, const bool cascade)
{
    /* ��ֹ��δ���� gmw_init ǰ������������ */
    if (pCGI->inited != CGI_INITED)
        return -1;

    pCGI->area_bgcolor = bgcolor;
    pCGI->area_fgcolor = fgcolor;

    if (cascade) {
        //��Ϸ������
        pCGI->CFI.bgcolor = bgcolor;
        pCGI->CFI.fgcolor = fgcolor;

        //״̬��
        gmw_set_status_line_color(pCGI, TOP_STATUS_LINE, bgcolor, fgcolor, bgcolor);
        gmw_set_status_line_color(pCGI, LOWER_STATUS_LINE, bgcolor, fgcolor, bgcolor);
    }

    return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ô��ڵ�����
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
           const char *fontname					���������ƣ�ֻ����"Terminal"��"������"���֣������򷵻�-1�����ı����壩
           const int fs_high					������߶ȣ�ȱʡ������Ϊ16�������������ƣ���Ϊ��֤��
           const int fs_width					������߶ȣ�ȱʡ������Ϊ8�������������ƣ���Ϊ��֤��
  �� �� ֵ��
  ˵    ����1����cmd_console_tools�е�setfontsize���ƣ�Ŀǰֻ֧�֡��������塱�͡������塱
            2������������������ֱ�ӷ��أ�����ԭ�������ò���
***************************************************************************/
int gmw_set_font(CONSOLE_GRAPHICS_INFO *const pCGI, const char *fontname, const int fs_high, const int fs_width)
{
    /* ��ֹ��δ���� gmw_init ǰ������������ */
    if (pCGI->inited != CGI_INITED)
        return -1;

    pCGI->CFT.font_size_high = (fs_high < 0 ? 16 : fs_high);
    pCGI->CFT.font_size_width = (fs_width < 0 ? 8 : fs_width);

    if (strcmp(fontname, "Terminal") == 0 || strcmp(fontname, "������") == 0)
        strcpy(pCGI->CFT.font_type, fontname);
    else
        return -1;

    return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�������ʱ
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
           const int type						����ʱ�����ͣ�ĿǰΪ3�֣�
           const int delay_ms					����msΪ��λ����ʱ
               ���߿����ʱ��0 ~ �������ޣ���Ϊ��֤��ȷ��<0����0��
               ��ɫ�����ʱ��0 ~ �������ޣ���Ϊ��֤��ȷ��<0����0��
               ɫ���ƶ�����ʱ��BLOCK_MOVED_DELAY_MS ~ �������ޣ���Ϊ��֤��ȷ�� <BLOCK_MOVED_DELAY_MS ���� BLOCK_MOVED_DELAY_MS��
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int gmw_set_delay(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const int delay_ms)
{
    /* ��ֹ��δ���� gmw_init ǰ������������ */
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

    return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ���������������Ϸ����ܽṹ֮����Ҫ�����Ķ�������
  ��    �ܣ�CONSOLE_GRAPHICS_INFO *const pCGI	��
           const int up_lines					���ϲ�������У�ȱʡ������Ϊ0���������ޣ���Ϊ��֤��
           const int down_lines				���²�������У�ȱʡ������Ϊ0���������ޣ���Ϊ��֤��
           const int left_cols					����߶�����У�ȱʡ������Ϊ0���������ޣ���Ϊ��֤��
           const int right_cols				���ұ߶�����У�ȱʡ������Ϊ0���������ޣ���Ϊ��֤��
  �� �� ֵ��
  ˵    �����������еı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_ext_rowcol(CONSOLE_GRAPHICS_INFO *const pCGI, const int up_lines, const int down_lines, const int left_cols, const int right_cols)
{
    /* ��ֹ��δ���� gmw_init ǰ������������ */
    if (pCGI->inited != CGI_INITED)
        return -1;

    pCGI->extern_up_lines = (up_lines < 0 ? 0 : up_lines);
    pCGI->extern_down_lines = (down_lines < 0 ? 0 : down_lines);
    pCGI->extern_left_cols = (left_cols < 0 ? 0 : left_cols);
    pCGI->extern_right_cols = (right_cols < 0 ? 0 : right_cols);

    return calc_CGI(pCGI);

    //return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_FRAME_TYPE �ṹ�е�11�����ͣ�ȱʡ4�֣�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
            const int type						��1 - ȫ˫�� 2 - ȫ���� 3 - ��˫���� 4 - �ᵥ��˫
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int gmw_set_frame_default_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const int type)
{
    /* ��ֹ��δ���� gmw_init ǰ������������ */
    if (pCGI->inited != CGI_INITED)
        return -1;

    switch (type) {
        case 1:
        default:
            gmw_set_frame_linetype(pCGI);
            break;
        case 2:
            gmw_set_frame_linetype(pCGI, "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��");
            break;
        case 3:
            gmw_set_frame_linetype(pCGI, "�V", "�\", "�Y", "�_", "�T", "��", "�h", "�k", "�b", "�e", "��");
            break;
        case 4:
            gmw_set_frame_linetype(pCGI, "�W", "�]", "�Z", "�`", "��", "�U", "�i", "�l", "�c", "�f", "�o");
            break;
    }

    return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_FRAME_TYPE �ṹ�е�11������
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
            const char *...						����11�֣������.h���˴���
  �� �� ֵ��
  ˵    ����Լ��Ϊһ�������Ʊ��������ʹ���������ݣ���Ϊ��֤2�ֽ�
            1������2�ֽ���ֻȡǰ2�ֽ�
            2�������NULL���������ո����
            3�������1�ֽڣ���һ���ո������˶�������ʾ�ң������
***************************************************************************/
int gmw_set_frame_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const char *top_left, const char *lower_left, const char *top_right,
    const char *lower_right, const char *h_normal, const char *v_normal, const char *h_top_separator,
    const char *h_lower_separator, const char *v_left_separator, const char *v_right_separator, const char *mid_separator)
{
    /* ��ֹ��δ���� gmw_init ǰ������������ */
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

    return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_FRAME_TYPE �ṹ�е�ɫ��������С���Ƿ���Ҫ�ָ��ߵ�
  ������������������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
            const int block_width						����ȣ�����ȱʡ2����ΪԼ�������Ϊ�����Ʊ�����������������Ҫ+1��
            const int block_high						���߶ȣ�����ȱʡ1��
            const bool separator						���Ƿ���Ҫ�ָ��ߣ�ȱʡΪtrue����Ҫ�ָ��ߣ�
  �� �� ֵ��
  ˵    ������ܴ�С/�Ƿ���Ҫ�ָ��ߵȵı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_frame_style(CONSOLE_GRAPHICS_INFO *const pCGI, const int block_width, const int block_high, const bool separator)
{
    /* ��ֹ��δ���� gmw_init ǰ������������ */
    if (pCGI->inited != CGI_INITED)
        return -1;

    pCGI->CFI.block_width = (block_width < 0 ? 2 : block_width + (block_width & 1));
    pCGI->CFI.block_high = (block_high < 0 ? 1 : block_high);
    pCGI->CFI.separator = separator;

    calc_CGI(pCGI);

    return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_BORDER_TYPE �ṹ�е���ɫ
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
            const int bg_color					������ɫ��ȱʡ -1��ʾ�ô��ڱ���ɫ��
            const int fg_color					��ǰ��ɫ��ȱʡ -1��ʾ�ô���ǰ��ɫ��
  �� �� ֵ��
  ˵    �����������ɫֵ���󼰳�ͻ����Ҫ��Ϊ��֤
                ������ɫ��0-15��ǰ��ɫ����ɫ��ֵһ�µ����޷��������ݵ�
***************************************************************************/
int gmw_set_frame_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int bgcolor, const int fgcolor)
{
    /* ��ֹ��δ���� gmw_init ǰ������������ */
    if (pCGI->inited != CGI_INITED)
        return -1;

    pCGI->CFI.bgcolor = bgcolor;
    pCGI->CFI.fgcolor = fgcolor;

    return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_BLOCK_INFO �ṹ�е�6�����ͣ�ȱʡ4�֣�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
            const int type						��1 - ȫ˫�� 2 - ȫ���� 3 - ��˫���� 4 - �ᵥ��˫
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int gmw_set_block_default_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const int type)
{
    /* ��ֹ��δ���� gmw_init ǰ������������ */
    if (pCGI->inited != CGI_INITED)
        return -1;

    switch (type) {
        case 1:
        default:
            gmw_set_block_linetype(pCGI);
        case 2:
            gmw_set_block_linetype(pCGI, "��", "��", "��", "��", "��", "��");
        case 3:
            gmw_set_block_linetype(pCGI, "�V", "�\", "�Y", "�_", "�T", "��");
        case 4:
            gmw_set_block_linetype(pCGI, "�W", "�]", "�Z", "�`", "��", "�U");
    }

    return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_BLOCK_INFO �ṹ�е�6������
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
           const char *...					����6�֣������.h���˴���
  �� �� ֵ��
  ˵    ����Լ��Ϊһ�������Ʊ��������ʹ���������ݣ���Ϊ��֤2�ֽ�
            1������2�ֽ���ֻȡǰ2�ֽ�
            2�������NULL���������ո����
            3�������1�ֽڣ���һ���ո������˶�������ʾ�ң������
***************************************************************************/
int gmw_set_block_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const char *top_left, const char *lower_left, const char *top_right, const char *lower_right, const char *h_normal, const char *v_normal)
{
    /* ��ֹ��δ���� gmw_init ǰ������������ */
    if (pCGI->inited != CGI_INITED)
        return -1;

    myStrCpy(pCGI->CBI.top_left, top_left, 2);
    myStrCpy(pCGI->CBI.lower_left, lower_left, 2);
    myStrCpy(pCGI->CBI.top_right, top_right, 2);
    myStrCpy(pCGI->CBI.lower_right, lower_right, 2);
    myStrCpy(pCGI->CBI.h_normal, h_normal, 2);
    myStrCpy(pCGI->CBI.v_normal, v_normal, 2);

    return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�����ÿ����Ϸɫ��(����)�Ƿ���ҪС�߿�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
           const bool on_off					��true - ��Ҫ flase - ����Ҫ��ȱʡfalse��
  �� �� ֵ��
  ˵    �����߿�Լ��Ϊ�����Ʊ����˫��
***************************************************************************/
int gmw_set_block_border_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
    /* ��ֹ��δ���� gmw_init ǰ������������ */
    if (pCGI->inited != CGI_INITED)
        return -1;

    pCGI->CBI.block_border = on_off;

    return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������Ƿ���ʾ����״̬��
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
            const int type						��״̬�����ͣ���/�£�
            const bool on_off					��true - ��Ҫ flase - ����Ҫ��ȱʡtrue��
  �� �� ֵ��
  ˵    ����1��״̬�������Լ�����£�
               1.1����״̬��ֻ��һ�У������������Ϸ�����/�б�����棬Ϊ��������ʼһ�У�����������Ͻ����������״̬�������꣩
               1.2����״̬��ֻ��һ�У������������·����ߵ�����
               1.3��״̬���Ŀ��Ϊ�������ȣ������Ϣ������ض�
           2�����еı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_status_line_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const bool on_off)
{
    /* ��ֹ��δ���� gmw_init ǰ������������ */
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

    return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���������״̬������ɫ
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
            const int type						��״̬�����ͣ���/�£�
            const int normal_bgcolor			�������ı�����ɫ��ȱʡ -1��ʾʹ�ô��ڱ���ɫ��
            const int normal_fgcolor			�������ı�ǰ��ɫ��ȱʡ -1��ʾʹ�ô���ǰ��ɫ��
            const int catchy_bgcolor			����Ŀ�ı�����ɫ��ȱʡ -1��ʾʹ�ô��ڱ���ɫ��
            const int catchy_fgcolor			����Ŀ�ı�ǰ��ɫ��ȱʡ -1��ʾʹ������ɫ��
  ���������
  �� �� ֵ��
  ˵    �����������ɫֵ���󼰳�ͻ����Ҫ��Ϊ��֤
                ������ɫ��0-15��ǰ��ɫ����ɫ��ֵһ�µ����޷��������ݵ�
***************************************************************************/
int gmw_set_status_line_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const int normal_bgcolor, const int normal_fgcolor, const int catchy_bgcolor, const int catchy_fgcolor)
{
    /* ��ֹ��δ���� gmw_init ǰ������������ */
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

    return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������Ƿ���ʾ�к�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
            const bool on_off					��true - ��ʾ flase - ����ʾ��ȱʡfalse��
  �� �� ֵ��
  ˵    ����1���к�Լ��Ϊ��ĸA��ʼ�������У��������26�����a��ʼ������52��ͳһΪ*��ʵ��Ӧ�ò����ܣ�
            2���Ƿ���ʾ�кŵı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_rowno_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
    /* ��ֹ��δ���� gmw_init ǰ������������ */
    if (pCGI->inited != CGI_INITED)
        return -1;

    pCGI->draw_frame_with_row_no = on_off;

    calc_CGI(pCGI);

    return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������Ƿ���ʾ�б�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
            const bool on_off					��true - ��ʾ flase - ����ʾ��ȱʡfalse��
  �� �� ֵ��
  ˵    ����1���б�Լ��Ϊ����0��ʼ�������У�����0-99������99ͳһΪ**��ʵ��Ӧ�ò����ܣ�
            2���Ƿ���ʾ�б�ı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_colno_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
    /* ��ֹ��δ���� gmw_init ǰ������������ */
    if (pCGI->inited != CGI_INITED)
        return -1;

    pCGI->draw_frame_with_col_no = on_off;

    calc_CGI(pCGI);

    return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���ӡ CONSOLE_GRAPHICS_INFO �ṹ���еĸ���Աֵ
  ���������
  �� �� ֵ��
  ˵    ����1�����������ã���ӡ��ʽ�Զ���
            2������������������δ���ù������Բ�ʵ��
***************************************************************************/
int gmw_print(const CONSOLE_GRAPHICS_INFO *const pCGI)
{
    /* ��ֹ��δ���� gmw_init ǰ������������ */
    if (pCGI->inited != CGI_INITED)
        return -1;

    return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��� CONSOLE_GRAPHICS_INFO �ṹ����ȱʡֵ���г�ʼ��
  ���������CONSOLE_GRAPHICS_INFO *const pCGI������ṹָ��
           const int row					����Ϸ����ɫ��������ȱʡ10��
           const int col					����Ϸ����ɫ��������ȱʡ10��
           const int bgcolor				���������ڱ���ɫ��ȱʡ COLOR_BLACK��
           const int fgcolor				���������ڱ���ɫ��ȱʡ COLOR_WHITE��
  �� �� ֵ��
  ˵    �������ڱ�����/ǰ���ף�����16*8�����������޶������У�����״̬�����У����к�/�б꣬�������Ϊ˫�ߣ�ɫ����2/�߶�1/��С�߿���ɫ��
***************************************************************************/
int gmw_init(CONSOLE_GRAPHICS_INFO *const pCGI, const int row, const int col, const int bgcolor, const int fgcolor)
{
    /* �����ñ�� */
    pCGI->inited = CGI_INITED;

    // ���ܿ�λextern_udlr_lc
    gmw_set_ext_rowcol(pCGI);

    // ��Ϸ��ɫ������
    gmw_set_rowcol(pCGI, row, col);

    // ȫ������ɫ
    gmw_set_color(pCGI, 0, 7, 1);

    // ���б��
    gmw_set_colno_switch(pCGI);
    gmw_set_rowno_switch(pCGI);

    // ��ʱʱ������
    gmw_set_delay(pCGI, DELAY_OF_DRAW_FRAME, 0);
    //pCGI->delay_of_draw_frame = 0;
    gmw_set_delay(pCGI, DELAY_OF_DRAW_BLOCK, 0);
    //pCGI->delay_of_draw_block = 0;
    gmw_set_delay(pCGI, DELAY_OF_BLOCK_MOVED, 0);
    //pCGI->delay_of_block_moved = 0;

    // �������Ϣ CFI
    gmw_set_frame_default_linetype(pCGI, 1);
    gmw_set_frame_style(pCGI);
    gmw_set_frame_color(pCGI);

    // ɫ����Ϣ CBI
    gmw_set_block_default_linetype(pCGI, 1);
    gmw_set_block_border_switch(pCGI);

    // ״̬�� SLI
    gmw_set_status_line_switch(pCGI, TOP_STATUS_LINE, 1);
    gmw_set_status_line_switch(pCGI, LOWER_STATUS_LINE, 1);
    gmw_set_status_line_color(pCGI, TOP_STATUS_LINE);
    gmw_set_status_line_color(pCGI, LOWER_STATUS_LINE);

    // ���� CFT
    gmw_set_font(pCGI);


    calc_CGI(pCGI);		//������������


    return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�������Ϸ���
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
  �� �� ֵ��
  ˵    ��������ɲο�demo��Ч��
***************************************************************************/
int gmw_draw_frame(const CONSOLE_GRAPHICS_INFO *const pCGI)
{
    /* ��ֹ��δ���� gmw_init ǰ������������ */
    if (pCGI->inited != CGI_INITED)
        return -1;

    //���ô���
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

    //���
    cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);	//�ָ�������ɫ
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

    cct_gotoxy(0, 0);   //��긴λ

    return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���״̬������ʾ��Ϣ
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
           const int type							��ָ������/��״̬��
           const char *msg						��������Ϣ
           const char *catchy_msg					����Ҫ�ر��ע����Ϣ����������Ϣǰ��ʾ��
  �� �� ֵ��
  ˵    ����1���������޶�Ϊ����ܵĿ�ȣ����к��б�λ�ã����������ȥ
            2��������һ���ַ���ĳ���ֵ�ǰ������ᵼ�º������룬Ҫ����
***************************************************************************/
int gmw_status_line(const CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const char *msg, const char *catchy_msg)
{
    /* ��ֹ��δ���� gmw_init ǰ������������ */
    if (pCGI->inited != CGI_INITED)
        return -1;


    int cnt_all = 1, cnt_cn;
    int stx = 0, sty = 0, c_bgc = 0, c_fgc = 0, bgc = 0, fgc = 0;

    if (type == TOP_STATUS_LINE) {
        if (!pCGI->top_status_line) {	//�������
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
        if (!pCGI->lower_status_line) {		//�������
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

    cct_gotoxy(0, 0);   //��긴λ

    return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���ʾĳһ��ɫ��(����Ϊ�ַ���������Ϊrow/col)
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
           const int row_no						���кţ���0��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
           const int col_no						���кţ���0��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
           const int bdi_value						����Ҫ��ʾ��ֵ
           const BLOCK_DISPLAY_INFO *const bdi		����Ÿ�ֵ��Ӧ����ʾ��Ϣ�Ľṹ������
  �� �� ֵ��
  ˵    ����1��BLOCK_DISPLAY_INFO �ĺ����ͷ�ļ����÷��ο���������
            2��bdi_valueΪ BDI_VALUE_BLANK ��ʾ�հ׿飬Ҫ���⴦��
***************************************************************************/
int gmw_draw_block(const CONSOLE_GRAPHICS_INFO *const pCGI, const int row_no, const int col_no, const int bdi_value, const BLOCK_DISPLAY_INFO *const bdi)
{
    /* ��ֹ��δ���� gmw_init ǰ������������ */
    if (pCGI->inited != CGI_INITED)
        return -1;

    //�˴�i,j���ǳ����˵����꣨����charΪһ���㣩
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

    cct_setcolor(); //�ָ�Ĭ����ɫ

    cct_gotoxy(0, 0);   //��긴λ

    Sleep(pCGI->delay_of_draw_block);   //��ʱ

    return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��ƶ�ĳһ��ɫ��
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
           const int row_no						���кţ���0��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
           const int col_no						���кţ���0��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
           const int bdi_value						����Ҫ��ʾ��ֵ
           const int blank_bdi_value				���ƶ����������ڶ���Ч����ʾʱ���ڱ�ʾ�հ׵�ֵ��һ��Ϊ0���˴���Ϊ�������룬�ǿ��ǵ����ܳ��ֵ����������
           const BLOCK_DISPLAY_INFO *const bdi		�������ʾֵ/�հ�ֵ��Ӧ����ʾ��Ϣ�Ľṹ������
           const int direction						���ƶ�����һ�����֣������cmd_gmw_tools.h
           const int distance						���ƶ����루��1��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
  �� �� ֵ��
  ˵    ����0:up 1:down 2:left 3:right�����ƶ������룬�����ķ��񶼻ᱻ blank_bdi_value ���ǣ����¹켣�������Խ���һ��һ�����
***************************************************************************/
int gmw_move_block(const CONSOLE_GRAPHICS_INFO *const pCGI, const int row_no, const int col_no, const int bdi_value, const int blank_bdi_value, const BLOCK_DISPLAY_INFO *const bdi, const int direction, const int distance)
{
    /* ��ֹ��δ���� gmw_init ǰ������������ */
    if (pCGI->inited != CGI_INITED)
        return -1;

    const int fx[4][2] = { {0,-1},{0,1},{-1,0},{1,0} };

    //�˴�i,j���ǳ����˵����꣨����charΪһ���㣩
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
            //��ܣ�ֻ�����ƶ�һ���ص��¶��������һС��
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

            //�հ�ֵ
            gmw_draw_block(pCGI, lstRow, lstCol, blank_bdi_value, bdi);

            //�ƶ�����
            startj += fx[direction][0];
            starti += fx[direction][1];
            draw_block(pCGI, pBDI, starti, startj, ilen, jlen);

            //�ӳ�
            Sleep(pCGI->delay_of_block_moved);
        }
        lstCol += fx[direction][0];
        lstRow += fx[direction][1];
    }

    cct_gotoxy(0, 0);   //��긴λ

    return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������̻����
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
           int &MAction							��������� CCT_MOUSE_EVENT�����ֵ��Ч��Ϊ MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK/MOUSE_RIGHT_BUTTON_CLICK ����֮һ
                                                       ������� CCT_KEYBOARD_EVENT�����ֵ��Ч
           int &MRow								��������� CCT_MOUSE_EVENT �� MAction = MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK�����ֵ��Ч����ʾ���ѡ�����Ϸ������кţ���0��ʼ��
                                                  ���������ֵ��Ч�����������Чֵ���´��󣬲��Ǳ������Ĵ�!!!��
           int &MCol								��������� CCT_MOUSE_EVENT �� MAction = MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK�����ֵ��Ч����ʾ���ѡ�����Ϸ������кţ���0��ʼ��
                                                  ���������ֵ��Ч�����������Чֵ���´��󣬲��Ǳ������Ĵ�!!!��
           int &KeyCode1							��������� CCT_KEYBOARD_EVENT�����ֵ��Ч��Ϊ�����ļ��루���˫���룬��Ϊ��һ����
                                                  ���������ֵ��Ч�����������Чֵ���´��󣬲��Ǳ������Ĵ�!!!��
           int &KeyCode2							��������� CCT_KEYBOARD_EVENT�����ֵ��Ч��Ϊ�����ļ��루���˫���룬��Ϊ�ڶ���������ǵ����룬��Ϊ0��
                                                  ���������ֵ��Ч�����������Чֵ���´��󣬲��Ǳ������Ĵ�!!!��
           const bool update_lower_status_line		������ƶ�ʱ���Ƿ�Ҫ�ڱ���������ʾ"[��ǰ���] *��*��/λ�÷Ƿ�"����Ϣ��true=��ʾ��false=����ʾ��ȱʡΪtrue��
  �� �� ֵ����������Լ��
           1�����������ƶ����õ���MRow/MCol�봫�����ͬ(���ָ��΢С���ƶ�)���򲻷��أ�������
                              �õ����зǷ�λ�ã��򲻷��أ����� update_lower_status_line ����������״̬����ʾ"[��ǰ���] λ�÷Ƿ�"
                              �õ���MRow/MCol�봫��Ĳ�ͬ(��������һ���仯)������ update_lower_status_line ����������״̬����ʾ"[��ǰ���] *��*��"���ٷ���??MOUSE_ONLY_MOVED����Щ��Ϸ���غ�Ҫ����ɫ��Ĳ�ͬ��ɫ��ʾ��
           2������ǰ������������ҵ�ǰ���ָ��ͣ��������Ϸ�����*��*���ϣ��򷵻� CCT_MOUSE_EVENT ��MAction Ϊ MOUSE_LEFT_BUTTON_CLICK, MRow Ϊ�кţ�MCol Ϊ�б�
                                  �ҵ�ǰ���ָ��ͣ���ڷǷ����򣨷���Ϸ������Ϸ�����еķָ��ߣ����򲻷��أ����� update_lower_status_line ����������״̬����ʾ"[��ǰ���] λ�÷Ƿ�"
           3������ǰ�������Ҽ������ж����ָ��ͣ�������Ƿ�Ϸ���ֱ�ӷ��� CCT_MOUSE_EVENT ��MAction Ϊ MOUSE_RIGHT_BUTTON_CLICK, MRow��MColȡ��ǰֵ
           4��������¼����ϵ�ĳ������˫���밴��������ֱ�ӷ��� CCT_KEYBOARD_EVENT��KeyCode1/KeyCode2��Ϊ��Ӧ�ļ���ֵ
 ˵    ����ͨ������ cmd_console_tools.cpp �е� read_keyboard_and_mouse ����ʵ��
***************************************************************************/
int gmw_read_keyboard_and_mouse(const CONSOLE_GRAPHICS_INFO *const pCGI, int &MAction, int &MRow, int &MCol, int &KeyCode1, int &KeyCode2, const bool update_lower_status_line)
{
    /* ��ֹ��δ���� gmw_init ǰ������������ */
    if (pCGI->inited != CGI_INITED)
        return -1;

    int cctRet, mx = -1, my = -1, mrow, mcol, bx, by;
    bx = (pCGI->CFI.block_width + pCGI->CFI.block_width_ext);   //һ����չ������
    by = (pCGI->CFI.block_high + pCGI->CFI.block_high_ext);     //...

    char msg[100];

    cct_enable_mouse(); //�������
    while (1) {

        cctRet = cct_read_keyboard_and_mouse(mx, my, MAction, KeyCode1, KeyCode2);


        if (cctRet == CCT_KEYBOARD_EVENT) {
            if (update_lower_status_line) {
                if (KeyCode2 == 0) {    //���ַ���λ
                    sprintf(msg, "[��������] %d", KeyCode1);
                }
                else if (KeyCode1 == 0xe0) {
                    switch (KeyCode2) {
                        case KB_ARROW_UP:
                            sprintf(msg, "[��������] ��");
                            break;
                        case KB_ARROW_DOWN:
                            sprintf(msg, "[��������] ��");
                            break;
                        case KB_ARROW_LEFT:
                            sprintf(msg, "[��������] ��");
                            break;
                        case KB_ARROW_RIGHT:
                            sprintf(msg, "[��������] ��");
                            break;
                    }
                }
                else {
                    sprintf(msg, "[��������] δ֪");
                }
                gmw_status_line(pCGI, LOWER_STATUS_LINE, msg);
            }

            break;
        }

        //����ȫ��CCT_MOUSE_EVENT

        //����Ҽ�
        if (MAction == MOUSE_RIGHT_BUTTON_CLICK) {
            if (update_lower_status_line)
                gmw_status_line(pCGI, LOWER_STATUS_LINE, "[��������Ҽ�]");
            break;
        }

        //���� mcol, mrow
        mx -= START_X + 2;    //+2����Ϸ��������
        my -= START_Y + 1;
        mcol = mx < 0 ? -1 : mx / bx;
        mrow = my < 0 ? -1 : my / by;


        if (mcol < 0 || mcol >= pCGI->col_num || (pCGI->CFI.separator && (mx + 1) % bx == 0) || mrow < 0 || mrow >= pCGI->row_num || (pCGI->CFI.separator && (my + 1) % by == 0)) {
            //�Ƿ�λ��
            if (update_lower_status_line) {
                sprintf(msg, "[��ǰ���] λ�÷Ƿ�");
                gmw_status_line(pCGI, LOWER_STATUS_LINE, msg);
            }
            continue;   //������
        }
        else {
            //�Ϸ�λ��
            if (update_lower_status_line) {
                sprintf(msg, "[��ǰ���] %c��%d��", rowNo(mrow), mcol);
                gmw_status_line(pCGI, LOWER_STATUS_LINE, msg);
            }

            //��������� �� ����һ�� �� ���ֵ���
            if (MAction == MOUSE_LEFT_BUTTON_CLICK || MAction==MOUSE_LEFTRIGHT_BUTTON_CLICK || MAction==MOUSE_WHEEL_CLICK) {
                MRow = mrow;
                MCol = mcol;
                break;
            }

            //����ƶ�
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

    cct_disable_mouse();	//�������
    return cctRet;

    return 0; //�˾�ɸ�����Ҫ�޸�
}


//bool &top_status_line = SLI.is_top_status_line;
//bool &lower_status_line = SLI.is_lower_status_line;