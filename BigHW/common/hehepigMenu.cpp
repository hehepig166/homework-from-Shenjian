/* ���� 2051995 ���� */
#include "../include/hehepigMenu.h"
#include "../include/hehepigIO.h"
#include <cstring>
#include <iostream>
#include <iomanip>
using namespace std;

/// ��Сд�����е��ж����� int ��Ӧ�� char �Ƿ���ͬ����ֵ���� char ��Χ�ڣ�ֱ���ж��Ƿ���ͬ
bool InsensitiveCheck(int x, int y)
{
    return
        (x >= 'A' && x <= 'Z' ? x + 'a' - 'A' : x)
        ==
        (y >= 'A' && y <= 'Z' ? y + 'a' - 'A' : y);
}


int Menu_char(int n, const char *OprChar, const char *HintStr[], int CaseSensitive)
{
    int ret, Maxlen;
    const char *p;

    for (int i = Maxlen = 0; i < n; i++)
        Maxlen = max(Maxlen, (int)strlen(HintStr[i]));

    // ������
    cout << setw(Maxlen + 1) << setfill('-') << "" << endl;
    for (int i = 0; i < n; i++)
        cout << OprChar[i] << '.' << HintStr[i] << endl;
    cout << setw(Maxlen + 1) << setfill('-') << "" << endl;
    cout << setfill(' ');   //��λsetfill
    cout << "> ";

    // ��ȡѡ��
    while (1)
    {
        ret = GetKey();
        for (p = OprChar + n - 1; p >= OprChar; p--)
        {
            if (!CaseSensitive && InsensitiveCheck(*p, ret)) {
                putchar(ret);
                puts("");
                return ret;
            }
            else if (*p == ret) {
                putchar(ret);
                puts("");
                return ret;
            }
        }
    }
}