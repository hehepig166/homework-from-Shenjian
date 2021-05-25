/* 2051995 朱泽凯 济美 */
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>

#include "16-b2-date.h"
#include "16-b2-time.h"
#include "16-b2-datetime.h"
using namespace std;

#if defined(__linux) || defined(__linux__) //Linux

void wait_for_enter()
{
    cout << endl << "按回车键继续";
    while (getchar() != '\n')
        ;
    cout << endl << endl;
}

#else //VS+Dev
#include <Windows.h>
#include <conio.h>


void wait_for_enter()
{
    cout << endl << "按回车键继续";
    while (_getch() != '\r')
        ;
    cout << endl << endl;
}
#endif


int test_date();




int main()
{

    Date da(1926, 8, 17), db(2002);
    cout << Date(0) << endl;
    cout << Date(1) << endl;
    cout << (Date(1900, 1, 1) - 1) << endl;
    cout << (Date(2099, 12, 31) + 1) << endl;
    cout << Date(2099, 12, 31) + (73048) << endl;
    cout << Date(2099, 12, 31) + (4 * 73049) << endl;
    cout << da - db - 15 << endl;
    cout << da - 15 - db << endl;
    cout << da - 19260817 << endl;
    cout << (da < db) << endl;
    cout << long long(da) << endl;
    cout << long long(Date(0)) << endl;

    puts("");


    Time ta(19, 26, 8), tb(17);

    cout << Time(15) << endl;
    cout << (Time(0) - 16) << endl;
    cout << Time(24 * 60 * 60 + 1) << endl;
    cout << Time(24 * 60 * 60) + 1 << endl;
    cout << ta - tb - 15 << endl;
    cout << (ta < tb) << endl;
    cout << int(ta) << endl;


    //test_date();

    return 0;
}












