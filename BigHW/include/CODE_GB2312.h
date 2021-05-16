/* 朱泽凯 2051995 济美 */

#ifndef CODE_GB2312_hehepig
#define CODE_GB2312_hehepig



//检查编码正确性，正确返回1，错误返回0
bool check_code(const char *incode);

//输出点阵信息到uchar数组，给出目标数组，待求的汉字机内码（两个char），是否是繁体
void get_pixels(unsigned char *out, const char *incode, const int is_fanti = 0);

//输出点阵信息到bool数组，给出目标数组，待求的汉字机内码（两个char），是否是繁体
void to_image(bool out[16][16], const char *incode, const int is_fanti);

//输出一行 16 个点信息，用 *，不输出换行符
void show_line(const unsigned char x1, const unsigned char x2);

//输出点阵到屏幕，用'*'代表点，仅测试用，功能简陋
void show_pixels(const char *incode, const int is_fanti = 0);


#endif