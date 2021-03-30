/* 济美 2051995 朱泽凯 */
#include <iostream>
#include <iomanip>
using namespace std;

const char *sp = "=====================================";

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：每个数字宽度为8，右对齐
***************************************************************************/
template<typename T, int n, int m>
void matrix_print(const char *hintStr, const T(&t)[n][m])	//将...替换为相应内容
{
    /* 按需增加内容 */

    cout << hintStr << endl;

    //t[n][m]
    for (int i = 0; i < n; i++, cout << endl)
        for (int j = 0; j < m; j++)
            cout << setw(8) << t[i][j];
    return;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
template<typename T, int n, int m, typename T1, int n1, int m1, typename T2, int n2, int m2>
void matrix_addition(T(&t)[n][m], const T1(&t1)[n1][m1], const T2(&t2)[n2][m2])	//将...替换为相应内容
{
    /* 按需增加内容 */
    //源矩阵1 : 行=3 列=4 占用空间=48字节
    cout << "源矩阵1 : 行=" << n1 << " 列=" << m1 << " 占用空间=" << sizeof(t1) << "字节" << endl;
    cout << "源矩阵2 : 行=" << n2 << " 列=" << m2 << " 占用空间=" << sizeof(t2) << "字节" << endl;
    cout << "和矩阵  : 行=" << n << " 列=" << m << " 占用空间=" << n * m * sizeof(T) << "字节" << endl;

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            t[i][j] = (T)(t1[i][j] + t2[i][j]);

    return;
}


/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
template<typename T, int n, int m, typename T1, int n1, int m1, typename T2, int n2, int m2>
void matrix_multiplication(T(&t)[n][m], const T1(&t1)[n1][m1], const T2(&t2)[n2][m2])	//将...替换为相应内容
{
    /* 按需增加内容 */
    //源矩阵1 : 行=3 列=4 占用空间=48字节
    cout << "源矩阵1 : 行=" << n1 << " 列=" << m1 << " 占用空间=" << sizeof(t1) << "字节" << endl;
    cout << "源矩阵2 : 行=" << n2 << " 列=" << m2 << " 占用空间=" << sizeof(t2) << "字节" << endl;
    cout << "积矩阵  : 行=" << n << " 列=" << m << " 占用空间=" << n * m * sizeof(T) << "字节" << endl;

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
        {
            t[i][j] = 0;
            for (int k = 0; k < m1; k++)
                t[i][j] += (T)(t1[i][k] * t2[k][j]);
        }

    return;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：main函数不准更改
***************************************************************************/
int main()
{
    int t1[3][4] = {
        {1,2,3,4},
        {5,6,7,8},
        {9,10,11,12}
    };
    int t2[3][4] = {
        {12,11,10,9},
        {8,7,6,5},
        {4,3,2,1}
    };
    int t3[4][2] = {
        {1, 2},
        {3, 4},
        {5, 6},
        {7, 8}
    };
    int t_add[3][4], t_mul[3][2];

    cout << sp << endl;
    matrix_print("加法运算，源矩阵1：", t1);
    matrix_print("加法运算，源矩阵2：", t2);
    matrix_addition(t_add, t1, t2);	//将t1和t2的和放入t_add中，人工保证三个矩阵行列一致
    matrix_print("加法运算，和矩阵 ：", t_add);
    cout << sp << endl;
    matrix_print("乘法运算，源矩阵1：", t1);
    matrix_print("乘法运算，源矩阵2：", t3);
    matrix_multiplication(t_mul, t1, t3);	//将t1和t2的和放入t_add中，人工保证三个矩阵行列一致
    matrix_print("乘法运算，积矩阵 ：", t_mul);
    cout << sp << endl;

    double d1[2][4] = {
        {1.1, 2.2, 3.3, 4.4},
        {5.5 ,6.6, 7.7, 8.8}
    };
    double d2[2][4] = {
        {8.8, 7.7, 6.6, 5.5},
        {4.4, 3.3, 2.2, 1.1}
    };
    double d_add[2][4];
    /* 不要问为什么矩阵乘法数据类型不同，故意的 */
    float f3[4][3] = {
        {12.12f, 11.11f, 10.10f},
        {9.9f, 8.8f, 7.7f},
        {6.6f, 5.5f, 4.4f},
        {3.3f, 2.2f, 1.1f}
    };
    int i_mul[2][3];

    matrix_print("加法运算，源矩阵1：", d1);
    matrix_print("加法运算，源矩阵2：", d2);
    matrix_addition(d_add, d1, d2);	//将d1和d2的和放入d_add中，人工保证三个矩阵行列一致
    matrix_print("加法运算，和矩阵 ：", d_add);
    cout << sp << endl;
    matrix_print("乘法运算，源矩阵1：", d1);
    matrix_print("乘法运算，源矩阵2：", f3);
    matrix_multiplication(i_mul, d1, f3);	//将t1和t2的和放入t_add中，人工保证三个矩阵行列一致
    matrix_print("乘法运算，积矩阵 ：", i_mul);
    cout << sp << endl;

    return 0;
}
