/* ���� 2051995 ���� */
#pragma once


/// <summary>
/// GetInt ���ַ���������ֵ������
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
/// ����ֵ��Χ���Ƶػ��һ�� int
/// </summary>
/// <param name="HintStr">��Ҫ�������ʾ�ַ���</param>
/// <returns>��������</returns>
int GetInt(const char* HintStr);


/// <summary>
/// ��ȡһ�����䷶Χ��ֵ�ڵ� int
/// </summary>
/// <param name="HintStr">��Ҫ�������ʾ�ַ���</param>
/// <param name="Min">��ȡ����Сֵ</param>
/// <param name="Max">��ȡ�����ֵ</param>
/// <returns>�������Ǹ��������������������Ϸ�Χ�����ٴ���ʾ���������룩</returns>
int GetInt(const char* HintStr, int Min, int Max);


/// <summary>
/// �����Ƶػ��һ�����̰�����Ϣ
/// </summary>
/// <param name="Echo">������Ĭ����0 ��0�� 1�ǣ�</param>
/// <returns>������һ�����µİ�ť�����ǵ���char��ɵģ�������ASCIIֵ�����򷵻ض�Ӧ��enumֵ��</returns>
int GetKey(int Echo = 0);

/// <summary>
/// �����Ƶػ��һ�����̰�����Ϣ
/// </summary>
/// <param name="Min">��СASCII��ֵ</param>
/// <param name="Max">���ASCII��ֵ</param>
/// <param name="Echo">������Ĭ����0��0�� 1�ǣ�</param>
/// <returns></returns>
int GetKey(int Min, int Max, int Echo = 0);

/// <summary>
/// ��ȡһ����ĸ����������ASCIIֵ
/// </summary>
/// <param name="Mode = 0">����ԭ��ĸ��Ӧ��ASCII</param>
/// <param name="Mode = 1">���ش�дASCII</param>
/// <param name="Mode = 2">����СдASCII</param>
/// <param name="Echo">������Ĭ����0��0�� 1�ǣ�</param>
/// <returns>ASCII</returns>
int GetAlpha(int Mode = 0, int Echo = 0);

/// <summary>
/// ��ȡһ����ĸ�������ַ�����������ASCIIֵ
/// </summary>
/// <param name="Mode = 0">����ԭ��ĸ��Ӧ��ASCII</param>
/// <param name="Mode = 1">���ش�дASCII</param>
/// <param name="Mode = 2">����СдASCII</param>
/// <param name="Echo">������Ĭ����0��0�� 1�ǣ�</param>
/// <returns>ASCII</returns>
int GetAlnum(int Mode = 0, int Echo = 0);

/// <summary>
/// ����ͣ�����룬��ü��̰�����Ϣ��
/// </summary>
/// <returns>��û�����̣��򷵻� tok_none </returns>
int AskKey();