int test_date()
{
    if (1) {
        Date d1;				//2000.1.1
        Date d2(2021, 4, 17);	//2021.4.17

        cout << "测试初始化，完全正确的赋值" << endl;

        cout << "d1应该是2000.1.1， 实际为：";
        d1.show();
        cout << "d2应该是2021.4.17，实际为：";
        d2.show();

        wait_for_enter();
    }

    if (1) {
        Date d1(1870, 3, 15);  //2000.3.15
        Date d2(2112, 3, 4);   //2000.3.4
        Date d3(2005, 13, 15); //2005.1.15
        Date d4(2021, 4, 32);  //2021.4.1

        cout << "测试初始化，年、月、日非法" << endl;

        cout << "d1应该是2000.3.15，实际为：";
        d1.show();
        cout << "d2应该是2000.3.4， 实际为：";
        d2.show();
        cout << "d3应该是2005.1.15，实际为：";
        d3.show();
        cout << "d4应该是2021.4.1， 实际为：";
        d4.show();

        /* 多个错，按年月日顺序处理 */
        Date d5(1899, 2, 29);  //2000.2.29
        Date d6(1899, 2, 30);  //2000.2.1
        Date d7(1899, 13, 31); //2000.1.31
        Date d8(1899, 13, 32); //2000.1.1
        cout << "d5应该是2000.2.29，实际为：";
        d5.show();
        cout << "d6应该是2000.2.1， 实际为：";
        d6.show();
        cout << "d7应该是2000.1.31，实际为：";
        d7.show();
        cout << "d8应该是2000.1.1， 实际为：";
        d8.show();

        wait_for_enter();
    }

    if (1) {
        Date d1(2021, 2, 29); //2021.2.1
        Date d2(2020, 2, 29); //2020.2.29

        cout << "测试初始化，闰年" << endl;

        cout << "d1应该是2021.2.1， 实际为：";
        d1.show();
        cout << "d2应该是2020.2.29，实际为：";
        d2.show();

        wait_for_enter();
    }

    if (1) {
        Date d1, d2, d3; //都是2000.1.1

        cout << "测试set，带缺省值" << endl;
        d1.set(2021, 4, 17);	//2021.4.17
        d2.set(2021, 4);		//2021.4.1
        d3.set(2021);		//2021.1.1

        cout << "d1应该是2021.4.17，实际为：";
        d1.show();
        cout << "d2应该是2021.4.1， 实际为：";
        d2.show();
        cout << "d3应该是2021.1.1， 实际为：";
        d3.show();

        wait_for_enter();
    }

    if (1) {
        Date d1(2021, 4, 17), d2(d1), d3(d1);

        cout << "测试set，部分不变" << endl;

        d1.set(2007, 0, 0);   //2007.4.17
        d2.set(0, 3, 18);   //2021.3.18
        d3.set(0, 0, 28);   //2021.4.28

        cout << "d1应该是2007.4.17，实际为：";
        d1.show();
        cout << "d2应该是2021.3.18，实际为：";
        d2.show();
        cout << "d3应该是2021.4.28，实际为：";
        d3.show();

        wait_for_enter();
    }

    if (1) {
        Date d1, d2, d3, d4;

        cout << "测试set，年、月、日非法" << endl;

        d1.set(2112, 2, 29);  //2000.2.29
        d2.set(2019, 2, 29);  //2019.2.1
        d3.set(2021, 5, 32);  //2021.5.1
        d4.set(2021, 13, 12); //2021.1.12

        cout << "d1应该是2000.2.29， 实际为：";
        d1.show();
        cout << "d2应该是2019.2.1，  实际为：";
        d2.show();
        cout << "d3应该是2021.5.1，  实际为：";
        d3.show();
        cout << "d4应该是2021.1.12， 实际为：";
        d4.show();

        /* 多个错+带缺省值，按年月日顺序处理 */
        Date d5(2020, 1, 31), d6(d5), d7(d5), d8(d5), d9(d5), d10(d5), d11(d5);  //2020.1.31

        d5.set(1899, 2, 29);  //2000.2.29
        d6.set(1899, 2, 30);  //2000.2.1
        d7.set(1899, 2, 0);   //2000.2.1
        d8.set(0, 2, 0);      //2020.2.1
        d9.set(0, 13, 32);    //2020.1.1
        d10.set(1899, 13, 31);  //2000.1.31
        d11.set(1899, 13, 32);  //2000.1.1
        cout << "d5应该是2000.2.29， 实际为：";
        d5.show();
        cout << "d6应该是2000.2.1，  实际为：";
        d6.show();
        cout << "d7应该是2000.2.1，  实际为：";
        d7.show();
        cout << "d8应该是2020.2.1，  实际为：";
        d8.show();
        cout << "d9应该是2020.1.1，  实际为：";
        d9.show();
        cout << "d10应该是2000.1.31，实际为：";
        d10.show();
        cout << "d11应该是2000.1.1， 实际为：";
        d11.show();

        Date d12(2020, 2, 29), d13(2021, 3, 31);

        d12.set(2021, 0, 0); //2021.2.1
        d13.set(0, 4, 0);   //2021.4.1

        cout << "d12应该是2021.2.1， 实际为：";
        d12.show();
        cout << "d13应该是2021.4.1， 实际为：";
        d13.show();

        wait_for_enter();
    }

    if (1) {
        Date d1(2021, 4, 17);
        int y, m, d;

        cout << "测试get" << endl;
        d1.get(y, m, d);

        cout << "应该y=2021,m=4,d=17，实际为：";
        cout << "y=" << y << ",m=" << m << ",d=" << d << endl;

        wait_for_enter();
    }

    if (1) {
        Date d1(2021, 4, 17);
        Date d2;

        cout << "测试cout" << endl;
        cout << "d1应该是2021.4.17，实际为：" << d1 << endl;
        cout << "d2应该是2000.1.1， 实际为：" << d2 << endl;

        wait_for_enter();
    }

    if (1) {
        Date d1;

        cout << "测试cin/cout" << endl;

        cout << "请输入2021 4 17" << endl;
        cin >> d1;
        cout << "d1应该是2020.4.17，实际为：" << d1 << endl;

        cout << "请输入2020 2 29" << endl;
        cin >> d1;
        cout << "d1应该是2020.2.29，实际为：" << d1 << endl;

        cout << "请输入2021 2 29" << endl;
        cin >> d1;
        cout << "d1应该是2021.2.1，实际为：" << d1 << endl;

        cout << "请输入1899 4 10" << endl;
        cin >> d1;
        cout << "d1应该是2000.4.10，实际为：" << d1 << endl;

        cout << "请输入2021 13 10" << endl;
        cin >> d1;
        cout << "d1应该是2021.1.10，实际为：" << d1 << endl;

        cout << "请输入2021 4 31" << endl;
        cin >> d1;
        cout << "d1应该是2021.4.1，实际为：" << d1 << endl;

        cout << "请输入2112 15 34" << endl;
        cin >> d1;
        cout << "d1应该是2000.1.1，实际为：" << d1 << endl;

        wait_for_enter();
    }

    if (1) {
        Date d1(1);          //1900.1.1
        Date d2(73049);      //2099.12.31
        Date d3(-100);       //1900.1.1
        Date d4(80000);      //2099.12.31

        cout << "转换构造函数测试，包括合法及非法值" << endl;

        cout << "d1应该是1900.1.1，  实际为：";
        d1.show();
        cout << "d2应该是2099.12.31，实际为：";
        d2.show();
        cout << "d3应该是1900.1.1，  实际为：";
        d3.show();
        cout << "d4应该是2099.12.31，实际为：";
        d4.show();

        d1 = 30000;
        cout << "d1应该是1982.2.19， 实际为：";
        d1.show();

        d2 = 44302;
        cout << "d2应该是2021.4.17， 实际为：";
        d2.show();

        wait_for_enter();
    }

    if (1) {
        Date d1(1900, 1, 1);
        Date d2(2099, 12, 31);
        Date d3(1982, 2, 19);
        Date d4(2021, 4, 17);

        cout << "类型转换函数测试，包括合法及非法值" << endl;

        cout << "d1应该是1，    实际为：" << int(d1) << endl;
        cout << "d2应该是73049，实际为：" << int(d2) << endl;
        cout << "d3应该是30000，实际为：" << int(d3) << endl;
        cout << "d4应该是44302，实际为：" << int(d4) << endl;

        wait_for_enter();
    }

    if (1) {
        Date d1(2021, 4, 17);
        Date d2;

        cout << "测试+/- n" << endl;

        d2 = d1 + 75;
        cout << "d2应该是2021.7.1，    实际为：" << d2 << endl;

        d2 = 75 + d1;
        cout << "d2应该是2021.7.1，    实际为：" << d2 << endl;

        d2 = d1 - 7;
        cout << "d2应该是2021.4.10，   实际为：" << d2 << endl;

        d1.set(2020, 2, 28);
        d2 = d1 + 3;
        cout << "d2应该是2020.3.2，    实际为：" << d2 << endl;

        d1.set(2021, 2, 28);
        d2 = d1 + 3;
        cout << "d2应该是2021.3.3，    实际为：" << d2 << endl;

        d1.set(2020, 12, 28);
        d2 = d1 + 5;
        cout << "d2应该是2021.1.2，    实际为：" << d2 << endl;

        d1.set(2021, 1, 1);
        d2 = d1 - 5;
        cout << "d2应该是2020.12.27，  实际为：" << d2 << endl;

        d1.set(2018, 12, 31);
        d2 = d1 + 366;
        cout << "d2应该是2020.1.1，    实际为：" << d2 << endl;

        d1.set(2019, 12, 31);
        d2 = d1 + 367;
        cout << "d2应该是2021.1.1，    实际为：" << d2 << endl;

        d1.set(2020, 1, 1);
        d2 = d1 - 366;
        cout << "d2应该是2018.12.31，  实际为：" << d2 << endl;

        d1.set(2021, 1, 1);
        d2 = d1 - 367;
        cout << "d2应该是2019.12.31，  实际为：" << d2 << endl;

        d1.set(1900, 1, 2);
        d2 = d1 - 10;
        cout << "d2应该是1900.1.1，    实际为：" << d2 << endl;

        d1.set(2099, 12, 30);
        d2 = d1 + 5;
        cout << "d2应该是2099.12.31，  实际为：" << d2 << endl;

        d1.set(2021, 5, 9);
        d2.set(2021, 4, 17);
        cout << "d1-d2应该是22，       实际为：" << d1 - d2 << endl;

        d1.set(2020, 4, 17);
        d2.set(2019, 4, 17);
        cout << "d1-d2应该是366，      实际为：" << d1 - d2 << endl;

        d1.set(2021, 4, 17);
        d2.set(2020, 4, 17);
        cout << "d1-d2应该是365，      实际为：" << d1 - d2 << endl;

        d1.set(2021, 4, 17);
        cout << "d1+14应该是2021.5.1， 实际为：" << d1 + 14 << endl;
        cout << "14+d1应该是2021.5.1， 实际为：" << 14 + d1 << endl;
        cout << "d1-17应该是2021.3.31，实际为：" << d1 - 17 << endl;

        wait_for_enter();
    }

    if (1) {
        Date d1(2021, 4, 17);
        Date d2(2021, 3, 11);

        cout << "测试-date" << endl;
        cout << "日期差值应为37， 实际为：" << d1 - d2 << endl;

        d2.set(2021, 4, 30);
        cout << "日期差值应为-13，实际为：" << d1 - d2 << endl;

        d1.set(2020, 3, 2);
        d2.set(2020, 2, 27);
        cout << "日期差值应为4，  实际为：" << d1 - d2 << endl;

        d1.set(2021, 3, 2);
        d2.set(2021, 2, 27);
        cout << "日期差值应为3，  实际为：" << d1 - d2 << endl;

        d1.set(2021, 1, 2);
        d2.set(2020, 12, 27);
        cout << "日期差值应为6，  实际为：" << d1 - d2 << endl;

        d1.set(2020, 1, 1);
        d2.set(2018, 12, 31);
        cout << "日期差值应为366，实际为：" << d1 - d2 << endl;

        d1.set(2021, 1, 1);
        d2.set(2019, 12, 31);
        cout << "日期差值应为367，实际为：" << d1 - d2 << endl;

        wait_for_enter();
    }

    if (1) {
        Date d1, d2;

        cout << "测试++/--" << endl;

        d1.set(2021, 4, 17);
        d2 = d1++;
        cout << "d1应该是2021.4.18， 实际为：" << d1 << endl;
        cout << "d2应该是2021.4.17， 实际为：" << d2 << endl;

        d1.set(2021, 4, 17);
        d2 = ++d1;
        cout << "d1应该是2021.4.18， 实际为：" << d1 << endl;
        cout << "d2应该是2021.4.18， 实际为：" << d2 << endl;

        d1.set(2021, 5, 1);
        d2 = d1--;
        cout << "d1应该是2021.4.30， 实际为：" << d1 << endl;
        cout << "d2应该是2021.5.1，  实际为：" << d2 << endl;

        d1.set(2021, 5, 1);
        d2 = --d1;
        cout << "d1应该是2021.4.30， 实际为：" << d1 << endl;
        cout << "d2应该是2021.4.30， 实际为：" << d2 << endl;

        d1.set(2020, 12, 31);
        d2 = d1++;
        cout << "d1应该是2021.1.1，  实际为：" << d1 << endl;
        cout << "d2应该是2020.12.31，实际为：" << d2 << endl;

        d1.set(2021, 1, 1);
        d2 = d1--;
        cout << "d1应该是2020.12.31，实际为：" << d1 << endl;
        cout << "d2应该是2021.1.1，  实际为：" << d2 << endl;

        d1.set(2020, 2, 28);
        d2 = d1++;
        cout << "d1应该是2020.2.29， 实际为：" << d1 << endl;
        cout << "d2应该是2020.2.28， 实际为：" << d2 << endl;

        d1.set(2021, 2, 28);
        d2 = d1++;
        cout << "d1应该是2021.3.1，  实际为：" << d1 << endl;
        cout << "d2应该是2021.2.28， 实际为：" << d2 << endl;

        d1.set(2020, 3, 1);
        d2 = d1--;
        cout << "d1应该是2020.2.29， 实际为：" << d1 << endl;
        cout << "d2应该是2020.3.1，  实际为：" << d2 << endl;

        d1.set(2021, 3, 1);
        d2 = d1--;
        cout << "d1应该是2021.2.28， 实际为：" << d1 << endl;
        cout << "d2应该是2021.3.1，  实际为：" << d2 << endl;

        d1.set(2099, 12, 31);
        d2 = d1++;
        cout << "d1应该是2099.12.31，实际为：" << d1 << endl;
        cout << "d2应该是2099.12.31，实际为：" << d2 << endl;

        d1.set(1900, 1, 1);
        d2 = d1--;
        cout << "d1应该是1900.1.1，  实际为：" << d1 << endl;
        cout << "d2应该是1900.1.1，  实际为：" << d2 << endl;

        wait_for_enter();
    }

    if (1) {
        /* 测试比较运算符 */
        Date d1(2021, 4, 17);
        Date d2(2021, 4, 17);
        Date d3(2021, 3, 27);

        cout << "测试比较运算符" << endl;

        cout << "比较结果应为0，实际为：" << (d1 > d2) << endl;
        cout << "比较结果应为1，实际为：" << (d1 >= d2) << endl;
        cout << "比较结果应为0，实际为：" << (d1 < d2) << endl;
        cout << "比较结果应为1，实际为：" << (d1 <= d2) << endl;
        cout << "比较结果应为1，实际为：" << (d1 == d2) << endl;
        cout << "比较结果应为0，实际为：" << (d1 != d2) << endl;

        cout << "比较结果应为1，实际为：" << (d1 > d3) << endl;
        cout << "比较结果应为1，实际为：" << (d1 >= d3) << endl;
        cout << "比较结果应为0，实际为：" << (d1 < d3) << endl;
        cout << "比较结果应为0，实际为：" << (d1 <= d3) << endl;
        cout << "比较结果应为0，实际为：" << (d1 == d3) << endl;
        cout << "比较结果应为1，实际为：" << (d1 != d3) << endl;

        wait_for_enter();
    }

    return 0;
}