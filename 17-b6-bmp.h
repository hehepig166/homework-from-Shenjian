/* 2051995 朱泽凯 济美 */
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
#pragma pack(1)     //强制连排(1字节对齐)
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
#pragma pack(1)     //强制连排(1字节对齐)
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

    int palcnt;         //调色板索引数
    my_RGBTRIPLE *palette; //调色板颜色信息
    my_RGBTRIPLE *image;   //连续紧密线性存储颜色信息，下标从0开始

    my_RGBTRIPLE &IMG(int h, int w);
    my_RGBTRIPLE &IMG_c(int h, int w) const;

    bool fgood;

    bool init(const char *filename);    //初始化

public:

    bitmap_image(const char *filename); //打开图片并读取头信息

    int width() const;	//返回图片的宽度
    int height() const; //返回图片的高度
    unsigned int get_pixel(int row, int col) const; //返回指定点的RGB颜色，下标从0开始

    bool good() const;    //若是可处理的文件，返回true，否则返回false


    //template<int height, int width>
    //void grayscale(int height, RGBTRIPLE[][width]);
    // 
    //template<int height, int width>
    //void edges(int height, RGBTRIPLE[][width]);

    

};