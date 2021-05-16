/* 朱泽凯 2051995 济美 */
#include "../include/hehepigMenu.h"
#include "../include/hehepigIO.h"
#include <cstring>
#include <iostream>
#include <iomanip>
using namespace std;

/// 大小写不敏感地判断两个 int 对应的 char 是否相同，若值不在 char 范围内，直接判断是否相同
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

    // 输出表格
    cout << setw(Maxlen + 1) << setfill('-') << "" << endl;
    for (int i = 0; i < n; i++)
        cout << OprChar[i] << '.' << HintStr[i] << endl;
    cout << setw(Maxlen + 1) << setfill('-') << "" << endl;
    cout << setfill(' ');   //归位setfill
    cout << "> ";

    // 获取选择
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