/* 2051995 朱泽凯 济美 */

//#include <Windows.h>
#include <iostream>
#include <fstream>
#include "17-b6-bmp.h"
using namespace std;

#define RGBTRIPLE my_RGBTRIPLE
#define BITMAPFILEHEADER my_BITMAPFILEHEADER
#define BITMAPINFOHEADER my_BITMAPINFOHEADER


bitmap_image::bitmap_image(const char *filename)
{
    fgood = init(filename);
    if (!fgood) {
        getchar();
        exit(-1);
    }
}

RGBTRIPLE &bitmap_image::IMG(int h, int w)
{
    return image[h * bi.biWidth + w];
}

RGBTRIPLE &bitmap_image::IMG_c(int h, int w) const
{
    return image[h * bi.biWidth + w];
}

bool bitmap_image::init(const char *filename)
{
    ifstream infile;

    infile.open(filename, ios::binary);
    if (!infile.is_open()) {
        cerr << "Could not open " << filename << endl;
        return false;
    }
    else {
        infile.read((char *)&bf, sizeof(BITMAPFILEHEADER));
        infile.read((char *)&bi, sizeof(BITMAPINFOHEADER));
        if (!infile.good() ||
            !(bf.bfType == 0x4d42 && bi.biSize == 40 && bi.biCompression == 0 &&
                (bi.biBitCount == 1 || bi.biBitCount == 4 || bi.biBitCount == 8 || bi.biBitCount == 24))) {
            cerr << "Unsupported file format." << endl;
            return false;
        }
    }

    //调色板
    palcnt = (bf.bfOffBits - sizeof(bi) - sizeof(bf)) / 4;  //调色板颜色数量（每个颜色占4字节）

    palette = new(nothrow) RGBTRIPLE[palcnt + 10];   //申请空间
    if (palette == NULL) {
        cerr << "Not enough memory for palette" << endl;
        return false;
    }
    BYTE paltmp[4];
    for (int i = 0; i < palcnt; i++) {
        infile.read((char *)paltmp, 4);
        //infile >> r >> g >> b >> alpha;       不能用 >> 读二进制
        palette[i].rgbtRed = paltmp[0];
        palette[i].rgbtGreen = paltmp[1];
        palette[i].rgbtBlue = paltmp[2];

    }

    //位图信息
    int height = bi.biHeight;
    int width = bi.biWidth;
    int bitcnt = width * bi.biBitCount;     //一行信息bit数
    int bytecnt = (bitcnt + 31) / 32 * 4;   //文件中一行所占字节数

    image = new(nothrow) RGBTRIPLE[height * width + 10];    //申请空间
    if (image == NULL) {
        cerr << "Not enough memory for image" << endl;
        return false;
    }

    for (int i = height - 1; i >= 0; i--) {
        if (bi.biBitCount == 1) {
            BYTE tmp;
            int cnt = 0;
            for (int j = 0; cnt < bytecnt; cnt++) {
                infile.read((char *)&tmp, 1);
                for (int k = 7; k >= 0 && j < width; j++, k--) {
                    IMG(i, j) = palette[(tmp & (1 << k)) > 0];
                }
            }
        }
        else if (bi.biBitCount == 4) {
            BYTE tmp;
            int cnt = 0;
            for (int j = 0; cnt < bytecnt; cnt++) {
                infile.read((char *)&tmp, 1);
                for (int k = 1; k >= 0 && j < width; j++, k--) {
                    IMG(i, j) = palette[(tmp & (0xf << (k * 4))) >> (k * 4)];
                }
            }
        }
        else if (bi.biBitCount == 8) {
            BYTE tmp;
            int cnt = 0;
            for (int j = 0; cnt < bytecnt; cnt++) {
                infile.read((char *)&tmp, 1);
                if (j < width) {
                    IMG(i, j) = palette[tmp];
                    j++;
                }
            }
        }
        else if (bi.biBitCount == 24) {
            int padding = bytecnt - bitcnt / 8;
            infile.read((char *)&IMG(i, 0), width * 3);
            infile.seekg(padding, ios::cur);
        }
    }

    infile.close();

    return true;
}

int bitmap_image::width() const
{
    return bi.biWidth;
}

int bitmap_image::height() const
{
    return bi.biHeight;
}

unsigned int bitmap_image::get_pixel(int row, int col) const
{
    return (IMG_c(row, col).rgbtRed << 16) | (IMG_c(row, col).rgbtGreen << 8) | (IMG_c(row, col).rgbtBlue);
}

bool bitmap_image::good() const
{
    return fgood;
}