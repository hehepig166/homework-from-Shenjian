/* ���� 2051995 ���� */

#ifndef CODE_GB2312_hehepig
#define CODE_GB2312_hehepig



//��������ȷ�ԣ���ȷ����1�����󷵻�0
bool check_code(const char *incode);

//���������Ϣ��uchar���飬����Ŀ�����飬����ĺ��ֻ����루����char�����Ƿ��Ƿ���
void get_pixels(unsigned char *out, const char *incode, const int is_fanti = 0);

//���������Ϣ��bool���飬����Ŀ�����飬����ĺ��ֻ����루����char�����Ƿ��Ƿ���
void to_image(bool out[16][16], const char *incode, const int is_fanti);

//���һ�� 16 ������Ϣ���� *����������з�
void show_line(const unsigned char x1, const unsigned char x2);

//���������Ļ����'*'����㣬�������ã����ܼ�ª
void show_pixels(const char *incode, const int is_fanti = 0);


#endif