/* ÷Ï‘Ûø≠ 2051995 º√√¿ */
#include "../include/hehepigIO.h"
#include <iostream>
#include <conio.h>
using namespace std;

void getascii() {
    int ch;
    while (1) {
        ch = _getch();
        cout << ch << endl;
    }
}

int main()
{
    cout << GetInt(1, 10) << endl;

    //getascii();

    cout <<GetKey() <<endl;
    GetKey(1);
    cout << endl;
    cout << GetKey('a', 'z') <<" " << endl;
    GetKey('0', '9', 1);
    cout << endl;


    cout << GetInt("GetInt(\"\") >") << endl;
    cout << GetInt("GetInt(\"\", 10, 15) >", 10, 15) << endl;
    cout << endl;
    

    return 0;
}