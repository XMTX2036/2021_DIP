#ifndef __BMP_H__
#define __BMP_H__

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef int LONG;
 
struct BITMAPFILEHEADER{
    // WORD bfType;
    DWORD bfSize;
    WORD bfReserved1, bfReserved2;
    DWORD bfOffBits;
};
 
struct BITMAPINFOHEADER{
    DWORD biSize;
    LONG biWidth;
    LONG biHeight;
    WORD biPlanes;
    WORD biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG biXPelsPerMeter, biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
};
 
struct RGBQUAD{
    BYTE rgbBlue;
    BYTE rgbRed;
    BYTE rgbGreen;
    BYTE rgbReserved;
};
struct RGB
{
	BYTE B;
	BYTE G;
	BYTE R;
};
 
struct YUV{
	BYTE Y;
	BYTE U;
	BYTE V;
};
 
BYTE overflow(int n);

#endif