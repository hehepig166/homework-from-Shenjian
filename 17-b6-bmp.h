/* 2051995 ���� ���� */
#pragma once
#include <stdint.h>
#include <fstream>
using namespace std;

/**
 * Common Data Types
 *
 * The data types in this section are essentially aliases for C/C++
 * primitive data types.
 *
 * Adapted from http://msdn.microsoft.com/en-us/library/cc230309.aspx.
 * See http://en.wikipedia.org/wiki/Stdint.h for more on stdint.h.
 */
typedef unsigned char  BYTE;
typedef unsigned long DWORD;
typedef long  LONG;
typedef uint16_t WORD;



/**
 * BITMAPFILEHEADER
 *
 * The BITMAPFILEHEADER structure contains information about the type, size,
 * and layout of a file that contains a DIB [device-independent bitmap].
 *
 * Adapted from http://msdn.microsoft.com/en-us/library/dd183374(VS.85).aspx.
 */
#pragma pack(1)     //ǿ������(1�ֽڶ���)
typedef struct {
    WORD   bfType;
    DWORD  bfSize;
    WORD   bfReserved1;
    WORD   bfReserved2;
    DWORD  bfOffBits;
} my_BITMAPFILEHEADER;
#pragma pack()



/**
 * BITMAPINFOHEADER
 *
 * The BITMAPINFOHEADER structure contains information about the
 * dimensions and color format of a DIB [device-independent bitmap].
 *
 * Adapted from http://msdn.microsoft.com/en-us/library/dd183376(VS.85).aspx.
 */
#pragma pack(1)     //ǿ������(1�ֽڶ���)
typedef struct {
    DWORD  biSize;
    LONG   biWidth;
    LONG   biHeight;
    WORD   biPlanes;
    WORD   biBitCount;
    DWORD  biCompression;
    DWORD  biSizeImage;
    LONG   biXPelsPerMeter;
    LONG   biYPelsPerMeter;
    DWORD  biClrUsed;
    DWORD  biClrImportant;
} my_BITMAPINFOHEADER;
#pragma pack()




/**
 * RGBTRIPLE
 *
 * This structure describes a color consisting of relative intensities of
 * red, green, and blue.
 *
 * Adapted from http://msdn.microsoft.com/en-us/library/aa922590.aspx.
 */
typedef struct {
    BYTE  rgbtRed;
    BYTE  rgbtGreen;
    BYTE  rgbtBlue;
} my_RGBTRIPLE;


class bitmap_image {
protected:
    my_BITMAPFILEHEADER bf;
    my_BITMAPINFOHEADER bi;

    int palcnt;         //��ɫ��������
    my_RGBTRIPLE *palette; //��ɫ����ɫ��Ϣ
    my_RGBTRIPLE *image;   //�����������Դ洢��ɫ��Ϣ���±��0��ʼ

    my_RGBTRIPLE &IMG(int h, int w);
    my_RGBTRIPLE &IMG_c(int h, int w) const;

    bool fgood;

    bool init(const char *filename);    //��ʼ��

public:

    bitmap_image(const char *filename); //��ͼƬ����ȡͷ��Ϣ

    int width() const;	//����ͼƬ�Ŀ��
    int height() const; //����ͼƬ�ĸ߶�
    unsigned int get_pixel(int row, int col) const; //����ָ�����RGB��ɫ���±��0��ʼ

    bool good() const;    //���ǿɴ�����ļ�������true�����򷵻�false


    //template<int height, int width>
    //void grayscale(int height, RGBTRIPLE[][width]);
    // 
    //template<int height, int width>
    //void edges(int height, RGBTRIPLE[][width]);

    

};