/* 朱泽凯 2051995 济美 */
#pragma once


/// <summary>
/// GetInt 多字符按键返回值的描述
/// </summary>
enum {
    key_left=256,
    key_up,
    key_right,
    key_down,
    key_Fn,
    key_unknown
};


int GetInt(int Min, int Max);

/// <summary>
/// 无数值范围限制地获得一个 int
/// </summary>
/// <param name="HintStr">需要输出地提示字符串</param>
/// <returns>读到的数</returns>
int GetInt(const char* HintStr);


/// <summary>
/// 获取一个区间范围数值内的 int
/// </summary>
/// <param name="HintStr">需要输出的提示字符串</param>
/// <param name="Min">可取的最小值</param>
/// <param name="Max">可取的最大值</param>
/// <returns>读到的那个数（若读到的数不符合范围，会再次提示并请求输入）</returns>
int GetInt(const char* HintStr, int Min, int Max);


/// <summary>
/// 无限制地获得一个键盘按键信息
/// </summary>
/// <param name="Echo">回显吗，默认是0 （0否 1是）</param>
/// <returns>键盘下一个按下的按钮（若是单个char组成的，返回其ASCII值，否则返回对应的enum值）</returns>
int GetKey(int Echo = 0);

/// <summary>
/// 有限制地获得一个键盘按键信息
/// </summary>
/// <param name="Min">最小ASCII码值</param>
/// <param name="Max">最大ASCII码值</param>
/// <param name="Echo">回显吗，默认是0（0否 1是）</param>
/// <returns></returns>
int GetKey(int Min, int Max, int Echo = 0);