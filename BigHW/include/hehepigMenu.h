/* 朱泽凯 2051995 济美 */
#pragma once

/// <summary>
/// 给出一个菜单供用户选择，选择范围是一系列单字符
/// </summary>
/// <param name="n">选项个数</param>
/// <param name="OprChar">每个选项对应的选择字符</param>
/// <param name="HintStr">每个选项对应的提示字符串</param>
/// <param name="CaseSensitive">大小写敏感吗？默认0（0不敏感 1敏感）</param>
/// <returns></returns>
int Menu_char(int n, const char *OprChar, const char* HintStr[], int CaseSensitive=0);
