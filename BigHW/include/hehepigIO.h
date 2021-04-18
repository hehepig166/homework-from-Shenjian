/* 朱泽凯 2051995 济美 */
#pragma once


/// <summary>
/// GetInt 多字符按键返回值的描述
/// </summary>
enum {
    key_left=512,
    key_up,
    key_right,
    key_down,
    key_Fn,
    key_unknown,

    key_none
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

/// <summary>
/// 读取一个字母，并返回其ASCII值
/// </summary>
/// <param name="Mode = 0">返回原字母对应的ASCII</param>
/// <param name="Mode = 1">返回大写ASCII</param>
/// <param name="Mode = 2">返回小写ASCII</param>
/// <param name="Echo">回显吗，默认是0（0否 1是）</param>
/// <returns>ASCII</returns>
int GetAlpha(int Mode = 0, int Echo = 0);

/// <summary>
/// 读取一个字母或数字字符，并返回其ASCII值
/// </summary>
/// <param name="Mode = 0">返回原字母对应的ASCII</param>
/// <param name="Mode = 1">返回大写ASCII</param>
/// <param name="Mode = 2">返回小写ASCII</param>
/// <param name="Echo">回显吗，默认是0（0否 1是）</param>
/// <returns>ASCII</returns>
int GetAlnum(int Mode = 0, int Echo = 0);

/// <summary>
/// 不暂停求输入，获得键盘按键信息。
/// </summary>
/// <returns>若没按键盘，则返回 tok_none </returns>
int AskKey();