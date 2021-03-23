/* 朱泽凯 2051995 济美 */
#include <iostream>
using namespace std;
int main()
{
    int m = -1;
    const char *monthName[] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };
    cout << "请输入月份(1-12)" << endl;
    cout << ((cin >> m) && !cin.fail() && m >= 1 && m <= 12 ? monthName[m - 1] : "Invalid") << endl;
}