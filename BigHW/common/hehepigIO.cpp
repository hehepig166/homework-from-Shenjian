/* ���� 2051995 ���� */
#include "../include/hehepigIO.h"
#include <iostream>
#include <conio.h>
using namespace std;


int GetInt(int Min, int Max)
{
    return GetInt("", Min, Max);
}

int GetInt(const char *HintStr)
{
    int ret;
    while (1)
    {
        cout << HintStr;
        cin >> ret;
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1024, '\n');
        }
        else
        {
            break;
        }
    }
    cin.ignore(1024, '\n');
    return ret;
}



int GetInt(const char *HintStr, int Min, int Max)
{
    int ret;
    while (1)
    {
        cout << HintStr;
        cin >> ret;
        if (cin.fail() || ret<Min || ret>Max)
        {
            cin.clear();
            cin.ignore(1024, '\n');
        }
        else
        {
            break;
        }
    }
    cin.ignore(1024, '\n');
    return ret;
}


/// <summary>
/// <para>���� GetKey ���л���</para>
/// <para>������ʽ ASCII ��ֵ�������Ӧ���ַ������������</para>
/// </summary>
/// <param name="ret">�����Ե�ֵ</param>
void EchoPrint(int ret)
{
    if (ret >= 32 && ret <= 126)
        putchar(ret);
    else if (ret == key_up)
        cout << "key_up";
    else if (ret == key_down)
        cout << "key_down";
    else if (ret == key_left)
        cout << "key_left";
    else if (ret == key_right)
        cout << "key_right";
    else if (ret == key_Fn)
        cout << "key_Fn";
    else
        cout << "can't print";
}

int GetKey(int Echo)
{
    int ch, ret;
    ch = _getch();

    if (ch == 224)
    {
        switch (_getch())
        {
            case 72:
                ret = key_up;
                break;
            case 75:
                ret = key_left;
                break;
            case 77:
                ret = key_right;
                break;
            case 80:
                ret = key_down;
                break;
            default:
                ret = key_unknown;
        }
    }
    else if (ch == 0)
    {
        ch = _getch();
        ret = key_Fn;
    }
    else if (ch > 0 && ch < 128)
    {
        ret = ch;
    }
    else
    {
        ret = key_unknown;
    }

    if (Echo)
        EchoPrint(ret);

    return ret;
}

int GetKey(int Min, int Max, int Echo)
{
    int ret = 0;
    do
    {
        ret = GetKey(0);
    } while (ret<Min || ret>Max);

    if (Echo)
        EchoPrint(ret);

    return ret;
}

int GetAlpha(int Mode, int Echo)
{
    // Mode==0: ����ԭ��ĸ��Ӧ��ASCII
    // Mode==1: ���ش�дASCII
    // Mode==2: ����СдASCII
    int ret = 0;
    while (1)
    {
        ret = GetKey(Echo);
        if (isalpha(ret))
        {
            switch (Mode)
            {
                case 0:
                    return ret;
                case 1:
                    return toupper(ret);
                case 2:
                    return tolower(ret);
            }
        }
    }
    return 0;
}

int GetAlnum(int Mode, int Echo)
{
    int ret = 0;
    while (1)
    {
        ret = GetKey(Echo);
        if (isalpha(ret))
        {
            switch (Mode)
            {
                case 0:
                    return ret;
                case 1:
                    return toupper(ret);
                case 2:
                    return tolower(ret);
            }
        }
        else if (isdigit(ret))
        {
            return ret;
        }
    }
    return 0;
}

int AskKey()
{
    int ret;
    if (_kbhit())
    {
        int ch = _getch();
        if (ch == 224)
        {
            switch (_getch())
            {
                case 72:
                    ret = key_up;
                    break;
                case 75:
                    ret = key_left;
                    break;
                case 77:
                    ret = key_right;
                    break;
                case 80:
                    ret = key_down;
                    break;
                default:
                    ret = key_unknown;
            }
        }
        else if (ch == 0)
        {
            ch = _getch();
            ret = key_Fn;
        }
        else if (ch > 0 && ch < 128)
        {
            ret = ch;
        }
        else
        {
            ret = key_unknown;
        }
    }
    else
    {
        ret = key_none;
    }

    return ret;
}