/* 2051995 ���� ���� */
#include <iostream>
#include <iomanip>
#include <cstring>
using namespace std;
typedef short DATA;
const char *STR_INIT = "��ʼ״̬��";
const char *STR_CUR = "��ǰ״̬1��";
const short BIT_EXIT = 1 << 14;

inline void show(const char *hint, DATA &_dat)
{
    cout << hint << "0x" << setfill('0') << setw(4) << hex << _dat << endl;
    puts("A   B   C   D   E   F   G   H   I   J");
    for (int i = 1, mi = 1 << 9; i <= mi; i <<= 1)
        cout << (i & _dat ? "ON" : "OFF") << (i == mi ? "" : " ");
    puts("");
    cout << endl;
}

short modify(int _dat)
{
    char bitIndex =' ',tmp[10];
    int k;
    
    while (1) {
        
        puts("����(\"A On / J Off\"��ʽ���룬����\"Q on / off\"�˳�)");

        for (cin.get(bitIndex); cin.good() && isspace(bitIndex); )
            cin.get(bitIndex);

        if (!cin.good()) {
            cin.ignore(1926,'\n');
            continue;
        }

        
        if (isalpha(bitIndex)) {
            k = tolower(bitIndex) - 'a';
            if (k >= 0 && k <= 9) {
                cin >> tmp;
                for (int i = 0; i < 10 && tmp[i] != '\0'; i++)
                    if (isalpha(tmp[i]))
                        tmp[i] = tolower(tmp[i]);
                if (strcmp(tmp, "on") == 0) {
                    return (_dat | (1 << k));
                }
                else if (strcmp(tmp, "off") == 0) {
                    return (_dat & (_dat ^ (1 << k)));
                }
            }
            else if (k == 17) {
                return  (_dat | BIT_EXIT);
            }
        }

        cin.ignore(1926,'\n');
    }
    return -1;
}

int main()
{
    DATA dat = 0;
    show(STR_INIT, dat);
    while (~dat & BIT_EXIT) {
        dat = modify(dat);
        if (~dat & BIT_EXIT)
            show(STR_CUR, dat);
    }
    return 0;
}