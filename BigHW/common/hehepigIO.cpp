/* 朱泽凯 2051995 济美 */
#include "../include/hehepigIO.h"
#include <iostream>
#include <conio.h>
using namespace std;


int GetInt(int Min, int Max) {
    return GetInt("", Min, Max);
}

int GetInt(const char* HintStr) {
    int ret;
    while (1) {
        cout << HintStr;
        cin >> ret;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1024, '\n');
        }
        else {
            break;
        }
    }
    cin.ignore(1024, '\n');
    return ret;
}



int GetInt(const char* HintStr, int Min, int Max) {
    int ret;
    while (1) {
        cout << HintStr;
        cin >> ret;
        if (cin.fail() || ret<Min || ret>Max) {
            cin.clear();
            cin.ignore(1024, '\n');
        }
        else {
            break;
        }
    }
    cin.ignore(1024, '\n');
    return ret;
}


/// <summary>
/// <para>辅助 GetKey 进行回显</para>
/// <para>若是显式 ASCII 码值则输出对应的字符，否则看情况吧</para>
/// </summary>
/// <param name="ret">待回显的值</param>
void EchoPrint(int ret) {
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


int GetKey(int Echo) {
    int ch, ret;
    ch = _getch();

    if (ch == 224) {
        switch (_getch()) {
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
    else if (ch == 0) {
        ch = _getch();
        ret = key_Fn;
    }
    else if (ch > 0 && ch < 128) {
        ret = ch;
    }
    else {
        ret = key_unknown;
    }

    if (Echo)
        EchoPrint(ret);

    return ret;
}

int GetKey(int Min, int Max, int Echo) {
    int ret;
    do {
        ret = GetKey(0);
    } while (ret<Min || ret>Max);

    if (Echo)
        EchoPrint(ret);

    return ret;
}