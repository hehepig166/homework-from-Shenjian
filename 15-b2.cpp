/* 2051995 朱泽凯 济美 */
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <time.h>
/* 如果有需要，此处可以添加头文件 */

using namespace std;

/* 允许定义常变量/宏定义，但不允许定义全局变量 */
const unsigned long long BJ_ID = 1ull << 52;
const unsigned long long RJ_ID = 1ull << 53;
const char number[13] = { '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A', '2' };
#if defined(__linux) || defined(__linux__)
const char pattern[4] = { 'D', 'H', 'C', 'S' };
#else
const char pattern[4] = { 4, 3, 5, 6 };
#endif
#define ID(no,pat) (1ull<<(((no)<<2)|(pat)))

/* 可以添加自己需要的函数 */

int Get_int(const char *prompt, const int Min, const int Max)
{
    int ret;
    do {
        puts("");
        puts(prompt);
        if (!cin.good()) {
            cin.clear();
            cin.ignore(1024, '\n');
        }
        cin >> ret;
    } while (!cin.good() || !(ret >= Min && ret <= Max));
    return ret;
}

/***************************************************************************
  函数名称：
  功    能：打印某个玩家的牌面信息，如果是地主，后面加标记
  输入参数：
  返 回 值：
  说    明：
 ***************************************************************************/
int print(const char *prompt, const bool landlord, const unsigned long long player)
{
    /* 只允许定义不超过三个基本类型的简单变量，不能定义数组变量、结构体、string等 */
    cout << prompt;
    int i, j;
    for (i = 0; i < 13; i++)
        for (j = 0; j < 4; j++)
            if (player & ID(i, j))
                cout << pattern[j] << number[i] << ' ';
    if (player & BJ_ID)
        cout << "BJ" << ' ';
    if (player & RJ_ID)
        cout << "RJ" << ' ';

    if (landlord)
        cout << "(地主)";
    cout << endl;
    return 0;
}

/***************************************************************************
  函数名称：
  功    能：发牌
  输入参数：
  返 回 值：
  说    明：
 ***************************************************************************/
int deal(unsigned long long *player)
{
    /* 只允许定义不超过十个基本类型的简单变量，不能定义数组变量、结构体、string等 */
    
    srand((unsigned int)time(0));

    unsigned long long status_all = 0;  //记录哪些牌发过了
    unsigned long long dizhu;   //三张地主牌
    int lun;  //第几轮
    int i, j;  //临时变量
    int dizhuID=-1;    //谁是地主;

    for (lun = 1; lun <= 17; lun++) {
        for (i = 0; i < 3; i++) {
            do {
                j = rand() % 54;
            } while ((1ull << j) & status_all);
            status_all |= player[i] |= (1ull << j);
        }
        cout << "第" << lun << "轮结束：" << endl;
        print("甲的牌：", 0, player[0]);
        print("乙的牌：", 0, player[1]);
        print("丙的牌：", 0, player[2]);
    }

    dizhu = ((1ull << 54) - 1) ^ status_all;
    dizhuID = Get_int("请选择一个地主[0-2]：", 0, 2);
    player[dizhuID] |= dizhu;

    //print("地主牌：", 0, dizhu);

    return dizhuID; //此处修改为选定的地主(0-2)
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：main函数，不准修改
 ***************************************************************************/
int main()
{
    /*
    for (int i = 0; i < 13; i++)
        for (int j = 0; j < 4; j++)
            printf("[%d %d]%lld\n", i, j, ID(i, j));
    printf("%lld\n%lld\n", BJ_ID, RJ_ID);
    printf("%lld\n", 1ll << 53);
    */

    unsigned long long player[3] = { 0 }; //存放三个玩家的发牌信息
    int landlord; //返回0-2表示哪个玩家是地主

    cout << "按回车键开始发牌";
    while (getchar() != '\n')
        ;

    landlord = deal(player);
    print("甲的牌：", (landlord == 0), player[0]);
    print("乙的牌：", (landlord == 1), player[1]);
    print("丙的牌：", (landlord == 2), player[2]);

    return 0;
}
