#ifndef __BMP_IO_H__
#define __BMP_IO_H__
#include <cstdio>

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;

typedef struct tagBITMAPFILEHEADER{
    // WORD bfType;
    DWORD bfSize;
    WORD bfReserved1;
	WORD bfReserved2;
    DWORD bfOffBits;
}BITMAPFILEHEADER;
typedef struct tagBITMAPINFOHEADER {
    DWORD biSize;
    DWORD biWidth;
    DWORD biHeight;
    WORD biPlanes;
    WORD biBitCount;
    DWORD biCompression; 
    DWORD biSizeImage;  
    DWORD biXPelsPerMeter; 
    DWORD biYPelsPerMeter; 
    DWORD biClrUsed; 
    DWORD biClrImportant; 
}BITMAPINFOHEADER;  

typedef struct tagRGBQUAD{
    BYTE rgbBlue; 
    BYTE rgbGreen; 
    BYTE rgbRed; 
    BYTE rgbReserved; 
}RGBQUAD;


void readHead(FILE **fp);
void write(BYTE *data, FILE **fp);
void RGB2gray(BYTE *data, int size);


#endif